#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)

private:
	CDevice();
	~CDevice();

public:
	LPDIRECT3DDEVICE9		Get_Device() { return m_pDevice; }
	LPD3DXSPRITE			Get_Sprite(void) { return m_pSprite; }
	LPD3DXFONT				Get_Font(void) { return m_pFont; }
public:
	// ��ġ �ʱ�ȭ ����

	//1. ��ġ�� ������ ��ü ����
	//2. ��ġ ���� ����(���� ����)
	//3. ��ġ�� ������ ��ü ����

	HRESULT		Init_Device(void);	
	
	void		Render_Begin(void);
	void		Render_End(HWND hWnd = nullptr);
	
	void		Release(void);

private:
	void		Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp);


private:
	LPDIRECT3D9			m_pSDK;	// ��ġ�� ���縦 ����� com��ü
	LPDIRECT3DDEVICE9	m_pDevice;	// �׷��� ��ġ�� �����ϴ�  com ��ü

	LPD3DXSPRITE		m_pSprite;	// dx �󿡼� 2d �̹����� ���, �����ϴ� com ��ü
	LPD3DXFONT			m_pFont;
};

