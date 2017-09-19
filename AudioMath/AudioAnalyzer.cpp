#include "pch.h"
#include "AudioAnalyzer.h"
#include <DirectXMath.h>
#include "XDSP.h"
#include <exception>
#include <algorithm>

using namespace DirectX;
using namespace XDSP;

namespace AudioMath
{
	CAudioAnalyzer::CAudioAnalyzer(size_t inputBufferSize) :
		_inputChannels(2),
		_fftLength(0),
		_stepFrames(0),
		_overlapFrames(0),
		_pWindow(nullptr),
		_pFftReal(nullptr),
		_pFftUnityTable(nullptr),
		_pFftBuffers(nullptr),
		_bCalculateRMS(true),
		_bCalculatePeak(true),
		_bCalculateSpectrum(true)
	{
		_spInputBuffer = std::make_shared<CAudioBuffer>(inputBufferSize);
		_spInputBuffer->SetFrameSize(_inputChannels);
	}


	CAudioAnalyzer::~CAudioAnalyzer()
	{
		FreeBuffers();
	}

	void CAudioAnalyzer::AllocateBuffers()
	{
		FreeBuffers();	// First free any existing memory allocations

		_pWindow = static_cast<XMVECTOR *>(_aligned_malloc_dbg(((_stepFrames+3)>>2) * sizeof(XMVECTOR),16, __FILE__, __LINE__));
		_pFftReal = static_cast<XMVECTOR *>(_aligned_malloc_dbg(_fftLength * sizeof(XMVECTOR) * _inputChannels, 16,__FILE__, __LINE__));	// For real data allocate space for all channels
		_pFftUnityTable = static_cast<XMVECTOR *> (_aligned_malloc_dbg(2 * _fftLength * sizeof(XMVECTOR), 16, __FILE__, __LINE__));	// Complex values 
		_pFftBuffers = static_cast<XMVECTOR *>(_aligned_malloc_dbg(2 * _fftLength * sizeof(XMVECTOR), 16, __FILE__, __LINE__));	// Preallocate buffers for FFT calculation 2*length of Fft

		if (_pFftReal == nullptr || _pWindow == nullptr || _pFftUnityTable == nullptr || _pFftBuffers == nullptr)
			throw std::exception("Out of memory", E_OUTOFMEMORY);
	}

	void CAudioAnalyzer::FreeBuffers()
	{
		if (_pFftReal != nullptr)
			_aligned_free(_pFftReal);
		_pFftReal = nullptr;
		if (_pFftUnityTable != nullptr)
			_aligned_free(_pFftUnityTable);
		_pFftUnityTable = nullptr;
		if (_pFftBuffers != nullptr)
			_aligned_free(_pFftBuffers);
		_pFftBuffers = nullptr;
		if (_pWindow != nullptr)
			_aligned_free(_pWindow);
		_pWindow = nullptr;
	}
	void CAudioAnalyzer::ConfigureAnalyzer(UINT32 fftLength, UINT32 stepFrames, UINT32 overlapFrames)
	{
		using namespace DirectX;
		_fftLength = fftLength;
		_fFftScale = 2.0f / _fftLength;
		_stepFrames = stepFrames;
		_overlapFrames = overlapFrames;
		AllocateBuffers();

		_fftLengthLog2 = 1;
		while (1U << _fftLengthLog2 != _fftLength)
			_fftLengthLog2++;

		// Initialize window, use Blackman-Nuttall window for low sidelobes
		float a0 = 0.3635819f, a1 = 0.4891775f, a2 = 0.1365995f, a3 = 0.0106411f;
		XMVECTOR vElementIndex = XMVectorSet(0, 1, 2, 3),vElementStep = XMVectorReplicate(4.0f);
		float fIndexScaler = 1.0f /(_stepFrames - 1.0f);
		size_t vWindowElements = _stepFrames >> 2;
		for (size_t vIndex = 0; vIndex < vWindowElements; vIndex++,vElementIndex+=vElementStep)
		{
			XMVECTOR vCosArg = XMVectorScale(DirectX::g_XMTwoPi * vElementIndex, fIndexScaler);
			_pWindow[vIndex] = 
				XMVectorReplicate(a0) -
				XMVectorScale(XMVectorCos(vCosArg), a1) +
				XMVectorScale(XMVectorCos(XMVectorScale(vCosArg, 2)), a2) -
				XMVectorScale(XMVectorCos(XMVectorScale(vCosArg, 3)), a3);
		}
		FFTInitializeUnityTable(_pFftUnityTable, _fftLength);
		_spInputBuffer->Configure(_stepFrames, _overlapFrames);
	}

	bool CAudioAnalyzer::IsOutputAvailable()
	{
		return _spInputBuffer->IsDataAvailable();
	}

	void CAudioAnalyzer::AddInput(float * pData, size_t frameCount, long frameIndex)
	{
		std::lock_guard<std::mutex> inputLock(_inputBufferAccess);
		if (frameIndex != -1 && _spInputBuffer->GetPosition() == -1)
			_spInputBuffer->SetPosition(frameIndex);
		_spInputBuffer->Add(pData, frameCount);
	}

	bool CAudioAnalyzer::Step(AnalyzerFrame **ppFrame)
	{
		if (ppFrame == nullptr)
			return FALSE;

		long position = -1;
		{	/* Code block for scoped lock during copying the data from the input buffer */	
			std::lock_guard<std::mutex> inputLock(_inputBufferAccess);
			if (!IsOutputAvailable())
				return false;

			position = _spInputBuffer->GetPosition();
			_spInputBuffer->Step((float*)_pFftReal, _fftLength);
		}

		ComPtr<AnalyzerFrame> spDataFrame = Make<AnalyzerFrame>(_inputChannels, _fftLength >> 1, position,_stepFrames - _overlapFrames);

		size_t vStep = _fftLength >> 2;

		if (_bCalculateRMS || _bCalculateRMS)
		{
			size_t vFromFrame = _overlapFrames >> 2, vToFrame = (_stepFrames - _overlapFrames) >> 2;
			float rmsScaler = 1.0f / ((vToFrame - vFromFrame) << 2);
			
				for (size_t channelIndex = 0,vOffset = 0; channelIndex < _inputChannels; channelIndex++,vOffset+=vStep)
			{
				XMVECTOR vRMSSum = XMVectorZero();
				XMVECTOR vPeak = XMVectorZero();
				for (size_t vIndex = vFromFrame + vOffset; vIndex < vToFrame + vOffset; vIndex++)
				{
					XMVECTOR vValue = _pFftReal[vIndex];
					vRMSSum +=  vValue * vValue;
					vPeak = XMVectorMax(vPeak, XMVectorAbs(vValue));
				}
				if (_bCalculateRMS)
				{
					XMVECTOR vRMS = XMVectorSqrt(XMVectorScale(XMVectorSum(vRMSSum), rmsScaler));
					((float *)spDataFrame->GetRMS())[channelIndex] = vRMS.m128_f32[0];
				}
				if (_bCalculatePeak)
				{
					float peakValue = std::max(std::max(std::max(vPeak.m128_f32[0], vPeak.m128_f32[1]), vPeak.m128_f32[2]), vPeak.m128_f32[3]);
					((float*)spDataFrame->GetPeak())[channelIndex] = peakValue;
				}
			}
		}
		if (_bCalculateSpectrum)
		{
			// First window the data
			for (size_t vIndex = 0; vIndex < (_stepFrames >> 2); vIndex++)
			{
				for (size_t channelIndex = 0,vElementIndex = vIndex; channelIndex < _inputChannels; channelIndex++, vElementIndex+=vStep)
				{
					_pFftReal[vElementIndex] *= _pWindow[vIndex];
				}
			}
			for (size_t channelIndex = 0; channelIndex < _inputChannels; channelIndex++)
			{
				XMVECTOR *pData = _pFftReal + channelIndex * vStep;
				XMVECTOR *pOutput = spDataFrame->GetSpectrum() + channelIndex * (vStep >> 1);

				XMVECTOR *pImag = _pFftBuffers;
				XMVECTOR *pRealUnswizzled = _pFftBuffers + vStep;

				memset(pImag, 0, sizeof(float)*_fftLength);	// Imaginary values are 0 for input

				XDSP::FFT(pData, pImag, _pFftUnityTable, _fftLength);
				XDSP::FFTUnswizzle(pRealUnswizzled, pData, _fftLengthLog2);
				XDSP::FFTUnswizzle(pData, pImag, _fftLengthLog2); // Use input data for temporary buffer for reordered imaginary data

				// Calculate abs value first half of FFT output and copy to output
				for (size_t vIndex = 0; vIndex < vStep>>1; vIndex++)	// vector length is 4 times shorter, copy only positive frequency values
				{
					XMVECTOR vRR = XMVectorMultiply(pRealUnswizzled[vIndex], pRealUnswizzled[vIndex]);
					XMVECTOR vII = XMVectorMultiply(pData[vIndex], pData[vIndex]);	// pData is used as buffer for reordered imaginary values
					XMVECTOR vRRplusvII = XMVectorAdd(vRR, vII);
					XMVECTOR vAbs = XMVectorSqrtEst(vRRplusvII);
					XMVECTOR vScaledAbs = XMVectorScale(vAbs, _fFftScale);
					pOutput[vIndex] = vScaledAbs;
				}
			}
		}
		*ppFrame = spDataFrame.Detach();
		return true;
	}

	void CAudioAnalyzer::Flush()
	{
		_spInputBuffer->Flush();
	}

}

