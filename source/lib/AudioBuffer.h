#pragma once
namespace AudioVisualizer
{
	namespace Math
	{
		class CAudioBuffer
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

			long _position;

			void FreeBuffer();
		public:
			CAudioBuffer(size_t buffer_size);
			~CAudioBuffer();

			long GetPosition() const { return _position; }	// Position tracks the sample index of the read position from overall stream
			void SetPosition(const long position) { _position = position; }
			size_t GetSize() const { return _size; }

			// Buffer configuration
			size_t GetStepLength() const { return _stepLength; }
			size_t GetStepOverlap() const { return _stepOverlap; }
			size_t GetFrameSize() const { return _frameSize; }
			void SetFrameSize(size_t frameSize) { _frameSize = frameSize; }
			size_t GetDownsampleFactor() const { return _downsampleFactor; }

			HRESULT Configure(size_t outputStepFrameCount, size_t overlap,size_t downsampleFactor) // Set output and overlap length in frames

			{
				if (overlap >= outputStepFrameCount)
					return E_INVALIDARG;

				_stepLength = outputStepFrameCount;
				_stepOverlap = overlap;
				_downsampleFactor = downsampleFactor;
				_downsampleCounter = 0;

				return S_OK;
			}

			bool IsDataAvailable()
			{
				return GetLength() >= (_stepLength - _stepOverlap);
			}

			size_t GetLength() const // Returns available data length in buffer in frames
			{
				size_t samplesAvailable = _readIndex <= _writeIndex ? (size_t)(_writeIndex - _readIndex) : _size + _writeIndex - _readIndex;
				return samplesAvailable / _frameSize;
			}

			void Flush()	// Sets buffer length to 0
			{
				_readIndex = _writeIndex = 0;
				_position = -1;
			}

			void Clear()	// Clears buffers used and unused space
			{
				if (_pData != nullptr && _size > 0)
					memset(_pData, 0, _size * sizeof(float));
			}

			HRESULT Add(const float *pFrames, size_t sampleCount);	// Add specified number of samples to the buffer
			HRESULT Step(float *pDest, size_t outputBufferLength = 0, const float *pWindow = 0);
		};
	}
}

