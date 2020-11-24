#ifndef ManualCol_h__
#define ManualCol_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CManualCol : public CVIBuffer
{
protected:
	explicit CManualCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CManualCol(const CManualCol& rhs);
	virtual ~CManualCol(void);

public:
	virtual HRESULT	Ready_Buffer(void) override;
	virtual void	Render_Buffer(void) override;
	static const _tchar* GetComponentTag() { return L"ManualCol"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

	void PushTriangleVertices(_vec3 _vTriPos1, _vec3 _vTriPos2, _vec3 _vTriPos3);
	void PopTriangleVertices(_int _iTriangleIndex);
	void SetTriangleColor(_int _iTriangleIndex, D3DXCOLOR _colTriangleColor = D3DCOLOR_ARGB(150, 255, 0, 0));
	void SetTriangleVertexPosition(_int _iTriangleIndex, _int _iVertexIndex, const _vec3 _vNewPosition);
	vector<_vec3>& GetVertices() { return m_vecVertices; }

public:
	static CManualCol*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void);

private:
	vector<_vec3> m_vecVertices;
};
END

#endif // TriCol_h__