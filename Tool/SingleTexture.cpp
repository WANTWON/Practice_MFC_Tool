#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"


CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{

}

CSingleTexture::~CSingleTexture()
{
	Release();
}

HRESULT CSingleTexture::Insert_Texture(const TCHAR* pFilePath, const TCHAR* pStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// D3DXGetImageInfoFromFile : ������ �̹��� ���Ͽ� ���� ������ ���� D3DXIMAGE_INFO ����ü�� ����ϴ� �Լ� 
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		AfxMessageBox(pFilePath);
		Safe_Delete(m_pTexInfo);
		return E_FAIL;
	}

	// D3DPOOL_DEFAULT : ���� ������ �޸𸮿� ����(���� �޸�)
	// D3DPOOL_MANAGED : DX3D�� ���� �ڿ��� �����ϸ� �׷��� �޸𸮸� ��������� �̸� RAM�� ���
	// D3DPOOL_SYSTEMMEM : �ý��� �޸𸮿� ����
	// D3DPOOL_SCRATCH : �ý��� �޸𸮸� ��������� DX��ġ�� ���� �Ұ�

	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(), 
		pFilePath,
		m_pTexInfo->tImgInfo.Width,
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels, 
		0,	// �Ű� ������ 0�ΰ�� �Ϲ����� �ؽ�ó, �ٸ� �ɼ����� D3DUSAGE_RENDERTARGET���� �����ϸ� ȭ�� ��¿� �ؽ�ó ����
		m_pTexInfo->tImgInfo.Format, 
		D3DPOOL_MANAGED, 
		D3DX_DEFAULT, // �̹��� ���͸� ���, �̹����� Ȯ��, ��� �� �ȼ����� ��� ó���� ���ΰ�
		D3DX_DEFAULT, // �Ӹ��� �̿��� �̹��� Ȯ��, ��� �� ���͸��� ��� �� ���ΰ�
		0, // ������ ����(0�� ��� �÷�Ű�� ��ȿȭ)
		nullptr, 
		nullptr,
		&(m_pTexInfo->pTexture))))
	{
		AfxMessageBox(L"Single Texture Load Failed");
		Safe_Delete(m_pTexInfo);
		return E_FAIL;
	}


	return S_OK;
}

void CSingleTexture::Release(void)
{
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}
