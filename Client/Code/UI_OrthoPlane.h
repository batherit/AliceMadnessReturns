#ifndef UI_OrthoPlane_h__
#define UI_OrthoPlane_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CUI_OrthoPlane : public Engine::CGameObject
{
public:
	enum E_PLANE_TYPE { TYPE_UI, TYPE_BACKGROUND, TYPE_END };
private: // 생성자, 소멸자
	explicit  CUI_OrthoPlane(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_OrthoPlane(const  CUI_OrthoPlane& rhs);
	virtual ~CUI_OrthoPlane(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_OrthoPlane*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetTexture(const _tchar* _pTextureTag);
	void SetSize(_float _fSizeX, _float _fSizeY) { m_fSizeX = _fSizeX; m_fSizeY = _fSizeY; }
	void SetPos(_float _fPosX, _float _fPosY) { m_fPosX = _fPosX;}
	void SetType(E_PLANE_TYPE _ePlaneType) { 
		switch (_ePlaneType) {
		case TYPE_UI:
			m_fPosZ = 0.f;
			break;
		case TYPE_BACKGROUND:
			m_fPosZ = 1.f;
			break;
		}
	}

private:
	Engine::CPolygonRenderer* m_pRenderer = nullptr;
	_float m_fSizeX = 1.f;
	_float m_fSizeY = 1.f;
	_float m_fPosX = 0.f;
	_float m_fPosY = 0.f;
	_float m_fPosZ = 0.f;
	_matrix m_matProj;
};

END

#endif // !LogoObject_h__