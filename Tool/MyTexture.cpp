#include "stdafx.h"
#include "MyTexture.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"
#include "ObjTool.h"


CMyTexture::CMyTexture()
{
}


CMyTexture::~CMyTexture()
{
}

void CMyTexture::Initialize(void)
{
	if (FAILED(CTextureMgr::Get_Instance()->ReadImgPath(L"../Data/ImgPath.txt")))
	{
		ERR_MSG(L"ImgPath Load Failed");
		return;
	}
}

void CMyTexture::Update(void)
{
}

void CMyTexture::Render(const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount, int iAlpha, int iRed, int iGreen, int iBlue, float fOffSetX, float fOffSetY)
{
	if (pObjKey == nullptr || pStateKey == nullptr || iCount < 0)
		return;

	const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(pObjKey,
		pStateKey,
		iCount);

	D3DXMATRIX	matWorld, matScale, matTrans;

	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixScaling(&matScale, 5.f, 4.f, 3.f);
	D3DXMatrixTranslation(&matTrans, 400.f + fOffSetX, 500.f + fOffSetY, 0.f);

	matWorld = matScale * matTrans;
		

	if (nullptr == pTexInfo)
		return;

	float	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY + pTexInfo->tImgInfo.Height * 0.5f, 0.f),
		nullptr,
		D3DCOLOR_ARGB(iAlpha, iRed, iGreen, iBlue));
}
