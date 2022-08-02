#include "stdafx.h"
#include "MyMap.h"
#include "Device.h"
#include "Obj.h"
#include "TextureMgr.h"


CMyMap::CMyMap()
{
}


CMyMap::~CMyMap()
{
}

HRESULT CMyMap::Initialize(void)
{
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Image/Stage/Field.png", TEX_SINGLE, L"Field")))
	{
		ERR_MSG(L"Field Image Insert Failed");
		return E_FAIL;
	}

	return S_OK;
}

int CMyMap::Update(void)
{
	return 0;
}

void CMyMap::Late_Update(void)
{
}

void CMyMap::Render(void)
{
	const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Field");

	if (nullptr == pTexInfo)
		return;

	float	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX	matWorld, matScale, matRotZ, matTrans;

	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, fCenterX + CObj::m_vScroll.x, fCenterY + CObj::m_vScroll.y, 0.f);

	matWorld = matScale *matTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);


	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMyMap::Release(void)
{
}
