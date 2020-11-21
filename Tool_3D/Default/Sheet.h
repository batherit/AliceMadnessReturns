#pragma once

#include "TerrainPage.h"

// Sheet

class Sheet : public CPropertySheet
{
	DECLARE_DYNAMIC(Sheet)

public:
	Sheet();
	Sheet(UINT nIDCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	Sheet(LPCTSTR pszCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	virtual ~Sheet();

protected:
	DECLARE_MESSAGE_MAP()

private:
	TerrainPage		m_tTerrainPage;
};


