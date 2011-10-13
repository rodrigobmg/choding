#ifndef _SUN_RESOURCE_STREAM_H_
#define _SUN_RESOURCE_STREAM_H_



class DLL_API sunStream : public sunRef
{
public:
	typedef enum _eLoadingState
	{
		NONE = 0,
		QUEUED,		// ť�� �����
		LOADING,	// �ε� ��
		LOADED,		// �ε� �Ϸ�
		FAILED		// �ε� ����
	} eLodingState;

public:
	sunStream();
	virtual ~sunStream();


public:
	void				SetState( eLodingState eState);
	eLodingState		GetState();


	void				SetName( const tstring& strFileName );
	const tstring&   GetName();

	void				SetFileName( const tstring& strFileName );
	const tstring&   GetFileName();

	void				SetFullFileName( const tstring& strFileName );
	const tstring&   GetFullFileName();


public:
	virtual	bool Load(const tstring& strFileName) = 0;

protected:

	eLodingState			m_eState;					//���� �ڿ��� ���� ����.

	tstring					m_strName;					// ���ҽ� �̸�
	tstring					m_strFileName;				// ���ҽ� ���ϸ�
	tstring					m_strFullPathFileName;		// ���ҽ� ���ϸ��� ������ ������



};

#endif