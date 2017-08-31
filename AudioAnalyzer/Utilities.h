#pragma once
#include <wrl.h>

// Shortcut QueryInterface
template<typename T, typename U>
inline Microsoft::WRL::ComPtr<T> As(Microsoft::WRL::ComPtr<U> const& u)
{
	static_assert(!std::is_same<T, U>::value, "types should differ");

	Microsoft::WRL::ComPtr<T> t;
	ThrowIfFailed(u.As(&t));
	return t;
}

template<typename T, typename U>
inline Microsoft::WRL::ComPtr<T> As(U* u)
{
	static_assert(!std::is_same<T, U>::value, "types should differ");

	Microsoft::WRL::ComPtr<T> t;
	ThrowIfFailed(u->QueryInterface(IID_PPV_ARGS(t.ReleaseAndGetAddressOf())));
	return t;
}

// Get statics
template<typename T>
inline Microsoft::WRL::ComPtr<T> GetStatics(HSTRING runtimeClassName)
{
	ComPtr<IActivationFactory> spFactory;
	ThrowIfFailed(GetActivationFactory(runtimeClassName, &spFactory));

	ComPtr<T> spStatics;
	ThrowIfFailed(spFactory.As(&spStatics));

	return spStatics;
}
