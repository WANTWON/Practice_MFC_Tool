#pragma once

#include "Include.h"

class CMiniView;
class CToolView;
class CMyTerrain
{
public:
	CMyTerrain();
	~CMyTerrain();

public:
	void		Initialize(void);
	void		Update(void);
	void		Render(void);
	void		Mini_Render(void); 
	void		Release(void);
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

public:
	void		Tile_Change(const D3DXVECTOR3& vPos, const BYTE& byDrawID);

	int			Get_TileIndex(const D3DXVECTOR3& vPos);
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);

	bool		Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

public:
	void		Set_MainView(CToolView* pView) { m_pMainView = pView; }
	void		Set_MiniView(CMiniView* pView) { m_pMiniView = pView; }
	vector<TILE*>&		Get_VecTile(void) { return m_vecTile; }
	void		Load_Tile(void);

private:
	vector<TILE*>		m_vecTile;
	CToolView*			m_pMainView = nullptr;
	CMiniView*			m_pMiniView = nullptr;

	int					m_iIndex;

};

