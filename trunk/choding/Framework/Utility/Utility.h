
#pragma once

#include "choding_stdafx.h"

namespace util
{
	namespace conversion_string
	{
		enum eCodePage{
			Japan = 932,		// 일본어
			China = 936,		// 중국어(간체)
			Korea = 949,		// 한국어
			Taiwan = 950,		// 중국어(번체)
			English = 1252,		// 라틴1, 일반적인 영어 문자
			utf_8	= 65001,

			DefaultCodePage = Korea,	// 기본
		};
		
		//---------------------------------------------------------------------------------------------
		// MBCS -> UNICODE
		bool mb2wc( wchar_t * buffer, const char * in, size_t bufferLength, eCodePage codepage = DefaultCodePage );
		std::wstring mb2wc( std::string const & ms, eCodePage codepage = DefaultCodePage );
		std::wstring mb2wc( char const * ms, eCodePage codepage = DefaultCodePage );

		//---------------------------------------------------------------------------------------------
		// UNICODE -> MBCS
		bool wc2mb( char * buffer, const wchar_t * in, size_t bufferLength, eCodePage codepage = DefaultCodePage );
		std::string wc2mb( std::wstring const & ws, eCodePage codepage = DefaultCodePage );
		std::string wc2mb( wchar_t const * ws, eCodePage codepage = DefaultCodePage );

	}// end of namespace string
}// end of namespace util

