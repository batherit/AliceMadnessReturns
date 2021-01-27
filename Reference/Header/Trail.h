#ifndef Trail_h__
#define Trail_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTrail : public CVIBuffer
{
protected:
	explicit CTrail(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTrail(const CTrail& rhs);
	virtual ~CTrail(void);

public:
	virtual HRESULT	Ready_Buffer(const _ulong& dwVtxMax);
	void Render_Buffer() override;
	//void Render_Buffer(const list<pair<_vec3, _vec3> >* pTrailList);

	HRESULT Add_Vertex(const _vec3 * pTopPos, const _vec3 * pBottomPos);
	HRESULT Clear_Vertex();
	HRESULT Add_Vertex_CatmullRom(const list<pair<_vec3, _vec3>>* pTrailList);
	void SetTrailList(const list<pair<_vec3, _vec3>>* _pTrailList) { m_pTrailList = _pTrailList; }

public:
	static CTrail*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwVtxMax);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void);

private:
	_ulong m_dwUsingVtxCnt;
	_ulong m_dwUsingTriCnt;
	const list<pair<_vec3, _vec3>>* m_pTrailList = nullptr;
};
END

#endif // Trail_h__
