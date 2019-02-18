#include "pch.h"
#include "BinaryScaleManager.h"


uint64_t double_as_uint64(double &value) {
	return *(uint64_t *)&value;
}
double uint64_as_double(uint64_t &value) {
	return *(double *)&value;
}

namespace winrt::AudioVisualizer::implementation
{
	BinaryScaleManager::BinaryScaleManager() {
		_exponentBias = 1023;
		_step = 1;
	}

	BinaryScaleManager::BinaryScaleManager(uint32_t offset, uint32_t step)
	{
		if (offset >= step || step == 0) {
			throw hresult_invalid_argument();
		}
		_exponentBias = offset + 1023;
		_step = step;
	}
    AudioVisualizer::ScaleRange BinaryScaleManager::GetRange(double value)
    {
		uint64_t bits = double_as_uint64(value);
		uint64_t sign = 0x8000000000000000 & bits;
		uint64_t exponent = ((bits >> 52) & 0x7ffL);
		if (_step != 1) {
			int normalizedExponent = (int)exponent - _exponentBias;
			int delta = abs(normalizedExponent) % _step;
			exponent = (int)exponent - (normalizedExponent >= 0 ? delta : _step - delta);
		}
		uint64_t from = sign | (exponent << 52);
		uint64_t to = sign | ((exponent + _step) << 52);
		return AudioVisualizer::ScaleRange
		{
			uint64_as_double(from),
			uint64_as_double(to),
		};
		
    }

}
