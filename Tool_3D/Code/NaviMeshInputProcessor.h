#ifndef NaviMeshInputProcessor_h__
#define NaviMeshInputProcessor_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"


class CNaviMeshTab;
BEGIN(Client)

class CNaviMeshInputProcessor : public Engine::CGameObject
{
private: // ������, �Ҹ���
	explicit  CNaviMeshInputProcessor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CNaviMeshInputProcessor(const  CNaviMeshInputProcessor& rhs);
	virtual ~CNaviMeshInputProcessor(void);

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;

public:
	static CNaviMeshInputProcessor*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	// InputMode���� ���Ǵ� �Լ��̴�.
	void Click();
	void Drag(const _float _fDeltaTime);
	void Idle(const _float _fDeltaTime);

	// TODO : NeviMeshTab���� ���Ǵ� �Լ� ����
	void SetPickMode(NAVIMESH_TAB::E_PICKMODE _ePickMode);

public:
	NAVIMESH_TAB::E_PICKMODE GetPickMode() const;

private:
	void PickTerrain();

private:
	// �Է� ó���ڴ� ó���� ������ UI�� �ݿ��ϱ� ���Ͽ� Tab �����͸� ���Ѵ�.
	CNaviMeshTab* m_pNaviMeshTab = nullptr;
};

END

#endif // !NaviMeshInputProcessorMap_h__
