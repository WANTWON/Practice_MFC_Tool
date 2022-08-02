#include "stdafx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
	: m_pSDK(nullptr), m_pDevice(nullptr), m_pSprite(nullptr), m_pFont(nullptr)
{
}


CDevice::~CDevice()
{
	Release();
}

HRESULT CDevice::Init_Device(void)
{

	// 1. 장치 조사 객체 생성(m_pSDK)

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION); // SDK 객체를 버전에 맞게 생성하는 함수

	// 2. 장치 조사(지원 수준을 조사)

	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : 장치에 대한 정보를 얻어오는 함수
	// D3DADAPTER_DEFAULT : 정보를 얻으려는 기본 그래픽 카드를 의미
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		AfxMessageBox(L"GetDeviceCaps Failed");
		return E_FAIL;
	}

	// 버텍스 프로세싱 = 정점의 변환 + 조명 연산

	DWORD	vp = 0;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;


	// 3. 장치를 제어할 객체 생성

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(d3dpp);

	//CreateDevice : 장치를 제어할 객체를 생성하는 함수
		if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									g_hWnd, 
									vp, 
									&d3dpp, 
									&m_pDevice)))
	{
		AfxMessageBox(L"Create Device Failed");
		return E_FAIL;
	}
	
	//return E_FAIL;

		// 스프라이트 컴 객체 생성

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}

	//Font초기화
	D3DXFONT_DESCW	tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"궁서");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"m_pFont Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::Render_Begin(void)
{
	// 후면버퍼를 지운다 -> 후면 버퍼에 그린다 -> 후면버퍼와 전면버퍼를 교환한다

	m_pDevice->Clear(0,			// 지울 렉트의 개수	
					nullptr,	// 지울 렉트들의 첫 번째 주소(만약 nullptr인 경우 전체 영역을 모두 지움)
					D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // 스텐실, 타겟, z버퍼 모두 비우겠다는 의미
					D3DCOLOR_ARGB(255, 125, 125, 125),	// 백 버퍼의 색상
					1.f,		// 깊이 버퍼의 초기화 값
					0);			// 스텐실 버퍼의 초기화 값

	m_pDevice->BeginScene();

	// 2D 이미지를 그릴 수 있도록 장치를 준비하고, 알파블랜딩을 지원하는 옵션을 넣어줌
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);


}

void CDevice::Render_End(HWND hWnd)
{
	m_pSprite->End();

	m_pDevice->EndScene();

	// 1, 2번 인자 : D3DSWAPEFFECT_COPY로 작성되지 않는 한 NULL
	// 3번 인자 : 출력 대상 윈도우 핸들
	// 4번 인자 :  D3DSWAPEFFECT_COPY로 작성되지 않는 한 NULL

	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}

void CDevice::Release(void)
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
	Safe_Release(m_pSDK);
}

void CDevice::Set_Parameters(D3DPRESENT_PARAMETERS & d3dpp)
{
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;

	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;


	// D3DSWAPEFFECT_DISCARD : 스왑 체인 방식
	// D3DSWAPEFFECT_FLIP : 버퍼 하나로 뒤집어 가면서 사용하는 방식
	// D3DSWAPEFFECT_COPY : 더블 버퍼링과 유사한 복사 방식

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = g_hWnd;
	
	// 창모드 or 전체화면 모드 
	d3dpp.Windowed = true;		// 창 모드 false인 경우 전체 화면

	d3dpp.EnableAutoDepthStencil = true;	// dx가 알아서 깊이 / 스텐실 버퍼를 만들고 관리할 것
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;


	// 프로그램과 모니터와의 관계

	// 창 모드 출력 시 장치가 아닌 OS에 의해 모니터 재생률을 관리
	// 전체 화면 모드 룰력 시 현재 모니터 재생률을 토대로 수행

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;


	// 모니터 재생률과 시연의 간격
	
	// D3DPRESENT_INTERVAL_IMMEDIATE; // 즉시 시연
	// D3DPRESENT_INTERVAL_DEFAULT; // 적절한 간격을 DX가 알아서 결정(보통 모니터 재생률을 따라감)

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

}
