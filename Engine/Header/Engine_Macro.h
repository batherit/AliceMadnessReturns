#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
	#define			USING(NAMESPACE)	using namespace NAMESPACE;

	#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
	#define			TAGMSG_BOX(_tag,_message)	MessageBoxW(NULL, _message, _tag, MB_OK)

	#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
	#define			END						}

	#ifdef ENGINE_EXPORTS
	#define ENGINE_DLL _declspec(dllexport)
	#else
	#define ENGINE_DLL _declspec(dllimport)
	#endif // ENGINE_EXPORTS


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

// 비트마스크 플래그 관련
#define IS_PRESS_(_Key) (GetAsyncKeyState(_Key) & 0x8000)
#define ALL_FLAG(_State, _StateSize) (flags = (1 << (size * 8)) - 1)
#define EMPTY_FLAG(_State) (_State = 0)
#define ON_FLAG(_State, _Mask) (_State |= _Mask) 
#define OFF_FLAG(_State, _Mask) (_State &= ~_Mask)
#define TOGGLE_FLAG(_State, _Mask) (_State ^= _Mask)
#define IS_ON_FLAG(_State, _Mask) ((bool)((_State & _Mask) != 0))

// Direct Input 관련
#define DI_IS_ON_KEY(_KeyState, _Key) ((bool)((_KeyState[_Key] & 0x80) != 0))  
#define DI_IS_ON_MB(_MouseState, _MouseButtonType) ((bool)((_MouseState.rgbButtons[_MouseButtonType]) != 0))  

#define WORLD_X_AXIS _vec3(1.f, 0.f, 0.f)
#define WORLD_Y_AXIS _vec3(0.f, 1.f, 0.f)
#define WORLD_Z_AXIS _vec3(0.f, 0.f, 1.f)
}

#endif // Engine_Macro_h__
