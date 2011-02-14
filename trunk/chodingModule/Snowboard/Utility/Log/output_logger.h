#pragma once

#if defined(_OUTPUT_ON_)

#	define TO_OUTPUT(msg)			::a2::ToOutput(msg)
#	define TO_OUTPUT_F(fmt, ...)	::a2::ToOutputF(fmt, __VA_ARGS__)

#else //_OUTPUT_ON_

#	define TO_OUTPUT(msg)
#	define TO_OUTPUT_F(fmt, ...)

#endif //_OUTPUT_ON_

#include "a2_type.h"
#include "common_macro.h"

namespace a2
{
	void ToOutput(wchar_t const* msg);
	void ToOutputF(wchar_t const* fmt, ...);
	void ToOutput(char const* msg);
	void ToOutputF(char const* fmt, ...);

}//namespace a2