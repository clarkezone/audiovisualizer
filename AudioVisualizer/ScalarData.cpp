#include "pch.h"
#include "ScalarData.h"
#include <memory>
#include <winrt/Windows.Foundation.Collections.h>


template <typename T, std::size_t Align = 16>
class aligned_allocator {
public:
	typedef T value_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	typedef T * pointer;
	typedef const T * const_pointer;

	typedef T & reference;
	typedef const T & const_reference;

public:
	inline aligned_allocator() throw () { }

	template <typename OtherType>
	inline aligned_allocator(const aligned_allocator<OtherType, Align> &) throw () { }

	inline ~aligned_allocator() throw () { }

	inline pointer adress(reference r) {
		return &r;
	}

	inline const_pointer adress(const_reference r) const {
		return &r;
	}

	inline pointer allocate(size_type n) {
		return (pointer)_aligned_malloc(n * sizeof(value_type), Align);
	}

	inline void deallocate(pointer p, size_type) {
		_aligned_free(p);
	}

	inline void construct(pointer p, const value_type & value) {
		new (p) value_type(value);
	}

	inline void destroy(pointer p) {
		p->~value_type();
	}

	inline size_type max_size() const throw () {
		return size_type(-1) / sizeof(value_type);
	}

	template <typename OtherType>
	struct rebind {
		typedef aligned_allocator<OtherType, Align> other;
	};

	bool operator!=(const aligned_allocator<T, Align>& other) const {
		return !(*this == other);
	}

	bool operator==(const aligned_allocator<T, Align>& other) const {
		return true;
	}
};


namespace winrt::AudioVisualizer::implementation
{
    AudioVisualizer::ScaleType ScalarData::AmplitudeScale()
    {
        return _amplitudeScale;
    }

    AudioVisualizer::ScalarData ScalarData::ConvertToDecibels(float minValue, float maxValue)
    {
        throw hresult_not_implemented();
    }

    AudioVisualizer::ScalarData ScalarData::ApplyRiseAndFall(AudioVisualizer::ScalarData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::Collections::IIterator<float> ScalarData::First()
    {
		
		return single_threaded_vector(std::move(_data)).GetView().First();
    }

    float ScalarData::GetAt(uint32_t index)
    {
		if (index >= _size)
			throw hresult_out_of_bounds();
		return _data[index];
	}

    bool ScalarData::IndexOf(float const& value, uint32_t& index)
    {
        throw hresult_not_implemented();
    }

    uint32_t ScalarData::GetMany(uint32_t startIndex, array_view<float> items)
    {
        throw hresult_not_implemented();
    }

    AudioVisualizer::ScalarData ScalarData::CreateEmpty(uint32_t channels)
    {
		return make<ScalarData>(channels, ScaleType::Linear, true);
    }

    AudioVisualizer::ScalarData ScalarData::Create(array_view<float const> values)
    {
        throw hresult_not_implemented();
    }

    AudioVisualizer::ScalarData ScalarData::ApplyRiseAndFallToEmpty(AudioVisualizer::ScalarData const& previous, Windows::Foundation::TimeSpan const& riseTime, Windows::Foundation::TimeSpan const& fallTime, Windows::Foundation::TimeSpan const& timeFromPrevious)
    {
        throw hresult_not_implemented();
    }

	ScalarData::ScalarData(size_t cElements, ScaleType scaleType, bool bInit)
	{
		_amplitudeScale = scaleType;
		_data.resize(cElements);

		_pData = nullptr;

		size_t vLength = (cElements + 3) >> 2;	// Vector size of the allocated buffer
		_pData = reinterpret_cast<DirectX::XMVECTOR *>(_aligned_malloc(vLength * sizeof(DirectX::XMVECTOR), 16));
		if (bInit)
		{
			memset(_pData, 0, vLength * sizeof(DirectX::XMVECTOR));
		}
		_size = cElements;

		auto data = std::vector<float, aligned_allocator<float>>(16);
		auto view = single_threaded_vector(std::move(data)).GetView();
	}

	ScalarData::~ScalarData()
	{
		_aligned_free(_pData);
	}


}
