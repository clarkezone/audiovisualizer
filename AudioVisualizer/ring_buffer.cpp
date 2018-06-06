#include "pch.h"
#include "ring_buffer.h"

namespace AudioVisualizer
{
	size_t ring_buffer::add(const float * pFrames, size_t sampleCount)	// Add specified number of samples to the buffer
	{
		if (sampleCount > _downsampleFactor * (_size - _frameSize))
			throw winrt::hresult_invalid_argument();

		size_t currentLength = length() * _frameSize;	// Record current samples in buffer

														// Copy sampleCount number of samples to the buffer, validate _writeIndex against _size 
														// To avoid copying when size is zero (uninitialized buffer)
		for (size_t index = 0; index < sampleCount; index++)
		{
			if (_downsampleFactor == 1 || _downsampleCounter < _frameSize)
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
				return ((currentLength + (sampleCount)-_size) / _frameSize) + 1;
		}
		return 0;
	}
	size_t ring_buffer::get(float * pDest, size_t outputBufferLength, const float * pWindow)
	{
		if (_stepLength == 0)
			return 0;

		size_t outputStride = outputBufferLength != 0 ? outputBufferLength : _stepLength;
		if (outputStride < _stepLength)
			return 0;

		if (empty()) // Not enough data to be copied
			return 0;

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

		return (long)_downsampleFactor * (_stepLength - _stepOverlap);
	}
}