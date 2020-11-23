#pragma once



// CTabForm 폼 보기

class CTerrainTab;
class CNaviMeshTab;

BEGIN(Client)
class CNaviMesh;
END

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
	CTerrainTab* GetTerrainTab() const		{ return m_pTerrainTab; }
	CNaviMeshTab* GetNaviMeshTab() const	{ return m_pNaviMeshTab; }
	
public:
	CTabCtrl		m_Tab;
	CTerrainTab*	m_pTerrainTab = nullptr;
	CNaviMeshTab*	m_pNaviMeshTab = nullptr;

	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
};


