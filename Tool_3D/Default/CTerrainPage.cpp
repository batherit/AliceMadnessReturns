// CTerrainPage.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CTerrainPage.h"
#include "afxdialogex.h"
#include "EditScene.h"


// CTerrainPage 대화 상자

IMPLEMENT_DYNAMIC(CTerrainPage, CPropertyPage)

CTerrainPage::CTerrainPage()
	: CPropertyPage(IDD_TERRAIN_PAGE)
{

}

CTerrainPage::~CTerrainPage()
{
}

void CTerrainPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_vPos.x);
	DDX_Text(pDX, IDC_EDIT2, m_vPos.y);
	DDX_Text(pDX, IDC_EDIT3, m_vPos.z);
	DDX_Text(pDX, IDC_EDIT4, m_vAngle.x);
	DDX_Text(pDX, IDC_EDIT5, m_vAngle.y);
	DDX_Text(pDX, IDC_EDIT6, m_vAngle.z);
	DDX_Text(pDX, IDC_EDIT7, m_vScale.x);
	DDX_Text(pDX, IDC_EDIT8, m_vScale.y);
	DDX_Text(pDX, IDC_EDIT9, m_vScale.z);
}


BEGIN_MESSAGE_MAP(CTerrainPage, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1, &CTerrainPage::OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT2, &CTerrainPage::OnEnChangeEditPosY)
	ON_EN_CHANGE(IDC_EDIT3, &CTerrainPage::OnEnChangeEditPosZ)
	ON_EN_CHANGE(IDC_EDIT4, &CTerrainPage::OnEnChangeEditRotX)
	ON_EN_CHANGE(IDC_EDIT5, &CTerrainPage::OnEnChangeEditRotY)
	ON_EN_CHANGE(IDC_EDIT6, &CTerrainPage::OnEnChangeEditRotZ)
	ON_EN_CHANGE(IDC_EDIT7, &CTerrainPage::OnEnChangeEditScaleX)
	ON_EN_CHANGE(IDC_EDIT8, &CTerrainPage::OnEnChangeEditScaleX)
	ON_EN_CHANGE(IDC_EDIT9, &CTerrainPage::OnEnChangeEditScaleX)
END_MESSAGE_MAP()


// CTerrainPage 메시지 처리기
void CTerrainPage::OnEnChangeEditPosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainPageView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pPickedObject = pEditScene->GetPickedObject();

	if (pPickedObject)
		pPickedObject->GetTransform()->SetPos(m_vPos);
	UpdateData(FALSE);
}


void CTerrainPage::OnEnChangeEditPosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainPageView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pPickedObject = pEditScene->GetPickedObject();

	if (pPickedObject)
		pPickedObject->GetTransform()->SetPos(m_vPos);
	UpdateData(FALSE);
}


void CTerrainPage::OnEnChangeEditPosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainPageView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pPickedObject = pEditScene->GetPickedObject();

	if (pPickedObject)
		pPickedObject->GetTransform()->SetPos(m_vPos);
	UpdateData(FALSE);
}


void CTerrainPage::OnEnChangeEditRotX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainPageView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pPickedObject = pEditScene->GetPickedObject();

	if (pPickedObject) {
		_vec3 vRadian = _vec3(D3DXToRadian(m_vAngle.x), D3DXToRadian(m_vAngle.y), D3DXToRadian(m_vAngle.z));
		pPickedObject->GetTransform()->SetAngle(vRadian);
	}

	UpdateData(FALSE);
}


void CTerrainPage::OnEnChangeEditRotY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainPageView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pPickedObject = pEditScene->GetPickedObject();

	if (pPickedObject) {
		_vec3 vRadian = _vec3(D3DXToRadian(m_vAngle.x), D3DXToRadian(m_vAngle.y), D3DXToRadian(m_vAngle.z));
		pPickedObject->GetTransform()->SetAngle(vRadian);
	}

	UpdateData(FALSE);
}


void CTerrainPage::OnEnChangeEditRotZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainPageView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pPickedObject = pEditScene->GetPickedObject();

	if (pPickedObject) {
		_vec3 vRadian = _vec3(D3DXToRadian(m_vAngle.x), D3DXToRadian(m_vAngle.y), D3DXToRadian(m_vAngle.z));
		pPickedObject->GetTransform()->SetAngle(vRadian);
	}

	UpdateData(FALSE);
}


void CTerrainPage::OnEnChangeEditScaleX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainPageView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pPickedObject = pEditScene->GetPickedObject();

	if (pPickedObject) {
		pPickedObject->GetTransform()->SetScale(m_vScale);
	}

	UpdateData(FALSE);
}


void CTerrainPage::OnEnChangeEditScaleY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainPageView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pPickedObject = pEditScene->GetPickedObject();

	if (pPickedObject) {
		pPickedObject->GetTransform()->SetScale(m_vScale);
	}

	UpdateData(FALSE);
}


void CTerrainPage::OnEnChangeEditScaleZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainPageView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pPickedObject = pEditScene->GetPickedObject();

	if (pPickedObject) {
		pPickedObject->GetTransform()->SetScale(m_vScale);
	}

	UpdateData(FALSE);
}