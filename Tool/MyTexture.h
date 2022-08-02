#pragma once



class CToolView;
class CMyTexture
{
public:
	CMyTexture();
	~CMyTexture();

public:
	void		Initialize(void);
	void		Update(void);
	void		Render(const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount, int iAlpha, int iRed, int iGreen, int iBlue, float fOffSetX, float fOffSetY);
};

