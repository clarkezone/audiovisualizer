#pragma once
namespace AudioMath
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

		HRESULT Configure(size_t outputStepFrameCount, size_t overlap) // Set output and overlap length in frames
		{
			if (overlap >= outputStepFrameCount)
				return E_INVALIDARG;

			_stepLength = outputStepFrameCount;
			_stepOverlap = overlap;
			return S_OK;
		}

		bool IsDataAvailable()
		{
			return GetLength() >= (_stepLength - _stepOverlap);
		}

		size_t GetLength()	// Returns available data length in buffer in frames
		{
			size_t samplesAvailable = _readIndex <= _writeIndex ? (size_t)(_writeIndex - _readIndex) : _size + _writeIndex - _readIndex + 1;
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

		/*
			Deinterleaves by FrameSize and copies samples to the pDest

			*Parameters*
			pDest				Pointer to the output buffer. Cannot be null
			outputBufferLength	Stride of the output buffer for channels. If 0 then StepLength is used
								If this is longer than StepLength, then data is 0 padded
								Cannot be shorter thant StepLength
			pWindow				If provided is expected to be the length of StepLength
								Each channel data will be windowed using this data (sample[index] *= window[index]
			*Return values*
			S_OK	Operation succeeded

			If input buffer contains data 1,2,3,4,5,6,7,8, read pointer is at 3 
											  ^ - readPointer
			then with FrameSize = 2, StepLength = 4, Overlap = 1
			calling Step(output,6,float [] = {0.1,1.0,10.0,100.0} will result output as
			{ 0.1,3,50.0,700.0,0,0,0.2,4.0,60.,800.0,0,0 }

		*/
		HRESULT Step(float *pDest,size_t outputBufferLength = 0,const float *pWindow = 0);	
	};
}

