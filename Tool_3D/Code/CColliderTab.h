#pragma once


// CColliderTab 대화 상자
BEGIN(Client)
class CDynamicObject;
END

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
	void RegisterMeshTag(Engine::MESHTYPE _eMeshType, const _tchar* _pMeshTag);

protected:
	void GenerateBoneTree(D3DXFRAME* _pFrame, HTREEITEM _treeItem = NULL);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_treeObjectList;
	HTREEITEM m_itemStatic = NULL;
	HTREEITEM m_itemDynamic = NULL;
	HTREEITEM m_hSelectedMesh = NULL;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickTreeObjectList(NMHDR *pNMHDR, LRESULT *pResult);
	CTreeCtrl m_treeBoneTree;
};
