// CMapTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CMapTab.h"
#include "EditScene.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "Gizmo.h"
#include "afxdialogex.h"


// CMapTab 대화 상자

IMPLEMENT_DYNAMIC(CMapTab, CDialogEx)

CMapTab::CMapTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAP_TAB, pParent)
	, m_cstrPosX(_T(""))
	, m_cstrPosY(_T(""))
	, m_cstrPosZ(_T(""))
	, m_cstrRotX(_T(""))
	, m_cstrRotY(_T(""))
	, m_cstrRotZ(_T(""))
	, m_cstrScaleX(_T(""))
	, m_cstrScaleY(_T(""))
	, m_cstrScaleZ(_T(""))
	, m_cstrFactor0(_T("NULL"))
	, m_cstrFactor1(_T("NULL"))
	, m_cstrFactor2(_T("NULL"))
	, m_cstrFactor3(_T("NULL"))
	, m_cstrFactor4(_T("NULL"))
	, m_cstrFactor5(_T("NULL"))
	, m_bIsCustomed(FALSE)
{

}

CMapTab::~CMapTab()
{
}

void CMapTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeObjectList);
	DDX_Control(pDX, IDC_TREE2, m_treeAddedObject);
	DDX_Control(pDX, IDC_EDIT1, m_editPosX);
	DDX_Control(pDX, IDC_EDIT3, m_editPosY);
	DDX_Control(pDX, IDC_EDIT11, m_editPosZ);
	DDX_Control(pDX, IDC_EDIT6, m_editRotX);
	DDX_Control(pDX, IDC_EDIT8, m_editRotY);
	DDX_Control(pDX, IDC_EDIT12, m_editRotZ);
	DDX_Control(pDX, IDC_EDIT7, m_editScaleX);
	DDX_Control(pDX, IDC_EDIT9, m_editScaleY);
	DDX_Control(pDX, IDC_EDIT13, m_editScaleZ);
	DDX_Control(pDX, IDC_BUTTON1, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON3, m_btnDelete);
	DDX_Control(pDX, IDC_RADIO5, m_rbtnPosition);
	DDX_Control(pDX, IDC_RADIO6, m_rbtnRotation);
	DDX_Control(pDX, IDC_RADIO7, m_rbtnScale);
	DDX_Text(pDX, IDC_EDIT1, m_cstrPosX);
	DDX_Text(pDX, IDC_EDIT3, m_cstrPosY);
	DDX_Text(pDX, IDC_EDIT11, m_cstrPosZ);
	DDX_Text(pDX, IDC_EDIT6, m_cstrRotX);
	DDX_Text(pDX, IDC_EDIT8, m_cstrRotY);
	DDX_Text(pDX, IDC_EDIT12, m_cstrRotZ);
	DDX_Text(pDX, IDC_EDIT7, m_cstrScaleX);
	DDX_Text(pDX, IDC_EDIT9, m_cstrScaleY);
	DDX_Text(pDX, IDC_EDIT13, m_cstrScaleZ);
	DDX_Text(pDX, IDC_EDIT18, m_cstrFactor0);
	DDX_Text(pDX, IDC_EDIT19, m_cstrFactor1);
	DDX_Text(pDX, IDC_EDIT20, m_cstrFactor2);
	DDX_Text(pDX, IDC_EDIT21, m_cstrFactor3);
	DDX_Text(pDX, IDC_EDIT22, m_cstrFactor4);
	DDX_Text(pDX, IDC_EDIT23, m_cstrFactor5);
	DDX_Control(pDX, IDC_CHECK4, m_btnCustomed);
	DDX_Control(pDX, IDC_EDIT18, m_editFactor0);
	DDX_Control(pDX, IDC_EDIT19, m_editFactor1);
	DDX_Control(pDX, IDC_EDIT20, m_editFactor2);
	DDX_Control(pDX, IDC_EDIT21, m_editFactor3);
	DDX_Control(pDX, IDC_EDIT22, m_editFactor4);
	DDX_Control(pDX, IDC_EDIT23, m_editFactor5);
	DDX_Check(pDX, IDC_CHECK4, m_bIsCustomed);
}


BEGIN_MESSAGE_MAP(CMapTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTab::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTab::OnBnClickedButtonDelete)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CMapTab::OnNMClickTreeObjectToAdd)
	ON_NOTIFY(NM_CLICK, IDC_TREE2, &CMapTab::OnNMClickTreeAddedObject)
	ON_EN_CHANGE(IDC_EDIT1, &CMapTab::OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT3, &CMapTab::OnEnChangeEditPosY)
	ON_EN_CHANGE(IDC_EDIT11, &CMapTab::OnEnChangeEditPosZ)
	ON_EN_CHANGE(IDC_EDIT6, &CMapTab::OnEnChangeEditRotX)
	ON_EN_CHANGE(IDC_EDIT8, &CMapTab::OnEnChangeEditRotY)
	ON_EN_CHANGE(IDC_EDIT12, &CMapTab::OnEnChangeEditRotZ)
	ON_EN_CHANGE(IDC_EDIT7, &CMapTab::OnEnChangeEditScaleX)
	ON_EN_CHANGE(IDC_EDIT9, &CMapTab::OnEnChangeEditScaleY)
	ON_EN_CHANGE(IDC_EDIT13, &CMapTab::OnEnChangeEditScaleZ)
	ON_BN_CLICKED(IDC_RADIO5, &CMapTab::OnBnClickedRadioPosition)
	ON_BN_CLICKED(IDC_RADIO6, &CMapTab::OnBnClickedRadioRotation)
	ON_BN_CLICKED(IDC_RADIO7, &CMapTab::OnBnClickedRadioScale)
	ON_BN_CLICKED(IDC_BUTTON7, &CMapTab::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON8, &CMapTab::OnBnClickedButtonLoad)
	ON_EN_CHANGE(IDC_EDIT18, &CMapTab::OnEnChangeEditFactor0)
	ON_EN_CHANGE(IDC_EDIT19, &CMapTab::OnEnChangeEditFactor1)
	ON_EN_CHANGE(IDC_EDIT20, &CMapTab::OnEnChangeEditFactor2)
	ON_EN_CHANGE(IDC_EDIT21, &CMapTab::OnEnChangeEditFactor3)
	ON_EN_CHANGE(IDC_EDIT22, &CMapTab::OnEnChangeEditFactor4)
	ON_EN_CHANGE(IDC_EDIT23, &CMapTab::OnEnChangeEditFactor5)
	ON_BN_CLICKED(IDC_CHECK4, &CMapTab::OnBnClickedCheckCustomed)
END_MESSAGE_MAP()


// CMapTab 메시지 처리기
BOOL CMapTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_rbtnPosition.SetCheck(TRUE);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 오브젝트 리스트 트리에 Static과 Custom 항목을 추가합니다.
	m_itemStaticL = m_treeObjectList.InsertItem(L"Static", NULL, NULL);
	m_itemDynamicL = m_treeObjectList.InsertItem(L"Dynamic", NULL, NULL);
	m_itemStaticR = m_treeAddedObject.InsertItem(L"Static", NULL, NULL);
	m_itemDynamicR = m_treeAddedObject.InsertItem(L"Dynamic", NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMapTab::OnBnClickedButtonAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_hSelectedTreeItem == nullptr)
		return;

	//const _tchar* pMeshTag =  m_treeObjectList.GetItemText(m_hSelectedTreeItem);
	
	auto hParent = m_treeObjectList.GetParentItem(m_hSelectedTreeItem);
	
	if (hParent) {
		CString pMeshTag = m_treeObjectList.GetItemText(m_hSelectedTreeItem);
		// 부모가 있다면, 
		if (m_treeObjectList.GetItemText(hParent) == L"Static") {
			AddStaticObject(pMeshTag);
		}
		else if (m_treeObjectList.GetItemText(hParent) == L"Dynamic") {
			AddDynamicObject(pMeshTag);
		}
	}
}


void CMapTab::OnBnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	// Delete버튼이 눌렸다는 것은 제거할 오브젝트가 선택되었다는 뜻.
	if (!m_pSelectedObject)
		return;

	UpdateData(TRUE);
	m_pSelectedObject->SetValid(false);
	HTREEITEM hItem = NULL;

	if (dynamic_cast<CStaticObject*>(m_pSelectedObject)) {
		g_pTool3D_Kernel->GetEditScene()->DeleteStaticObject(m_iSelectedObjectIndex);
		if (m_treeAddedObject.ItemHasChildren(m_itemStaticR)) {
			hItem = m_treeAddedObject.GetChildItem(m_itemStaticR);

			for (int i = 0; i < m_iSelectedObjectIndex; ++i) {
				hItem = m_treeAddedObject.GetNextSiblingItem(hItem);
			}
		}
	}
	else if (dynamic_cast<CDynamicObject*>(m_pSelectedObject)) {
		g_pTool3D_Kernel->GetEditScene()->DeleteDynamicObject(m_iSelectedObjectIndex);
		if (m_treeAddedObject.ItemHasChildren(m_itemDynamicR)) {
			hItem = m_treeAddedObject.GetChildItem(m_itemDynamicR);

			for (int i = 0; i < m_iSelectedObjectIndex; ++i) {
				hItem = m_treeAddedObject.GetNextSiblingItem(hItem);
			}
		}
	}
	else
		return;

	if(hItem)
		m_treeAddedObject.DeleteItem(hItem);

	m_editPosX.EnableWindow(FALSE);
	m_editPosY.EnableWindow(FALSE);
	m_editPosZ.EnableWindow(FALSE);
	m_editRotX.EnableWindow(FALSE);
	m_editRotY.EnableWindow(FALSE);
	m_editRotZ.EnableWindow(FALSE);
	m_editScaleX.EnableWindow(FALSE);
	m_editScaleY.EnableWindow(FALSE);
	m_editScaleZ.EnableWindow(FALSE);
	m_btnAdd.EnableWindow(FALSE);
	m_btnDelete.EnableWindow(FALSE);

	m_btnCustomed.EnableWindow(FALSE);
	m_editFactor0.EnableWindow(FALSE);
	m_editFactor1.EnableWindow(FALSE);
	m_editFactor2.EnableWindow(FALSE);
	m_editFactor3.EnableWindow(FALSE);
	m_editFactor4.EnableWindow(FALSE);
	m_editFactor5.EnableWindow(FALSE);

	m_pSelectedObject = nullptr;
	m_iSelectedObjectIndex = -1;

	CGizmo* pGizmo = g_pTool3D_Kernel->GetEditScene()->GetGizmo();
	pGizmo->SetObject(nullptr);
	pGizmo->ActivateGizmo(false);

	UpdateData(FALSE);
}


void CMapTab::OnNMClickTreeObjectToAdd(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	POINT point = Engine::GetClientCursorPoint(m_treeObjectList.m_hWnd);
	UINT nFlags = 0;
	HTREEITEM hItem = m_treeObjectList.HitTest(point, &nFlags);

	if (!hItem)
		return;

	auto hParent = m_treeObjectList.GetParentItem(hItem);

	if (hParent) {
		m_hSelectedTreeItem = hItem;
	}
	else
		return;

	UpdateData(TRUE);
	m_editPosX.EnableWindow(FALSE);
	m_editPosY.EnableWindow(FALSE);
	m_editPosZ.EnableWindow(FALSE);
	m_editRotX.EnableWindow(FALSE);
	m_editRotY.EnableWindow(FALSE);
	m_editRotZ.EnableWindow(FALSE);
	m_editScaleX.EnableWindow(FALSE);
	m_editScaleY.EnableWindow(FALSE);
	m_editScaleZ.EnableWindow(FALSE);
	m_btnAdd.EnableWindow(TRUE);
	m_btnDelete.EnableWindow(FALSE);

	m_btnCustomed.EnableWindow(FALSE);
	m_editFactor0.EnableWindow(FALSE);
	m_editFactor1.EnableWindow(FALSE);
	m_editFactor2.EnableWindow(FALSE);
	m_editFactor3.EnableWindow(FALSE);
	m_editFactor4.EnableWindow(FALSE);
	m_editFactor5.EnableWindow(FALSE);

	CGizmo* pGizmo = g_pTool3D_Kernel->GetEditScene()->GetGizmo();
	pGizmo->SetObject(nullptr);
	pGizmo->ActivateGizmo(false);

	m_pSelectedObject = nullptr;
	m_iSelectedObjectIndex = -1;
	UpdateData(FALSE);

	*pResult = 0;
}

void CMapTab::OnNMClickTreeAddedObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	POINT point = Engine::GetClientCursorPoint(m_treeAddedObject.m_hWnd);
	UINT nFlags = 0;
	HTREEITEM hItem = m_treeAddedObject.HitTest(point, &nFlags);

	if (!hItem)
		return;

	auto hParent = m_treeAddedObject.GetParentItem(hItem);

	if (!hParent)
		return;

	UpdateData(TRUE);
	HTREEITEM hSibling = NULL;
	m_iSelectedObjectIndex = 0;
	if (m_treeObjectList.GetItemText(hParent) == L"Static") {
		if (m_treeAddedObject.ItemHasChildren(m_itemStaticR)) {
			hSibling = m_treeAddedObject.GetChildItem(m_itemStaticR);

			while (hSibling)
			{
				if (hSibling == hItem) break;
				hSibling = m_treeAddedObject.GetNextSiblingItem(hSibling);
				++m_iSelectedObjectIndex;
			}

			m_pSelectedObject = g_pTool3D_Kernel->GetEditScene()->GetStaticObject(m_iSelectedObjectIndex);
		}
	}
	else if (m_treeObjectList.GetItemText(hParent) == L"Dynamic") {
		if (m_treeAddedObject.ItemHasChildren(m_itemDynamicR)) {
			hSibling = m_treeAddedObject.GetChildItem(m_itemDynamicR);

			while (hSibling)
			{
				if (hSibling == hItem) break;
				hSibling = m_treeAddedObject.GetNextSiblingItem(hSibling);
				++m_iSelectedObjectIndex;
			}

			m_pSelectedObject = g_pTool3D_Kernel->GetEditScene()->GetDynamicObject(m_iSelectedObjectIndex);
		}
	}
	m_hSelectedTreeItem = hItem;

	
	
	CGizmo* pGizmo = g_pTool3D_Kernel->GetEditScene()->GetGizmo();

	if (m_pSelectedObject) {
		pGizmo->SetObject(m_pSelectedObject);
		pGizmo->ActivateGizmo(true);

		_vec3 vPos = m_pSelectedObject->GetTransform()->GetPos();
		_vec3 vAngle = m_pSelectedObject->GetTransform()->GetAngle();
		_vec3 vScale = m_pSelectedObject->GetTransform()->GetScale();

		CString strValue = L"";
		// 위치 
		strValue.Format(_T("%f"), vPos.x);
		m_editPosX.SetWindowTextW(strValue);
		strValue.Format(_T("%f"), vPos.y);
		m_editPosY.SetWindowTextW(strValue);
		strValue.Format(_T("%f"), vPos.z);
		m_editPosZ.SetWindowTextW(strValue);

		// 회전
		strValue.Format(_T("%f"), D3DXToDegree(vAngle.x));
		m_editRotX.SetWindowTextW(strValue);
		strValue.Format(_T("%f"), D3DXToDegree(vAngle.y));
		m_editRotY.SetWindowTextW(strValue);
		strValue.Format(_T("%f"), D3DXToDegree(vAngle.z));
		m_editRotZ.SetWindowTextW(strValue);

		// 스케일
		strValue.Format(_T("%f"), vScale.x);
		m_editScaleX.SetWindowTextW(strValue);
		strValue.Format(_T("%f"), vScale.y);
		m_editScaleY.SetWindowTextW(strValue);
		strValue.Format(_T("%f"), vScale.z);
		m_editScaleZ.SetWindowTextW(strValue);

		m_editPosX.EnableWindow(TRUE);
		m_editPosY.EnableWindow(TRUE);
		m_editPosZ.EnableWindow(TRUE);
		m_editRotX.EnableWindow(TRUE);
		m_editRotY.EnableWindow(TRUE);
		m_editRotZ.EnableWindow(TRUE);
		m_editScaleX.EnableWindow(TRUE);
		m_editScaleY.EnableWindow(TRUE);
		m_editScaleZ.EnableWindow(TRUE);
		m_btnAdd.EnableWindow(FALSE);
		m_btnDelete.EnableWindow(TRUE);
		m_btnCustomed.EnableWindow(TRUE);


		// 커스텀 상태
		if (dynamic_cast<CStaticObject*>(m_pSelectedObject)) {
			CStaticObject* pStaticObject = dynamic_cast<CStaticObject*>(m_pSelectedObject);
			m_bIsCustomed = pStaticObject->IsCustomed();

			if (m_bIsCustomed) {
				m_editFactor0.EnableWindow(TRUE);
				m_editFactor1.EnableWindow(TRUE);
				m_editFactor2.EnableWindow(TRUE);
				m_editFactor3.EnableWindow(TRUE);
				m_editFactor4.EnableWindow(TRUE);
				m_editFactor5.EnableWindow(TRUE);

				m_cstrFactor0 = pStaticObject->GetFactorRef(0);
				m_cstrFactor1 = pStaticObject->GetFactorRef(1);
				m_cstrFactor2 = pStaticObject->GetFactorRef(2);
				m_cstrFactor3 = pStaticObject->GetFactorRef(3);
				m_cstrFactor4 = pStaticObject->GetFactorRef(4);
				m_cstrFactor5 = pStaticObject->GetFactorRef(5);
			}
		}
		else if (dynamic_cast<CDynamicObject*>(m_pSelectedObject)) {
			CDynamicObject* pDynamicObject = dynamic_cast<CDynamicObject*>(m_pSelectedObject);
			m_bIsCustomed = pDynamicObject->IsCustomed();
			//m_btnCustomed.SetCheck(pDynamicObject->IsCustomed());

			if (m_bIsCustomed) {
				m_editFactor0.EnableWindow(TRUE);
				m_editFactor1.EnableWindow(TRUE);
				m_editFactor2.EnableWindow(TRUE);
				m_editFactor3.EnableWindow(TRUE);
				m_editFactor4.EnableWindow(TRUE);
				m_editFactor5.EnableWindow(TRUE);

				m_cstrFactor0 = pDynamicObject->GetFactorRef(0);
				m_cstrFactor1 = pDynamicObject->GetFactorRef(1);
				m_cstrFactor2 = pDynamicObject->GetFactorRef(2);
				m_cstrFactor3 = pDynamicObject->GetFactorRef(3);
				m_cstrFactor4 = pDynamicObject->GetFactorRef(4);
				m_cstrFactor5 = pDynamicObject->GetFactorRef(5);
			}
		}

		// 커스텀 상태가 체크된 상태라면,
		if(!m_bIsCustomed) {
			m_editFactor0.EnableWindow(FALSE);
			m_editFactor1.EnableWindow(FALSE);
			m_editFactor2.EnableWindow(FALSE);
			m_editFactor3.EnableWindow(FALSE);
			m_editFactor4.EnableWindow(FALSE);
			m_editFactor5.EnableWindow(FALSE);

			m_cstrFactor0 = L"NULL";
			m_cstrFactor1 = L"NULL";
			m_cstrFactor2 = L"NULL";
			m_cstrFactor3 = L"NULL";
			m_cstrFactor4 = L"NULL";
			m_cstrFactor5 = L"NULL";
		}
	}
	else {
		pGizmo->SetObject(nullptr);
		pGizmo->ActivateGizmo(false);
		m_pSelectedObject = nullptr;
		m_iSelectedObjectIndex = -1;
	}
	UpdateData(FALSE);

	*pResult = 0;
}

void CMapTab::UpdateAddedTree(const vector<CStaticObject*>& rStaticObjects, const vector<CDynamicObject*>& rDynamicObjects)
{
	UpdateData(TRUE);

	m_treeAddedObject.DeleteAllItems();
	m_itemStaticR = m_treeAddedObject.InsertItem(L"Static", NULL, NULL);
	m_itemDynamicR = m_treeAddedObject.InsertItem(L"Dynamic", NULL, NULL);

	for (auto& rStaticObj : rStaticObjects) {
		if (rStaticObj->IsValid()) {
			m_treeAddedObject.InsertItem(rStaticObj->GetMeshTag() , m_itemStaticR, NULL);
		}
	}
	for (auto& rDynamicObj : rDynamicObjects) {
		if (rDynamicObj->IsValid()) {
			m_treeAddedObject.InsertItem(rDynamicObj->GetMeshTag(), m_itemDynamicR, NULL);
		}
	}

	UpdateData(FALSE);
}

void CMapTab::UpdatePos(const _vec3 & vPos)
{
	UpdateData(TRUE);
	CString strValue = L"";
	strValue.Format(_T("%f"), vPos.x);
	m_editPosX.SetWindowTextW(strValue);
	strValue.Format(_T("%f"), vPos.y);
	m_editPosY.SetWindowTextW(strValue);
	strValue.Format(_T("%f"), vPos.z);
	m_editPosZ.SetWindowTextW(strValue);
	UpdateData(FALSE);
}

void CMapTab::UpdateAngle(const _vec3 & vAngle)
{
	UpdateData(TRUE);
	CString strValue = L"";
	strValue.Format(_T("%f"), D3DXToDegree(vAngle.x));
	m_editRotX.SetWindowTextW(strValue);
	strValue.Format(_T("%f"), D3DXToDegree(vAngle.y));
	m_editRotY.SetWindowTextW(strValue);
	strValue.Format(_T("%f"), D3DXToDegree(vAngle.z));
	m_editRotZ.SetWindowTextW(strValue);
	UpdateData(FALSE);
}

void CMapTab::UpdateScale(const _vec3 & vScale)
{
	UpdateData(TRUE);
	CString strValue = L"";
	strValue.Format(_T("%f"), vScale.x);
	m_editScaleX.SetWindowTextW(strValue);
	strValue.Format(_T("%f"), vScale.y);
	m_editScaleY.SetWindowTextW(strValue);
	strValue.Format(_T("%f"), vScale.z);
	m_editScaleZ.SetWindowTextW(strValue);
	UpdateData(FALSE);
}

void CMapTab::SetSelectedObject(Engine::CGameObject* _pObject)
{
	if (!_pObject)
		return;

	m_pSelectedObject = _pObject;

	if (dynamic_cast<CStaticObject*>(_pObject)) {
		auto& rStaticObjectList = g_pTool3D_Kernel->GetEditScene()->GetStaticObjectList();

		m_iSelectedObjectIndex = 0;
		for (auto& rObj : rStaticObjectList) {
			if (rObj == _pObject) {
				break;
			}
			++m_iSelectedObjectIndex;
		}

		if (m_iSelectedObjectIndex == rStaticObjectList.size()) {
			m_iSelectedObjectIndex = -1;
			abort();
			return;
		}
	}
	else if (dynamic_cast<CDynamicObject*>(_pObject)) {
		auto& rCustomObjectList = g_pTool3D_Kernel->GetEditScene()->GetDynamicObjectList();

		m_iSelectedObjectIndex = 0;
		for (auto& rObj : rCustomObjectList) {
			if (rObj == _pObject) {
				break;
			}
			++m_iSelectedObjectIndex;
		}

		if (m_iSelectedObjectIndex == rCustomObjectList.size()) {
			m_iSelectedObjectIndex = -1;
			abort();
			return;
		}
	}
	else
		return;

	m_editPosX.EnableWindow(TRUE);
	m_editPosY.EnableWindow(TRUE);
	m_editPosZ.EnableWindow(TRUE);
	m_editRotX.EnableWindow(TRUE);
	m_editRotY.EnableWindow(TRUE);
	m_editRotZ.EnableWindow(TRUE);
	m_editScaleX.EnableWindow(TRUE);
	m_editScaleY.EnableWindow(TRUE);
	m_editScaleZ.EnableWindow(TRUE);
	m_btnAdd.EnableWindow(FALSE);
	m_btnDelete.EnableWindow(TRUE);
	m_btnCustomed.EnableWindow(TRUE);
}

_bool CMapTab::AddStaticObject(const _tchar * _pMeshTag)
{
	if (g_pTool3D_Kernel->GetEditScene()->AddStaticObject(_pMeshTag)) {
		m_treeAddedObject.InsertItem(_pMeshTag, m_itemStaticR, NULL);
		return true;
	}
	return false;
}

_bool CMapTab::AddDynamicObject(const _tchar * _pMeshTag)
{
	if (g_pTool3D_Kernel->GetEditScene()->AddDynamicObject(_pMeshTag)) {
		m_treeAddedObject.InsertItem(_pMeshTag, m_itemDynamicR, NULL);
		return true;
	}
	return false;
}

void CMapTab::RegisterMeshTag(Engine::MESHTYPE _eMeshType, const _tchar * _pMeshTag)
{
	switch (_eMeshType) {
	case Engine::TYPE_STATIC:
		m_treeObjectList.InsertItem(_pMeshTag, m_itemStaticL, NULL);
		break;
	case Engine::TYPE_DYNAMIC:
		m_treeObjectList.InsertItem(_pMeshTag, m_itemDynamicL, NULL);
		break;
	default:
		break;
	}
}

void CMapTab::OnEnChangeEditPosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strPosX;
	m_editPosX.GetWindowTextW(strPosX);
	_float fPosX = static_cast<_float>(_tstof(strPosX));
	m_pSelectedObject->GetTransform()->SetPosX(fPosX);

	g_pTool3D_Kernel->GetEditScene()->GetGizmo()->GetTransform()->SetPos(m_pSelectedObject->GetTransform()->GetPos());
	
	UpdateData(FALSE);
}


void CMapTab::OnEnChangeEditPosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	CString strPosY;
	m_editPosY.GetWindowTextW(strPosY);
	_float fPosY = static_cast<_float>(_tstof(strPosY));
	m_pSelectedObject->GetTransform()->SetPosY(fPosY);

	g_pTool3D_Kernel->GetEditScene()->GetGizmo()->GetTransform()->SetPos(m_pSelectedObject->GetTransform()->GetPos());
	
	UpdateData(FALSE);
}


void CMapTab::OnEnChangeEditPosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strPosZ;
	m_editPosZ.GetWindowTextW(strPosZ);
	_float fPosZ = static_cast<_float>(_tstof(strPosZ));
	m_pSelectedObject->GetTransform()->SetPosZ(fPosZ);

	g_pTool3D_Kernel->GetEditScene()->GetGizmo()->GetTransform()->SetPos(m_pSelectedObject->GetTransform()->GetPos());

	UpdateData(FALSE);
}


void CMapTab::OnEnChangeEditRotX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strRotX;
	m_editRotX.GetWindowTextW(strRotX);
	_float fRotX = static_cast<_float>(_tstof(strRotX));
	m_pSelectedObject->GetTransform()->SetAngleX(D3DXToRadian(fRotX));

	UpdateData(FALSE);
}


void CMapTab::OnEnChangeEditRotY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strRotY;
	m_editRotY.GetWindowTextW(strRotY);
	_float fRotY = static_cast<_float>(_tstof(strRotY));
	m_pSelectedObject->GetTransform()->SetAngleY(D3DXToRadian(fRotY));

	UpdateData(FALSE);
}


void CMapTab::OnEnChangeEditRotZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strRotZ;
	m_editRotZ.GetWindowTextW(strRotZ);
	_float fRotZ = static_cast<_float>(_tstof(strRotZ));
	m_pSelectedObject->GetTransform()->SetAngleZ(D3DXToRadian(fRotZ));

	UpdateData(FALSE);
}


void CMapTab::OnEnChangeEditScaleX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strScaleX;
	m_editScaleX.GetWindowTextW(strScaleX);
	_float fScaleX = static_cast<_float>(_tstof(strScaleX));
	m_pSelectedObject->GetTransform()->SetScaleX(fScaleX);

	UpdateData(FALSE);
}


void CMapTab::OnEnChangeEditScaleY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strScaleY;
	m_editScaleY.GetWindowTextW(strScaleY);
	_float fScaleY = static_cast<_float>(_tstof(strScaleY));
	m_pSelectedObject->GetTransform()->SetScaleY(fScaleY);

	UpdateData(FALSE);
}


void CMapTab::OnEnChangeEditScaleZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CString strScaleZ;
	m_editScaleZ.GetWindowTextW(strScaleZ);
	_float fScaleZ = static_cast<_float>(_tstof(strScaleZ));
	m_pSelectedObject->GetTransform()->SetScaleZ(fScaleZ);

	UpdateData(FALSE);
}




void CMapTab::OnBnClickedRadioPosition()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_pTool3D_Kernel->GetEditScene()->GetGizmo()->SetGizmoMode(MAP_TAB::MODE_POSITION);
}

void CMapTab::OnBnClickedRadioRotation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_pTool3D_Kernel->GetEditScene()->GetGizmo()->SetGizmoMode(MAP_TAB::MODE_ROTATION);
}

void CMapTab::OnBnClickedRadioScale()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_pTool3D_Kernel->GetEditScene()->GetGizmo()->SetGizmoMode(MAP_TAB::MODE_SCALE);
}




void CMapTab::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_pTool3D_Kernel->GetEditScene()->SaveMap();
}


void CMapTab::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_pTool3D_Kernel->GetEditScene()->LoadMap();
	UpdateAddedTree(g_pTool3D_Kernel->GetEditScene()->GetStaticObjectList(), g_pTool3D_Kernel->GetEditScene()->GetDynamicObjectList());
}

void CMapTab::OnEnChangeEditFactor0()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (dynamic_cast<CStaticObject*>(m_pSelectedObject)) {
		dynamic_cast<CStaticObject*>(m_pSelectedObject)->GetFactorRef(0) = m_cstrFactor0;
	}
	else if (dynamic_cast<CDynamicObject*>(m_pSelectedObject)) {
		dynamic_cast<CDynamicObject*>(m_pSelectedObject)->GetFactorRef(0) = m_cstrFactor0;
	}

	UpdateData(FALSE);
}


void CMapTab::OnEnChangeEditFactor1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (dynamic_cast<CStaticObject*>(m_pSelectedObject)) {
		dynamic_cast<CStaticObject*>(m_pSelectedObject)->GetFactorRef(1) = m_cstrFactor1;
	}
	else if (dynamic_cast<CDynamicObject*>(m_pSelectedObject)) {
		dynamic_cast<CDynamicObject*>(m_pSelectedObject)->GetFactorRef(1) = m_cstrFactor1;
	}

	UpdateData(FALSE);
}


void CMapTab::OnEnChangeEditFactor2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (dynamic_cast<CStaticObject*>(m_pSelectedObject)) {
		dynamic_cast<CStaticObject*>(m_pSelectedObject)->GetFactorRef(2) = m_cstrFactor2;
	}
	else if (dynamic_cast<CDynamicObject*>(m_pSelectedObject)) {
		dynamic_cast<CDynamicObject*>(m_pSelectedObject)->GetFactorRef(2) = m_cstrFactor2;
	}

	UpdateData(FALSE);
}


void CMapTab::OnEnChangeEditFactor3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (dynamic_cast<CStaticObject*>(m_pSelectedObject)) {
		dynamic_cast<CStaticObject*>(m_pSelectedObject)->GetFactorRef(3) = m_cstrFactor3;
	}
	else if (dynamic_cast<CDynamicObject*>(m_pSelectedObject)) {
		dynamic_cast<CDynamicObject*>(m_pSelectedObject)->GetFactorRef(3) = m_cstrFactor3;
	}

	UpdateData(FALSE);
}


void CMapTab::OnEnChangeEditFactor4()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (dynamic_cast<CStaticObject*>(m_pSelectedObject)) {
		dynamic_cast<CStaticObject*>(m_pSelectedObject)->GetFactorRef(4) = m_cstrFactor4;
	}
	else if (dynamic_cast<CDynamicObject*>(m_pSelectedObject)) {
		dynamic_cast<CDynamicObject*>(m_pSelectedObject)->GetFactorRef(4) = m_cstrFactor4;
	}

	UpdateData(FALSE);
}



void CMapTab::OnEnChangeEditFactor5()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (dynamic_cast<CStaticObject*>(m_pSelectedObject)) {
		dynamic_cast<CStaticObject*>(m_pSelectedObject)->GetFactorRef(5) = m_cstrFactor5;
	}
	else if (dynamic_cast<CDynamicObject*>(m_pSelectedObject)) {
		dynamic_cast<CDynamicObject*>(m_pSelectedObject)->GetFactorRef(5) = m_cstrFactor5;
	}

	UpdateData(FALSE);
}


void CMapTab::OnBnClickedCheckCustomed()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (dynamic_cast<CStaticObject*>(m_pSelectedObject)) {
		CStaticObject* pStaticObject = dynamic_cast<CStaticObject*>(m_pSelectedObject);
		pStaticObject->SetCustomed(m_bIsCustomed);
		if (m_bIsCustomed) {
			m_editFactor0.EnableWindow(TRUE);
			m_editFactor1.EnableWindow(TRUE);
			m_editFactor2.EnableWindow(TRUE);
			m_editFactor3.EnableWindow(TRUE);
			m_editFactor4.EnableWindow(TRUE);
			m_editFactor5.EnableWindow(TRUE);

			m_cstrFactor0 = pStaticObject->GetFactorRef(0);
			m_cstrFactor1 = pStaticObject->GetFactorRef(1);
			m_cstrFactor2 = pStaticObject->GetFactorRef(2);
			m_cstrFactor3 = pStaticObject->GetFactorRef(3);
			m_cstrFactor4 = pStaticObject->GetFactorRef(4);
			m_cstrFactor5 = pStaticObject->GetFactorRef(5);
		}
	}
	else if (dynamic_cast<CDynamicObject*>(m_pSelectedObject)) {
		CDynamicObject* pDynamicObject = dynamic_cast<CDynamicObject*>(m_pSelectedObject);
		pDynamicObject->SetCustomed(m_bIsCustomed);
		if (m_bIsCustomed) {
			m_editFactor0.EnableWindow(TRUE);
			m_editFactor1.EnableWindow(TRUE);
			m_editFactor2.EnableWindow(TRUE);
			m_editFactor3.EnableWindow(TRUE);
			m_editFactor4.EnableWindow(TRUE);
			m_editFactor5.EnableWindow(TRUE);

			m_cstrFactor0 = pDynamicObject->GetFactorRef(0);
			m_cstrFactor1 = pDynamicObject->GetFactorRef(1);
			m_cstrFactor2 = pDynamicObject->GetFactorRef(2);
			m_cstrFactor3 = pDynamicObject->GetFactorRef(3);
			m_cstrFactor4 = pDynamicObject->GetFactorRef(4);
			m_cstrFactor5 = pDynamicObject->GetFactorRef(5);
		}
	}

	// 커스텀 상태가 체크된 상태라면,
	if(!m_bIsCustomed) {
		m_editFactor0.EnableWindow(FALSE);
		m_editFactor1.EnableWindow(FALSE);
		m_editFactor2.EnableWindow(FALSE);
		m_editFactor3.EnableWindow(FALSE);
		m_editFactor4.EnableWindow(FALSE);
		m_editFactor5.EnableWindow(FALSE);

		m_cstrFactor0 = L"NULL";
		m_cstrFactor1 = L"NULL";
		m_cstrFactor2 = L"NULL";
		m_cstrFactor3 = L"NULL";
		m_cstrFactor4 = L"NULL";
		m_cstrFactor5 = L"NULL";
	}

	UpdateData(FALSE);
}
