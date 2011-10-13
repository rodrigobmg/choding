#ifndef _SNOWBOARD_TYPE_H_
#define _SNOWBOARD_TYPE_H_

using namespace std;
typedef std::basic_string<TCHAR> tstring;

typedef char				int8;
typedef short				int16;
typedef int					int32;
typedef __int64				int64;

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned __int64	uint64;

struct GDSVERTEX
{
	enum _FVF { FVF=(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1) };
	D3DXVECTOR3		p;
	D3DXVECTOR3		n;
	D3DXVECTOR2		t;
};

struct GDSINDEX
{
	WORD _0 , _1, _2;
	GDSINDEX(){};
	GDSINDEX( WORD __0 , WORD __1 , WORD __2 ):_0(__0),_1(__1),_2(__2)
	{}
};

#if defined(DEBUG) || defined(_DEBUG)
class IgnoreCheck{};
template <class T> class rt_chk
{
	const	T		val_;
	mutable bool	check_;
public:
	rt_chk(T const & val) : val_(val), check_(false){}
	rt_chk(rt_chk const & rhs) : val_(rhs.val_), check_(rhs.check_){rhs.check_=true;}
	~rt_chk(){ASSERT(check_ && "check the return value!");}
	operator T() const {check_=true; return val_;}
	operator IgnoreCheck() const {check_=true; return IgnoreCheck();}
private:
	const rt_chk& operator=( const rt_chk& );
};
typedef rt_chk<uint64>	uint64_chk;
typedef rt_chk<int64>		int64_chk;
typedef rt_chk<uint32>	uint32_chk;
typedef rt_chk<int32>		int32_chk;
typedef rt_chk<uint16>	uint16_chk;
typedef rt_chk<int16>		int16_chk;
typedef rt_chk<uint8>		uint8_chk;
typedef rt_chk<int8>		int8_chk;
typedef rt_chk<bool>		bool_chk;
#else
typedef uint64			uint64_chk;
typedef int64				int64_chk;
typedef uint32			uint32_chk;
typedef int32				int32_chk;
typedef uint16			uint16_chk;
typedef int16				int16_chk;
typedef uint8				uint8_chk;
typedef int8				int8_chk;
typedef bool				bool_chk;
#	define IgnoreCheck(exp)		exp
#endif //_DEBUG


// gameobject
#define GAMEOBJECT_DUMMY			-1

// component
#define COMPONENT_VISUAL_FAMILY		1
#define COMPONENT_VISUAL_RECT		100
#define COMPONENT_VISUAL_MESH		101

#define COMPONENT_ANIMATION_FAMILY	2
#define COMPONENT_ANIMATION_MOVEABLE 200

#endif