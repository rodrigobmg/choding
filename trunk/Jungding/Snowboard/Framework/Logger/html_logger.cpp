#include "stdafx.h"

namespace util
{

	HTML_Logger::HTML_Logger(wchar_t const * fname)
		: fp_(NULL)
		, buffSize_(BUFF_SIZE_STEP)
		, buff_(NULL)
	{
		if (fname) {
			vInit(fname);
		}
	}

	HTML_Logger::~HTML_Logger()
	{
		vFlush();
		vClose();
	}

	void HTML_Logger::LogMsg( wchar_t const * msg, uint32 len/*=0xffffffff*/ )
	{
		if (0xffffffff == len)
			len = static_cast<uint32>( wcslen(msg) );

		vWrite (msg, len);
		vWrite(L"<br>\n", 5);
	}

	void HTML_Logger::LogMsg( char const * msg, uint32 len/*=0xffffffff*/ )
	{
		if (0xffffffff == len)
			len = static_cast<uint32>( strlen(msg) );

		if (vCnvtMB2W(msg, len)) {
			vWrite (buff_, len);
			vWrite(L"<br>\n", 5);
		}
	}

	void HTML_Logger::LogString( wstring const & str )
	{
		vWrite(str.c_str(), str.length());
		vWrite(L"<br>\n", 5);
	}

	void HTML_Logger::LogEvent(wchar_t const * fmt, ...)
	{
		uint32 const BUFF_SIZE(2048);
		wchar_t buff[BUFF_SIZE];

		va_list args; 
		va_start(args, fmt); 
		int length = _vsnwprintf_s(buff, BUFF_SIZE, _TRUNCATE, fmt, args);
		va_end(args);
		ASSERT(length > 0);
		ASSERT(BUFF_SIZE > length+5);

		if (length > 0)
		{
			wcscpy_s(buff+length, 6, L"<br>\n");

			vWrite(buff,length+5);
		}
	}

	void HTML_Logger::StartBlock(wchar_t const * block_name)
	{
		wchar_t buff[128];
		int writen = swprintf_s(buff, 128, L"<br><ul>\n<li>%s</li>\n", block_name);
		ASSERT(-1 != writen);

		if (-1 != writen) {
			vWrite(buff,writen);
		}
	}
	void HTML_Logger::EndBlock()
	{
		vWrite(L"</ul><br>\n",10);
	}

	void HTML_Logger::SetFont( wchar_t const * szFont, int32 textsize, uint32 color )
	{
		wchar_t buff[128];
		int writen = swprintf_s(buff, 128
			, L"<font AseFACE=\"%s\" size=\"%d\" color=\"#%06x\">\n"
			, szFont
			, textsize
			, color);
		ASSERT(-1 != writen);

		if (-1 != writen) {
			vWrite(buff,writen);
		}
	}
	void HTML_Logger::EndFont()
	{
		vWrite(L"</font>\n",8);
	}

	bool HTML_Logger::vWrite(wchar_t const * str, size_t len)
	{
		ASSERT(fp_);
		if (fp_) {
			size_t writen = ::fwrite(str, sizeof(wchar_t), len, fp_);

			ASSERT(writen == len);
			fflush(fp_);
			return (writen == len);
		}
		return false;
	}

	bool HTML_Logger::vInit( wchar_t const * fname )
	{
		vClose();

		ASSERT(NULL == buff_);
		buff_ = (wchar_t*)malloc(buffSize_ * sizeof(wchar_t));
		ASSERT(buff_);

		_wfopen_s(&fp_, fname, L"wtc,ccs=utf-8");
		//ASSERT(fp_ && "_wfopen_s failed!");

		if (fp_) {
			LPCTSTR str = _T("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
			vWrite(str, wcslen(str));
			SetFont();
			return true;
		}

		return false;
	}

	bool HTML_Logger::Init( wchar_t const * fname )
	{
		return vInit(fname);
	}

	void HTML_Logger::vClose()
	{
		if (fp_) {
			fclose(fp_);
			fp_ = NULL;
		}

		if (buff_) {
			free(buff_);
			buff_ = NULL;
		}
	}

	void HTML_Logger::vFlush()
	{
		if (fp_)
			fflush(fp_);
	}

	void HTML_Logger::vIncreaseBuff()
	{
		buffSize_ += BUFF_SIZE_STEP;
		buff_ = (wchar_t*)realloc(buff_, buffSize_);
	}

	bool HTML_Logger::vCnvtMB2W( char const * str, size_t len )
	{
		while(buffSize_ <= len)
			vIncreaseBuff();

		using util::string::mb2wc;
		return buff_ ? mb2wc(buff_, str, buffSize_/*, util::string::eLangCode::utf_8*/) : false;
	}
	namespace PRIVATE{
		HTML_Logger & GlobalLogger()
		{
			static HTML_Logger	html_logger(L"log.html");
			return html_logger;
		};		
	}
}//namespace util