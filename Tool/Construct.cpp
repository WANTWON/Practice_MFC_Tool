#include "stdafx.h"
#include "Construct.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"
#include "MiniView.h"
#include "MainFrm.h"

CConstruct::CConstruct()
{
}


CConstruct::~CConstruct()
{
	Release();
}

void CConstruct::Initialize(void)
{

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Image/PathFinder/Object/Tree/Tree%d.png", TEX_MULTI, L"Object", L"Tree", 50)))
		return;

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Image/PathFinder/Object/House/House%d.png", TEX_MULTI, L"Object", L"House", 4)))
		return;

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Image/PathFinder/Object/Fence/Fence%d.png", TEX_MULTI, L"Object", L"Fence", 16)))
		return;

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Image/PathFinder/Object/Plant/Plant%d.png", TEX_MULTI, L"Object", L"Plant", 19)))
		return;

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Image/PathFinder/Object/Etc/Etc%d.png", TEX_MULTI, L"Object", L"Etc", 38)))
		return;
}

void CConstruct::Update(void)
{
}

void CConstruct::Render(void)
{
	D3DXMATRIX	matWorld, matScale, matRotZ, matTrans;

	for (auto& iter : m_vecConstruct)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, iter->vScale.x, iter->vScale.y, iter->vScale.z);
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


		const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture((iter)->ObjKey.c_str(), (iter)->StateKey.c_str(), iter->byDrawID);

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

void CConstruct::Mini_Render(void)
{
	D3DXMATRIX	matWorld, matScale, matRotZ, matTrans;

	for (auto& iter : m_vecConstruct)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, iter->vScale.x, iter->vScale.y, iter->vScale.z);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x,
			iter->vPos.y,
			iter->vPos.z);

		matWorld = matScale * matTrans;

		RECT	rc;
		// GetClientRect : 현재 클라이언트영역의 rect 정보를 얻어옴
		::GetClientRect(m_pMiniView->m_hWnd, &rc);

		float	fX = WINCX / float(rc.right - rc.left);
		float	fY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(&matWorld, fX*0.1f, fY*0.1f);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(((iter)->ObjKey).c_str(), ((iter)->StateKey).c_str(), iter->byDrawID);

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

void CConstruct::Following_Render(void)
{

	D3DXMATRIX	matWorld, matScale, matRotZ, matTrans;

	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixScaling(&matScale, m_pFollowObj->vScale.x, m_pFollowObj->vScale.y, m_pFollowObj->vScale.z);
	D3DXMatrixTranslation(&matTrans, m_pFollowObj->vPos.x - m_pMainView->GetScrollPos(0),
		m_pFollowObj->vPos.y - m_pMainView->GetScrollPos(1),
		m_pFollowObj->vPos.z);

	matWorld = matScale * matTrans;

	RECT	rc;
	// GetClientRect : 현재 클라이언트영역의 rect 정보를 얻어옴
	::GetClientRect(m_pMainView->m_hWnd, &rc);

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);

	Set_Ratio(&matWorld, fX, fY);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);


	const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture((m_pFollowObj)->ObjKey.c_str(), (m_pFollowObj)->StateKey.c_str(), m_pFollowObj->byDrawID);

	if (nullptr == pTexInfo)
		return;

	float	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(155, 255, 255, 255));
}

void CConstruct::Release(void)
{
	Safe_Delete(m_pFollowObj);

	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

	//for_each(m_vecConstruct.begin(), m_vecConstruct.end(), CDeleteObj());
	m_vecConstruct.clear();
	m_vecConstruct.shrink_to_fit();
}

void CConstruct::Set_Ratio(D3DXMATRIX * pOut, float fRatioX, float fRatioY)
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

int CConstruct::Get_TileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t index = 0; index < m_vecTile.size(); ++index)
	{
		if (Picking(vPos, index))
			return index;
	}

	return -1;		// 제대로 picking이 이뤄지지 않았거나 인덱스 범위를 벗어난 지점을 피킹처리한 경우
}

bool CConstruct::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
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

void CConstruct::Add_Constructure(const D3DXVECTOR3 & vPos, const CString strObj, const CString strState, const BYTE & byDrawID, int Horizontal, int Vertical, float ScaleX, float ScaleY)
{
	int		iIndex = Get_TileIndex(vPos);

	if (-1 == iIndex)
		return;

	if (m_vecTile[iIndex]->beEmplaced)
		return;

	CONSTRUCT*		pObj = new CONSTRUCT;
	pObj->vPos = { m_vecTile[iIndex]->vPos.x,  m_vecTile[iIndex]->vPos.y, 0.f };
	pObj->byDrawID = byDrawID;
	pObj->vScale.x = ScaleX;
	pObj->vScale.y = ScaleY;


	pObj->ObjKey = strObj.GetString();
	pObj->StateKey = strState.GetString();
	
	if (Horizontal == 1)
		pObj->vScale.x *= -1;

	if (Vertical == 1)
		pObj->vScale.y *= -1;

	m_vecConstruct.push_back(pObj);
	m_vecTile[iIndex]->beEmplaced = true;
}

void CConstruct::Delete_Constructure(const D3DXVECTOR3 & vPos)
{

	int		iIndex = Get_TileIndex(vPos);

	if (-1 == iIndex)
		return;

	if (!m_vecTile[iIndex]->beEmplaced)
		return;

	int i = 0;
	for (auto& iter : m_vecConstruct)
	{
		if (iter->vPos == m_vecTile[iIndex]->vPos)
		{
			m_vecConstruct.erase(remove(m_vecConstruct.begin(), m_vecConstruct.end(), iter), m_vecConstruct.end());
			break;
		}
		++i;
	}
	m_vecTile[iIndex]->beEmplaced = false;
}

void CConstruct::Following(const D3DXVECTOR3 & vPos, const CString strObj, const CString strState, const BYTE & byDrawID, int Horizontal, int Vertical, float scaleX, float scaleY)
{
	int		iIndex = Get_TileIndex(vPos);

	if (-1 == iIndex)
		return;

	if (m_vecTile[iIndex]->beEmplaced)
		return;

	m_pFollowObj->vPos = { m_vecTile[iIndex]->vPos.x,  m_vecTile[iIndex]->vPos.y, 0.f };
	m_pFollowObj->byDrawID = byDrawID;
	m_pFollowObj->vScale = { scaleX , scaleY, 1 };

	m_pFollowObj->ObjKey = strObj.GetString();
	m_pFollowObj->StateKey = strState.GetString();

	if (Horizontal == 1)
		m_pFollowObj->vScale.x *= -1;

	if (Vertical == 1)
		m_pFollowObj->vScale.y *= -1;

}

void CConstruct::Load_Data(void)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HANDLE		hFile = CreateFile(L"../Data/Construct.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD	dwByte = 0;
	CONSTRUCT*	pConstruct = nullptr;

	while (true)
	{
		pConstruct = new CONSTRUCT;


		ReadFile(hFile, pConstruct, sizeof(CONSTRUCT), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pConstruct);
			break;
		}

		m_vecConstruct.push_back(pConstruct);

		int		iIndex = Get_TileIndex(pConstruct->vPos);

		m_vecTile[iIndex]->beEmplaced = true;

	}
	CloseHandle(hFile);

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	pToolView->Invalidate(FALSE);

	return;
}
