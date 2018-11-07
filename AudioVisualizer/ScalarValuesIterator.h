#pragma once
namespace winrt::AudioVisualizer::implementation
{
	struct ScalarValuesIterator : implements<ScalarValuesIterator, Windows::Foundation::Collections::IIterator<float>>
	{
		const float *_pData;
		size_t _size;
		size_t _currentIndex;
		IInspectable _owner{ nullptr };

		ScalarValuesIterator(const float *pData, size_t size, IInspectable const & owner) {
			_pData = pData;
			_size = size;
			_currentIndex = 0;
			_owner = owner;
		}

		uint32_t GetMany(array_view<float>)
		{
			throw hresult_not_implemented();
		}

		bool MoveNext()
		{
			if (_currentIndex < _size)
			{
				_currentIndex++;
			}
			return  _currentIndex < _size;
		}

		bool HasCurrent()
		{
			return _currentIndex < _size;
		}

		float Current()
		{
			if (_currentIndex >= _size) {
				throw hresult_out_of_bounds();
			}
			return _pData[_currentIndex];
		}

	};
}