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

	// D3DXGetImageInfoFromFile : 지정한 이미지 파일에 관한 정보를 얻어와 D3DXIMAGE_INFO 구조체에 기록하는 함수 
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		AfxMessageBox(pFilePath);
		Safe_Delete(m_pTexInfo);
		return E_FAIL;
	}

	// D3DPOOL_DEFAULT : 가장 적합한 메모리에 보관(비디오 메모리)
	// D3DPOOL_MANAGED : DX3D에 의해 자원을 관리하며 그래픽 메모리를 사용하지만 이를 RAM에 백업
	// D3DPOOL_SYSTEMMEM : 시스템 메모리에 보관
	// D3DPOOL_SCRATCH : 시스템 메모리를 사용하지만 DX장치가 접근 불가

	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(), 
		pFilePath,
		m_pTexInfo->tImgInfo.Width,
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels, 
		0,	// 매개 변수가 0인경우 일반적인 텍스처, 다른 옵션으로 D3DUSAGE_RENDERTARGET으로 설정하면 화면 출력용 텍스처 생성
		m_pTexInfo->tImgInfo.Format, 
		D3DPOOL_MANAGED, 
		D3DX_DEFAULT, // 이미지 필터링 방식, 이미지를 확대, 축소 시 픽셀들을 어떻게 처리할 것인가
		D3DX_DEFAULT, // 밉맵을 이용한 이미지 확대, 축소 시 필터링을 어떻게 할 것인가
		0, // 제거할 색상(0인 경우 컬러키를 무효화)
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
