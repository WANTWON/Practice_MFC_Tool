#include "stdafx.h"
#include "Terrain.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"


CTerrain::CTerrain()
{
	m_vecTile.reserve(TILEX * TILEY);
}


CTerrain::~CTerrain()
{
	Release();
}

HRESULT CTerrain::Initialize(void)
{
	if (FAILED(Load_Tile(L"../Data/Tile.dat")))
		return E_FAIL;

	Ready_Adjacency();

	return S_OK;
}

int CTerrain::Update(void)
{
	D3DXVECTOR3		vMouse = ::Get_Mouse();

	if (0.f > vMouse.x)
		CObj::m_vScroll.x += 500.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (WINCX < vMouse.x)
		CObj::m_vScroll.x -= 500.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (0.f > vMouse.y)
		CObj::m_vScroll.y += 500.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (WINCY < vMouse.y)
		CObj::m_vScroll.y -= 500.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	return OBJ_NOEVENT;
}

void CTerrain::Late_Update(void)
{

}

void CTerrain::Render(void)
{
	D3DXMATRIX	matWorld, matScale, matRotZ, matTrans;

	int	iCullX = int(-CObj::m_vScroll.x) / TILECX;
	int	iCullY = int(-CObj::m_vScroll.y) / (TILECY / 2);

	int	iCullMaxX = WINCX / TILECX + 2;
	int	iCullMaxY = WINCY / (TILECY / 2) + 2;

	TCHAR	szBuf[MIN_STR] = L"";

	for (int i = iCullY; i < iCullY + iCullMaxY; ++i)
	{
		for (int j = iCullX; j < iCullX + iCullMaxX; ++j)
		{
			int		iIndex = i * TILEX + j;

			if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			D3DXMatrixIdentity(&matWorld);

			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			D3DXMatrixTranslation(&matTrans, m_vecTile[iIndex]->vPos.x + CObj::m_vScroll.x,
				m_vecTile[iIndex]->vPos.y + CObj::m_vScroll.y,
				m_vecTile[iIndex]->vPos.z);

			matWorld = matScale * matTrans;

			CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

			const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Object", L"Tile", m_vecTile[iIndex]->byDrawID);

			if (nullptr == pTexInfo)
				return;

			float	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float	fCenterY = pTexInfo->tImgInfo.Height * 0.5;

			CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
				nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
				nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			swprintf_s(szBuf, L"%d", iIndex);

			CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(), szBuf, lstrlen(szBuf), nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

		}
	}

}

void CTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

HRESULT CTerrain::Load_Tile(const TCHAR* pFilePath)
{
	HANDLE		hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	TILE*	pTile = nullptr;

	while (true)
	{
		pTile = new TILE;

		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pTile);
			break;
		}

		m_vecTile.push_back(pTile);

	}
	CloseHandle(hFile);

	return S_OK;
}

void CTerrain::Ready_Adjacency(void)
{
	m_vecAdjancency.resize(m_vecTile.size());

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int		iIndex = i * TILEX + j;

			// 좌 상단

			// 맨 왼쪽과						맨 위쪽이 아닌 경우
			if (0 != iIndex % (TILEX * 2) && (0 != i))
			{
				// 홀수->짝수 20감소, 짝수->홀수 21감소

				if ((0 == i % 2) && (!m_vecTile[iIndex - (TILEX + 1)]->byOption))
					m_vecAdjancency[iIndex].push_back(m_vecTile[iIndex - (TILEX + 1)]);

				else if ((0 != i % 2) && (!m_vecTile[iIndex - TILEX]->byOption))
					m_vecAdjancency[iIndex].push_back(m_vecTile[iIndex - TILEX]);
			}

			// 맨 오른쪽과						맨 위쪽이 아닌 경우
			if ((TILEX * 2 - 1) != iIndex % (TILEX * 2) && (0 != i))
			{
				// 짝수->홀수 20감소

				if ((0 == i % 2) && (!m_vecTile[iIndex - TILEX]->byOption))
					m_vecAdjancency[iIndex].push_back(m_vecTile[iIndex - TILEX]);

				// 홀수->짝수 19감소
				else if ((0 != i % 2) && (!m_vecTile[iIndex - (TILEX - 1)]->byOption))
					m_vecAdjancency[iIndex].push_back(m_vecTile[iIndex - (TILEX - 1)]);
			}

			// 맨 왼쪽과						맨 아래쪽이 아닌 경우
			if (0 != iIndex % (TILEX * 2) && (i != TILEY - 1))
			{
				// 짝수->홀수 19증가

				if ((0 == i % 2) && (!m_vecTile[iIndex + (TILEX - 1)]->byOption))
					m_vecAdjancency[iIndex].push_back(m_vecTile[iIndex + (TILEX - 1)]);

				// 홀수->짝수 20증가

				else if ((0 != i % 2) && (!m_vecTile[iIndex + TILEX]->byOption))
					m_vecAdjancency[iIndex].push_back(m_vecTile[iIndex + TILEX]);
			}


			// 맨 오른쪽과						맨 아래쪽이 아닌 경우
			if ((TILEX * 2 - 1) != iIndex % (TILEX * 2) && (i != TILEY - 1))
			{
				// 짝수->홀수 20증가

				if ((0 == i % 2) && (!m_vecTile[iIndex + TILEX]->byOption))
					m_vecAdjancency[iIndex].push_back(m_vecTile[iIndex + TILEX]);

				// 홀수->짝수 21증가
				else if ((0 != i % 2) && (!m_vecTile[iIndex + (TILEX + 1)]->byOption))
					m_vecAdjancency[iIndex].push_back(m_vecTile[iIndex + (TILEX + 1)]);
			}
		}
	}

}
