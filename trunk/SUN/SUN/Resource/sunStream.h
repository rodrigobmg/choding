#ifndef _SUN_RESOURCE_STREAM_H_
#define _SUN_RESOURCE_STREAM_H_



class DLL_API sunStream : public sunRef
{
public:
	typedef enum _eLoadingState
	{
		NONE = 0,
		QUEUED,		// 큐에 적재됨
		LOADING,	// 로딩 중
		LOADED,		// 로딩 완료
		FAILED		// 로딩 실패
	} eLodingState;

public:
	sunStream();
	virtual ~sunStream();


public:
	inline void				SetState( eLodingState eState);
	inline eLodingState		GetState();


	inline void				SetName( const tstring& strFileName );
	inline const tstring&   GetName();

	inline void				SetFileName( const tstring& strFileName );
	inline const tstring&   GetFileName();

	inline void				SetFullFileName( const tstring& strFileName );
	inline const tstring&   GetFullFileName();


public:
	virtual	bool Load() = 0;

protected:

	eLodingState			m_eState;					//현재 자원의 상태 정보.

	tstring					m_strName;					// 리소스 이름
	tstring					m_strFileName;				// 리소스 파일명
	tstring					m_strFullPathFileName;		// 리소스 파일명을 포함한 절대경로



};

#endif