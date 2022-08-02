#pragma once
#include "Include.h"

class CToolView;
class CMiniView;
class CStage
{
public:
	CStage();
	~CStage();

public:
	void		Initialize(void);
	void		Update(void);
	void		Render(void);
	void		Mini_Render(void);
	void		Release(void);
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

public:
	void		Set_MainView(CToolView* pView) { m_pMainView = pView; }
	void		Set_MiniView(CMiniView* pView) { m_pMiniView = pView; }

private:
	CToolView*			m_pMainView = nullptr;
	CMiniView*			m_pMiniView = nullptr;
};

