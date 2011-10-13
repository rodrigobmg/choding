#ifndef _GDS_CAMERAMANAGERDX9_H_
#define _GDS_CAMERAMANAGERDX9_H_


class GdsCameraManagerDX9
{

private:

	typedef std::vector< GdsCameraPtr >		CAM_CONTAINER;	
	CAM_CONTAINER		m_camaraContainer;
	int32				m_iCurCamIndex;
	bool				m_bMouseDrag;

protected:

	virtual void		vClear();
	

public:

	GdsCameraManagerDX9(void);
	virtual ~GdsCameraManagerDX9(void);

	void				Init();

	GdsCameraPtr		GetCamNode( int iCamIndex );

	void				Attach( GdsCameraPtr camnode );
	void				Detach( int32 index );
	void				DetachAll();

	void				SetCurCam( int32 index ){ m_iCurCamIndex = index; }
	GdsCameraPtr		GetCurCam(){ return m_camaraContainer.at( m_iCurCamIndex); }

	void				MoveForwardLookat( float delta );
	void				MoveBackLookat( float delta );
	void				MoveLeftLookat( float delta );
	void				MoveRightLookat( float delta );
	void				MoveUpLookat( float delta );
	void				MoveDownLookat( float delta );

	void				MoveForwardEye( float delta );
	void				MoveBackEye( float delta );
	void				MoveLeftEye( float delta );
	void				MoveRightEye( float delta );
	void				MoveUpEye( float delta );
	void				MoveDownEye( float delta );

	void				Update( float fElapsedtime );

};

typedef boost::shared_ptr< GdsCameraManagerDX9 > GdsCameraManagerDX9Ptr;

#define CAMMGR	singleton_holder< GdsCameraManagerDX9 >::instance()

#endif
