#pragma once


// CTerrainPage 대화 상자

class CTerrainPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTerrainPage)

public:
	CTerrainPage();
	virtual ~CTerrainPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAIN_PAGE };
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
};
