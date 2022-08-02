#include "stdafx.h"
#include "MultiTexture.h"
#include "Device.h"


CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO* CMultiTexture::Get_Texture(const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	auto	iter = find_if(m_mapMultiTex.begin(), m_mapMultiTex.end(), [&](auto& MyPair)->bool {

		if (MyPair.first == pStateKey)
			return true;

		return false;
	});

	if (iter == m_mapMultiTex.end())
		return nullptr;

	return iter->second[iCount];
}

HRESULT CMultiTexture::Insert_Texture(const TCHAR* pFilePath, const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	// 경로의 길이는 절대 520바이트를 넘어서면 안된다.
	TCHAR	szFullPath[MAX_PATH] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, pFilePath, i);

		TEXINFO*	pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			AfxMessageBox(szFullPath);
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

	
		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
			szFullPath,
			pTexInfo->tImgInfo.Width,
			pTexInfo->tImgInfo.Height,
			pTexInfo->tImgInfo.MipLevels,
			0,	// 매개 변수가 0인경우 일반적인 텍스처, 다른 옵션으로 D3DUSAGE_RENDERTARGET으로 설정하면 화면 출력용 텍스처 생성
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT, // 이미지 필터링 방식, 이미지를 확대, 축소 시 픽셀들을 어떻게 처리할 것인가
			D3DX_DEFAULT, // 밉맵을 이용한 이미지 확대, 축소 시 필터링을 어떻게 할 것인가
			0, // 제거할 색상(0인 경우 컬러키를 무효화)
			nullptr,
			nullptr,
			&(pTexInfo->pTexture))))
		{
			AfxMessageBox(L"Multi Texture Load Failed");
			Safe_Delete(pTexInfo);
			return E_FAIL;
		}

		m_mapMultiTex[pStateKey].push_back(pTexInfo);
	}
	
	return S_OK;
}

void CMultiTexture::Release(void)
{
	for_each(m_mapMultiTex.begin(), m_mapMultiTex.end(), [](auto& MyPair)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(), [](auto& iter)
		{
			Safe_Release(iter->pTexture);
			Safe_Delete(iter);
		});
		MyPair.second.clear();
	});

	m_mapMultiTex.clear();
}
