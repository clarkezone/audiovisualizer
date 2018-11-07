#include "pch.h"
#include "ring_buffer.h"

namespace AudioVisualizer
{
	/* Add specified number of samples to the ring buffer. Downsample if neccessary */
	void ring_buffer::add_samples(const float * pSamples, size_t sampleCount)
	{
		if (_data == nullptr)
			throw winrt::hresult_error(E_NOT_VALID_STATE);

		// The buffer needs to be as large to fit all samples + one frame
		if (sampleCount > _downsampleRate * (_size - _frameSize))
			throw winrt::hresult_invalid_argument(L"Audio frame larger than the buffer");	

		size_t samplesInBufferBeforeAdd = samples_in_buffer();	// Record current samples in buffer
		size_t samplesCopied = sampleCount / _downsampleRate;

		// Copy sampleCount number of samples to the buffer, validate _writeIndex against _size 
		// To avoid copying when size is zero (uninitialized buffer)

		const float *source = pSamples;
		for (size_t sampleIndex = 0; sampleIndex < sampleCount; sampleIndex++,source++)
		{
			// Copy input samples if no downsampling or copying the samples from first frame
			if (_downsampleRate == 1 || _downsampleCounter < _frameSize)
			{
				*_writer = *source;
				_writer++;
				if (_writer == _data + _size) {	// Wrap pointer over end
					_writer = _data;
				}
			}
			// If downsampling increate the downsample counter
			if (_downsampleRate != 1)
			{
				_downsampleCounter++;
				if (_downsampleCounter >= _downsampleRate * _frameSize)
					_downsampleCounter = 0;
			}
		}
		// If overflow happened, advance read pointer so _size - _frameSize samples are available
		if (samplesInBufferBeforeAdd + samplesCopied >= _size)
		{
			if (_writer + _frameSize != _data + _size) {
				_reader = _writer + _frameSize;
			}
			else {
				_reader = _data;
			}
			readPositionFrameIndex += (samplesInBufferBeforeAdd + samplesCopied - _size) / _frameSize;
		}
	}
	void ring_buffer::get_deinterleaved(float * pOutput, size_t outputBufferStride)
	{
		if (_data == nullptr)
			throw winrt::hresult_error(E_NOT_VALID_STATE);

		if (outputBufferStride < _stepFrames)
			throw winrt::hresult_invalid_argument();

		if (empty()) // Not enough data to be copied
			throw winrt::hresult_error(E_NOT_VALID_STATE);

		int overlapSamples = (int)(_overlapFrames * _frameSize);

		auto readIndex = _reader - _data;

		// Create source iterator counting back overlap samples from current read position
		const float *source = 
			readIndex >= overlapSamples ? 
				_reader - overlapSamples : 
				_data + (_size - (overlapSamples - readIndex));

		// Iterate over all the output items
		for (size_t frameIndex = 0; frameIndex < outputBufferStride; frameIndex++)
		{
			// Copy or pad output values
			for (size_t channelIndex = 0, outIndex = frameIndex; channelIndex < _frameSize; channelIndex++, outIndex += outputBufferStride)
			{
				if (frameIndex < _overlapFrames + _stepFrames)
				{
					pOutput[outIndex] = *source++;
					if (source == _data + _size)
						source = _data;
				}
				else
				{
					pOutput[outIndex] = 0.0f;
				}
			}
		}
		_reader = source;
		readPositionFrameIndex += (int64_t)(_stepFrames);
	}
}