#pragma once

#include "SingleTexture.h"
#include "MultiTexture.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

public:
	CTextureMgr();
	~CTextureMgr();
	
public:
	const TEXINFO*		Get_Texture(const TCHAR* pObjKey,
									const TCHAR* pStateKey = L"",
									const int& iCount = 0);

	const wstring&		Get_String(void) { return m_wstrFullPath; }

	void				Set_String(wstring wstrFullPath) { m_wstrFullPath = wstrFullPath; }



public:
	HRESULT		ReadImgPath(const wstring& wstrpath);

	HRESULT		Insert_Texture(const TCHAR* pFilePath,	
								TEXTYPE	eType,
								const TCHAR* pObjKey,// �̹��� ���
								const TCHAR* pStateKey = L"",	// ��Ƽ �ؽ�ó�� ��� ���Ǵ� ���� Ű ��
								const int& iCount = 0);			// ��Ƽ �ؽ�ó�� ��� ���Ǵ� �̹��� ���

	 void		Release(void);

private:
	map<wstring, CTexture*>			m_mapTex;
	wstring							m_wstrFullPath = L"";
};

