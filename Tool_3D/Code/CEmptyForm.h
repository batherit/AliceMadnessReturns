#pragma once



// CEmptyForm 폼 보기
class Sheet;

class CEmptyForm : public CFormView
{
	DECLARE_DYNCREATE(CEmptyForm)

protected:
	CEmptyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CEmptyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMPTY_FORM };
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

private:
	Sheet* m_pSheet = nullptr;

public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
};


