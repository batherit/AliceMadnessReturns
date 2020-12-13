// CAABBColTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CAABBColTab.h"
#include "afxdialogex.h"
#include "CColliderTab.h"


// CAABBColTab 대화 상자

IMPLEMENT_DYNAMIC(CAABBColTab, CDialogEx)

CAABBColTab::CAABBColTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AABB_COLTAB, pParent)
	, m_cstrPosX(_T(""))
	, m_cstrPosY(_T(""))
	, m_cstrPosZ(_T(""))
	, m_cstrScaleX(_T(""))
	, m_cstrScaleY(_T(""))
	, m_cstrScaleZ(_T(""))
{

}

CAABBColTab::~CAABBColTab()
{
}

void CAABBColTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_cstrPosX);
	DDX_Text(pDX, IDC_EDIT2, m_cstrPosY);
	DDX_Text(pDX, IDC_EDIT3, m_cstrPosZ);
	DDX_Text(pDX, IDC_EDIT7, m_cstrScaleX);
	DDX_Text(pDX, IDC_EDIT8, m_cstrScaleY);
	DDX_Text(pDX, IDC_EDIT9, m_cstrScaleZ);
	DDX_Control(pDX, IDC_EDIT1, m_editPosX);
	DDX_Control(pDX, IDC_EDIT2, m_editPosY);
	DDX_Control(pDX, IDC_EDIT3, m_editPosZ);
	DDX_Control(pDX, IDC_EDIT7, m_editScaleX);
	DDX_Control(pDX, IDC_EDIT8, m_editScaleY);
	DDX_Control(pDX, IDC_EDIT9, m_editScaleZ);
}

void CAABBColTab::UpdateTab(Engine::CColliderObject_AABB * _pCollider)
{
	if (!_pCollider)
		return;

	UpdateData(TRUE);

	_vec3 vPos = _pCollider->GetTransform()->GetPos(Engine::CTransform::COORD_TYPE_LOCAL);
	_vec3 vScale = _pCollider->GetTransform()->GetScale();
	m_cstrPosX.Format(L"%f", vPos.x);
	m_cstrPosY.Format(L"%f", vPos.y);
	m_cstrPosZ.Format(L"%f", vPos.z);
	m_cstrScaleX.Format(L"%f", vScale.x);
	m_cstrScaleY.Format(L"%f", vScale.y);
	m_cstrScaleZ.Format(L"%f", vScale.z);

	UpdateData(FALSE);
}

void CAABBColTab::EnableTab(_bool _bEnable)
{
	m_editPosX.EnableWindow(_bEnable);
	m_editPosY.EnableWindow(_bEnable);
	m_editPosZ.EnableWindow(_bEnable);
	m_editScaleX.EnableWindow(_bEnable);
	m_editScaleY.EnableWindow(_bEnable);
	m_editScaleZ.EnableWindow(_bEnable);
}


BEGIN_MESSAGE_MAP(CAABBColTab, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CAABBColTab::OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT2, &CAABBColTab::OnEnChangeEditPosY)
	ON_EN_CHANGE(IDC_EDIT3, &CAABBColTab::OnEnChangeEditPosZ)
	ON_EN_CHANGE(IDC_EDIT7, &CAABBColTab::OnEnChangeEditScaleX)
	ON_EN_CHANGE(IDC_EDIT8, &CAABBColTab::OnEnChangeEditScaleY)
	ON_EN_CHANGE(IDC_EDIT9, &CAABBColTab::OnEnChangeEditScaleZ)
END_MESSAGE_MAP()


// CAABBColTab 메시지 처리기


void CAABBColTab::OnEnChangeEditPosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_AABB* pCollider = dynamic_cast<Engine::CColliderObject_AABB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetPosX(static_cast<_float>(_ttof(m_cstrPosX)));
	}
	UpdateData(FALSE);
}


void CAABBColTab::OnEnChangeEditPosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_AABB* pCollider = dynamic_cast<Engine::CColliderObject_AABB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetPosY(static_cast<_float>(_ttof(m_cstrPosY)));
	}
	UpdateData(FALSE);
}


void CAABBColTab::OnEnChangeEditPosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_AABB* pCollider = dynamic_cast<Engine::CColliderObject_AABB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetPosZ(static_cast<_float>(_ttof(m_cstrPosZ)));
	}
	UpdateData(FALSE);
}


void CAABBColTab::OnEnChangeEditScaleX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_AABB* pCollider = dynamic_cast<Engine::CColliderObject_AABB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetScaleX(static_cast<_float>(_ttof(m_cstrScaleX)));
	}
	UpdateData(FALSE);
}


void CAABBColTab::OnEnChangeEditScaleY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_AABB* pCollider = dynamic_cast<Engine::CColliderObject_AABB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetScaleY(static_cast<_float>(_ttof(m_cstrScaleY)));
	}
	UpdateData(FALSE);
}


void CAABBColTab::OnEnChangeEditScaleZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_AABB* pCollider = dynamic_cast<Engine::CColliderObject_AABB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetScaleZ(static_cast<_float>(_ttof(m_cstrScaleZ)));
	}
	UpdateData(FALSE);
}


BOOL CAABBColTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	EnableTab(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
