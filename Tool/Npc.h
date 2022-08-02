#pragma once

#include "Include.h"

class CMiniView;
class CToolView;

class CNpc
{
public:
	CNpc();
	~CNpc();

public:
	void		Initialize(void);
	void		Update(void);
	void		Render(void);
	void		Mini_Render(void);
	void		Following_Render(void);
	void		Release(void);
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

public:
	int			Get_TileIndex(const D3DXVECTOR3& vPos);
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	void		Add_NPC(const D3DXVECTOR3 & vPos, const CString strObj, const CString strState, int Alpha, int R, int G, int B, float OffSetX, float OffsetY);
	void		Delete_NPC(const D3DXVECTOR3& vPos);
	void		Following(const D3DXVECTOR3 & vPos, const CString strObj, const CString strState, float OffSetX, float OffsetY);
	void		Load_Data(void);

public:
	void		Set_MainView(CToolView* pView) { m_pMainView = pView; }
	void		Set_MiniView(CMiniView* pView) { m_pMiniView = pView; }
	void		Set_Tile(vector<TILE*> pView) { m_vecTile = pView; }
	const vector<NPC*>& Get_NPC(void) const { return m_vecNPC; }
	//void		Set_Scale(float _x, float _y) { }
private:
	CToolView*			m_pMainView = nullptr;
	CMiniView*			m_pMiniView = nullptr;
	vector<TILE*>		m_vecTile;
	vector<NPC*>  m_vecNPC;

	NPC*		m_pFollowObj = new NPC;
};

