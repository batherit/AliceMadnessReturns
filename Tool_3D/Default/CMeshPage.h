#pragma once


// CMeshPage 대화 상자

class CMeshPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMeshPage)

public:
	CMeshPage();
	virtual ~CMeshPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESH_PAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CTreeCtrl m_treeNavi;
public:
	virtual BOOL OnInitDialog();
};
