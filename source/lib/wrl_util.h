#pragma once
#include <wrl.h>
#include <windows.foundation.h>
#include <windows.foundation.collections.h>
#include <windows.ui.xaml.h>

using namespace Microsoft::WRL;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::UI::Xaml;

namespace wrl_util
{
	template<typename T>
	class Nullable : public Microsoft::WRL::RuntimeClass<ABI::Windows::Foundation::IReference<T>>
	{
		InspectableClass(ABI::Windows::Foundation::IReference<T>::z_get_rc_name_impl(), BaseTrust);

	public:
		// T_abi and T are normally the same, but when T=bool, T_abi=boolean.
		typedef typename ABI::Windows::Foundation::Internal::GetAbiType<typename RuntimeClass::IReference::T_complex>::type T_abi;

	private:
		T_abi m_value;

		static_assert(std::is_pod<T>::value, "T must be plain-old-data");

	public:
		Nullable(T_abi const& value)
			: m_value(value)
		{}

		IFACEMETHODIMP get_Value(T_abi* value)
		{
			if (!value)
				return E_POINTER;

			*value = m_value;
			return S_OK;
		}
	};

	class DependencyPropertyChangedCallbackImpl : 
		public RuntimeClass<
		RuntimeClassFlags<RuntimeClassType::ClassicCom>,
		IDependencyPropertyChangedCallback>
	{
		ComPtr<ITypedEventHandler<IInspectable *, IInspectable *>> _handler;
	public:
		DependencyPropertyChangedCallbackImpl(ITypedEventHandler<IInspectable *, IInspectable *> *pHandler)
		{
			_handler = pHandler;
		}
		virtual STDMETHODIMP Invoke(
			IDependencyObject *sender,
			IDependencyProperty *dp)
		{
			return _handler->Invoke(sender, dp);
		}

	};

	template<class T> class IteratorImpl : public RuntimeClass<IIterator<T>>
	{
		unsigned _currentIndex = 0;
		unsigned _size;
		T* _pData;

		InspectableClass(IIterator<T>::z_get_rc_name_impl(), BaseTrust);
	public:
		IteratorImpl(T *pArray, unsigned size)
		{
			_size = size;
			_pData = pArray;
			_currentIndex = 0;
		}

		virtual /* propget */ HRESULT STDMETHODCALLTYPE get_Current(_Out_ T *current)
		{
			if (_currentIndex >= _size)
				return E_BOUNDS;
			*current = _pData[_currentIndex];
			return S_OK;
		};
		virtual /* propget */ HRESULT STDMETHODCALLTYPE get_HasCurrent(_Out_ boolean *hasCurrent)
		{
			if (hasCurrent == nullptr)
				return E_POINTER;
			*hasCurrent = _currentIndex < _size;
			return S_OK;
		}
		virtual HRESULT STDMETHODCALLTYPE MoveNext(_Out_ boolean *hasCurrent)
		{
			if (hasCurrent == nullptr)
				return E_POINTER;
			if (_currentIndex < _size)
			{
				_currentIndex++;
			}
			*hasCurrent = _currentIndex < _size;
			return S_OK;
		}
		virtual HRESULT STDMETHODCALLTYPE GetMany(_In_ unsigned capacity, _Out_writes_to_(capacity, *actual) T *value, _Out_ unsigned *actual)
		{
			return E_NOTIMPL;
		}
	};

}