#pragma once
namespace AudioVisualizer
{
	class ring_buffer
	{
		float *_pData;
		int _readIndex;
		int _writeIndex;
		size_t _size;
		size_t _stepLength;
		size_t _stepOverlap;
		size_t _frameSize;
		size_t _downsampleFactor;
		size_t _downsampleCounter;
	public:

		ring_buffer(size_t buffer_size) :
			_pData(nullptr),
			_size(buffer_size),
			_readIndex(0),
			_writeIndex(0),
			_stepLength(0),
			_stepOverlap(0),
			_downsampleFactor(1),
			_downsampleCounter(0),
			_frameSize(0)
		{
			_pData = (float *)malloc(buffer_size * sizeof(float));
			clear();
		}

		~ring_buffer()
		{
			if (_pData != nullptr)
				free(_pData);
			_pData = nullptr;
		}

		size_t size() const { return _size; }

		// Buffer configuration
		size_t stepLength() const { return _stepLength; }
		size_t stepOverlap() const { return _stepOverlap; }
		size_t frameSize() const { return _frameSize; }
		void frameSize(size_t frameSize) { _frameSize = frameSize; }
		size_t downsampleFactor() const { return _downsampleFactor; }

		HRESULT configure(size_t stepLength, size_t overlap, size_t downsampleFactor) // Set output and overlap length in frames

		{
			if (overlap >= stepLength)
				return E_INVALIDARG;

			_stepLength = stepLength;
			_stepOverlap = overlap;
			_downsampleFactor = downsampleFactor;
			_downsampleCounter = 0;

			return S_OK;
		}

		bool empty()
		{
			return length() < (_stepLength - _stepOverlap);
		}

		size_t length() const // Returns available data length in buffer in frames
		{
			size_t samplesAvailable = _readIndex <= _writeIndex ? (size_t)(_writeIndex - _readIndex) : _size + _writeIndex - _readIndex;
			return samplesAvailable / _frameSize;
		}

		void flush()	// Sets buffer length to 0
		{
			_readIndex = _writeIndex = 0;
		}

		void clear()	// Clears buffers used and unused space
		{
			if (_pData != nullptr && _size > 0)
				memset(_pData, 0, _size * sizeof(float));
		}

		size_t add(const float *pFrames, size_t sampleCount);
		size_t get(float *pDest, size_t outputBufferLength = 0, const float *pWindow = 0);
	};
}