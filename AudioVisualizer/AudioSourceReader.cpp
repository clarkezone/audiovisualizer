#include "pch.h"
#include "AudioSourceReader.h"
#include <windows.media.mediaproperties.h>
#include <windows.media.core.interop.h>
#include <windows.media.h>

static bool g_MFInitialized = false;

void InitializeMediaFoundation() {
	if (!g_MFInitialized) {
		MFStartup(MF_VERSION);
		g_MFInitialized = true;
	}
}


namespace winrt::AudioVisualizer::implementation
{

	int32_t AudioSourceReader::FirstAudioStreamIndex()
	{
		return MF_SOURCE_READER_FIRST_AUDIO_STREAM;
	}

	void AudioSourceReader::Close()
	{
		reader = nullptr;
	}

	int32_t AudioSourceReader::StreamIndex()
	{
		if (!reader) {
			throw hresult_error(RO_E_CLOSED);
		}
		return streamIndex;
	}

	void AudioSourceReader::StreamIndex(int32_t index)
	{
		if (index != streamIndex) {
			com_ptr<IMFMediaType> nativeType;
			check_hresult(reader->GetNativeMediaType(index, 0, nativeType.put()));
			GUID majorType;
			nativeType->GetMajorType(&majorType);
			if (majorType != MFMediaType_Audio) {
				throw hresult_invalid_argument(L"Not an audio stream");
			}
			// Unselect old stream, select new stream
			reader->SetStreamSelection(streamIndex, FALSE);
			reader->SetStreamSelection(index, TRUE);
			streamIndex = index;
			SetFormatProperties();
			com_ptr<IMFMediaType> mediaType;
			check_hresult(reader->GetNativeMediaType(streamIndex, 0, mediaType.put()));
			GUID subType;
			memset(&subType, 0, sizeof(GUID));
			check_hresult(mediaType->GetGUID(MF_MT_SUBTYPE, &subType));
			b_IsMp3 = subType == MFAudioFormat_MP3;
		}
	}

	void AudioSourceReader::Seek(Windows::Foundation::TimeSpan position)
	{
		if (!reader) {
			throw hresult_error(RO_E_CLOSED);
		}
		PROPVARIANT pvPosition;
		PropVariantInit(&pvPosition);
		pvPosition.vt = VT_I8;
		pvPosition.hVal.QuadPart = position.count();
		check_hresult(reader->SetCurrentPosition(GUID_NULL, pvPosition));
		currentSampleIndex = -1;
	}

	Windows::Media::AudioFrame AudioSourceReader::Read()
	{
		if (!reader) {
			throw hresult_error(RO_E_CLOSED);
		}
		com_ptr<IMFSample> sample;
		DWORD dwStreamFlags = 0;
		check_hresult(reader->ReadSample(streamIndex, 0, nullptr, &dwStreamFlags, nullptr, sample.put()));

		flags = (AudioSourceReaderFlags)dwStreamFlags;
		if (dwStreamFlags & MF_SOURCE_READERF_ERROR) {
			throw hresult_error(E_FAIL);
		}

		Windows::Media::AudioFrame result{ nullptr };

		if (sample)
		{
			// Fix up position if non-compressed format
			if (samplesPerSecond != 0) {
				DWORD sampleBufferLength = 0;
				check_hresult(sample->GetTotalLength(&sampleBufferLength));
				int64_t sampleCount = sampleBufferLength / bytesPerSample;

				// Read the sample time position from the reader
				if (currentSampleIndex == -1) {
					REFERENCE_TIME sampleTime = 0;
					check_hresult(sample->GetSampleTime(&sampleTime));
					currentSampleIndex = time_to_samples(sampleTime);
				}
				else {
					// Fix up sample time based on tracked sample index
					check_hresult(sample->SetSampleTime(samples_to_time(currentSampleIndex)));
					check_hresult(sample->SetSampleDuration(samples_to_time(sampleCount)));
				}
				currentSampleIndex += sampleCount;
			}
			// Convert sample to audio frame IAudioFrameNativeFactory allows that without copy
			MULTI_QI qiFactory[1] = { { &__uuidof(IAudioFrameNativeFactory),nullptr,S_OK } };
			check_hresult(CoCreateInstanceFromApp(CLSID_AudioFrameNativeFactory, nullptr, CLSCTX_INPROC_SERVER, nullptr, 1, qiFactory));
			check_hresult(qiFactory[0].hr);

			com_ptr<IAudioFrameNativeFactory> nativeFactory;
			copy_from_abi(nativeFactory, qiFactory[0].pItf);

			// Now use the factory to create frame out of IMFSample
			com_ptr<ABI::Windows::Media::IAudioFrame> frame;
			check_hresult(nativeFactory->CreateFromMFSample(sample.get(), false, IID_PPV_ARGS(frame.put())));

			copy_from_abi(result, frame.get());
		}

		return result;
	}

	AudioSourceReader::AudioSourceReader(Windows::Storage::Streams::IRandomAccessStream const& stream)
	{
		InitializeMediaFoundation();

		com_ptr<IMFByteStream> byteStream;
		HRESULT hr = MFCreateMFByteStreamOnStreamEx(reinterpret_cast<IUnknown*>(winrt::get_abi(stream)), byteStream.put());
		check_hresult(hr);

		hr = MFCreateSourceReaderFromByteStream(byteStream.get(), nullptr, reader.put());
		check_hresult(hr);

		StreamIndex(AudioSourceReader::FirstAudioStreamIndex());
	}
	Windows::Foundation::TimeSpan AudioSourceReader::Duration()
	{
		if (!reader) {
			throw hresult_error(RO_E_CLOSED);
		}
		PROPVARIANT pvDuration;
		PropVariantInit(&pvDuration);
		check_hresult(reader->GetPresentationAttribute((DWORD)MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &pvDuration));
		return Windows::Foundation::TimeSpan(pvDuration.hVal.QuadPart);
	}
	Windows::Media::MediaProperties::IMediaEncodingProperties AudioSourceReader::GetNativeFormat(int32_t forStreamIndex)
	{
		if (!reader) {
			throw hresult_error(RO_E_CLOSED);
		}
		com_ptr<IMFMediaType> mediaType;
		HRESULT hr = reader->GetNativeMediaType(forStreamIndex, 0, mediaType.put());
		if (hr == MF_E_INVALIDSTREAMNUMBER)
			return nullptr;

		check_hresult(hr);
		com_ptr<ABI::Windows::Media::MediaProperties::IMediaEncodingProperties> abiMediaProps;
		check_hresult(MFCreatePropertiesFromMediaType(mediaType.get(), IID_PPV_ARGS(abiMediaProps.put())));

		Windows::Media::MediaProperties::IMediaEncodingProperties mediaProperties;
		winrt::copy_from_abi(mediaProperties, abiMediaProps.get());
		return mediaProperties;
	}

	Windows::Media::MediaProperties::IMediaEncodingProperties AudioSourceReader::Format()
	{
		if (!reader) {
			throw hresult_error(RO_E_CLOSED);
		}
		com_ptr<IMFMediaType> mediaType;
		check_hresult(reader->GetCurrentMediaType(streamIndex, mediaType.put()));
		com_ptr<ABI::Windows::Media::MediaProperties::IMediaEncodingProperties> abiMediaProps;
		check_hresult(MFCreatePropertiesFromMediaType(mediaType.get(), IID_PPV_ARGS(abiMediaProps.put())));

		Windows::Media::MediaProperties::IMediaEncodingProperties mediaProperties;
		winrt::copy_from_abi(mediaProperties, abiMediaProps.get());
		return mediaProperties;
	}

	void AudioSourceReader::Format(Windows::Media::MediaProperties::IMediaEncodingProperties const & format)
	{
		if (!reader) {
			throw hresult_error(RO_E_CLOSED);
		}
		com_ptr<IMFMediaType> mediaType;
		check_hresult(MFCreateMediaTypeFromProperties(reinterpret_cast<IUnknown*>(winrt::get_abi(format)), mediaType.put()));
		check_hresult(reader->SetCurrentMediaType(streamIndex, nullptr, mediaType.get()));
		SetFormatProperties();
	}

	void AudioSourceReader::SetFormatProperties()
	{
		com_ptr<IMFMediaType> mediaType;
		check_hresult(reader->GetCurrentMediaType(streamIndex, mediaType.put()));
		GUID subType;
		memset(&subType, 0, sizeof(subType));
		check_hresult(mediaType->GetGUID(MF_MT_SUBTYPE, &subType));
		if (subType == MFAudioFormat_PCM || subType == MFAudioFormat_Float) {
			// The next property is really frames per second
			UINT32 framesPerSecond = 0;
			check_hresult(mediaType->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, &framesPerSecond));
			UINT32 channelCount = 0;
			check_hresult(mediaType->GetUINT32(MF_MT_AUDIO_NUM_CHANNELS, &channelCount));
			samplesPerSecond = framesPerSecond * channelCount;
			UINT32 bitsPerSample = 0;
			check_hresult(mediaType->GetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, &bitsPerSample));
			bytesPerSample = bitsPerSample >> 3;
		}
		else {
			samplesPerSecond = 0;
			bytesPerSample = 0;
		}
	}
}
