#include "stdafx.h"
#include "Stage.h"
#include "ToolView.h"
#include "TextureMgr.h"
#include "Device.h"
#include "MainFrm.h"
#include "MiniView.h"

CStage::CStage()
{
}


CStage::~CStage()
{
	Release();
}

void CStage::Initialize(void)
{
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Image/Stage/Field.png", TEX_SINGLE, L"Field")))
	{
		AfxMessageBox(L"Field Image Insert Failed");
		return;
	}
}

void CStage::Update(void)
{
}

void CStage::Render(void)
{
	const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Field");

	if (nullptr == pTexInfo)
		return;

	float	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX	matWorld, matScale, matTrans;

	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, fCenterX - m_pMainView->GetScrollPos(0), fCenterY - m_pMainView->GetScrollPos(1), 0.f);

	matWorld = matScale *matTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);



	RECT	rc;
	// GetClientRect : 현재 클라이언트영역의 rect 정보를 얻어옴
	::GetClientRect(m_pMainView->m_hWnd, &rc);

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);

	Set_Ratio(&matWorld, fX, fY);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CStage::Mini_Render(void)
{
	const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Field");

	if (nullptr == pTexInfo)
		return;

	float	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float	fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXMATRIX	matWorld, matScale, matRotZ, matTrans;

	D3DXMatrixIdentity(&matWorld);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, fCenterX , fCenterY, 0.f);

	matWorld = matScale *matTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	RECT	rc;
	// GetClientRect : 현재 클라이언트영역의 rect 정보를 얻어옴
	::GetClientRect(m_pMiniView->m_hWnd, &rc);

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);

	Set_Ratio(&matWorld, fX*0.1, fY*0.1);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CStage::Release(void)
{

}

void CStage::Set_Ratio(D3DXMATRIX * pOut, float fRatioX, float fRatioY)
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
