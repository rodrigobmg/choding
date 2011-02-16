#include "output_logger.h"
#include "../../Framework/Snowboard_type.h"
#include <Windows.h>
#include <stdio.h>

namespace util
{
	static uint32_t const BUFF_SIZE(2048);
	void ToOutput(wchar_t const* msg)
	{
		::OutputDebugStringW(msg);
	}
	void ToOutputF(wchar_t const* fmt, ...)
	{
		wchar_t buff[BUFF_SIZE];

		int writen = 0;
		va_list	ap;
		va_start(ap, fmt);

		writen	= vswprintf_s(buff, BUFF_SIZE, fmt, ap);

		va_end(ap);

		if (writen >= BUFF_SIZE-2) {
			buff[BUFF_SIZE -2] = L'\n';
			buff[BUFF_SIZE -1] = L'\0';
		}
		::OutputDebugStringW(buff);
	}

	void ToOutput(char const* msg)
	{
		::OutputDebugStringA(msg);
	}
	void ToOutputF(char const* fmt, ...)
	{
		char buff[BUFF_SIZE];

		int writen = 0;
		va_list	ap;
		va_start(ap, fmt);

		writen	= vsprintf_s(buff, BUFF_SIZE, fmt, ap);

		va_end(ap);

		if (writen >= BUFF_SIZE-2) {
			buff[BUFF_SIZE -2] = '\n';
			buff[BUFF_SIZE -1] = '\0';
		}
		::OutputDebugStringA(buff);
	}
}//namespace util