#pragma once


// COBBColTab 대화 상자

class COBBColTab : public CDialogEx
{
	DECLARE_DYNAMIC(COBBColTab)

public:
	COBBColTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~COBBColTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBB_COLTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
