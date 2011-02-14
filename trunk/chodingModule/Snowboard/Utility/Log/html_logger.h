#pragma once
#include "../Framework/Snowboard_stdafx.h"

namespace util
{
	namespace eTextColor
	{
		enum Enum
		{
			ELC_RED			= 0x00FF0000,
			ELC_DK_RED	    = 0x007F0000,
			ELC_GREEN		= 0x0000FF00,
			ELC_DK_GREEN    = 0x00007F00,
			ELC_BLUE		= 0x000000FF,
			ELC_DK_BLUE     = 0x0000007F,
			ELC_BLACK		= 0x00000000,

			ELC_CYAN		= 0x000000FF | 0x0000FF00,
			ELC_MAGENTA		= 0x00FF0000 | 0x000000FF,
			ELC_YELLOW		= 0x00FF0000 | 0x0000AF00,
		};
	}
	///< gpg4 1.2
	class HTML_Logger
	{
		typedef std::wstring		wstring;
		static const int BUFF_SIZE_STEP = 1024;

		FILE *		fp_;
		size_t		buffSize_;
		wchar_t*	buff_;
		//Lock		lock_;
	public:
		explicit HTML_Logger(wchar_t const * fname = NULL);
		~HTML_Logger();

		bool Init(wchar_t const * fname);

		void LogMsg(wchar_t const * msg, uint32_t len=0xffffffff);
		void LogMsg(char const * msg, uint32_t len=0xffffffff);
		void LogString(wstring const & str);
		void LogEvent(wchar_t const * fmt, ...);

		void StartBlock(wchar_t const * block_name=L"block");
		void EndBlock();

		void SetFont( wchar_t const * szFont=L"Courier"
			, int32_t textsize = 2
			, uint32_t color=0x00000000);
		void EndFont();

	private:
		bool vInit(wchar_t const * fname);
		void vClose();
		void vIncreaseBuff();
		bool vCnvtMB2W(char const * str, size_t len);
		void vFlush();
		bool vWrite(wchar_t const * str, size_t len);
	};

	namespace PRIVATE{HTML_Logger & GlobalLogger();};
	struct LogBlock 
	{
		explicit LogBlock(wchar_t const * block_name=L"block")
		{
			PRIVATE::GlobalLogger().StartBlock(block_name);
		};
		~LogBlock()
		{
			PRIVATE::GlobalLogger().EndBlock();
		};
	};
}//namespace util
