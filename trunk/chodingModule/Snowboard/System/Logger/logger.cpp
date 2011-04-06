#include "logger.h"

#include <tchar.h>
#pragma warning(push)
#pragma warning(disable: 4201)
#include <mmsystem.h>
#pragma warning(pop)
#include <Windows.h>
#include <memory>

namespace util
{
	Logger::Logger()
		: pConsole_(NULL)
		, pHTML_(NULL)
		, fptrWarningOnScreen_(NULL)
		, fptrErrorOnScreen_(NULL)
		, fptrWarningOnScreenA_(NULL)
		, fptrErrorOnScreenA_(NULL)
		, flag_(0)
	{
	}

	Logger::~Logger()
	{
		SAFE_DELETE(pHTML_);
		SAFE_DELETE(pConsole_);
	}

	void Logger::Init( output_fptr fptrWarningOnScreen
		, output_fptr fptrErrorOnString
		, output_fptrA fptrWarningOnScreenA
		, output_fptrA fptrErrorOnStringA  )
	{
		int const BSIZE = _MAX_FNAME;
		wchar_t buffer [BSIZE];
		struct _stat s;
		const wchar_t * OPTIONFILE = L".\\logger.ini";
		if (0 == _wstat(OPTIONFILE, &s))
		{
			uint32_t flag = 0;

			GetPrivateProfileString(L"Options", L"OutputDebugString", L"on"
				, buffer, BSIZE, OPTIONFILE);
			if(util::string::str2bool(buffer))
			{
				OutputDebugString(util::string::make_string(L"Logger : OutputDebugString = %s\n", buffer).c_str());
				ADD_FLAGS(flag, FLAG_OUTPUT);
			}

			GetPrivateProfileString(L"Options", L"Console", L"off"
				, buffer, BSIZE, OPTIONFILE);
			if(util::string::str2bool(buffer))
			{
				OutputDebugString(util::string::make_string(L"Logger : Console = %s\n", buffer).c_str());
				ADD_FLAGS(flag, FLAG_CONSOLE);
			}

			GetPrivateProfileString(L"Options", L"HTML", L"on"
				, buffer, BSIZE, OPTIONFILE);
			if(util::string::str2bool(buffer))
			{
				OutputDebugString(util::string::make_string(L"Logger : HTML = %s\n", buffer).c_str());
				ADD_FLAGS(flag, FLAG_HTML);
			}

			GetPrivateProfileString(L"Options", L"Screen", L"on"
				, buffer, BSIZE, OPTIONFILE);
			if(util::string::str2bool(buffer))
			{
				OutputDebugString(util::string::make_string(L"Logger : Screen = %s\n", buffer).c_str());
				ADD_FLAGS(flag, FLAG_SCREEN);
			}

			GetPrivateProfileString(L"Options", L"ReportFile", L".\\audition2_log.html"
				, buffer, BSIZE, OPTIONFILE);
			if (wcslen(buffer) == 0) {
				wcsncpy_s(buffer, BSIZE, L".\\choding_log.html", _TRUNCATE);
			}

			OutputDebugString(util::string::make_string(L"Logger : htlm_fname = %s\n", buffer).c_str());
			vInit(flag, buffer, fptrWarningOnScreen, fptrErrorOnString, fptrWarningOnScreenA, fptrErrorOnStringA);
		}
		else
		{
			wchar_t directory[MAX_PATH];
			::GetCurrentDirectory(MAX_PATH, directory);
			OutputDebugString(util::string::make_string(L"Logger : \'%s\' is not found, current directory is \'%s\'\n", OPTIONFILE, directory).c_str());
		}
	}

	void Logger::LogMsg(wchar_t const * str, uint32_t len)
	{
		if(0xffffffff == len)
			len = static_cast<uint32_t>( wcslen(str) );
		vLog (str, len);
	}
	void Logger::LogMsg(char const * str, uint32_t len)
	{
		if(0xffffffff == len)
			len = static_cast<uint32_t>( strlen(str) );
		vLog (str, len);
	}

	void Logger::LogF( wchar_t const * fmt, ... )
	{
		uint32_t const BUFF_SIZE(2048);
		wchar_t buff[BUFF_SIZE];

		va_list	ap;
		va_start(ap, fmt);

		int writen	= _vstprintf_s(buff, BUFF_SIZE, fmt, ap);

		va_end(ap);

		if(writen >= BUFF_SIZE-2)
		{
			buff[BUFF_SIZE-2] = L'\n';
			buff[BUFF_SIZE-1] = L'\0';
			writen = BUFF_SIZE-1;
		}
		else if(buff[writen-1] != L'\n')
		{
			buff[writen] = L'\n';
			buff[writen+1] = L'\0';
			writen += 1;
		}

		vLog (buff, writen);
	}
	void Logger::LogF( char const * fmt, ... )
	{
		uint32_t const BUFF_SIZE(2048);
		char buff[BUFF_SIZE];

		va_list	ap;
		va_start(ap, fmt);

		int writen	= vsprintf_s(buff, BUFF_SIZE, fmt, ap);

		va_end(ap);

		if(writen >= BUFF_SIZE-2)
		{
			buff[BUFF_SIZE-2] = '\n';
			buff[BUFF_SIZE-1] = '\0';
			writen = BUFF_SIZE-1;
		}
		else if(buff[writen-1] != '\n')
		{
			buff[writen] = '\n';
			buff[writen+1] = '\0';
			writen += 1;
		}

		vLog (buff, writen);
	}

	void Logger::LogString( wstring const & str )
	{
		if(!str.empty())
		{
			vLog(str.c_str(), static_cast<uint32_t>( str.length() ) );
			if(L'\n' != *str.rbegin())
				vLog(L"\n", 1);
		}
	}

	void Logger::StartBlock(wchar_t const * block_name)
	{
		vStartBlock(block_name);
	}

	void Logger::EndBlock()
	{
		vEndBlock();
	}

	void Logger::SetForLog() 
	{ 
		vSetForLog(); 
	}

	void Logger::SetForWarning(wchar_t const* posInSrc) 
	{ 
		vSetForWarning(posInSrc); 
	}

	void Logger::SetForError(wchar_t const* posInSrc) 
	{ 
		vSetForError(posInSrc); 
	}

	void Logger::vInit( uint32_t flag, wchar_t const * fname
		, output_fptr fptrWarningOnScreen, output_fptr fptrErrorOnString
		, output_fptrA fptrWarningOnScreenA, output_fptrA fptrErrorOnStringA )
	{
		flag_ = flag;

		if (CHK_FLAGS (flag_, FLAG_SCREEN))
		{
			fptrWarningOnScreen_ = fptrWarningOnScreen;
			fptrErrorOnScreen_ = fptrErrorOnString;
			fptrWarningOnScreenA_ = fptrWarningOnScreenA;
			fptrErrorOnScreenA_ = fptrErrorOnStringA;
		}

		if (CHK_FLAGS (flag_, FLAG_HTML)) {
			std::auto_ptr<HTML_Logger>	tmp(new HTML_Logger);
			if (fname && tmp->Init(fname)) {
				pHTML_ = tmp.release();
			} else {
				REM_FLAGS (flag_, FLAG_HTML);
			}
		}

		if (CHK_FLAGS (flag_, FLAG_CONSOLE)) {
			pConsole_ = new ConsoleLogger;
			if(fname)
				pConsole_->SetTitle(fname);
		}
	}

	void Logger::vLog( wchar_t const * str, uint32_t len )
	{
		if (CHK_FLAGS (flag_, FLAG_OUTPUT))
			OutputDebugString(str);

		if(pConsole_)
			pConsole_->WriteMsg (str, len);
		if(pHTML_)
			pHTML_->LogMsg (str, len);

		if (CHK_FLAGS(flag_, INNER_FLAG_WARNING) && fptrWarningOnScreen_)
			fptrWarningOnScreen_(str);

		if (CHK_FLAGS(flag_, INNER_FLAG_ERROR) && fptrErrorOnScreen_)
			fptrErrorOnScreen_(str);
	}
	void Logger::vLog( char const * str, uint32_t len )
	{
		if (CHK_FLAGS (flag_, FLAG_OUTPUT))
			OutputDebugStringA(str);

		if(pConsole_)
			pConsole_->WriteMsg (str, len);
		if(pHTML_)
			pHTML_->LogMsg (str, len);

		if (CHK_FLAGS(flag_, INNER_FLAG_WARNING) && fptrWarningOnScreenA_)
			fptrWarningOnScreenA_(str);

		if (CHK_FLAGS(flag_, INNER_FLAG_ERROR) && fptrErrorOnScreenA_)
			fptrErrorOnScreenA_(str);
	}

	void Logger::vStartBlock(wchar_t const * block_name)
	{ 
		if(pConsole_)
			pConsole_->StartBlock(block_name);
		if(pHTML_)
			pHTML_->StartBlock(block_name);
	}

	void Logger::vEndBlock()
	{
		if(pConsole_)
			pConsole_->EndBlock();
		if(pHTML_)
			pHTML_->EndBlock();
	}

	void Logger::vSetForLog()
	{
		REM_FLAGS(flag_, INNER_FLAG_WARNING | INNER_FLAG_ERROR);
		vColorBLACK();
	}

	void Logger::vSetForWarning(wchar_t const* posInSrc)
	{
		ADD_FLAGS(flag_, INNER_FLAG_WARNING);
		vColorYELLOW();

		wchar_t const SZ_WARNING[] = L"[[WARNING]] ";

		OutputDebugString(posInSrc);
		OutputDebugString(SZ_WARNING);

		if(pConsole_) {
			pConsole_->WriteMsg (SZ_WARNING, ARRAYSIZE(SZ_WARNING));
		}
	}

	void Logger::vSetForError(wchar_t const* posInSrc)
	{
		ADD_FLAGS(flag_, INNER_FLAG_ERROR);
		vColorRED();

		wchar_t const SZ_ERROR[] = L"[[ERROR]] ";

		OutputDebugString(posInSrc);
		OutputDebugString(SZ_ERROR);

		if(pConsole_) {
			pConsole_->WriteMsg (SZ_ERROR, ARRAYSIZE(SZ_ERROR));
		}
	}

	void Logger::vColorBLACK()
	{
		if(pConsole_)
			pConsole_->SetColor();
		if(pHTML_)
			pHTML_->SetFont();
	}

	void Logger::vColorRED()
	{
		if(pConsole_)
			pConsole_->SetColor(eConsolColor::HI_RED);
		if(pHTML_)
			pHTML_->SetFont(L"Courier", 2, eTextColor::ELC_RED);
	}

	void Logger::vColorGREEN()
	{
		if(pConsole_)
			pConsole_->SetColor(eConsolColor::HI_GREEN);
		if(pHTML_)
			pHTML_->SetFont(L"Courier", 2, eTextColor::ELC_GREEN);
	}

	void Logger::vColorBLUE()
	{
		if(pConsole_)
			pConsole_->SetColor(eConsolColor::HI_BLUE);
		if(pHTML_)
			pHTML_->SetFont(L"Courier", 2, eTextColor::ELC_BLUE);
	}

	void Logger::vColorCYAN()
	{
		if(pConsole_)
			pConsole_->SetColor(eConsolColor::HI_CYAN);
		if(pHTML_)
			pHTML_->SetFont(L"Courier", 2, eTextColor::ELC_CYAN);
	}

	void Logger::vColorMAGENTA()
	{
		if(pConsole_)
			pConsole_->SetColor(eConsolColor::HI_MAGENTA);
		if(pHTML_)
			pHTML_->SetFont(L"Courier", 2, eTextColor::ELC_MAGENTA);
	}

	void Logger::vColorYELLOW()
	{
		if(pConsole_)
			pConsole_->SetColor(eConsolColor::HI_YELLOW);
		if(pHTML_)
			pHTML_->SetFont(L"Courier", 2, eTextColor::ELC_YELLOW);
	}
}//namespace util