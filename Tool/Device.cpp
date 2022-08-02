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

	// 1. ��ġ ���� ��ü ����(m_pSDK)

	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION); // SDK ��ü�� ������ �°� �����ϴ� �Լ�

	// 2. ��ġ ����(���� ������ ����)

	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : ��ġ�� ���� ������ ������ �Լ�
	// D3DADAPTER_DEFAULT : ������ �������� �⺻ �׷��� ī�带 �ǹ�
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		AfxMessageBox(L"GetDeviceCaps Failed");
		return E_FAIL;
	}

	// ���ؽ� ���μ��� = ������ ��ȯ + ���� ����

	DWORD	vp = 0;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;


	// 3. ��ġ�� ������ ��ü ����

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(d3dpp);

	//CreateDevice : ��ġ�� ������ ��ü�� �����ϴ� �Լ�
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

		// ��������Ʈ �� ��ü ����

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"D3DXCreateSprite Failed");
		return E_FAIL;
	}

	//Font�ʱ�ȭ
	D3DXFONT_DESCW	tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"�ü�");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"m_pFont Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::Render_Begin(void)
{
	// �ĸ���۸� ����� -> �ĸ� ���ۿ� �׸��� -> �ĸ���ۿ� ������۸� ��ȯ�Ѵ�

	m_pDevice->Clear(0,			// ���� ��Ʈ�� ����	
					nullptr,	// ���� ��Ʈ���� ù ��° �ּ�(���� nullptr�� ��� ��ü ������ ��� ����)
					D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // ���ٽ�, Ÿ��, z���� ��� ���ڴٴ� �ǹ�
					D3DCOLOR_ARGB(255, 125, 125, 125),	// �� ������ ����
					1.f,		// ���� ������ �ʱ�ȭ ��
					0);			// ���ٽ� ������ �ʱ�ȭ ��

	m_pDevice->BeginScene();

	// 2D �̹����� �׸� �� �ֵ��� ��ġ�� �غ��ϰ�, ���ĺ����� �����ϴ� �ɼ��� �־���
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);


}

void CDevice::Render_End(HWND hWnd)
{
	m_pSprite->End();

	m_pDevice->EndScene();

	// 1, 2�� ���� : D3DSWAPEFFECT_COPY�� �ۼ����� �ʴ� �� NULL
	// 3�� ���� : ��� ��� ������ �ڵ�
	// 4�� ���� :  D3DSWAPEFFECT_COPY�� �ۼ����� �ʴ� �� NULL

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


	// D3DSWAPEFFECT_DISCARD : ���� ü�� ���
	// D3DSWAPEFFECT_FLIP : ���� �ϳ��� ������ ���鼭 ����ϴ� ���
	// D3DSWAPEFFECT_COPY : ���� ���۸��� ������ ���� ���

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = g_hWnd;
	
	// â��� or ��üȭ�� ��� 
	d3dpp.Windowed = true;		// â ��� false�� ��� ��ü ȭ��

	d3dpp.EnableAutoDepthStencil = true;	// dx�� �˾Ƽ� ���� / ���ٽ� ���۸� ����� ������ ��
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;


	// ���α׷��� ����Ϳ��� ����

	// â ��� ��� �� ��ġ�� �ƴ� OS�� ���� ����� ������� ����
	// ��ü ȭ�� ��� ��� �� ���� ����� ������� ���� ����

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;


	// ����� ������� �ÿ��� ����
	
	// D3DPRESENT_INTERVAL_IMMEDIATE; // ��� �ÿ�
	// D3DPRESENT_INTERVAL_DEFAULT; // ������ ������ DX�� �˾Ƽ� ����(���� ����� ������� ����)

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

}
