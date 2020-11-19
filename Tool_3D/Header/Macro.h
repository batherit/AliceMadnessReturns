#ifndef Macro_h__
#define Macro_h__

namespace Client
{
#define  WINCX 800
#define  WINCY 600

#define VTXCNTX 129
#define VTXCNTZ 129
#define VTXITV  1

#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			END						}

#ifndef MSG_BOX
#define	MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#define NULL_CHECK( _ptr)	\
{if( _ptr == 0){__asm { int 3 };return;}}

#define NULL_CHECK_RETURN( _ptr, _return)	\
{if( _ptr == 0){__asm { int 3 };return _return;}}

#define NULL_CHECK_MSG( _ptr, _message )		\
{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };return _return;}}


#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return _return;}

#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBoxW(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return E_FAIL;}

#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBoxW(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return _return;}



#define NO_COPY(CLASSNAME)							\
	private:										\
	CLASSNAME(const CLASSNAME&);					\
	CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)				\
	NO_COPY(CLASSNAME)								\
	private:										\
	static CLASSNAME*	m_pInstance;				\
	public:											\
	static CLASSNAME*	GetInstance( void );		\
	static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
	CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
	CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
		if(NULL == m_pInstance) {					\
			m_pInstance = new CLASSNAME;			\
		}											\
		return m_pInstance;							\
	}												\
	void CLASSNAME::DestroyInstance( void ) {		\
		if(NULL != m_pInstance)	{					\
			delete m_pInstance;						\
			m_pInstance = NULL;						\
		}											\
	}

	// 오브젝트 유효성 체크
#define IS_VALID_OBJ(_pObj)				((_pObj) && (_pObj->IsValid()))
#define DO_IF_IS_NOT_VALID_OBJ(_pObj)	if(!IS_VALID_OBJ(_pObj))
#define DO_IF_IS_VALID_OBJ(_pObj)		if(IS_VALID_OBJ(_pObj))
#define ELSE else

// 수학 관련
#define PI acos(-1.f) //== acos(cos(pi)) == pi
#define TWO_PI (2.f * PI)

#define TO_RADIAN(_fDegree) ((_fDegree) * TWO_PI / 360.f)
#define TO_DEGREE(_fRadian) ((_fRadian) * 360.f / TWO_PI)

#define DIR_X_DEGREE(_fDegree) cosf(TO_RADIAN((_fDegree)))
#define DIR_Y_DEGREE(_fDegree) sinf(TO_RADIAN((_fDegree)))

#define DIR_X_RADIAN(_fRadian) cosf((_fRadian))
#define DIR_Y_RADIAN(_fRadian) sinf((_fRadian))

#define CLAMP_RADIAN(_fRadian) ((_fRadian) -static_cast<int>((_fRadian) / TWO_PI) * TWO_PI)
#define CLAMP_DEGREE(_fDegree) ((_fDegree) - static_cast<int>((_fDegree) / 360.f) * 360.f)

// 비트마스크 플래그 관련
#define IS_PRESS_(_Key) (GetAsyncKeyState(_Key) & 0x8000)
#define ALL_FLAG(_State, _StateSize) (flags = (1 << (size * 8)) - 1)
#define EMPTY_FLAG(_State) (_State = 0)
#define ON_FLAG(_State, _Mask) (_State |= _Mask) 
#define OFF_FLAG(_State, _Mask) (_State &= ~_Mask)
#define TOGGLE_FLAG(_State, _Mask) (_State ^= _Mask)
#define IS_ON_FLAG(_State, _Mask) ((bool)((_State & _Mask) != 0))

// 키입력 관련
//#define	KM_LEFT		0x00000001
//#define	KM_RIGHT	0x00000002 
//#define KM_UP		0x00000004
//#define KM_DOWN		0x00000008
//
//#define	KM_LBUTTON	0x00000010
//#define	KM_RBUTTON	0x00000020 
//#define KM_Q		0x00000040
//#define KM_E		0x00000080
//
//#define KM_W		0x00000100
//#define KM_A		0x00000200
//#define KM_S		0x00000400
//#define KM_D		0x00000800
//
//#define KM_LSHIFT	0x00001000
//#define KM_P		0x00002000
//#define KM_G		0x00004000
//#define KM_SPACE	0x00008000
//
//#define KM_R		0x00010000
//#define KM_TAB		0x00020000
//#define KM_M		0x00040000			// 미니맵 관련
//#define KM_F		0x00080000			// 아이템과 상호작용
//
//#define KM_Z		0x00100000			
//#define KM_X		0x00200000
}

#endif // Macro_h__
