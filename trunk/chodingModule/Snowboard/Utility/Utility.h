
#pragma once

#include "../Framework/Snowboard_stdafx.h"

namespace util
{
	namespace conversion_string
	{
		enum eCodePage{
			Japan = 932,		// �Ϻ���
			China = 936,		// �߱���(��ü)
			Korea = 949,		// �ѱ���
			Taiwan = 950,		// �߱���(��ü)
			English = 1252,		// ��ƾ1, �Ϲ����� ���� ����
			utf_8	= 65001,

			DefaultCodePage = Korea,	// �⺻
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

