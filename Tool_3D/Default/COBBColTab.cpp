// COBBColTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "COBBColTab.h"
#include "afxdialogex.h"
#include "CColliderTab.h"


// COBBColTab 대화 상자

IMPLEMENT_DYNAMIC(COBBColTab, CDialogEx)

COBBColTab::COBBColTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OBB_COLTAB, pParent)
	, m_cstrPosX(_T(""))
	, m_cstrPosY(_T(""))
	, m_cstrPosZ(_T(""))
	, m_cstrAngleX(_T(""))
	, m_cstrAngleY(_T(""))
	, m_cstrAngleZ(_T(""))
	, m_cstrScaleX(_T(""))
	, m_cstrScaleY(_T(""))
	, m_cstrScaleZ(_T(""))
{

}

COBBColTab::~COBBColTab()
{
}

void COBBColTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_cstrPosX);
	DDX_Text(pDX, IDC_EDIT2, m_cstrPosY);
	DDX_Text(pDX, IDC_EDIT3, m_cstrPosZ);
	DDX_Text(pDX, IDC_EDIT4, m_cstrAngleX);
	DDX_Text(pDX, IDC_EDIT5, m_cstrAngleY);
	DDX_Text(pDX, IDC_EDIT6, m_cstrAngleZ);
	DDX_Text(pDX, IDC_EDIT7, m_cstrScaleX);
	DDX_Text(pDX, IDC_EDIT8, m_cstrScaleY);
	DDX_Text(pDX, IDC_EDIT9, m_cstrScaleZ);
	DDX_Control(pDX, IDC_EDIT1, m_editPosX);
	DDX_Control(pDX, IDC_EDIT2, m_editPosY);
	DDX_Control(pDX, IDC_EDIT3, m_editPosZ);
	DDX_Control(pDX, IDC_EDIT4, m_editRotX);
	DDX_Control(pDX, IDC_EDIT5, m_editRotY);
	DDX_Control(pDX, IDC_EDIT6, m_editRotZ);
	DDX_Control(pDX, IDC_EDIT7, m_editScaleX);
	DDX_Control(pDX, IDC_EDIT8, m_editScaleY);
	DDX_Control(pDX, IDC_EDIT9, m_editScaleZ);
}

void COBBColTab::UpdateTab(Engine::CColliderObject_OBB * _pCollider)
{
	if (!_pCollider)
		return;

	UpdateData(TRUE);

	_vec3 vPos = _pCollider->GetTransform()->GetPos(Engine::CTransform::COORD_TYPE_LOCAL);
	_vec3 vAngle = _pCollider->GetTransform()->GetAngle();
	_vec3 vScale = _pCollider->GetTransform()->GetScale();
	m_cstrPosX.Format(L"%f", vPos.x);
	m_cstrPosY.Format(L"%f", vPos.y);
	m_cstrPosZ.Format(L"%f", vPos.z);
	m_cstrAngleX.Format(L"%f", D3DXToDegree(vAngle.x));
	m_cstrAngleY.Format(L"%f", D3DXToDegree(vAngle.x));
	m_cstrAngleZ.Format(L"%f", D3DXToDegree(vAngle.x));
	m_cstrScaleX.Format(L"%f", vScale.x);
	m_cstrScaleY.Format(L"%f", vScale.y);
	m_cstrScaleZ.Format(L"%f", vScale.z);

	UpdateData(FALSE);
}

void COBBColTab::EnableTab(_bool _bEnable)
{
	m_editPosX.EnableWindow(_bEnable);
	m_editPosY.EnableWindow(_bEnable);
	m_editPosZ.EnableWindow(_bEnable);
	m_editRotX.EnableWindow(_bEnable);
	m_editRotY.EnableWindow(_bEnable);
	m_editRotZ.EnableWindow(_bEnable);
	m_editScaleX.EnableWindow(_bEnable);
	m_editScaleY.EnableWindow(_bEnable);
	m_editScaleZ.EnableWindow(_bEnable);
}


BEGIN_MESSAGE_MAP(COBBColTab, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &COBBColTab::OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT2, &COBBColTab::OnEnChangeEditPosY)
	ON_EN_CHANGE(IDC_EDIT3, &COBBColTab::OnEnChangeEditPosZ)
	ON_EN_CHANGE(IDC_EDIT4, &COBBColTab::OnEnChangeEditRotX)
	ON_EN_CHANGE(IDC_EDIT5, &COBBColTab::OnEnChangeEditRotY)
	ON_EN_CHANGE(IDC_EDIT6, &COBBColTab::OnEnChangeEditRotZ)
	ON_EN_CHANGE(IDC_EDIT7, &COBBColTab::OnEnChangeEditScaleX)
	ON_EN_CHANGE(IDC_EDIT8, &COBBColTab::OnEnChangeEditScaleY)
	ON_EN_CHANGE(IDC_EDIT9, &COBBColTab::OnEnChangeEditScaleZ)
END_MESSAGE_MAP()


// COBBColTab 메시지 처리기


void COBBColTab::OnEnChangeEditPosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_OBB* pCollider = dynamic_cast<Engine::CColliderObject_OBB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetPosX(static_cast<_float>(_ttof(m_cstrPosX)));
	}
	UpdateData(FALSE);
}


void COBBColTab::OnEnChangeEditPosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_OBB* pCollider = dynamic_cast<Engine::CColliderObject_OBB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetPosY(static_cast<_float>(_ttof(m_cstrPosY)));
	}
	UpdateData(FALSE);
}


void COBBColTab::OnEnChangeEditPosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_OBB* pCollider = dynamic_cast<Engine::CColliderObject_OBB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetPosZ(static_cast<_float>(_ttof(m_cstrPosZ)));
	}
	UpdateData(FALSE);
}


void COBBColTab::OnEnChangeEditRotX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_OBB* pCollider = dynamic_cast<Engine::CColliderObject_OBB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetAngleX(D3DXToRadian(static_cast<_float>(_ttof(m_cstrAngleX))));
	}
	UpdateData(FALSE);
}


void COBBColTab::OnEnChangeEditRotY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_OBB* pCollider = dynamic_cast<Engine::CColliderObject_OBB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetAngleY(D3DXToRadian(static_cast<_float>(_ttof(m_cstrAngleY))));
	}
	UpdateData(FALSE);
}


void COBBColTab::OnEnChangeEditRotZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_OBB* pCollider = dynamic_cast<Engine::CColliderObject_OBB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetAngleZ(D3DXToRadian(static_cast<_float>(_ttof(m_cstrAngleZ))));
	}
	UpdateData(FALSE);
}


void COBBColTab::OnEnChangeEditScaleX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_OBB* pCollider = dynamic_cast<Engine::CColliderObject_OBB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetScaleX(static_cast<_float>(_ttof(m_cstrScaleX)));
	}
	UpdateData(FALSE);
}


void COBBColTab::OnEnChangeEditScaleY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_OBB* pCollider = dynamic_cast<Engine::CColliderObject_OBB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetScaleY(static_cast<_float>(_ttof(m_cstrScaleY)));
	}
	UpdateData(FALSE);
}


void COBBColTab::OnEnChangeEditScaleZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Engine::CColliderObject_OBB* pCollider = dynamic_cast<Engine::CColliderObject_OBB*>(m_pColTab->m_pSelectedCollider);
	if (pCollider) {
		pCollider->GetTransform()->SetScaleZ(static_cast<_float>(_ttof(m_cstrScaleZ)));
	}
	UpdateData(FALSE);
}


BOOL COBBColTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	EnableTab(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
