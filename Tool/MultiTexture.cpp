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
	// ����� ���̴� ���� 520����Ʈ�� �Ѿ�� �ȵȴ�.
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
			0,	// �Ű� ������ 0�ΰ�� �Ϲ����� �ؽ�ó, �ٸ� �ɼ����� D3DUSAGE_RENDERTARGET���� �����ϸ� ȭ�� ��¿� �ؽ�ó ����
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT, // �̹��� ���͸� ���, �̹����� Ȯ��, ��� �� �ȼ����� ��� ó���� ���ΰ�
			D3DX_DEFAULT, // �Ӹ��� �̿��� �̹��� Ȯ��, ��� �� ���͸��� ��� �� ���ΰ�
			0, // ������ ����(0�� ��� �÷�Ű�� ��ȿȭ)
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
