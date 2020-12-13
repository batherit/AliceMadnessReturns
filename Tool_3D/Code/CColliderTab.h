#pragma once


// CColliderTab 대화 상자
BEGIN(Client)
class CDynamicObject;
END

class CSphereColTab;
class CAABBColTab;
class COBBColTab;
class CColliderTab : public CDialogEx
{
	DECLARE_DYNAMIC(CColliderTab)

public:
	CColliderTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CColliderTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDER_TAB };
#endif

public:
	void UpdateBoneTree(CDynamicObject* _pDynamicObject);
	void UpdateAttachedColliders(CDynamicObject* _pDynamicObject);
	void RegisterMeshTag(Engine::MESHTYPE _eMeshType, const _tchar* _pMeshTag);

protected:
	void GenerateBoneTree(D3DXFRAME* _pFrame, HTREEITEM _treeItem = NULL);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNMClickTreeObjectList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTreeBoneTree(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonAdd();

public:
	void ActivateColTab(const Engine::E_COLLIDER_TYPE _eColTabType);
	void UpdateColliderTag(Engine::CColliderObject* _pCollider);

public:
	CTreeCtrl m_treeObjectList;
	HTREEITEM m_itemStatic = NULL;
	HTREEITEM m_itemDynamic = NULL;
	HTREEITEM m_hSelectedMesh = NULL;
	HTREEITEM m_hSelectedBone = NULL;
	HTREEITEM m_hSelectedCollider = NULL;
	Engine::CColliderObject* m_pSelectedCollider = nullptr;
	virtual BOOL OnInitDialog();
	CTreeCtrl m_treeBoneTree;

	// 콜라이더 타입 탭
	CTabCtrl m_ColTab;
	CSphereColTab* m_pSphereColTab = nullptr;
	CAABBColTab* m_pAABBColTab = nullptr;
	COBBColTab* m_pOBBColTab = nullptr;

	vector<CDialogEx*> m_vecColTabs;
	
	CTreeCtrl m_treeColliders;
	CButton m_btnAdd;
	CButton m_btnDelete;
	afx_msg void OnTcnSelchangeColTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnNMClickTreeColliders(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_editColliderTag;
	CString m_cstrColliderTag;
	afx_msg void OnEnChangeEditColliderTag();
};
