#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;
	D3DXVECTOR3		vSize;

	D3DXMATRIX		matWorld;
}INFO;

typedef		struct tagTexture
{
	LPDIRECT3DTEXTURE9	pTexture;	// 객체, 이미지와 관련된 각종 기능을 제공하는 클래스

	D3DXIMAGE_INFO		tImgInfo;	// 구조체, 이미지와 관련된 실제적인 데이터를 저장하는 구조체

}TEXINFO;


typedef struct tagTile
{
	D3DXVECTOR3	vPos;
	D3DXVECTOR3	vSize = { 1,1,1 };
	BYTE		byOption;
	BYTE		byDrawID;

	int				iIndex = 0;
	int				iParentIndex = 0;
	bool			beEmplaced = false;

}TILE;

typedef struct tagObj
{
	D3DXVECTOR3	vPos;
	D3DXVECTOR3	vSize;
	BYTE		byDrawID;
	wstring		ObjKey;
	wstring		StateKey;

	D3DXVECTOR3	vScale = {1,1,1};

}CONSTRUCT;

typedef struct tagUnitData
{
#ifndef _AFX
	wstring		strName;

#else	
	CString		strName;
#endif

	int			iAttack;
	int			iHp;
	BYTE		byJobIndex;
	BYTE		byItem;

}UNITDATA;


static D3DXVECTOR3		Get_Mouse()
{
	POINT	Pt{};

	GetCursorPos(&Pt);
	ScreenToClient(g_hWnd, &Pt);

	return D3DXVECTOR3((float)Pt.x, (float)Pt.y, 0.f);
}


typedef	struct tagTexturePath
{
	wstring			wstrObjKey		= L"";
	wstring			wstrStateKey	= L"";
	wstring			wstrPath		= L"";
	int				iCount			= 0;

}IMGPATH;

typedef	struct tagFrame
{
	float	fFrame = 0;	// 현재 애니메이션 재생 지점
	float	fMax;	// 최대 애니메이션 개수

}FRAME;

typedef struct tagNpc
{
	INFO  m_tInfo;
	FRAME m_tFrame;

	wstring		ObjKey;
	wstring		StateKey;


	int Alpha = 255;
	int R = 255;
	int G = 255;
	int B = 255;

	float OffsetX = 0;
	float OffsetY = 0;


}NPC;
