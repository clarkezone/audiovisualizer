#pragma once
#include <DirectXMath.h>
#include "AudioBuffer.h"
#include <memory>

namespace AudioMath
{
	class CAudioAnalyzer
	{
		UINT32 _inputSampleRate;
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

		void AllocateBuffers();
		void FreeBuffers();
	public:
		CAudioAnalyzer(size_t inputBufferSize);
		~CAudioAnalyzer();

		void ConfigureInput(UINT32 inputSampleRate, UINT32 inputChannels)
		{
			_inputSampleRate = inputSampleRate;
			_inputChannels = inputChannels;
			_spInputBuffer->SetFrameSize(_inputChannels);
		}
		// Configures spectral analyzer with specified FFT Length and analyzer step lengths
		void ConfigureAnalyzer(UINT32 fftLength, UINT32 stepFrames, UINT32 overlapFrames);
		// Returns true if output data is available
		bool IsOutputAvailable();
		
		// Adds data to the input buffer
		void AddInput(float *pData, size_t frameCount, long frameIndex = -1);
		
		// Fetches next set of data from buffer and analyzes
		bool Step(long *pPosition,float *pRMS, float *pPeak, DirectX::XMVECTOR *pSpectrum);
		
		// Flushes all data from the input buffer
		void Flush();
	};
}

