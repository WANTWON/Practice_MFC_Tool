#include "stdafx.h"
#include "Player.h"
#include "TextureMgr.h"
#include "Device.h"
#include "AstarMgr.h"
#include "TimeMgr.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	m_tInfo.vPos = { 600.f, 600.f, 0.f };
	m_wstrObjKey = L"Player";
	m_wstrStateKey = L"RunLD";
	m_fSpeed = 300.f;

	m_tFrame.fFrame = 0;
	m_tFrame.fMax = 10;


	return S_OK;
}

int CPlayer::Update(void)
{
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		D3DXVECTOR3	vMouse = Get_Mouse() - CObj::m_vScroll;
		CAstarMgr::Get_Instance()->Start_Astar(m_tInfo.vPos, vMouse);
	}

	Move_Astar();

	D3DXMATRIX	matScale, matTrans;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + CObj::m_vScroll.x,
		m_tInfo.vPos.y + CObj::m_vScroll.y,
		0.f);

	m_tInfo.matWorld = matScale * matTrans;


	return OBJ_NOEVENT;
}

void CPlayer::Late_Update(void)
{
	Move_Frame();
}

void CPlayer::Render(void)
{
	const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(),
		m_wstrStateKey.c_str(),
		(int)m_tFrame.fFrame);

	if (nullptr == pTexInfo)
		return;

	float	fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float	fCenterY = pTexInfo->tImgInfo.Height ;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPlayer::Release(void)
{

}

void CPlayer::Move_Astar(void)
{
	list<TILE*>&	BestList = CAstarMgr::Get_Instance()->Get_BestList();

	if (!BestList.empty())
	{
		int	iIndex = BestList.front()->iIndex;

		D3DXVECTOR3	vDir = BestList.front()->vPos - m_tInfo.vPos;

		float	fDistance = D3DXVec3Length(&vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		m_tInfo.vPos += vDir * m_fSpeed * CTimeMgr::Get_Instance()->Get_TimeDelta();


		if (1.f >= fDistance)
			BestList.pop_front();
	}

}
