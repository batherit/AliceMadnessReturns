#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CNaviMesh : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CNaviMesh(const  CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CNaviMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	_bool IsValidTriangleIndex(_int _iTriangleIndex);
	_bool IsValidVertexIndex(_int _iVertexIndex);
	_bool IsValidIndex(_int _iTriangleIndex, _int _iVertexIndex);
	void PushTriangleVertices(_vec3 _vTriPos1, _vec3 _vTriPos2, _vec3 _vTriPos3);
	void PopTriangleVertices(_int _iTriangleIndex);
	void MarkTriangle(_int _iTriangleIndex);
	void ReleaseMarkedTriangle();
	void SetTriangleVertexPosition(_int _iTriangleIndex, _int _iVertexIndex, const _vec3& _vNewPosition);
	_vec3 GetTriangleVertexPosition(_int _iTriangleIndex, _int _iVertexIndex) const;
	vector<_vec3>& GetNaviVertices();
	_int GetMarkedTriangleIndex() const { return m_iMarkedTriangleIndex; }
	void GenerateNewNaviMesh(vector<_vec3>& _rVertices);

	// 파일 입출력
	virtual _bool SaveInfo(HANDLE& _hfOut) override;
	virtual _bool LoadInfo(HANDLE& _hfIn) override;

private:
	Engine::CPolygonRenderer* m_pRenderer = nullptr;
	Engine::CManualCol* m_pManualCol = nullptr;
	_int m_iMarkedTriangleIndex = -1;		//-1 => 선택된 삼각형이 없다.
};

END

#endif // !TerrainMap_h__