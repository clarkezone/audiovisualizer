#pragma once
#include<mutex>
#include<unordered_map>
#include <typeindex>
#include <string>
#include <locale>
#include <codecvt>
#include <windows.foundation.diagnostics.h>
#include <wrl.h>
#include "trace.h"

#ifndef ENABLE_LIFESPAN_TRACKER 
	#ifdef DEBUG
		#define ENABLE_LIFESPAN_TRACKER
	#endif // DEBUG
#endif
#ifdef ENABLE_LIFESPAN_TRACKER

class LifespanInfo
{
	struct State
	{
		std::mutex mutex;
		std::unordered_map<std::type_index, size_t> objectCounts;
	};

	static State m_state;

	LifespanInfo() = delete;

public:
	// Records that a new object is being allocated.
	static void AddObject(const void*pObj,type_info const& type,ABI::Windows::Foundation::Diagnostics::ILoggingActivity **ppActivity)
	{
		std::lock_guard<std::mutex> lock(m_state.mutex);
		size_t objectCount = ++m_state.objectCounts[type];
		std::wstring_convert<std::codecvt<wchar_t, char, mbstate_t>> conv;
		auto typeName = conv.from_bytes(type.name());
		AudioVisualizer::Diagnostics::Trace::Log_ctor(typeName.c_str(), pObj, objectCount,ppActivity);
	}


	// Records that an object is being freed.
	static void RemoveObject(const void*pObj,type_info const& type, ABI::Windows::Foundation::Diagnostics::ILoggingActivity *pActivity)
	{
		std::lock_guard<std::mutex> lock(m_state.mutex);

		size_t objectCount = --m_state.objectCounts[type];
		std::wstring_convert<std::codecvt<wchar_t, char, mbstate_t>> conv;
		auto typeName = conv.from_bytes(type.name());
		AudioVisualizer::Diagnostics::Trace::Log_dtor(typeName.c_str(), pObj, objectCount, pActivity);
	}

	static void CloseObject(const void*pObj, type_info const& type, ABI::Windows::Foundation::Diagnostics::ILoggingActivity *pActivity)
	{
		std::lock_guard<std::mutex> lock(m_state.mutex);

		size_t objectCount = m_state.objectCounts[type];
		std::wstring_convert<std::codecvt<wchar_t, char, mbstate_t>> conv;
		auto typeName = conv.from_bytes(type.name());
		AudioVisualizer::Diagnostics::Trace::Log_CloseObject(typeName.c_str(), pObj, objectCount, pActivity);

	}
};

// Derive from this to make a type trackable.
template<typename T>
class LifespanTracker
{
	//friend class AudioVisualizer::Diagnostics::Trace;

	Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Diagnostics::ILoggingActivity> _traceLog_Activity;
protected:
	LifespanTracker()
	{
		LifespanInfo::AddObject(this,typeid(T),&_traceLog_Activity);
	}

	~LifespanTracker()
	{
		LifespanInfo::RemoveObject(this,typeid(T),_traceLog_Activity.Get());
	}

	void Lifespan_CloseObject()
	{
		LifespanInfo::CloseObject(this, typeid(T), _traceLog_Activity.Get());
	}
public:
	ABI::Windows::Foundation::Diagnostics::ILoggingActivity *GetLoggingActivity() { return _traceLog_Activity.Get(); }
};


#else
template<typename T>
struct LifespanTracker { };

#endif
