#ifndef DefaultScene_h__
#define DefaultScene_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

//END
BEGIN(Client)
class CDefaultScene : public Engine::CScene
{
private: // ������, �Ҹ���
	explicit CDefaultScene(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDefaultScene(const CDefaultScene& rhs);
	virtual ~CDefaultScene(void);

public:
	// CScene��(��) ���� ��ӵ�
	virtual void ResetScene(void) override;
	virtual HRESULT Ready(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CDefaultScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};

END
#endif // MainApp_h__