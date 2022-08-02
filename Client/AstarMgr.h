#pragma once

#include "Terrain.h"

class CAstarMgr
{
	DECLARE_SINGLETON(CAstarMgr)
private:
	CAstarMgr();
	~CAstarMgr();

public:
	list<TILE*>&	Get_BestList(void) { return m_BestList; }
	void		Start_Astar(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal);

private:
	bool		Make_Route(int iStartIdx, int iGoalIdx);
	void		Make_BestList(int iStartIdx, int iGoalIdx);

private:
	int			GetTileIdx(const D3DXVECTOR3& vPos);
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);

private:
	bool		Check_OpenList(int iIndex);
	bool		Check_CloseList(int iIndex);

	void		Release(void);


private:
	list<int>		m_OpenList;
	list<int>		m_CloseList;
	list<TILE*>		m_BestList;

	int				m_iStartIdx = 0;
};

