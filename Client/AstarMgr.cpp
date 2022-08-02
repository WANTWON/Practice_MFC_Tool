#include "stdafx.h"
#include "AstarMgr.h"
#include "ObjMgr.h"

IMPLEMENT_SINGLETON(CAstarMgr)

CAstarMgr::CAstarMgr()
{
}


CAstarMgr::~CAstarMgr()
{
	Release();
}

void CAstarMgr::Start_Astar(const D3DXVECTOR3 & vStart, const D3DXVECTOR3 & vGoal)
{
	Release();

	CObj*	pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&	vecTile = dynamic_cast<CTerrain*>(pTerrain)->Get_VecTile();

	if (vecTile.empty())
		return;

	m_iStartIdx = GetTileIdx(vStart);

	int	iGoalIdx = GetTileIdx(vGoal);

	// 존재하는 타일 인덱스 범위를 넘어선 경우 길찾기 수행을 중단
	if (0 > m_iStartIdx || 0 > iGoalIdx || m_iStartIdx >= TILEX * TILEY || iGoalIdx >= TILEX * TILEY)
		return;

	if (m_iStartIdx == iGoalIdx)
		return;

	if (0 != vecTile[iGoalIdx]->byOption)
		return;

	if (true == Make_Route(m_iStartIdx, iGoalIdx))
		Make_BestList(m_iStartIdx, iGoalIdx);
}

bool CAstarMgr::Make_Route(int iStartIdx, int iGoalIdx)
{
	CObj*	pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&	vecTile = dynamic_cast<CTerrain*>(pTerrain)->Get_VecTile();
	vector<list<TILE*>>& vecAdj = dynamic_cast<CTerrain*>(pTerrain)->Get_VecAdj();

	if (!m_OpenList.empty())
	{
		m_OpenList.pop_front();
	}

	m_CloseList.push_back(iStartIdx);

	for (auto& Tile : vecAdj[iStartIdx])
	{
		if (iGoalIdx == Tile->iIndex)
		{
			Tile->iParentIndex = iStartIdx;
			return true;
		}

		if (false == Check_CloseList(Tile->iIndex) &&
			false == Check_OpenList(Tile->iIndex))
		{
			Tile->iParentIndex = iStartIdx;
			m_OpenList.push_back(Tile->iIndex);
		}
	}

	// 더이상 찾을 길이 없는 경우
	if (m_OpenList.empty())
		return false;

	int	iOriginStart = m_iStartIdx;

	m_OpenList.sort([&vecTile, &iGoalIdx, &iOriginStart](int iTemp, int iSour)->bool
	{
		D3DXVECTOR3	vPCost1 = vecTile[iOriginStart]->vPos - vecTile[iTemp]->vPos;
		D3DXVECTOR3	vPCost2 = vecTile[iOriginStart]->vPos - vecTile[iSour]->vPos;

		D3DXVECTOR3	vGCost1 = vecTile[iGoalIdx]->vPos - vecTile[iTemp]->vPos;
		D3DXVECTOR3	vGCost2 = vecTile[iGoalIdx]->vPos - vecTile[iSour]->vPos;

		float	fCost1 = D3DXVec3Length(&vPCost1) + D3DXVec3Length(&vGCost1);
		float	fCost2 = D3DXVec3Length(&vPCost2) + D3DXVec3Length(&vGCost2);

		return fCost1 < fCost2;
	});

	return Make_Route(m_OpenList.front(), iGoalIdx);
}

void CAstarMgr::Make_BestList(int iStartIdx, int iGoalIdx)
{
	CObj*	pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&	vecTile = dynamic_cast<CTerrain*>(pTerrain)->Get_VecTile();

	if (vecTile.empty())
		return;

	m_BestList.push_front(vecTile[iGoalIdx]);

	int	iRouteIdx = vecTile[iGoalIdx]->iParentIndex;

	while (true)
	{
		if (iRouteIdx == iStartIdx)
			break;

		m_BestList.push_front(vecTile[iRouteIdx]);
		iRouteIdx = vecTile[iRouteIdx]->iParentIndex;
	}
}

int CAstarMgr::GetTileIdx(const D3DXVECTOR3 & vPos)
{
	CObj*	pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&	vecTile = dynamic_cast<CTerrain*>(pTerrain)->Get_VecTile();

	if (vecTile.empty())
		return -1;


	for (size_t index = 0; index < vecTile.size(); ++index)
	{
		if (Picking(vPos, index))
			return index;
	}

	return -1;
}

bool CAstarMgr::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	CObj*	pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&	vecTile = dynamic_cast<CTerrain*>(pTerrain)->Get_VecTile();

	if (vecTile.empty())
		return false;

	D3DXVECTOR3		vPoint[4] =
	{
		{ vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ vecTile[iIndex]->vPos.x + (TILECX / 2.f), vecTile[iIndex]->vPos.y , 0.f },
		{ vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ vecTile[iIndex]->vPos.x - (TILECX / 2.f), vecTile[iIndex]->vPos.y, 0.f }
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

bool CAstarMgr::Check_OpenList(int iIndex)
{
	for (int& iOpenIdx : m_OpenList)
	{
		if (iIndex == iOpenIdx)
			return true;
	}

	return false;
}

bool CAstarMgr::Check_CloseList(int iIndex)
{
	for (int& iCloseIdx : m_CloseList)
	{
		if (iIndex == iCloseIdx)
			return true;
	}

	return false;
}

void CAstarMgr::Release(void)
{
	m_CloseList.clear();
	m_OpenList.clear();

	m_BestList.clear();
}
