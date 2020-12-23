// CTerrainTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CTerrainTab.h"
#include "Terrain.h"
#include "afxdialogex.h"
#include "EditScene.h"


// CTerrainTab 대화 상자

IMPLEMENT_DYNAMIC(CTerrainTab, CDialogEx)

CTerrainTab::CTerrainTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TERRAIN_TAB, pParent)
	, m_fSizeWidth(0)
	, m_fSizeDepth(0)
	, m_uiVtxNumWidth(0)
	, m_uiVtxNumHeight(0)
	, m_fU(0)
	, m_fV(0)
	, m_cstrPosX(_T("0"))
	, m_cstrPosY(_T("0"))
	, m_cstrPosZ(_T("0"))
	, m_cstrAngleX(_T("0"))
	, m_cstrAngleY(_T("0"))
	, m_cstrAngleZ(_T("0"))
	, m_cstrScaleX(_T("1"))
	, m_cstrScaleY(_T("1"))
	, m_cstrScaleZ(_T("1"))
{

}

CTerrainTab::~CTerrainTab()
{
}

void CTerrainTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	/*DDX_Text(pDX, IDC_EDIT1, m_vPos.x);
	DDX_Text(pDX, IDC_EDIT2, m_vPos.y);
	DDX_Text(pDX, IDC_EDIT3, m_vPos.z);
	DDX_Text(pDX, IDC_EDIT4, m_vAngle.x);
	DDX_Text(pDX, IDC_EDIT5, m_vAngle.y);
	DDX_Text(pDX, IDC_EDIT6, m_vAngle.z);
	DDX_Text(pDX, IDC_EDIT7, m_vScale.x);
	DDX_Text(pDX, IDC_EDIT8, m_vScale.y);
	DDX_Text(pDX, IDC_EDIT9, m_vScale.z);*/
	DDX_Control(pDX, IDC_LIST2, m_lbxTexture);
	DDX_Text(pDX, IDC_EDIT11, m_fSizeWidth);
	DDX_Text(pDX, IDC_EDIT12, m_fSizeDepth);
	DDX_Text(pDX, IDC_EDIT14, m_uiVtxNumWidth);
	DDX_Text(pDX, IDC_EDIT15, m_uiVtxNumHeight);
	DDX_Text(pDX, IDC_EDIT16, m_fU);
	DDX_Text(pDX, IDC_EDIT17, m_fV);
	DDX_Text(pDX, IDC_EDIT1, m_cstrPosX);
	DDX_Text(pDX, IDC_EDIT2, m_cstrPosY);
	DDX_Text(pDX, IDC_EDIT3, m_cstrPosZ);
	DDX_Text(pDX, IDC_EDIT4, m_cstrAngleX);
	DDX_Text(pDX, IDC_EDIT5, m_cstrAngleY);
	DDX_Text(pDX, IDC_EDIT6, m_cstrAngleZ);
	DDX_Text(pDX, IDC_EDIT7, m_cstrScaleX);
	DDX_Text(pDX, IDC_EDIT8, m_cstrScaleY);
	DDX_Text(pDX, IDC_EDIT9, m_cstrScaleZ);
	DDX_Control(pDX, IDC_LIST1, m_lbxHeightMap);
}

BEGIN_MESSAGE_MAP(CTerrainTab, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CTerrainTab::OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT2, &CTerrainTab::OnEnChangeEditPosY)
	ON_EN_CHANGE(IDC_EDIT3, &CTerrainTab::OnEnChangeEditPosZ)
	ON_EN_CHANGE(IDC_EDIT4, &CTerrainTab::OnEnChangeEditRotX)
	ON_EN_CHANGE(IDC_EDIT5, &CTerrainTab::OnEnChangeEditRotY)
	ON_EN_CHANGE(IDC_EDIT6, &CTerrainTab::OnEnChangeEditRotZ)
	ON_EN_CHANGE(IDC_EDIT7, &CTerrainTab::OnEnChangeEditScaleX)
	ON_EN_CHANGE(IDC_EDIT8, &CTerrainTab::OnEnChangeEditScaleY)
	ON_EN_CHANGE(IDC_EDIT9, &CTerrainTab::OnEnChangeEditScaleZ)
	ON_EN_CHANGE(IDC_EDIT11, &CTerrainTab::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT12, &CTerrainTab::OnEnChangeEditDepth)
	ON_EN_CHANGE(IDC_EDIT14, &CTerrainTab::OnEnChangeEditVtxNumWidth)
	ON_EN_CHANGE(IDC_EDIT15, &CTerrainTab::OnEnChangeEditVtxNumDepth)
	ON_EN_CHANGE(IDC_EDIT16, &CTerrainTab::OnEnChangeEditU)
	ON_EN_CHANGE(IDC_EDIT17, &CTerrainTab::OnEnChangeEditV)
	ON_BN_CLICKED(IDC_BUTTON9, &CTerrainTab::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON6, &CTerrainTab::OnBnClickedButtonLoad)
	ON_LBN_SELCHANGE(IDC_LIST2, &CTerrainTab::OnLbnSelchangeListTexture)
	ON_LBN_SELCHANGE(IDC_LIST1, &CTerrainTab::OnLbnSelchangeListHeightMap)
END_MESSAGE_MAP()


// CTerrainTab 메시지 처리기
void CTerrainTab::OnEnChangeEditPosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		_float fPosX = static_cast<_float>(_tstof(m_cstrPosX));
		pTerrain->GetTransform()->SetPosX(fPosX);
	}
		
	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditPosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		_float fPosY = static_cast<_float>(_tstof(m_cstrPosY));
		pTerrain->GetTransform()->SetPosY(fPosY);
	}
	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditPosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		_float fPosZ = static_cast<_float>(_tstof(m_cstrPosZ));
		pTerrain->GetTransform()->SetPosZ(fPosZ);
	}
	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditRotX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		_float fAngleX = D3DXToRadian(static_cast<_float>(_tstof(m_cstrAngleX)));
		pTerrain->GetTransform()->SetAngleX(fAngleX);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditRotY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		_float fAngleY = D3DXToRadian(static_cast<_float>(_tstof(m_cstrAngleY)));
		pTerrain->GetTransform()->SetAngleY(fAngleY);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditRotZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		_float fAngleZ = D3DXToRadian(static_cast<_float>(_tstof(m_cstrAngleZ)));
		pTerrain->GetTransform()->SetAngleZ(fAngleZ);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditScaleX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		_float fScaleX = static_cast<_float>(_tstof(m_cstrScaleX));
		pTerrain->GetTransform()->SetScaleX(fScaleX);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditScaleY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		_float fScaleY = static_cast<_float>(_tstof(m_cstrScaleY));
		pTerrain->GetTransform()->SetScaleY(fScaleY);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditScaleZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CTerrainTabView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		_float fScaleZ = static_cast<_float>(_tstof(m_cstrScaleZ));
		pTerrain->GetTransform()->SetScaleZ(fScaleZ);
	}

	UpdateData(FALSE);
}

void CTerrainTab::OnEnChangeEditWidth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		/*if (m_fSizeWidth < 2) {
			UpdateData(FALSE);
			m_fSizeWidth = 2;
			UpdateData(TRUE);
		}*/
			
		pTerrain->SetSize(m_fSizeWidth, m_fSizeDepth);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditDepth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		/*if (m_fSizeDepth < 2) {
			UpdateData(FALSE);
			m_fSizeDepth = 2;
			UpdateData(TRUE);
		}*/
			
		pTerrain->SetSize(m_fSizeWidth, m_fSizeDepth);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditVtxNumWidth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		pTerrain->SetVtxNum(m_uiVtxNumWidth, m_uiVtxNumHeight);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditVtxNumDepth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		pTerrain->SetVtxNum(m_uiVtxNumWidth, m_uiVtxNumHeight);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditU()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		pTerrain->SetUV(m_fU, m_fV);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnEnChangeEditV()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	auto pEditScene = g_pTool3D_Kernel->GetEditScene();
	auto pTerrain = pEditScene->GetTerrain();

	if (pTerrain) {
		pTerrain->SetUV(m_fU, m_fV);
	}

	UpdateData(FALSE);
}


void CTerrainTab::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_pTool3D_Kernel->GetEditScene()->SaveTerrain();
}


void CTerrainTab::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_pTool3D_Kernel->GetEditScene()->LoadTerrain();
	CTerrain* pTerrain = g_pTool3D_Kernel->GetEditScene()->GetTerrain();
	Engine::CTerrainTex* pTerrainTex = pTerrain->GetComponent<Engine::CTerrainTex>();

	// 트랜스폼 갱신
	UpdateData(TRUE);
	_vec3 vPos = pTerrain->GetTransform()->GetPos();
	m_cstrPosX.Format(L"%f", vPos.x);
	m_cstrPosY.Format(L"%f", vPos.y);
	m_cstrPosZ.Format(L"%f", vPos.z);
	_vec3 vAngle = pTerrain->GetTransform()->GetAngle();
	m_cstrAngleX.Format(L"%f", D3DXToDegree(vAngle.x));
	m_cstrAngleY.Format(L"%f", D3DXToDegree(vAngle.y));
	m_cstrAngleZ.Format(L"%f", D3DXToDegree(vAngle.z));
	_vec3 vScale = pTerrain->GetTransform()->GetScale();
	m_cstrScaleX.Format(L"%f", vScale.x);
	m_cstrScaleY.Format(L"%f", vScale.y);
	m_cstrScaleZ.Format(L"%f", vScale.z);
	
	// 사이즈 갱신
	m_fSizeWidth = pTerrainTex->GetWidth();
	m_fSizeDepth = pTerrainTex->GetHeight();

	// 정점 수 갱신
	m_uiVtxNumWidth = pTerrainTex->GetNumOfVerticesW();
	m_uiVtxNumHeight = pTerrainTex->GetNumOfVerticesH();

	// UV 갱신
	m_fU = pTerrainTex->GetU();
	m_fV = pTerrainTex->GetV();
	UpdateData(FALSE);
}


BOOL CTerrainTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	UpdateData(TRUE);
	m_fSizeWidth = 100.f;
	m_fSizeDepth = 100.f;
	m_uiVtxNumWidth = 2;
	m_uiVtxNumHeight = 2;
	m_fU = 1.f;
	m_fV = 1.f;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTerrainTab::OnLbnSelchangeListTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString szTextureTag;
	m_lbxTexture.GetText(m_lbxTexture.GetCurSel(), szTextureTag);
	g_pTool3D_Kernel->GetEditScene()->GetTerrain()->SetTextureTag(szTextureTag);
}



void CTerrainTab::OnLbnSelchangeListHeightMap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString szHeightMapTag;
	m_lbxHeightMap.GetText(m_lbxHeightMap.GetCurSel(), szHeightMapTag);
	g_pTool3D_Kernel->GetEditScene()->GetTerrain()->SetHeightMapFileName(L"../../Resource/Texture/Terrain/" + szHeightMapTag + L".bmp");
}
