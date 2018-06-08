#pragma once
namespace AudioVisualizer
{
	struct ring_buffer
	{
		std::vector<float> _data;
		std::vector<float>::const_iterator _readPointer;
		std::vector<float>::iterator _writePointer;
		size_t _stepFrames;
		size_t _overlapFrames;
		size_t _frameSize;
		size_t _downsampleRate;
		size_t _downsampleCounter;


	public:
		int64_t readPositionFrameIndex;

		ring_buffer() : _stepFrames(0),
			_overlapFrames(0),
			_frameSize(0),
			_downsampleRate(0),
			_downsampleCounter(0),
			readPositionFrameIndex(0)
		{
			
		}
		ring_buffer(size_t buffer_size,size_t frameSize,size_t stepFrames,size_t overlapFrames,size_t downsampleRate) :
			_stepFrames(stepFrames),
			_overlapFrames(overlapFrames),
			_frameSize(frameSize),
			_downsampleRate(downsampleRate),
			_downsampleCounter(0),
			readPositionFrameIndex(0)
		{
			_data.resize(buffer_size + frameSize);	// Allocate buffer + one frame		
			flush();
		}

		bool empty()
		{
			return samples_in_buffer() < _stepFrames * _frameSize;
		}

		size_t samples_in_buffer() const // Returns available data length in buffer in frames
		{
			return _readPointer <= _writePointer ? (size_t)(_writePointer - _readPointer) : _writePointer + _data.size() - _readPointer;
		}

		void flush()	// Sets buffer length to 0
		{
			_readPointer = _data.cbegin();
			_writePointer = _data.begin();
			// To avoid long fills with big buffers only fill the overlap area preciding the read&write pointers
			std::fill(_data.end() - (_overlapFrames * _frameSize), _data.end(),0);
		}

		size_t downsampleRate() const { return _downsampleRate; }

		void add_samples(const float *pSamples, size_t sampleCount);
		void get_deinterleaved(float *pOutput, size_t outputBufferStride);
	};
}