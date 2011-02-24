#ifndef _GDS_H_MACRO_
#define _GDS_H_MACRO_

#define SAFE_DELETE(v) {if(v){delete v; v = NULL;}}
#define SAFE_DELETE_ARRAY(v) { if(v) { delete[] v; v = NULL; } }
#define SAFE_RELEASE(v) {if(v){v->Release(); } }

#define ASSERT( exp )	assert( exp )

#define STRING2(x)	#x
#define STRING(x)	STRING2(x)

#define WSTRING2(x)	L#x
#define WSTRING(x)	WSTRING2(x)

#define FILE_LINEW __FILEW__ L"(" WSTRING(__LINE__) L"): "
#define FILE_LINEA __FILE__ "(" STRING(__LINE__) "): "

#define __SLASH(x)	/##x
#define __DOUBLE_SLASH	__SLASH(/)

#if defined(DEBUG) || defined(_DEBUG)
#	define D_LINE	
#	define R_LINE	__DOUBLE_SLASH
#else 
#	define D_LINE	__DOUBLE_SLASH
#	define R_LINE	
#endif	//_DEBUG

#if defined(_SHIPPING)
#	define DEV_LINE	__DOUBLE_SLASH
#	define SVC_LINE	
#else
#	define DEV_LINE	
#	define SVC_LINE	__DOUBLE_SLASH
#endif //_SHIPPING

#define ADD_FLAGS( var, flags )		( (var) |=  (flags) )
#define REM_FLAGS( var, flags )		( (var) &= ~(flags) )
#define CHK_FLAGS( var, flags )		( (var) &   (flags) )


// 부스트풀 메모리
#define ImplementBoostPool(class) boost::pool<> class::bpool(sizeof(class));
#define DeclareBoostPool \
	void* operator new(size_t s) \
{ \
	return bpool.malloc(); \
} \
	void operator delete(void *p) \
{ \
	bpool.free(p); \
} \
private: \
	static boost::pool<> bpool; \


typedef std::basic_string<TCHAR> tstring;

#endif