#pragma once



// CTabForm 폼 보기

class CTerrainTab;
class CMeshTab;

class CTabForm : public CFormView
{
	DECLARE_DYNCREATE(CTabForm)

protected:
	CTabForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTabForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	CTerrainTab*	m_pTerrainTab;
	CMeshTab*		m_pMeshTab;
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
};


