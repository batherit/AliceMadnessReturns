#ifndef PopDomino_h__
#define PopDomino_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAliceW;
class CPopDomino : public Engine::CGameObject
{
private: // ������, �Ҹ���
	explicit  CPopDomino(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CPopDomino(const  CPopDomino& rhs);
	virtual ~CPopDomino(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);

public:
	static CPopDomino*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CStaticMesh* GetDynamicMesh() const { return m_pMesh; }

private:
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	CAliceW* m_pAlice = nullptr;
	_bool m_bIsVisible = true;
};

END

#endif // !LogoObject_h__