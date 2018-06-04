#pragma once

namespace util
{
	template<typename EnumType>
	inline bool enum_has_flag(const EnumType& value, const EnumType & flag)
	{
		return (value & flag) != static_cast<EnumType>(0);
	}
}
