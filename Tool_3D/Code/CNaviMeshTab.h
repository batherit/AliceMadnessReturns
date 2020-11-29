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
	//afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnEnChangeEditPosX();
	afx_msg void OnEnChangeEditPosY();
	afx_msg void OnEnChangeEditPosZ();
	afx_msg void OnBnClickedButtonCombine();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedCheckGroup();
	afx_msg void OnEnChangeEditGroupRange();
	afx_msg void OnBnClickedRadioNavi();
	afx_msg void OnBnClickedCheckNaviMagnet();
	afx_msg void OnBnClickedRadioVertex();
	afx_msg void OnBnClickedRadioTriangle();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	//afx_msg void OnTvnKeydownTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);

public:
	void UpdateNaviTree(Client::CNaviMesh* _pNaviMesh);
	void UpdateVertexPos(const _vec3& _vPos);

public:
	virtual BOOL OnInitDialog();
	// 트리 UI
	CTreeCtrl m_treeNavi;
	HTREEITEM m_hSelectedTreeItem = nullptr;
	
	CButton m_btnDelete;
	CButton m_btnCombine;
	CButton m_btnCancel;
	CButton m_chkGroup;

	_vec3 m_vVertexPos;
	CEdit m_editPosX;
	CEdit m_editPosY;
	CEdit m_editPosZ;
	
	// 컴바인 기능 관련
	pair<_int, _int> m_pairLastPickedVertex = make_pair(-1, -1);
	
	BOOL m_bIsGrouping;
	
	// 그룹
	_float m_fGroupRange;
	CEdit m_editGroupRange;
	
	
	CButton m_rbtnNavi;
	CButton m_rbtnVertex;
	CButton m_rbtnTriangle;

	BOOL m_bIsNaviMagnet;
	
};
