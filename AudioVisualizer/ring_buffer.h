#pragma once
namespace AudioVisualizer
{
	struct ring_buffer
	{
		/*std::vector<float> _data;
		std::vector<float>::const_iterator _readPointer;
		std::vector<float>::iterator _writePointer;*/
		float *_data;

		size_t _stepFrames;
		size_t _overlapFrames;
		size_t _frameSize;
		size_t _downsampleRate;
		size_t _downsampleCounter;
		size_t _size;
		const float *_reader;
		float *_writer;

	public:
		int64_t readPositionFrameIndex;

		ring_buffer() : _stepFrames(0),
			_overlapFrames(0),
			_frameSize(0),
			_downsampleRate(0),
			_downsampleCounter(0),
			readPositionFrameIndex(0),
			_data(nullptr),
			_size(0),
			_reader(nullptr),
			_writer(nullptr)
		{
			
		}
		ring_buffer(size_t buffer_size,size_t frameSize,size_t stepFrames,size_t overlapFrames,size_t downsampleRate) :
			_stepFrames(stepFrames),
			_overlapFrames(overlapFrames),
			_frameSize(frameSize),
			_downsampleRate(downsampleRate),
			_downsampleCounter(0),
			readPositionFrameIndex(0),
			_data(nullptr),
			_size(0),
			_reader(nullptr),
			_writer(nullptr)

		{
			_size = frameSize * (buffer_size + 1);
			_data = (float*) malloc( _size * sizeof(float));
			flush();
		}

		bool empty()
		{
			return samples_in_buffer() < _stepFrames * _frameSize;
		}

		size_t samples_in_buffer() const // Returns available data length in buffer in frames
		{
			return _reader <= _writer ? 
				(size_t)(_writer - _reader) : 
				_size - (_reader - _writer);
		}

		void flush()	// Sets buffer length to 0
		{
			_reader = _data;
			_writer = _data;
			// To avoid long fills with big buffers only fill the overlap area preciding the read&write pointers
			size_t overlapSampleCount = _overlapFrames * _frameSize;
			memset(_data + (_size - overlapSampleCount), 0, overlapSampleCount * sizeof(float));
		}

		size_t downsampleRate() const { return _downsampleRate; }

		void add_samples(const float *pSamples, size_t sampleCount);
		void get_deinterleaved(float *pOutput, size_t outputBufferStride);
	};
}