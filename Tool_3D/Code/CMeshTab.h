#pragma once


// CMeshTab 대화 상자
BEGIN(Client)
class CNaviMesh;
END

class CMeshTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMeshTab)

public:
	CMeshTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMeshTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESH_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void UpdateNaviTree(Client::CNaviMesh* _pNaviMesh);

public:
	virtual BOOL OnInitDialog();
	CButton m_rbtnNavi;
	CButton m_rbtnObject;
	CTreeCtrl m_treeNavi;
	_int m_iPickedRadioBtnIndex;
	afx_msg void OnBnClickedRadioNavi();
	afx_msg void OnBnClickedRadioObject();
};
