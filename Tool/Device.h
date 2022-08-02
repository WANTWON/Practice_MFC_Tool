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
	// 장치 초기화 과정

	//1. 장치를 조사할 객체 생성
	//2. 장치 조사 시작(지원 수준)
	//3. 장치를 제어할 객체 생성

	HRESULT		Init_Device(void);	
	
	void		Render_Begin(void);
	void		Render_End(HWND hWnd = nullptr);
	
	void		Release(void);

private:
	void		Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp);


private:
	LPDIRECT3D9			m_pSDK;	// 장치를 조사를 담당할 com객체
	LPDIRECT3DDEVICE9	m_pDevice;	// 그래픽 장치를 제어하는  com 객체

	LPD3DXSPRITE		m_pSprite;	// dx 상에서 2d 이미지를 출력, 제어하는 com 객체
	LPD3DXFONT			m_pFont;
};

