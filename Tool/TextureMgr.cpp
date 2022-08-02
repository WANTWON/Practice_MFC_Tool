#include "stdafx.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO * CTextureMgr::Get_Texture(const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	auto	iter = find_if(m_matTex.begin(), m_matTex.end(), [&](auto& MyPair)->bool
	{
		if (MyPair.first == pObjKey)
			return true;

		return false;
	});

	if (iter == m_matTex.end())
		return nullptr;
	
	return iter->second->Get_Texture(pStateKey, iCount);
}

HRESULT CTextureMgr::Insert_Texture(const TCHAR * pFilePath, TEXTYPE eType, const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	auto	iter = find_if(m_matTex.begin(), m_matTex.end(), [&](auto& MyPair)->bool
	{
		if (MyPair.first == pObjKey)
			return true;

		return false;
	});

	if (iter == m_matTex.end())
	{
		CTexture*		pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->Insert_Texture(pFilePath, pStateKey, iCount)))
		{
			AfxMessageBox(pFilePath);
			return E_FAIL;
		}

		m_matTex.insert({ pObjKey, pTexture });
	}

	else if (eType == TEX_MULTI)
	{
		iter->second->Insert_Texture(pFilePath, pStateKey, iCount);
	}

	return S_OK;
}

void CTextureMgr::Release(void)
{
	for_each(m_matTex.begin(), m_matTex.end(), [](auto& MyPair)
	{
		Safe_Delete<CTexture*>(MyPair.second);
	});
	m_matTex.clear();
}

HRESULT CTextureMgr::ReadImgPath(const wstring& wstrPath)
{
	wifstream		fin;

	fin.open(L"../Data/ImgPath.txt", ios::in);

	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR	szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MAX_STR] = L"";
		TCHAR	szPath[MAX_PATH] = L"";

		wstring	wstrCombined = L"";


		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			int	iCount = _ttoi(szCount);

			if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(szPath, TEX_MULTI, szObjKey, szStateKey, iCount)))
			{
				ERR_MSG(L"Tile Image Insert Failed");
				return E_FAIL;
			}

		}

		fin.close();
	}

	return S_OK;
}