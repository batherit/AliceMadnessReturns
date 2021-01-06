#ifndef StaticObject_h__
#define StaticObject_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CStaticObject : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CStaticObject(const  CStaticObject& rhs);
	virtual ~CStaticObject(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CStaticObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	_bool SetRenderInfo(const _tchar* _pMeshTag, Engine::RENDERID _eRenderID = Engine::RENDER_NONALPHA);
	const _tchar* GetMeshTag() const { return m_tcMeshTag; }
	Engine::CStaticMesh* GetStaticMesh() const { return m_pMesh; }

	virtual void		Free(void);

	virtual _bool SaveInfo(HANDLE& _hfOut) override;
	virtual _bool LoadInfo(HANDLE& _hfIn) override;
	_bool LoadCollidersInfo();
	CString& GetFactorRef(_int _iIndex) { return m_strFactor[_iIndex]; }

	void SetCustomed(_bool _bIsCustomed) { m_bIsCustomed = _bIsCustomed; }
	_bool IsCustomed() const { return m_bIsActivated; }


private:
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	//const _tchar* m_pMeshTag = nullptr;
	_tchar m_tcMeshTag[MAX_PATH] = L"";

	_bool m_bIsCustomed = false;
	CString m_strFactor[6] = { L"NULL", L"NULL", L"NULL", L"NULL" , L"NULL", L"NULL" };
};

END

#endif // !LogoObject_h__
