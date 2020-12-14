#include "pch.h"
#include "Map.h"
#include "Terrain.h"
#include "StaticObject.h"


CMap::CMap(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CMap::CMap(const CMap & rhs)
	:
	CGameObject(rhs)
{
}

CMap::~CMap(void)
{
}


HRESULT CMap::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CRenderer>();
	m_pNaviMesh = Engine::CNaviMesh::Create(m_pGraphicDev);
	m_mapComponent[Engine::ID_STATIC].emplace(L"COM_NAVIMESH", m_pNaviMesh);

	m_pTerrain = CTerrain::Create(m_pGraphicDev);

	AddChild(m_pTerrain);

	return S_OK;
}

int CMap::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return 0;
}

void CMap::Render_Object(void)
{
	m_pNaviMesh->Render_NaviMeshes();
}

CMap * CMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMap*	pInstance = new CMap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CMap::Free(void)
{
	CGameObject::Free();
}

void CMap::LoadMap(Engine::CLayer* pLayer, const _tchar * _pTerrainFilePath, const _tchar * _pNaviFilePath, const _tchar * _pObjectsFilePath)
{
	LoadTerrain(_pTerrainFilePath);
	LoadNaviMesh(_pNaviFilePath);
	LoadStaticObjects(pLayer, _pObjectsFilePath);
}

void CMap::LoadTerrain( const _tchar * _pFilePath)
{
	if (!_pFilePath)
		return;

	// TODO : 터레인 파일을 읽기 위한 코드를 작성합니다.
	HANDLE hFile = CreateFile(_pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;

	m_pTerrain->LoadInfo(hFile);

	CloseHandle(hFile);

	
}

void CMap::LoadNaviMesh(const _tchar * _pFilePath)
{
	if (!_pFilePath)
		return;

	// TODO : 네비메쉬 파일을 읽기 위한 코드를 작성합니다.
	// TODO : 터레인 파일을 읽기 위한 코드를 작성합니다.
	HANDLE hFile = CreateFile(_pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;

	//if (m_pNaviMesh) {
		Engine::Safe_Release(m_pNaviMesh);
		m_pNaviMesh = Engine::CNaviMesh::Create(m_pGraphicDev);
		m_mapComponent[Engine::ID_STATIC][L"COM_NAVIMESH"] = m_pNaviMesh;
	//}

	DWORD dwByte = 0;
	_int iVerticesSize = 0;
	ReadFile(hFile, &iVerticesSize, sizeof(_int), &dwByte, nullptr);
	vector<_vec3> vecVertices;
	vecVertices.reserve(iVerticesSize + 10);

	_vec3 vPos;
	for (_int i = 0; i < iVerticesSize; ++i) {
		ReadFile(hFile, &vPos, sizeof(vPos), &dwByte, nullptr);
		vecVertices.emplace_back(vPos);
	}
	
	_int iTriangleSize = iVerticesSize / 3;
	_vec3 vPosArr[3];
	for (_int i = 0; i < iTriangleSize; ++i) {
		for (_int j = 0; j < 3; ++j) {
			vPosArr[j] = vecVertices[i * 3 + j];
		}
		m_pNaviMesh->AddCell(vPosArr[0], vPosArr[1], vPosArr[2]);
	}
	m_pNaviMesh->Link_Cell();

	CloseHandle(hFile);
}

void CMap::LoadStaticObjects(Engine::CLayer* pLayer, const _tchar * _pFilePath)
{
	if (!_pFilePath)
		return;

	// TODO : 정적 객체 파일을 읽기 위한 코드를 작성합니다.
	HANDLE hFile = CreateFile(_pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return;

	if (m_vecChildList.size() > 1) {
		for_each(m_vecChildList.begin() + 1, m_vecChildList.end(), Engine::CDeleteObj());
		m_vecChildList.erase(remove_if(
			m_vecChildList.begin() + 1, m_vecChildList.end(),
			[](auto& rObj) { return rObj == nullptr; }),
			m_vecChildList.end());
	}

	_int iVecSize = 0;
	DWORD dwByte = 0;
	ReadFile(hFile, &iVecSize, sizeof(iVecSize), &dwByte, nullptr);
	CStaticObject* pStaticObject = nullptr;
	for (_int i = 0; i < iVecSize; ++i) {
		pStaticObject = CStaticObject::Create(m_pGraphicDev);
		if (!pStaticObject->LoadInfo(hFile))
			Engine::Safe_Release(pStaticObject);
		else {
			pLayer->Add_GameObject(L"StaticObjects", pStaticObject);
		}
	}

	CloseHandle(hFile);
}
