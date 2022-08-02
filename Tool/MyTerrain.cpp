#include "stdafx.h"
#include "MyTerrain.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"
#include "MiniView.h"
#include "MainFrm.h"

CMyTerrain::CMyTerrain()
{
	m_vecTile.reserve(TILEX * TILEY);
}

CMyTerrain::~CMyTerrain()
{
	Release();
}

void CMyTerrain::Initialize(void)
{

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Image/PathFinder/Object/Tile/Tile%d.png", TEX_MULTI, L"Object", L"Tile", 5)))
	{
		AfxMessageBox(L"Tile Image Insert Failed");
		return;
	}

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE*		pTile = new TILE;

			float fX = (TILECX * j) + (i % 2) * (TILECX / 2.f);
			float fY = (TILECY / 2.f) * i;

			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { (float)TILECX, (float)TILECY, 0.f };
			pTile->byOption = 0;
			pTile->byDrawID = 0;
			pTile->iIndex = i * TILEX + j;

			m_vecTile.push_back(pTile);
		}
	}

}

void CMyTerrain::Update(void)
{
}

void CMyTerrain::Render(void)
{
	D3DXMATRIX	matWorld, matScale, matRotZ, matTrans;

	TCHAR	szBuf[MIN_STR] = L"";
	m_iIndex = 0;
	for (auto& iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x - m_pMainView->GetScrollPos(0), 
										 iter->vPos.y - m_pMainView->GetScrollPos(1),
										 iter->vPos.z);
	
		matWorld = matScale * matTrans;

		RECT	rc;
		// GetClientRect : 현재 클라이언트영역의 rect 정보를 얻어옴
		::GetClientRect(m_pMainView->m_hWnd, &rc);

		float	fX = WINCX / float(rc.right - rc.left);
		float	fY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(&matWorld, fX, fY);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);


		const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Object", L"Tile", iter->byDrawID);

		if (nullptr == pTexInfo)
			return;

		float	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		//실험
		swprintf_s(szBuf, L"%d", m_iIndex++);

		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(), szBuf, lstrlen(szBuf), nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

	}
}

void CMyTerrain::Mini_Render(void)
{
	D3DXMATRIX	matWorld, matScale, matRotZ, matTrans;

	for (auto& iter : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x,
			iter->vPos.y,
			iter->vPos.z);

		matWorld = matScale * matTrans;

		RECT	rc;
		// GetClientRect : 현재 클라이언트영역의 rect 정보를 얻어옴
		::GetClientRect(m_pMiniView->m_hWnd, &rc);

		float	fX = WINCX / float(rc.right - rc.left);
		float	fY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(&matWorld, fX*0.1, fY*0.1);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Object", L"Tile", iter->byDrawID);

		if (nullptr == pTexInfo)
			return;

		float	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}


void CMyTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CMyTerrain::Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
{
	pOut->_11 *= fRatioX;
	pOut->_21 *= fRatioX;
	pOut->_31 *= fRatioX;
	pOut->_41 *= fRatioX;

	pOut->_12 *= fRatioY;
	pOut->_22 *= fRatioY;
	pOut->_32 *= fRatioY;
	pOut->_42 *= fRatioY;

}

void CMyTerrain::Tile_Change(const D3DXVECTOR3 & vPos, const BYTE & byDrawID)
{
	int		iIndex = Get_TileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = byDrawID;

	
	m_vecTile[iIndex]->byOption = byDrawID;	// 장애물인 경우 옵션 값을 1로 하겠다.

}

int CMyTerrain::Get_TileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t index = 0; index < m_vecTile.size(); ++index)
	{
		if (Picking_Dot(vPos, index))
			return index;
	}
	
	return -1;		// 제대로 picking이 이뤄지지 않았거나 인덱스 범위를 벗어난 지점을 피킹처리한 경우
}

bool CMyTerrain::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	// 12 -> 3 -> 6 -> 9 방향으로 향하는 방정식 배치
	float		fGrdient[4] = 
	{
		(TILECY / 2.f) / (TILECX / 2.f) * -1.f, 
		(TILECY / 2.f) / (TILECX / 2.f),
		(TILECY / 2.f) / (TILECX / 2.f) * -1.f,
		(TILECY / 2.f) / (TILECX / 2.f)
	};

	D3DXVECTOR3		vPoint[4] = 
	{
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y , 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f }
	};

	// y = ax + b 
	// -b = -y + ax
	// b = y - ax

	float		fB[4] = 
	{
		vPoint[0].y - fGrdient[0] * vPoint[0].x, 
		vPoint[1].y - fGrdient[1] * vPoint[1].x,
		vPoint[2].y - fGrdient[2] * vPoint[2].x,
		vPoint[3].y - fGrdient[3] * vPoint[3].x
	};

//y = ax + b	// Y - AX - B

//		0 == ax + b - y	// 선상에 있는 경우
//		0 > ax + b - y	// 직선 위에 있는 경우
//		0 < ax + b - y	// 직선 아래 있는 경우

	bool	bCheck[4]{ false };

	//12 -> 3
	if (0 < fGrdient[0] * vPos.x + fB[0] - vPos.y)
		bCheck[0] = true;
	// 3 -> 6
	if (0 > fGrdient[1] * vPos.x + fB[1] - vPos.y)
		bCheck[1] = true;
	// 6 -> 9
	if (0 > fGrdient[2] * vPos.x + fB[2] - vPos.y)
		bCheck[2] = true;

	// 9 -> 12
	if (0 < fGrdient[3] * vPos.x + fB[3] - vPos.y)
		bCheck[3] = true;
	
	return bCheck[0] && bCheck[1] && bCheck[2] && bCheck[3];
}

bool CMyTerrain::Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex)
{
	D3DXVECTOR3		vPoint[4] =
	{
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y , 0.f },
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f }
	};

	D3DXVECTOR3		vDir[4] = {

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3],
	};

	//2d 공간상에서 (x, y)의 방향의 법선을 (-y, x)로 표기할 수 있다
	D3DXVECTOR3		vNormal[4] =
	{
		{ -vDir[0].y, vDir[0].x, 0.f },
		{ -vDir[1].y, vDir[1].x, 0.f },
		{ -vDir[2].y, vDir[2].x, 0.f },
		{ -vDir[3].y, vDir[3].x, 0.f }
	};

	// 각 꼭지점에서 마우스를 클릭한 지점으로 향하는 방향벡터를 구하자
	D3DXVECTOR3		vMouseDir[4] = 
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
	{		
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		// 예각인 경우(타일 밖에 있는 경우)
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

void CMyTerrain::Load_Tile(void)
{
	HANDLE		hFile = CreateFile(L"../Data/Tile.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
	m_vecTile.reserve(TILEX * TILEY);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

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

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	pToolView->Invalidate(FALSE);

	return;
}
