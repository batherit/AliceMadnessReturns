#include "AniCtrl.h"

USING(Engine)

Engine::CAniCtrl::CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	: m_pAniCtrl(pAniCtrl)
	, m_iCurrentTrack(0)
	, m_iNewTrack(1)
	, m_fAccTime(0.f)
	, m_iOldAniIdx(99)
{
	m_pAniCtrl->AddRef();
}

Engine::CAniCtrl::CAniCtrl(const CAniCtrl& rhs)
	: m_iCurrentTrack(rhs.m_iCurrentTrack)
	, m_iNewTrack(rhs.m_iNewTrack)
	, m_fAccTime(rhs.m_fAccTime)
	, m_iOldAniIdx(rhs.m_iOldAniIdx)
{
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(), // ���� �� ���� ��ü���� �����ǰ� �ִ� �ִϸ��̼��� ����
												rhs.m_pAniCtrl->GetMaxNumAnimationSets(), // ���� ������ �ִϸ��̼��� �ִ� ����(��Դ� 1�� ����)
												rhs.m_pAniCtrl->GetMaxNumTracks(),		  // �ִϸ��̼��� ����ϱ� ���� �ִ� Ʈ���� ����(��κ� �� ���� ���, ���� ���� ����ؾ� �� �� ���)
												rhs.m_pAniCtrl->GetMaxNumEvents(),		  // �ִϸ��̼� ��� �� ����Ǵ� ��Ư�� �̺�Ʈ(�츮�� ��� ����)
												&m_pAniCtrl);
}

Engine::CAniCtrl::~CAniCtrl(void)
{

}

HRESULT Engine::CAniCtrl::Ready_AnimationCtrl(void)
{
	return S_OK;
}

void Engine::CAniCtrl::Set_AnimationSet(const _uint& iIndex)
{
	if (m_iOldAniIdx == iIndex)
		return;
	
	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET		pAS = NULL;
	
	// �ε��� ���� �ش��ϴ� �ִϸ��̼� ���� ���� �Լ�
	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);

	// m_pAniCtrl->GetAnimationSetByName()
	m_dPeriod = pAS->GetPeriod(); // �ִϸ��̼� ���� ��� �ð��� ��ȯ�ϴ� �Լ�

	// 0�� Ʈ���� �ִϸ��̼� �� ����
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);
	

	// �ִϸ��̼� ���� �ȿ� ���ԵǾ� �ִ� �̺�Ʈ ������ �����ϴ� �Լ�(�̺�Ʈ�� ���� ������ ó��)
	// : �츮�� �̺�Ʈ�� ����� �� ���� ��Ȳ�ε�, ������ ���ԵǾ� �ִ� ��� ���� �ִϸ��̼��� ���� ���� �ʴ� ������ �߻�
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// ���� ������ Ʈ���� ��� �Ǵ� ���� ��Ű�� ���� �Լ�(3���� : �������� ���� Ʈ���� ������ ���ΰ�)
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.31f* 0.5f);

	// ���ڰ����� ������ Ʈ���� ���õ� �ִϸ��̼� ���� � �ӵ��� ������ ������ �����ϴ� �Լ�(�ӵ��� ��� ���� 1)
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.3f* 0.5f, D3DXTRANSITION_LINEAR);
	// ���ڰ����� ������ Ʈ���� ����ġ�� �����ϴ� �Լ�
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.0000001f, m_fAccTime, 0.3f* 0.5f, D3DXTRANSITION_LINEAR);


	// New Ʈ���� Ȱ��ȭ�� �����ϴ� �Լ�
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.36f* 0.5, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.999999f, m_fAccTime, 0.36f* 0.5, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->ResetTime(); // AdvanceTime ȣ�� �� ���������� �����Ǵ� �ð��� �ʱ�ȭ�ϴ� �Լ�
	m_fAccTime = 0.f;

	// ������ ��� ���̴� Ʈ������ ���ο� Ʈ������ ������ �߱� ������ ��� ������ 0��(0�� ��ġ�� �ʱ�ȭ)�� �����ϵ��� �����ϴ� �Լ�
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIdx = iIndex;
	m_iCurrentTrack = m_iNewTrack;
}

void Engine::CAniCtrl::Play_Animation(const _float& fTimeDelta)
{
	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);	// 2���� : �ִϸ��̼� ����� ���� ���峪, ����Ʈ�� ���� ����, ������ �Ⱦ�.
	// AdvanceTime ȣ�� �� ���������� �����Ǵ� �ð� ���� ����

	m_fAccTime += fTimeDelta;
}

Engine::CAniCtrl* Engine::CAniCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAniCtrl*	pInstance = new CAniCtrl(pAniCtrl);

	if (FAILED(pInstance->Ready_AnimationCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CAniCtrl* Engine::CAniCtrl::Create(const CAniCtrl& rhs)
{
	CAniCtrl*	pInstance = new CAniCtrl(rhs);

	if (FAILED(pInstance->Ready_AnimationCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CAniCtrl::Free(void)
{
	Safe_Release(m_pAniCtrl);
}

_bool Engine::CAniCtrl::Is_AnimationSetEnd(void)
{
	D3DXTRACK_DESC		tTrackInfo;
	ZeroMemory(&tTrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &tTrackInfo);

	if (tTrackInfo.Position >= m_dPeriod - 0.36f * 0.5f)
		return true;

	return false;
}

_float CAniCtrl::GetAnimationProgress(void)
{
	D3DXTRACK_DESC		tTrackInfo;
	ZeroMemory(&tTrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &tTrackInfo);

	return static_cast<_float>(Clamp(tTrackInfo.Position / m_dPeriod, 0.0, 1.0));
}

