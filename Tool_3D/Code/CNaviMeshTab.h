#pragma once


// CNaviMeshTab 대화 상자
BEGIN(Client)
class CNaviMesh;
END

class CNaviMeshTab : public CDialogEx
{
	DECLARE_DYNAMIC(CNaviMeshTab)

public:
	CNaviMeshTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CNaviMeshTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVI_MESH_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void UpdateNaviTree(Client::CNaviMesh* _pNaviMesh);

public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_treeNavi;
	HTREEITEM m_hSelectedTreeItem = nullptr;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_btnDelete;
	afx_msg void OnBnClickedButtonDelete();
	_vec3 m_vVertexPos;
	CEdit m_editPosX;
	CEdit m_editPosY;
	CEdit m_editPosZ;
	afx_msg void OnEnChangeEditPosX();
	afx_msg void OnEnChangeEditPosY();
	afx_msg void OnEnChangeEditPosZ();
};
