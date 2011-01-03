#include "Utility.h"

namespace util
{
	namespace conversion_string
	{
		//---------------------------------------------------------------------------------------------
		// MBCS -> UNICODE
		bool mb2wc( wchar_t * buffer, const char * in, size_t bufferLength, eCodePage codepage )
		{
			assert(buffer);
			assert(in);
			assert(bufferLength>0);

			const uint32_t FLAG = ( codepage== utf_8) ? MB_ERR_INVALID_CHARS : MB_PRECOMPOSED;
			const int32_t NEED_LEN = ::MultiByteToWideChar(codepage, FLAG, in, -1, NULL, 0);

			assert(0 != NEED_LEN);
			assert(NEED_LEN <= (int32_t)bufferLength);
			if(0 == NEED_LEN || NEED_LEN > (int32_t)bufferLength)
				return false;

			wmemset(buffer, 0, NEED_LEN);

			int32_t num = ::MultiByteToWideChar(codepage, FLAG, in, -1, buffer, (int32_t)bufferLength);
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
			assert(strlen(ms) < 256 && "_alloca 를 사용하므로 너무 큰 놈은 지양하자");
			return mb2wc(std::string(ms), codepage);
		}

		//---------------------------------------------------------------------------------------------
		// UNICODE -> MBCS
		bool wc2mb( char * buffer, const wchar_t * in, size_t bufferLength, eCodePage codepage )
		{
			assert(buffer);
			assert(in);
			assert(bufferLength>0);

			const int32_t NEED_LEN = WideCharToMultiByte(codepage, 0, in, -1, 0, 0, NULL, NULL);

			assert(0 != NEED_LEN);
			assert((int32_t)bufferLength >= NEED_LEN);
			if(0 == NEED_LEN || NEED_LEN > (int32_t)bufferLength)
				return false;

			memset(buffer, 0, NEED_LEN);

			int32_t num = WideCharToMultiByte(codepage, 0, in, -1, buffer, (int32_t)bufferLength, NULL, NULL);
			assert(num == NEED_LEN && "WideCharToMultiByte failed");
			return (NEED_LEN == num);
		}

		std::string wc2mb( std::wstring const & ws, eCodePage codepage )
		{
			uint32_t const length = (uint32_t)(ws.length()*2);

			char * buff = (char*)_alloca( sizeof(char) * (length + 1) );
			if(wc2mb(buff, ws.c_str(), length+1, codepage))
				return std::string(buff);

			return std::string("wc2mb failed!");
		}

		std::string wc2mb( wchar_t const * ws, eCodePage codepage )
		{
			assert(ws);
			assert(wcslen(ws) < 128 && "_alloca 를 사용하므로 너무 큰 놈은 지양하자");
			return wc2mb(std::wstring(ws), codepage);
		}
	}
}