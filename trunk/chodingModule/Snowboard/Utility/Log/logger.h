#pragma once

#if defined(_LOGGER_ON_)

#define LOG_INIT(fptrWarning, fptrError)					\
	a2::Logger::getInstance().Init(fptrWarning, fptrError)

#define LOG_BLOCK(block_name)								\
	a2::Logger::Block MAKE_NAME(block)(block_name); MAKE_NAME(block)

#define LOG_INFO(str)			a2::Logger::getInstance().LogMsg(str)


#define LOG_ERROR(str)										\
	do														\
	{														\
		a2::Logger & g_logger = a2::Logger::getInstance();	\
		g_logger.SetForError(FILE_LINEW);					\
		g_logger.LogMsg(str);								\
		g_logger.LogString( ::a2::util::string::GetLastErrorString(__FILEW__, __LINE__) );	\
		g_logger.SetForLog();								\
	__pragma(warning(disable:4127))							\
	}while(0)												\
	__pragma (warning(default:4127))

#define LOG_WARNING(str)									\
	do														\
	{														\
		a2::Logger & g_logger = a2::Logger::getInstance();	\
		g_logger.SetForWarning(FILE_LINEW);					\
		g_logger.LogMsg(str);								\
		g_logger.LogString( ::a2::util::string::GetLastErrorString(__FILEW__, __LINE__) );	\
		g_logger.SetForLog();								\
	__pragma(warning(disable:4127))							\
	}while(0)												\
	__pragma (warning(default:4127))

#define LOG_INFO_F(fmt, ...)								\
	a2::Logger::getInstance().LogF(fmt, __VA_ARGS__)

#define LOG_ERROR_F(fmt, ...)								\
	do														\
	{														\
		a2::Logger & g_logger = a2::Logger::getInstance();	\
		g_logger.SetForError(FILE_LINEW);					\
		g_logger.LogF(fmt, __VA_ARGS__);					\
		g_logger.LogString( ::a2::util::string::GetLastErrorString(__FILEW__, __LINE__) );	\
		g_logger.SetForLog();								\
	__pragma(warning(disable:4127))							\
	}while(0)												\
	__pragma (warning(default:4127))

#define LOG_WARNING_F(fmt, ...)								\
	do														\
	{														\
		a2::Logger & g_logger = a2::Logger::getInstance();	\
		g_logger.SetForWarning(FILE_LINEW);					\
		g_logger.LogF(fmt, __VA_ARGS__);					\
		g_logger.LogString( ::a2::util::string::GetLastErrorString(__FILEW__, __LINE__) );	\
		g_logger.SetForLog();								\
	__pragma(warning(disable:4127))							\
	}while(0)												\
	__pragma (warning(default:4127))

#define LOG_COLOR(COLOR, str)								\
	do														\
	{														\
		a2::Logger & g_logger = a2::Logger::getInstance();	\
		g_logger.SetColorTo##COLOR();						\
		g_logger.LogMsg(str);								\
		g_logger.SetColorToBLACK();							\
	__pragma(warning(disable:4127))							\
	}while(0)												\
	__pragma (warning(default:4127))

#define LOG_RED(str)		LOG_COLOR(RED		, str)
#define LOG_GREEN(str)		LOG_COLOR(GREEN		, str)
#define LOG_BLUE(str)		LOG_COLOR(BLUE		, str)
#define LOG_CYAN(str)		LOG_COLOR(CYAN		, str)
#define LOG_MAGENTA(str)	LOG_COLOR(MAGENTA	, str)
#define LOG_YELLOW(str)		LOG_COLOR(YELLOW	, str)

#define LOG_COLOR_F(COLOR, fmt, ...)						\
	do														\
	{														\
		a2::Logger & g_logger = a2::Logger::getInstance();	\
		g_logger.SetColorTo##COLOR();						\
		g_logger.LogF(fmt, __VA_ARGS__);					\
		g_logger.SetColorToBLACK();							\
	__pragma(warning(disable:4127))							\
	}while(0)												\
	__pragma (warning(default:4127))

#define LOG_RED_F(fmt, ...)		LOG_COLOR_F(RED		, fmt , __VA_ARGS__)
#define LOG_GREEN_F(fmt, ...)	LOG_COLOR_F(GREEN	, fmt , __VA_ARGS__)
#define LOG_BLUE_F(fmt, ...)	LOG_COLOR_F(BLUE	, fmt , __VA_ARGS__)
#define LOG_CYAN_F(fmt, ...)	LOG_COLOR_F(CYAN	, fmt , __VA_ARGS__)
#define LOG_MAGENTA_F(fmt, ...)	LOG_COLOR_F(MAGENTA	, fmt , __VA_ARGS__)
#define LOG_YELLOW_F(fmt, ...)	LOG_COLOR_F(YELLOW	, fmt , __VA_ARGS__)

#else //defined(_LOGGER_ON_)

#define LOG_INIT(fptrWarning, fptrError)

#define LOG_BLOCK(block_name)

#define LOG_INFO(str)
#define LOG_ERROR(str)
#define LOG_WARNING(str)

#define LOG_INFO_F(fmt, ...)
#define LOG_ERROR_F(fmt, ...)
#define LOG_WARNING_F(fmt, ...)

#define LOG_RED(str)		
#define LOG_GREEN(str)		
#define LOG_BLUE(str)		
#define LOG_CYAN(str)		
#define LOG_MAGENTA(str)		
#define LOG_YELLOW(str)		

#define LOG_RED_F(fmt, ...)
#define LOG_GREEN_F(fmt, ...)
#define LOG_BLUE_F(fmt, ...)
#define LOG_CYAN_F(fmt, ...)
#define LOG_MAGENTA_F(fmt, ...)
#define LOG_YELLOW_F(fmt, ...)

#endif //defined(_LOGGER_ON_)

#include "../singleton.h"
#include "html_logger.h"
#include "console_logger.h"

namespace util
{
	class ConsoleLogger;
	class HTML_Logger;

	class Logger : public Singleton<Logger>
	{		
		USING_SINGLETON(Logger);

		typedef std::wstring wstring;

	public:

		struct Block {
			Block (wchar_t const * block_name) { 
				util::Logger::getInstance().StartBlock(block_name); 
			}
			~Block () {
				util::Logger::getInstance().EndBlock();
			}
		};

		typedef void (*output_fptr)(wchar_t const * str);
		typedef void (*output_fptrA)(char const * str);
		static const uint32_t	FLAG_OUTPUT		= 0x00000001;
		static const uint32_t	FLAG_HTML		= 0x00000002;
		static const uint32_t	FLAG_CONSOLE	= 0x00000004;
		static const uint32_t	FLAG_SCREEN		= 0x00000008;

		void Init(output_fptr fptrWarningOnScreen = NULL
			, output_fptr fptrErrorOnString = NULL
			, output_fptrA fptrWarningOnScreenA = NULL
			, output_fptrA fptrErrorOnStringA = NULL );

		void LogMsg(wchar_t const * str, uint32_t len=0xffffffff);
		void LogF(wchar_t const * fmt, ...);

		void LogMsg(char const * str, uint32_t len=0xffffffff);
		void LogF(char const * fmt, ...);

		void LogString(wstring const & str);
		void StartBlock(wchar_t const * block_name);
		void EndBlock();

		void SetForLog();
		void SetForWarning(wchar_t const* posInSrc);
		void SetForError(wchar_t const* posInSrc);

		void SetColorToBLACK	() { vColorBLACK	(); }
		void SetColorToRED		() { vColorRED		(); }
		void SetColorToGREEN	() { vColorGREEN	(); }
		void SetColorToBLUE		() { vColorBLUE		(); }
		void SetColorToCYAN		() { vColorCYAN		(); }
		void SetColorToMAGENTA	() { vColorMAGENTA	(); }
		void SetColorToYELLOW	() { vColorYELLOW	(); }

	private:

		static const uint32_t	INNER_FLAG_WARNING	= 0x00020000;
		static const uint32_t	INNER_FLAG_ERROR	= 0x00010000;

		ConsoleLogger *	pConsole_;
		HTML_Logger *	pHTML_;
		output_fptr		fptrWarningOnScreen_;
		output_fptr		fptrErrorOnScreen_;
		output_fptrA	fptrWarningOnScreenA_;
		output_fptrA	fptrErrorOnScreenA_;
		uint32_t		flag_;

	private:

		void vInit(uint32_t flag
			, wchar_t const * fname
			, output_fptr fptrWarningOnScreen
			, output_fptr fptrErrorOnString
			, output_fptrA fptrWarningOnScreenA
			, output_fptrA fptrErrorOnStringA );

		void vLog (wchar_t const * str, uint32_t len);
		void vLog( char const * str, uint32_t len );
		void vStartBlock(wchar_t const * block_name);
		void vEndBlock();

		void vSetForLog();
		void vSetForWarning(wchar_t const* posInSrc);
		void vSetForError(wchar_t const* posInSrc);

		void vColorBLACK	();
		void vColorRED		();
		void vColorGREEN	();
		void vColorBLUE		();
		void vColorCYAN		();
		void vColorMAGENTA	();
		void vColorYELLOW	();
		
	};
}//namespace a2
