#pragma once
#include <atomic>

struct MFEngine {
	static std::atomic_int32_t _engineReferenceCount;
	MFEngine();
	~MFEngine();
};