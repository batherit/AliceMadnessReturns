#ifndef DynamicObject_h__
#define DynamicObject_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CDynamicObject : public Engine::CGameObject
{
private: // ������, �Ҹ���
	explicit  CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CDynamicObject(const  CDynamicObject& rhs);
	virtual ~CDynamicObject(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CDynamicObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	_bool SetRenderInfo(const _tchar* _pMeshTag, Engine::RENDERID _eRenderID = Engine::RENDER_NONALPHA);
	const _tchar* GetMeshTag() const { return m_tcMeshTag; }
	Engine::CDynamicMesh* GetDynamicMesh() const { return m_pMesh; }

	virtual void		Free(void);

	virtual _bool SaveInfo(HANDLE& _hfOut) override;
	virtual _bool LoadInfo(HANDLE& _hfIn) override;
	/*virtual _bool SaveCollidersInfo(HANDLE& _hfOut) override;
	virtual _bool LoadCollidersInfo(HANDLE& _hfIn) override;*/
	_bool LoadCollidersInfo();
	CString& GetFactorRef(_int _iIndex) { return m_strFactor[_iIndex]; }

	void SetCustomed(_bool _bIsCustomed) { m_bIsCustomed = _bIsCustomed; }
	_bool IsCustomed() const { return m_bIsCustomed; }

private:
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	//const _tchar* m_pMeshTag = nullptr;
	_tchar m_tcMeshTag[MAX_PATH] = L"";

	_bool m_bIsCustomed = false;
	CString m_strFactor[6] = { L"NULL", L"NULL", L"NULL", L"NULL" , L"NULL", L"NULL" };
};

END

#endif // !LogoObject_h__
