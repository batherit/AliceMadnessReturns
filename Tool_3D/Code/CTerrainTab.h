#pragma once


// CTerrainTab 대화 상자

class CTerrainTab : public CDialogEx
{
	DECLARE_DYNAMIC(CTerrainTab)

public:
	CTerrainTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTerrainTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAIN_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeEditPosX();
	afx_msg void OnEnChangeEditPosY();
	afx_msg void OnEnChangeEditPosZ();
	afx_msg void OnEnChangeEditRotX();
	afx_msg void OnEnChangeEditRotY();
	afx_msg void OnEnChangeEditRotZ();
	afx_msg void OnEnChangeEditScaleX();
	afx_msg void OnEnChangeEditScaleY();
	afx_msg void OnEnChangeEditScaleZ();

private:
	_vec3 m_vPos{ 0.f, 0.f, 0.f };
	_vec3 m_vAngle{ 0.f, 0.f, 0.f };
	_vec3 m_vScale{ 1.f, 1.f, 1.f };

public:
	CListBox m_lbxTexture;
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditDepth();
	afx_msg void OnEnChangeEditVtxNumWidth();
	afx_msg void OnEnChangeEditVtxNumDepth();
	afx_msg void OnEnChangeEditU();
	afx_msg void OnEnChangeEditV();
	_float m_fSizeWidth;
	_float m_fSizeDepth;
	_uint m_uiVtxNumWidth;
	_uint m_uiVtxNumHeight;
	_float m_fU;
	_float m_fV;
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListTexture();
};
