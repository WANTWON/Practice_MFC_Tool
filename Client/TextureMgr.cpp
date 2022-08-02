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
	auto	iter = find_if(m_mapTex.begin(), m_mapTex.end(), [&](auto& Pair)->bool
	{
		if (pObjKey == Pair.first)
			return true;

		return false;
	});

	if (iter == m_mapTex.end())
		return nullptr;

	return iter->second->Get_Texture(pStateKey, iCount);
}

HRESULT CTextureMgr::ReadImgPath(const wstring & wstrpath)
{
	wifstream fin;
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

			int iCount = _ttoi(szCount);

			if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(szPath, TEX_MULTI, szObjKey, szStateKey, iCount)))
				return E_FAIL;

			//if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Image/PathFinder/Object/Tree/Tree%d.png", TEX_MULTI, L"Tree", L"Tree", 50)))
			//	return E_FAIL;

			//if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Image/PathFinder/Object/House/House%d.png", TEX_MULTI, L"House", L"House", 4)))
			//	return E_FAIL;

			//if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Image/PathFinder/Object/Fence/Fence%d.png", TEX_MULTI, L"Fence", L"Fence", 16)))
			//	return E_FAIL;

			//if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Image/PathFinder/Object/Plant/Plant%d.png", TEX_MULTI, L"Plant", L"Plant", 19)))
			//	return E_FAIL;

			//if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Image/PathFinder/Object/Etc/Etc%d.png", TEX_MULTI, L"Etc", L"Etc", 38)))
			//	return E_FAIL;

			//if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Image/PathFinder/Player/RunLD/Maximin_LD_Run%d.png", TEX_MULTI, L"Player", L"RunLD", 10)))
			//	return E_FAIL;

			if (fin.eof())
				break;
	
		}
		fin.close();
	}
	return S_OK;
}

HRESULT CTextureMgr::Insert_Texture(const TCHAR * pFilePath, TEXTYPE eType, const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	//auto	iter = find_if(m_mapTex.begin(), m_mapTex.end(), CTag_Finder(pObjKey));

	m_wstrFullPath = pFilePath;

	auto	iter = find_if(m_mapTex.begin(), m_mapTex.end(), [&](auto& Pair)->bool 
	{
		if (pObjKey == Pair.first)
			return true;

		return false;
	});

	if (iter == m_mapTex.end())
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

		if (FAILED(pTexture->InsertTexture(pFilePath, pStateKey, iCount)))
		{
			ERR_MSG(pFilePath);
			return E_FAIL;
		}

		m_mapTex.emplace(pObjKey, pTexture);
	}

	else if (eType == TEX_MULTI)
		iter->second->InsertTexture(pFilePath, pStateKey, iCount);

	return S_OK;
}

void CTextureMgr::Release(void)
{
	//for_each(m_mapTex.begin(), m_mapTex.end(), CDeleteMap());

	for_each(m_mapTex.begin(), m_mapTex.end(), [](auto& Pair)
	{
		Safe_Delete(Pair.second);
	});

	m_mapTex.clear();
}
