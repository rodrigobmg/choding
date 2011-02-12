#pragma once

#include "Singleton.h"
#include "../Framework/Snowboard_stdafx.h"
#include <wtypes.h>

namespace util
{
	namespace eConsolColor
	{
		enum Enum{
			//LOW
			BLACK		,
			BLUE		,
			GREEN		,
			RED			,
			CYAN		,	//Ã»·Ï
			MAGENTA		,	//ÀÚÈ«
			YELLOW		,
			WHITE		,


			//HI
			GRAY		,
			HI_BLUE		,
			HI_GREEN	,
			HI_RED		,
			HI_CYAN		,
			HI_MAGENTA	,
			HI_YELLOW	,
			HI_WHITE	,

			NUM			,
		};
	}
	class ConsoleLogger : public CSingleton<ConsoleLogger> 
	{
	//	friend class storage_t;
		typedef eConsolColor::Enum	color_t;

		HANDLE						handle_;
		CONSOLE_SCREEN_BUFFER_INFO	csbi_;
		color_t						foreground_color_;
		color_t						background_color_;
		long __declspec(align(4))	depth_;

		//Lock						lock_;

		ConsoleLogger(wchar_t const* title=L"dbg_consol");
	public:
		~ConsoleLogger(void);

		void Clear();

		void WriteMsg(wchar_t const* msg, uint32_t len=0xffffffff);
		void WriteMsg(char const* msg, uint32_t len=0xffffffff);
		void WriteString( std::wstring const & msg);
		void WriteMsgFormatted( wchar_t const* fmt, ...);

		void SetColor( eConsolColor::Enum foreground_color=eConsolColor::WHITE
			, eConsolColor::Enum background_color=eConsolColor::BLACK);

		void StartBlock(wchar_t const* block_name);
		void EndBlock();

		void SetTitle(wchar_t const* title);

	private:
		void vUpdateColr(color_t fg_color, color_t bg_color);
		void vWriteMsg(wchar_t const* msg, uint32_t length);
		void vWriteMsg(char const* msg, uint32_t length);
		void vWriteDepth();
		void vClear();
		void vSetTitle(wchar_t const* title);
	};
	struct ConsoleLogBlock
	{
		ConsoleLogBlock(LPCWSTR block_name)
		{
			::util::ConsoleLogger::getInstance().StartBlock(block_name);
		}
		~ConsoleLogBlock()
		{
			::util::ConsoleLogger::getInstance().EndBlock();
		}
	};
}//namespace a2


#if defined(_CONSOLE_ON_)

#define TOCON(msg)																\
	::a2::ConsoleLogger::getInstance().WriteString(msg)
#define TOCON_F(fmt, ...)														\
	::a2::ConsoleLogger::getInstance().WriteMsgFormatted(fmt, __VA_ARGS__)

#define TOCON_COLR(msg, color)													\
	::a2::ConsoleLogger::getInstance().SetColor(eConsolColor::HI_##color);		\
	TOCON(msg);																	\
	::a2::ConsoleLogger::getInstance().SetColor()
#define TOCON_COLR_F(color, fmt, ...)											\
	::a2::ConsoleLogger::getInstance().SetColor(eConsolColor::HI_##color);		\
	::a2::ConsoleLogger::getInstance().WriteMsgFormatted(fmt, __VA_ARGS__);		\
	::a2::ConsoleLogger::getInstance().SetColor()

#define TOCON_BLUE_F(fmt, ...)			TOCON_COLR_F(BLUE, fmt, __VA_ARGS__)
#define TOCON_GREEN_F(fmt, ...)			TOCON_COLR_F(GREEN, fmt, __VA_ARGS__)
#define TOCON_RED_F(fmt, ...)			TOCON_COLR_F(RED, fmt, __VA_ARGS__)
#define TOCON_CYAN_F(fmt, ...)			TOCON_COLR_F(CYAN, fmt, __VA_ARGS__)
#define TOCON_MAGENTA_F(fmt, ...)		TOCON_COLR_F(MAGENTA, fmt, __VA_ARGS__)
#define TOCON_YELLOW_F(fmt, ...)		TOCON_COLR_F(YELLOW, fmt, __VA_ARGS__)

#define TOCON_BLUE(msg)					TOCON_COLR(msg, BLUE)
#define TOCON_GREEN(msg)				TOCON_COLR(msg, GREEN)
#define TOCON_RED(msg)					TOCON_COLR(msg, RED)
#define TOCON_CYAN(msg)					TOCON_COLR(msg, CYAN)
#define TOCON_MAGENTA(msg)				TOCON_COLR(msg, MAGENTA)
#define TOCON_YELLOW(msg)				TOCON_COLR(msg, YELLOW)

#define LOG_TO_CONSOLE(msg)				TOCON(msg)
#define ERROR_TO_CONSOLE(msg)			TOCON_COLR(msg, RED)
#define WARN_TO_CONSOLE(msg)			TOCON_COLR(msg, YELLOW)

#else //defined(_CONSOLE_ON_)


#define TOCON(msg)
#define TOCON_F(fmt, ...)

#define TOCON_COLR(msg, color)
#define TOCON_COLR_F(color, fmt, ...)

#define TOCON_BLUE_F(fmt, ...)		
#define TOCON_GREEN_F(fmt, ...)		
#define TOCON_RED_F(fmt, ...)		
#define TOCON_CYAN_F(fmt, ...)		
#define TOCON_MAGENTA_F(fmt, ...)	
#define TOCON_YELLOW_F(fmt, ...)	

#define TOCON_BLUE(msg)				
#define TOCON_GREEN(msg)			
#define TOCON_RED(msg)				
#define TOCON_CYAN(msg)				
#define TOCON_MAGENTA(msg)			
#define TOCON_YELLOW(msg)			

#define LOG_TO_CONSOLE(msg)			
#define ERROR_TO_CONSOLE(msg)		
#define WARN_TO_CONSOLE(msg)		

#endif //defined(_CONSOLE_ON_)