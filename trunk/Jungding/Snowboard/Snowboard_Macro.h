#ifndef _SNOWBOARD_MACRO_H_
#define _SNOWBOARD_MACRO_H_

#define SAFE_DELETE(v) {if(v){delete v; v = NULL;}}
#define SAFE_DELETE_ARRAY(v) { if(v) { delete[] v; v = NULL; } }
#define SAFE_RELEASE(v) {if(v){v->Release(); v = NULL;} }

#define ASSERT( exp )	assert( exp )

#define STRING2(x)	#x
#define STRING(x)	STRING2(x)

#define WSTRING2(x)	L#x
#define WSTRING(x)	WSTRING2(x)

#define FILE_LINEW __FILEW__ L"(" WSTRING(__LINE__) L"): "
#define FILE_LINEA __FILE__ "(" STRING(__LINE__) "): "

#define __SLASH(x)	/##x
#define __DOUBLE_SLASH	__SLASH(/)
//@{
// /* 으로 시작하는 주석부분 이므로 /**/ 으로 맺음을 해줘야 한다.
#define __BEGIN_COMMENT	/##*
//@}

#if defined(DEBUG) || defined(_DEBUG)

#	define D_LINE	
#	define R_LINE	__DOUBLE_SLASH

#	define D_BEGIN	__DOUBLE_SLASH
#	define R_BEGIN	__BEGIN_COMMENT

#else 

#	define D_LINE	__DOUBLE_SLASH
#	define R_LINE	

#	define D_BEGIN	__BEGIN_COMMENT
#	define R_BEGIN	__DOUBLE_SLASH

#endif	//_DEBUG

#if defined(_SHIPPING)

#	define DEV_LINE		__DOUBLE_SLASH
#	define SVC_LINE	

#	define DEV_BEGIN	__BEGIN_COMMENT
#	define SVC_BEGIN	__DOUBLE_SLASH

#else

#	define DEV_LINE	
#	define SVC_LINE		__DOUBLE_SLASH

#	define DEV_BEGIN	__DOUBLE_SLASH
#	define SVC_BEGIN	__BEGIN_COMMENT

#endif //_SHIPPING

#define ADD_FLAGS( var, flags )		( (var) |=  (flags) )
#define REM_FLAGS( var, flags )		( (var) &= ~(flags) )
#define CHK_FLAGS( var, flags )		( (var) &   (flags) )

#define DeclearBoostpool \
public: \
	static	void*	operator new( size_t size )\
	{\
		void* p = bpool.malloc();\
		return p; \
	}  \
	static void		operator delete( void* p )\
	{\
		bpool.free(p);\
	}\
	static				boost::pool<>	bpool;\

#define ImplementBoostpool(class) \
	boost::pool<> class##::bpool(sizeof( class ) );

//메모리 릭 탐지
#define _CRTDBG_MAP_ALLOC

#endif