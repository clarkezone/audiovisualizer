#include "pch.h"
#include "AudioBuffer.h"
#include <mutex>

namespace AudioVisualizer
{
	namespace Math
	{
		CAudioBuffer::CAudioBuffer(size_t buffer_size) :
			_pData(nullptr),
			_size(buffer_size),
			_position(-1),	// -1 = not set
			_readIndex(0),
			_writeIndex(0),
			_stepLength(0),
			_stepOverlap(0),
			_downsampleFactor(1),
			_downsampleCounter(0),
			_frameSize(2)	// Default value is stereo
		{
			_pData = (float *)malloc(buffer_size * sizeof(float));
			Clear();
		}
		CAudioBuffer::~CAudioBuffer()
		{
			FreeBuffer();
		}

		void CAudioBuffer::FreeBuffer()
		{
			if (_pData != nullptr)
				free(_pData);
			_pData = nullptr;
		}

		HRESULT CAudioBuffer::Add(const float * pFrames, size_t sampleCount)
		{
			if (sampleCount > _downsampleFactor * (_size - _frameSize))
				return E_INVALIDARG;

			size_t currentLength = GetLength() * _frameSize;	// Record current samples in buffer

			// Copy sampleCount number of samples to the buffer, validate _writeIndex against _size 
			// To avoid copying when size is zero (uninitialized buffer)
			for (size_t index = 0; index < sampleCount; index++)
			{
				if (_downsampleFactor == 1 || _downsampleCounter <  _frameSize)
				{
					_pData[_writeIndex++] = pFrames[index];
					if (_writeIndex >= (int)_size)	// Wrap pointer over end
						_writeIndex = 0;
				}
				if (_downsampleFactor != 1)
				{
					_downsampleCounter++;
					if (_downsampleCounter >= _downsampleFactor * _frameSize)
						_downsampleCounter = 0;
				}
			}
			// Are we going to overflow?If overflow happened, 
			// advance read pointer so _size - _frameSize samples are available
			if (currentLength + (sampleCount / _downsampleFactor) >= _size)
			{
				_readIndex = _writeIndex + (int)_frameSize;
				if (_readIndex >= (int)_size)
				{
					_readIndex -= (int)_size;
				}
				// Calculate new position of readIndex
				if (_position != -1)
				{
					_position += (((long)currentLength + (long)(sampleCount)-(long)_size) / (long)_frameSize) + 1;
				}
			}
			return S_OK;
		}
		HRESULT CAudioBuffer::Step(float * pDest, size_t outputBufferLength, const float *pWindow)
		{
			if (_stepLength == 0)
				return E_NOT_VALID_STATE;

			size_t outputStride = outputBufferLength != 0 ? outputBufferLength : _stepLength;
			if (outputStride < _stepLength)
				return E_INVALIDARG;

			if (!IsDataAvailable()) // Not enough data to be copied
				return E_FAIL;

			int overlapSamples = (int)(_stepOverlap * _frameSize);
			// Calculate the index to start copying from, wrap the index over the start if needed
			int srcIndex = _readIndex >= overlapSamples ? _readIndex - overlapSamples : (int)_size - overlapSamples + _readIndex;

			// Iterate over all the output items
			for (size_t frameIndex = 0; frameIndex < outputStride; frameIndex++)
			{
				// Copy or pad output values
				for (size_t channelIndex = 0, outIndex = frameIndex; channelIndex < _frameSize; channelIndex++, outIndex += outputStride)
				{
					if (frameIndex < _stepLength)
					{
						if (pWindow != nullptr)
							pDest[outIndex] = _pData[srcIndex++] * pWindow[frameIndex];
						else
							pDest[outIndex] = _pData[srcIndex++];
						if (srcIndex >= (int)_size)
							srcIndex = 0;
					}
					else
					{
						pDest[outIndex] = 0.0f;
					}
				}
			}
			_readIndex = srcIndex;
			if (_position != -1)
			{
				_position += (long)_downsampleFactor * (_stepLength - _stepOverlap);
			}
			return S_OK;
		}
	}
}