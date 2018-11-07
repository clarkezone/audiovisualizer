#pragma once

#include "VectorData.g.h"

namespace winrt::AudioVisualizer::implementation
{
    struct VectorData : VectorDataT<VectorData>
    {
		uint32_t _size;
		float *_pData;
		IInspectable _owner{ nullptr };

        VectorData() = delete;
		VectorData(float *pData, uint32_t cElements, IInspectable const &owner);

        Windows::Foundation::Collections::IIterator<float> First();
        float GetAt(uint32_t index);
        uint32_t Size();
        bool IndexOf(float const& value, uint32_t& index);
        uint32_t GetMany(uint32_t startIndex, array_view<float> items);
    };
}
