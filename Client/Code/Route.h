#pragma once
#include "Component.h"
#include "Typedef.h"

BEGIN(Client)

class CRoute : public Engine::CComponent {
protected:
	explicit CRoute();
	explicit CRoute(const CRoute& Physics);
	virtual ~CRoute(void);
public:
	virtual HRESULT Ready() override;
	virtual _int	Update(const _float& fTimeDelta) override;

public:
	static const _tchar* GetComponentTag() { return L"Route"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }
	static CComponent* Create();
	virtual CComponent* Clone(void) override;
	virtual void Free(void) override;

public:
	void SetPosList(const vector<_vec3>& _vecPosList);
	//_bool UpdateNextPosition(const _float& _fDeltaTime); // 다음 위치를 갱신하고 갱신 성공 여부를 반환한다.
	//_vec3 GetNextPosition(const _float& _fDeltaTime);	// 다음 좌표를 갖고 온다.
	_vec3 GetNextPosition(const _float& _fDeltaTime, const _float& _fSpeed);
	_vec3 GetPrevPosition(const _float& _fDeltaTime, const _float& _fSpeed);
	_vec3 GetCurrentPosition(void);	// 현재 목표점을 갖고 온다.
	_vec3 GetStartPosition(void) { return m_vecPosList[0]; }
	_vec3 GetEndPosition(void) {
		if (!m_vecPosList.empty())
			return m_vecPosList[m_vecPosList.size() - 1];
		else
			return { 0.f, 0.f, 0.f };
	}
	_bool IsEmpty(void) { return m_vecPosList.empty(); }
	_bool IsNextPositionExisted(void);
	_bool IsPrevPositionExisted(void);

private:
	int m_iFrom = 0; // m_iToIndex는 m_iFromIndex + 1을 하여 구할 수 있다.
	_float m_fElapsedSec = 0.f; // 출발 좌표에서 도착 좌표를 항로하는데 경과된 시간이다.
	_vec3 m_vCurrentPos{ 0.f, 0.f, 0.f };
	vector<_vec3> m_vecPosList;
};
END