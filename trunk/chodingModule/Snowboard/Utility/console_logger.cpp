
#include "console_logger.h"
#include <tchar.h>

namespace util
{
	namespace PRIVATE
	{
		static uint16_t const COLOR[eConsolColor::NUM]	= {  
			0
			, FOREGROUND_BLUE
			, FOREGROUND_GREEN
			, FOREGROUND_RED
			, FOREGROUND_BLUE | FOREGROUND_GREEN
			, FOREGROUND_RED | FOREGROUND_BLUE
			, FOREGROUND_GREEN | FOREGROUND_RED
			, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED


			, FOREGROUND_INTENSITY
			, FOREGROUND_BLUE | FOREGROUND_INTENSITY
			, FOREGROUND_GREEN | FOREGROUND_INTENSITY
			, FOREGROUND_RED | FOREGROUND_INTENSITY
			, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
			, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY
			, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
			, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
		};
	}//namespace PRIVATE

	ConsoleLogger::ConsoleLogger(wchar_t const * title)
		: handle_(NULL)
		, foreground_color_(eConsolColor::WHITE)
		, background_color_(eConsolColor::BLACK)
		, depth_(0U)
	{
		AllocConsole();
		handle_ = GetStdHandle( STD_OUTPUT_HANDLE);
		if(handle_)
		{
			_wsetlocale(LC_ALL, L"Korean" );

			::SetConsoleTitle(title);
		}
	}

	ConsoleLogger::~ConsoleLogger(void)
	{
		FreeConsole();
	}

	void ConsoleLogger::Clear()
	{
//		SCOPE_LOCK(lock_);
		vClear();
	}

	void ConsoleLogger::WriteString( std::wstring const & msg )
	{
//		SCOPE_LOCK(lock_);
		vWriteMsg(msg.c_str(), (uint32_t)msg.length());
	}

	void ConsoleLogger::WriteMsgFormatted( wchar_t const * fmt, ...)
	{
		uint32_t const BUFF_SIZE(2048);
		wchar_t buff[BUFF_SIZE];

		int writen = 0;
		va_list	ap;
		va_start(ap, fmt);

		writen	= _vstprintf_s(buff, BUFF_SIZE, fmt, ap);

		va_end(ap);

		if(writen < BUFF_SIZE)
		{
//			SCOPE_LOCK(lock_);
			vWriteMsg(buff, writen);
		}
		else
		{
			using namespace eConsolColor;

			const wchar_t * const ERR_MSG = L"increase local BUFF_SIZE!";
			const uint32_t ERR_MSG_LEN = wcsnlen_s(ERR_MSG, 128);

			const color_t RESTORE_FG_COLR = foreground_color_;
			const color_t RESTORE_BG_COLR = background_color_;

//			SCOPE_LOCK(lock_);
			vUpdateColr(HI_RED, BLACK);
			vWriteMsg(ERR_MSG, ERR_MSG_LEN);
			vUpdateColr(RESTORE_FG_COLR, RESTORE_BG_COLR);
		}
	}


	void ConsoleLogger::SetColor( color_t fg_color, color_t bg_color )
	{
//		SCOPE_LOCK(lock_);
		vUpdateColr(fg_color, bg_color);
	}

	void ConsoleLogger::vUpdateColr( color_t fg_color, color_t bg_color )
	{
		if( fg_color != foreground_color_ || bg_color != background_color_ )
		{
			using PRIVATE::COLOR;
			if( ::SetConsoleTextAttribute(handle_, COLOR[fg_color]|(COLOR[bg_color]<<4)) )
			{
				foreground_color_ = fg_color;
				background_color_ = bg_color;
			}
		}
	}

	void ConsoleLogger::vWriteMsg(wchar_t const * msg, uint32_t length)
	{
		vWriteDepth();
		DWORD writen = 0 ;
		if(0 == WriteConsole( handle_, (void*)msg, length, &writen, NULL ) )
		{ 
			DWORD errcode = ::GetLastError();
			errcode;
		}
		ASSERT(length == writen);
	}
	void ConsoleLogger::vWriteMsg(char const* msg, uint32_t length)
	{
		vWriteDepth();
		DWORD writen = 0 ;
		if(0 == WriteConsoleA( handle_, (void*)msg, length, &writen, NULL ) )
		{ 
			DWORD errcode = ::GetLastError();
			errcode;
		}
		ASSERT(length == writen);
	}

	void ConsoleLogger::vClear( )
	{
		if( ::GetConsoleScreenBufferInfo(handle_, &csbi_) )
		{
			DWORD writen = 0;
			DWORD const size = DWORD(csbi_.dwSize.X * csbi_.dwSize.Y);
			COORD const coord  = {0,0};

			FillConsoleOutputCharacter( handle_, L' ', size, coord, &writen);
			GetConsoleScreenBufferInfo(handle_, &csbi_);
			FillConsoleOutputAttribute(handle_, csbi_.wAttributes, size, coord, &writen);
			SetConsoleCursorPosition(handle_, coord);
		}
	}

	void ConsoleLogger::StartBlock(wchar_t const * block_name)
	{
		vWriteMsg(block_name, wcslen(block_name));
		vWriteMsg(L"\n", 1U);

		InterlockedIncrement(&depth_);
	}

	void ConsoleLogger::EndBlock()
	{
		InterlockedDecrement(&depth_);
	}

	void ConsoleLogger::vWriteDepth()
	{
		if(depth_)
		{
			uint32_t const LEN = (depth_<<2);
			void * str = _malloca(sizeof(wchar_t)*(LEN));
			wmemset ((wchar_t*)str, L' ', LEN);

			DWORD writen = 0 ;
	//		VERIFY(WriteConsole( handle_, str, LEN, &writen, NULL ));
			ASSERT(LEN == writen);

			_freea(str);
		}
	}

	void ConsoleLogger::SetTitle( wchar_t const * title )
	{
//		SCOPE_LOCK(lock_);
		vSetTitle(title);
	}

	void ConsoleLogger::vSetTitle( wchar_t const * title )
	{
		if(handle_)
			::SetConsoleTitle(title);
	}

	void ConsoleLogger::WriteMsg( wchar_t const * msg, uint32_t len/*=0xffffffff*/ )
	{
		if(0xffffffff == len)
			len = wcslen(msg);
		vWriteMsg(msg, len);
	}
	void ConsoleLogger::WriteMsg( char const * msg, uint32_t len/*=0xffffffff*/ )
	{
		if(0xffffffff == len)
			len = strlen(msg);
		vWriteMsg(msg, len);
	}

}
