#include "stdafx.h"
#include "Construction.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"


CConstruction::CConstruction()
{
}


CConstruction::~CConstruction()
{
	Release();
}

HRESULT CConstruction::Initialize(void)
{
	if (FAILED(Load_Construct(L"../Data/Construct.dat")))
		return E_FAIL;

	return S_OK;
}

int CConstruction::Update(void)
{
	return 0;
}

void CConstruction::Late_Update(void)
{
}

void CConstruction::Render(void)
{
	D3DXMATRIX	matWorld, matScale, matRotZ, matTrans;

	for (auto& iter : m_vecConstruct)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, iter->vScale.x, iter->vScale.y, iter->vScale.z);
		D3DXMatrixTranslation(&matTrans, iter->vPos.x + CObj::m_vScroll.x,
			iter->vPos.y + CObj::m_vScroll.y,
			iter->vPos.z);

		matWorld = matScale * matTrans;


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

void CConstruction::Release(void)
{
	//for_each(m_vecConstruct.begin(), m_vecConstruct.end(), CDeleteObj());
	m_vecConstruct.clear();
	m_vecConstruct.shrink_to_fit();
}

HRESULT CConstruction::Load_Construct(const TCHAR * pFilePath)
{
	HANDLE		hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

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

	}
	CloseHandle(hFile);

	return S_OK;
}
