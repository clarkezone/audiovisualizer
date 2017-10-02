#include "pch.h"
#include "LifeSpanTracker.h"

#ifdef _DEBUG
	__declspec(selectany) LifespanInfo::State LifespanInfo::m_state;
#endif