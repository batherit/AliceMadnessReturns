#pragma once


// CSphereColTab 대화 상자
class CColliderTab;
class CSphereColTab : public CDialogEx
{
	DECLARE_DYNAMIC(CSphereColTab)

public:
	CSphereColTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSphereColTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPHERE_COLTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeEditPosX();
	afx_msg void OnEnChangeEditPosY();
	afx_msg void OnEnChangeEditPosZ();
	afx_msg void OnEnChangeEditRadius();

public:
	void UpdateTab(Engine::CColliderObject_Sphere* _pCollider);
	void EnableTab(_bool _bEnable);
	
public:
	CColliderTab* m_pColTab = nullptr;
	CString m_cstrPosX;
	CString m_cstrPosY;
	CString m_cstrPosZ;
	CString m_cstrRadius;
	CEdit m_editPosX;
	CEdit m_editPosY;
	CEdit m_editPosZ;
	CEdit m_editRadius;
	virtual BOOL OnInitDialog();
};
