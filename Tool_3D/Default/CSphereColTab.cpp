// CSphereColTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CSphereColTab.h"
#include "afxdialogex.h"
#include "CColliderTab.h"


// CSphereColTab 대화 상자

IMPLEMENT_DYNAMIC(CSphereColTab, CDialogEx)

CSphereColTab::CSphereColTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SPHERE_COLTAB, pParent)
	, m_cstrPosX(_T(""))
	, m_cstrPosY(_T(""))
	, m_cstrPosZ(_T(""))
	, m_cstrRadius(_T(""))
{

}

CSphereColTab::~CSphereColTab()
{
}

void CSphereColTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT18, m_cstrPosX);
	DDX_Text(pDX, IDC_EDIT2, m_cstrPosY);
	DDX_Text(pDX, IDC_EDIT3, m_cstrPosZ);
	DDX_Text(pDX, IDC_EDIT1, m_cstrRadius);
	DDX_Control(pDX, IDC_EDIT18, m_editPosX);
	DDX_Control(pDX, IDC_EDIT2, m_editPosY);
	DDX_Control(pDX, IDC_EDIT3, m_editPosZ);
	DDX_Control(pDX, IDC_EDIT1, m_editRadius);
}

void CSphereColTab::UpdateTab(Engine::CColliderObject_Sphere * _pCollider)
{
	if (!_pCollider)
		return;

	UpdateData(TRUE);

	_vec3 vPos = _pCollider->GetTransform()->GetPos(Engine::CTransform::COORD_TYPE_LOCAL);
	m_cstrPosX.Format(L"%f", vPos.x);
	m_cstrPosY.Format(L"%f", vPos.y);
	m_cstrPosZ.Format(L"%f", vPos.z);
	m_cstrRadius.Format(L"%f", _pCollider->GetRadius());

	UpdateData(FALSE);
}

void CSphereColTab::EnableTab(_bool _bEnable)
{
	m_editPosX.EnableWindow(_bEnable);
	m_editPosY.EnableWindow(_bEnable);
	m_editPosZ.EnableWindow(_bEnable);
	m_editRadius.EnableWindow(_bEnable);
}


BEGIN_MESSAGE_MAP(CSphereColTab, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT18, &CSphereColTab::OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT2, &CSphereColTab::OnEnChangeEditPosY)
	ON_EN_CHANGE(IDC_EDIT3, &CSphereColTab::OnEnChangeEditPosZ)
	ON_EN_CHANGE(IDC_EDIT1, &CSphereColTab::OnEnChangeEditRadius)
END_MESSAGE_MAP()


// CSphereColTab 메시지 처리기


void CSphereColTab::OnEnChangeEditPosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_Sphere* pCollider = dynamic_cast<Engine::CColliderObject_Sphere*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetPosX(static_cast<_float>(_ttof(m_cstrPosX)));
	}
	UpdateData(FALSE);
}


void CSphereColTab::OnEnChangeEditPosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_Sphere* pCollider = dynamic_cast<Engine::CColliderObject_Sphere*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetPosY(static_cast<_float>(_ttof(m_cstrPosY)));
	}
	UpdateData(FALSE);
}


void CSphereColTab::OnEnChangeEditPosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_Sphere* pCollider = dynamic_cast<Engine::CColliderObject_Sphere*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetPosZ(static_cast<_float>(_ttof(m_cstrPosZ)));
	}
	UpdateData(FALSE);
}


void CSphereColTab::OnEnChangeEditRadius()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_Sphere* pCollider = dynamic_cast<Engine::CColliderObject_Sphere*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->SetRadius(static_cast<_float>(_ttof(m_cstrRadius)));
	}
	UpdateData(FALSE);
}


BOOL CSphereColTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	EnableTab(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
