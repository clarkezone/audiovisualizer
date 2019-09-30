#include "pch.h"
#include "mfengine.h"

std::atomic_int32_t MFEngine::_engineReferenceCount{ 0 };

MFEngine::MFEngine()
{
	if (_engineReferenceCount.fetch_add(1) <= 0) {
		winrt::check_hresult(MFStartup(MF_VERSION));
	}
}
MFEngine::~MFEngine()
{
	if (_engineReferenceCount.fetch_sub(1) <= 0) {
		MFShutdown();
	}
}