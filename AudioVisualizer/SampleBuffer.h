#pragma once
#include <winrt/Windows.Media.h>
#include <MemoryBuffer.h>

namespace winrt::AudioVisualizer
{
	template <typename T = float> class SampleBuffer
	{
	private:
		Windows::Media::AudioBuffer _buffer { nullptr };
		winrt::Windows::Foundation::IMemoryBufferReference _bufferReference { nullptr };
		com_ptr<::Windows::Foundation::IMemoryBufferByteAccess> _memoryBuffer;
		uint32_t _length = 0;
		uint32_t _capacity = 0;

		T *_pData = nullptr;
	public:
		SampleBuffer(Windows::Media::AudioFrame const & frame, Windows::Media::AudioBufferAccessMode const &mode = winrt::Windows::Media::AudioBufferAccessMode::Read) {
			_buffer = frame.LockBuffer(mode);
			_length = _buffer.Length() / sizeof(T);
			_capacity = _buffer.Capacity() / sizeof(T);
			_bufferReference = _buffer.CreateReference();
			_bufferReference.as(_memoryBuffer);
			UINT32 capacity = 0;
			_memoryBuffer->GetBuffer((BYTE**)&_pData, &capacity);
		}
		~SampleBuffer()
		{
			// _memoryBuffer = nullptr;
			_bufferReference.Close();
			_buffer.Close();
		}
		uint32_t Length() const { return _length; }
		uint32_t Capacity() const { return _capacity; }

		T& operator[](int32_t index) 
		{
			return _pData[index];
		};
	};
}
