#include "pch.h"
#include "VectorData.h"
#include "ScalarValuesIterator.h"

namespace winrt::AudioVisualizer::implementation
{
	VectorData::VectorData(float * pData, uint32_t cElements, IInspectable const & owner)
	{
		_owner = owner;
		_pData = pData;
		_size = cElements;
	}
	Windows::Foundation::Collections::IIterator<float> VectorData::First()
    {
		return make<ScalarValuesIterator>(_pData, _size,_owner);
	}

    float VectorData::GetAt(uint32_t index)
    {
		if (index >= _size)
			throw hresult_out_of_bounds();
		return _pData[index];
	}

    uint32_t VectorData::Size()
    {
		return _size;
    }

    bool VectorData::IndexOf(float const&, uint32_t&)
    {
        throw hresult_not_implemented();
    }

    uint32_t VectorData::GetMany(uint32_t, array_view<float>)
    {
        throw hresult_not_implemented();
    }
}
