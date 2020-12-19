#ifndef Cell_h__
#define Cell_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Line.h"

BEGIN(Engine)

class ENGINE_DLL CCell : public CBase
{
public:
	enum POINT {	POINT_A, POINT_B,POINT_C, POINT_END};
	enum LINE  { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum NEIGHBOR {	NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };
	enum MOVING { INSIDE, OUTSIDE };

private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCell(void);

public:
	const _vec3*	Get_Point(POINT eType) const { return &m_vPoint[eType]; }
	CCell*			Get_Neighbor(NEIGHBOR eType) const { return m_pNeighbor[eType]; }
	void			Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor) { m_pNeighbor[eType] = pNeighbor; }
	const _int*	Get_Index(void) { return &m_iIndex; }
	CLine*			GetLine(const CCell::LINE& _eLine) const { return m_pLine[_eLine]; }
	_vec3			GetPosInCell(const _vec3& _vPos);
	_float			GetHeight(const _vec3& _vPos);
	_int			GetTagIndex() const { return m_iTagIndex; }
	_bool			IsPosInCell(const _vec3& _vPos);
	_bool			IsCollided(const _vec3& _vFromPos, const _vec3& _vToPos, _vec3* _pHitPos = nullptr);

public:
	HRESULT				Ready_Cell(	const _int& iIndex,
									const _vec3* pPointA,
									const _vec3* pPointB,
									const _vec3* pPointC,
									const _int& _iTagIndex);

	_bool				Compare_Point(const _vec3* pFirstPoint, const _vec3* pSecondPoint, CCell* pCell);
	MOVING				CompareCell(const _vec3* pEndPos, _int* pCellIndex);
	void				Render_Cell(void);

private:
	_vec3				m_vPoint[POINT_END];
	CCell*				m_pNeighbor[NEIGHBOR_END];
	CLine*				m_pLine[LINE_END];
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LPD3DXLINE			m_pD3DXLine;
	_int				m_iIndex = 0;
	_int				m_iTagIndex = 0;

public:
	static CCell*			Create(LPDIRECT3DDEVICE9 pGraphicDev, const _int& iIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, const _int& _iTagIndex);
	virtual void			Free(void);

};

END
#endif // Cell_h__
