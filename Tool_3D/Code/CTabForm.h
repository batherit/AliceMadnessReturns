#pragma once



// CTabForm 폼 보기
class CTerrainTab;
class CNaviMeshTab;
class CMapTab;
class CColliderTab;

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
	enum E_TAB_TYPE { TYPE_TERRAIN, TYPE_NAVI, TYPE_MAP, TYPE_COLLIDER, TYPE_END };
	void ActivateTab(const E_TAB_TYPE& _eTabType);
	CTerrainTab* GetTerrainTab() const		{ return m_pTerrainTab; }
	CNaviMeshTab* GetNaviMeshTab() const	{ return m_pNaviMeshTab; }
	CMapTab* GetMapTab() const { return m_pMapTab; }
	CColliderTab* GetColliderTab() const { return m_pColliderTab; }
	
public:
	CTabCtrl		m_Tab;
	CTerrainTab*	m_pTerrainTab = nullptr;
	CNaviMeshTab*	m_pNaviMeshTab = nullptr;
	CMapTab*		m_pMapTab = nullptr;
	CColliderTab*	m_pColliderTab = nullptr;
	vector<CDialogEx*> m_vecTabs;

	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
};


