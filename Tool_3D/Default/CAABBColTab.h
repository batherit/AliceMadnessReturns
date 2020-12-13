#pragma once


// CAABBColTab 대화 상자
class CColliderTab;
class CAABBColTab : public CDialogEx
{
	DECLARE_DYNAMIC(CAABBColTab)

public:
	CAABBColTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAABBColTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AABB_COLTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void UpdateTab(Engine::CColliderObject_AABB* _pCollider);
	void EnableTab(_bool _bEnable);

public:
	CColliderTab* m_pColTab = nullptr;
	CString m_cstrPosX;
	CString m_cstrPosY;
	CString m_cstrPosZ;
	CString m_cstrScaleX;
	CString m_cstrScaleY;
	CString m_cstrScaleZ;
	afx_msg void OnEnChangeEditPosX();
	afx_msg void OnEnChangeEditPosY();
	afx_msg void OnEnChangeEditPosZ();
	afx_msg void OnEnChangeEditScaleX();
	afx_msg void OnEnChangeEditScaleY();
	afx_msg void OnEnChangeEditScaleZ();
	CEdit m_editPosX;
	CEdit m_editPosY;
	CEdit m_editPosZ;
	CEdit m_editScaleX;
	CEdit m_editScaleY;
	CEdit m_editScaleZ;
	virtual BOOL OnInitDialog();
};
