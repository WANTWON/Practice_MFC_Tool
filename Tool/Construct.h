#pragma once
#include "Include.h"

class CMiniView;
class CToolView;
class CConstruct
{
public:
	CConstruct();
	~CConstruct();

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
	void		Add_Constructure(const D3DXVECTOR3 & vPos, const CString strObj, const CString strState, const BYTE & byDrawID, int Horizontal, int Vertical, float ScaleX, float ScaleY);
	void		Delete_Constructure(const D3DXVECTOR3& vPos);
	void		Following(const D3DXVECTOR3 & vPos, const CString strObj, const CString strState, const BYTE & byDrawID, int Horizontal, int Vertical, float scaleX, float scaleY);
	void		Load_Data(void);

public:
	void		Set_MainView(CToolView* pView) { m_pMainView = pView; }
	void		Set_MiniView(CMiniView* pView) { m_pMiniView = pView; }
	void		Set_Tile(vector<TILE*> pView) { m_vecTile = pView; }
	const vector<CONSTRUCT*>& Get_Construct(void) const  { return m_vecConstruct; }
	//void		Set_Scale(float _x, float _y) { }
private:
	CToolView*			m_pMainView = nullptr;
	CMiniView*			m_pMiniView = nullptr;
	vector<TILE*>		m_vecTile;
	vector<CONSTRUCT*>  m_vecConstruct;

	CONSTRUCT*		m_pFollowObj = new CONSTRUCT;
};

