﻿#pragma once


// CMapTab 대화 상자

class CMapTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMapTab)

public:
	CMapTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMapTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAP_TAB };
#endif

public:
	CTreeCtrl m_treeObjectList;
	CTreeCtrl m_treeAddedObject;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnNMClickTreeObjectToAdd(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	HTREEITEM m_hSelectedTreeItem = nullptr;
	
};
