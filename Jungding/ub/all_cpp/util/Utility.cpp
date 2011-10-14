#include "stdafx.h"
#include "Utility.h"

namespace util
{
	namespace string
	{
		//---------------------------------------------------------------------------------------------
		// MBCS -> UNICODE
		bool mb2wc( wchar_t * buffer, const char * in, size_t bufferLength, eCodePage codepage )
		{
			assert(buffer);
			assert(in);
			assert(bufferLength>0);

			const unsigned int FLAG = ( codepage== utf_8) ? MB_ERR_INVALID_CHARS : MB_PRECOMPOSED;
			const int NEED_LEN = ::MultiByteToWideChar(codepage, FLAG, in, -1, NULL, 0);

			assert(0 != NEED_LEN);
			assert(NEED_LEN <= (int)bufferLength);
			if(0 == NEED_LEN || NEED_LEN > (int)bufferLength)
				return false;

			wmemset(buffer, 0, NEED_LEN);

			int num = ::MultiByteToWideChar(codepage, FLAG, in, -1, buffer, (int)bufferLength);
			assert(num == NEED_LEN && "MultiByteToWideChar failed");
			return (num == NEED_LEN);
		}

		std::wstring mb2wc( std::string const & ms, eCodePage codepage )
		{

			wchar_t * buff = (wchar_t*)_alloca( sizeof(wchar_t) * (ms.length() + 1) );

			if(mb2wc(buff, ms.c_str(), ms.length() + 1, codepage))
				return std::wstring(buff);

			return std::wstring(L"mb2wc failed");
		}

		std::wstring mb2wc( char const * ms, eCodePage codepage )
		{
			assert(ms);
			assert(strlen(ms) < 256 && "_alloca �� ����ϹǷ� �ʹ� ū ���� ��������");
			return mb2wc(std::string(ms), codepage);
		}

		//---------------------------------------------------------------------------------------------
		// UNICODE -> MBCS
		bool wc2mb( char * buffer, const wchar_t * in, size_t bufferLength, eCodePage codepage )
		{
			assert(buffer);
			assert(in);
			assert(bufferLength>0);

			const int NEED_LEN = WideCharToMultiByte(codepage, 0, in, -1, 0, 0, NULL, NULL);

			assert(0 != NEED_LEN);
			assert((int)bufferLength >= NEED_LEN);
			if(0 == NEED_LEN || NEED_LEN > (int)bufferLength)
				return false;

			memset(buffer, 0, NEED_LEN);

			int num = WideCharToMultiByte(codepage, 0, in, -1, buffer, (int)bufferLength, NULL, NULL);
			assert(num == NEED_LEN && "WideCharToMultiByte failed");
			return (NEED_LEN == num);
		}

		std::string wc2mb( std::wstring const & ws, eCodePage codepage )
		{
			unsigned int const length = (unsigned int)(ws.length()*2);

			char * buff = (char*)_alloca( sizeof(char) * (length + 1) );
			if(wc2mb(buff, ws.c_str(), length+1, codepage))
				return std::string(buff);

			return std::string("wc2mb failed!");
		}

		std::string wc2mb( wchar_t const * ws, eCodePage codepage )
		{
			assert(ws);
			assert(wcslen(ws) < 128 && "_alloca �� ����ϹǷ� �ʹ� ū ���� ��������");
			return wc2mb(std::wstring(ws), codepage);
		}

		std::wstring make_string(wchar_t const * fmt, ...)
		{
			const int BUFF_SIZE(2048);
			wchar_t buff[BUFF_SIZE];

			int writen = 0;
			va_list	ap;
			va_start(ap, fmt);

			writen	= vswprintf_s(buff, fmt, ap);

			va_end(ap);

			if(writen < BUFF_SIZE)
				return std::wstring(buff);

			return std::wstring(L"writen >! BUFF_SIZE, increase BUFF_SIZE");
		};

		std::string make_string(const char * fmt, ...)
		{
			const int BUFF_SIZE(2048);
			char buff[BUFF_SIZE];

			int writen = 0;
			va_list	ap;
			va_start(ap, fmt);

			writen	= vsprintf_s(buff, fmt, ap);

			va_end(ap);

			if(writen < BUFF_SIZE)
				return std::string(buff);

			return std::string("writen >! BUFF_SIZE, increase BUFF_SIZE");
		}	

		std::wstring GetLastErrorString( wchar_t const * file/*=L"where"*/, unsigned int line/*=0xffff*/ )
		{
			DWORD errcode = ::GetLastError();

			unsigned int const BUFF_SIZE(2048);
			wchar_t buff[BUFF_SIZE];

			DWORD ret = ::FormatMessage(
				FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE
				, NULL
				, errcode
				, 0U
				, buff
				, BUFF_SIZE
				, NULL);
			if(ret != 0)
			{
				return make_string(L"errcode : 0x%08x(PROCESS), %s @ %s(%d)\n", errcode, buff, file, line);
			}

			ret = ::FormatMessage(
				FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM
				, NULL
				, errcode
				, 0U
				, buff
				, BUFF_SIZE
				, NULL);
			if(ret != 0)
			{
				return make_string(L"errcode : 0x%08x(SYSTEM), %s @ %s(%d)\n", errcode, buff, file, line);
			}

			return make_string(L"errcode : 0x%08x, error string is not found! @ %s(%d)\n", errcode, file, line);
		}

		bool str2bool (TCHAR const * s) 
		{
			wchar_t *end;

			if ((_tcscmp(s, L"true") == 0) ||
				(_tcscmp(s, L"yes") == 0) ||
				(_tcscmp(s, L"on") == 0) ||
				(_tcstol(s, &end, 10) == 1)) {
					return true;
			}
			else {
				return false;
			}
		}
	} // end of string

}// end of util