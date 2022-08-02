#include "stdafx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Terrain.h"
#include "MyMap.h"
#include "Construction.h"
#include "Player.h"

CStage::CStage()
{
}


CStage::~CStage()
{
	Release_Scene();
}

HRESULT CStage::Ready_Scene()
{
	if (FAILED(CTextureMgr::Get_Instance()->ReadImgPath(L"../Data/ImgPath.txt")))
	{
		ERR_MSG(L"ImgPath Load Failed");
		return E_FAIL;
	}

	CObj*		pTerrain = new CTerrain;

	if (nullptr != pTerrain)
		pTerrain->Initialize();

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::TERRAIN, pTerrain);

	CObj*		pMyMap = new CMyMap;

	if (nullptr != pMyMap)
		pMyMap->Initialize();

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::MAP, pMyMap);

	CObj*  pConstruct = new CConstruction;

	if (nullptr != pConstruct)
		pConstruct->Initialize();

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::OBJECT, pConstruct);

	CObj* pPlayer = new CPlayer;
	if (nullptr != pPlayer)
		pPlayer->Initialize();

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::PLAYER, pPlayer);

	return S_OK;
}

void CStage::Update_Scene()
{
	CObjMgr::Get_Instance()->Update();
}

void CStage::Late_Update_Scene()
{
	CObjMgr::Get_Instance()->Late_Update();
}

void CStage::Render_Scene()
{

	CObjMgr::Get_Instance()->Render();
}

void CStage::Release_Scene()
{
}

CStage* CStage::Create(void)
{
	CStage*		pInstance = new CStage;

	if (FAILED(pInstance->Ready_Scene()))
		return nullptr;

	return pInstance;
}
