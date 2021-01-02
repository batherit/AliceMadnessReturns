#ifndef UI_HPPiece_h__
#define UI_HPPiece_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "UI_Image.h"

BEGIN(Client)

class CUI_HPPiece : public CUI_Image
{
private: // ������, �Ҹ���
	explicit  CUI_HPPiece(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_HPPiece(const  CUI_HPPiece& rhs);
	virtual ~CUI_HPPiece(void);

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_HPPiece*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

private:
	Engine::CRenderer* m_pRenderer = nullptr;
};

END

#endif // !LogoObject_h__