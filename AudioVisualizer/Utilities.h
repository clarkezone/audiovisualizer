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


// Helper for marking our callback delegates as agile, by mixing in FtmBase.
// Without this WinRT would marshal everything back to the UI thread.
template<typename T>
struct AddFtmBase
{
	typedef Microsoft::WRL::Implements<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, T, Microsoft::WRL::FtmBase> Type;
};

