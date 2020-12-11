#pragma once


// CSphereColTab 대화 상자

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
};
