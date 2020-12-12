#pragma once


// CAABBColTab 대화 상자

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
};
