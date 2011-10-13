#ifndef _SUN_COREMANAGER_CORE_HTMLLOG_H_
#define _SUN_COREMANAGER_CORE_HTMLLOG_H_




class DLL_API sunHTMLLog
{
public:
	sunHTMLLog();
	~sunHTMLLog();


public:
	//void SetWinformLog( )

	void WriteErrorLog( const TCHAR* strMessage, ...);
	void WriteLogDBG( const TCHAR* strMessage, ...);

protected:
	TCHAR				m_strLogBuffer[2048];
	
	MessengerToCSharp*  m_pMessenger;

	

};

#endif