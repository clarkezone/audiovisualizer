#pragma once
#include <DirectXMath.h>
#include "AudioBuffer.h"
#include <memory>
#include <mutex>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;

namespace AudioVisualizer
{
	namespace Math
	{

		/*class AnalyzerFrame : public RuntimeClass<RuntimeClassFlags<RuntimeClassType::ClassicCom>,IUnknown>
		{
			XMVECTOR *_pRms;
			XMVECTOR *_pPeak;
			XMVECTOR *_pData;
			long _position;
			long _duration;
		public:
			XMVECTOR *GetRMS() { return _pRms; }
			XMVECTOR *GetPeak() { return _pPeak; }
			XMVECTOR *GetSpectrum() { return _pData; }
			long GetPosition() const { return  _position; }
			long GetDuration() const { return _duration; }

			AnalyzerFrame(size_t channels, size_t spectrumBins,long position,long duration) :
				_pData(nullptr),
				_pRms(nullptr),
				_pPeak(nullptr),
				_position(position),
				_duration(duration)
			{
				size_t vScalarElements = (channels + 3) >> 2;
				size_t vSpectrumElements = (spectrumBins + 3) >> 2;
				// Allocate one buffer as |--- spectrum --- | rms | peak |
				size_t vMallocSize = vSpectrumElements * channels + 2 * vScalarElements;

				_pData = static_cast<DirectX::XMVECTOR *>(_aligned_malloc(vMallocSize*sizeof(DirectX::XMVECTOR),16));
				_pRms = _pData + vSpectrumElements * channels;
				_pPeak = _pRms + vScalarElements;
			}

			~AnalyzerFrame()
			{
				if (_pData != nullptr)
					_aligned_free(_pData);
				_pData = nullptr;
				_pRms = nullptr;
				_pPeak = nullptr;
			}
		};*/

		class CAudioAnalyzer
		{
			UINT32 _inputChannels;
			UINT32 _fftLength;
			UINT32 _fftLengthLog2;

			UINT32 _stepFrames;
			UINT32 _overlapFrames;

			DirectX::XMVECTOR *_pWindow;
			DirectX::XMVECTOR *_pFftReal;
			DirectX::XMVECTOR *_pFftUnityTable;
			DirectX::XMVECTOR *_pFftBuffers;
			float _fFftScale;	// 2/N scale factor for fft output
			std::shared_ptr<CAudioBuffer> _spInputBuffer;
			std::mutex _inputBufferAccess;

			void AllocateBuffers();
			void FreeBuffers();

		public:
			CAudioAnalyzer(size_t inputBufferSize);
			~CAudioAnalyzer();

			void ConfigureInput(UINT32 inputChannels)
			{
				_inputChannels = inputChannels;
				_spInputBuffer->SetFrameSize(_inputChannels);
			}
			// Configures spectral analyzer with specified FFT Length and analyzer step lengths
			void ConfigureAnalyzer(UINT32 fftLength, UINT32 stepFrames, UINT32 overlapFrames);
			// Returns true if output data is available
			bool IsOutputAvailable();

			// Adds data to the input buffer. NB size is in samples!
			void AddInput(float *pData, size_t sampleCount, long frameIndex = -1);

			// Fetches next set of data from buffer and analyzes
			bool Step(long *pPosition, XMVECTOR *pRms, DirectX::XMVECTOR *pPeak, DirectX::XMVECTOR *pSpectrum);

			// Flushes all data from the input buffer
			void Flush();

			long GetPosition() const { return _spInputBuffer->GetPosition(); }
		};
	}
}

