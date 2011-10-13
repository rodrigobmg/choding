#include "Precompiled.h"
#include "sunHTMLLog.h"

#define BUILDING_STRING va_list list;va_start( list, strMessage );vswprintf_s( m_strLogBuffer, 2048, strMessage, list );va_end( list );

sunHTMLLog::sunHTMLLog()
: m_pMessenger(NULL)
{
	memset(m_strLogBuffer, 0, sizeof(m_strLogBuffer));
}


sunHTMLLog::~sunHTMLLog()
{
}

void sunHTMLLog::WriteLogDBG(const TCHAR *strMessage, ...)
{
	BUILDING_STRING;

	if(m_pMessenger)
		m_pMessenger->WriteLog( m_strLogBuffer);

}

void sunHTMLLog::WriteErrorLog(const TCHAR *strMessage, ...)
{
	BUILDING_STRING;

	if(m_pMessenger)
		m_pMessenger->WriteLog( m_strLogBuffer);

}
