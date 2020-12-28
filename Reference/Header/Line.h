#ifndef Line_h__
#define Line_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLine : public CBase
{
public:
	enum POINT { POINT_START, POINT_FINISH, POINT_END};
	enum COMPARE {		COMPARE_LEFT, COMPARE_RIGHT	};

private:
	explicit CLine(void);
	virtual ~CLine(void);

public:
	HRESULT		Ready_Line(const _vec2* pPointA, const _vec2* pPointB);
	COMPARE		Compare(const _vec2* pPos);
	_vec2		GetPoint(const CLine::POINT& _ePoint) const { return m_vPoint[_ePoint]; }
	_vec2		GetDirection() const { return m_vDirection; }
	_vec2		GetNormal()	const { return m_vNormal; }
	_float		GetGradient() const { return m_fGradient; }
	_float		GetIntercept() const { return m_fIntercept; }
	_vec2		GetPosInLine(const _vec2 & _vPosXZ);

private:
	// 모두 xz에 대한 값들입니다. 높이는 아직 잘 모름/
	_vec2		m_vPoint[POINT_END];
	_vec2		m_vDirection;		
	_vec2		m_vNormal;			
	_float		m_fGradient;		
	_float		m_fIntercept;

public:
	static CLine*		Create(const _vec2* pPointA, const _vec2* pPointB);
	virtual void		Free(void);
};

END
#endif // Line_h__
