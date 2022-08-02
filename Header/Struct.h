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
	LPDIRECT3DTEXTURE9	pTexture;	// ��ü, �̹����� ���õ� ���� ����� �����ϴ� Ŭ����

	D3DXIMAGE_INFO		tImgInfo;	// ����ü, �̹����� ���õ� �������� �����͸� �����ϴ� ����ü

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
	float	fFrame = 0;	// ���� �ִϸ��̼� ��� ����
	float	fMax;	// �ִ� �ִϸ��̼� ����

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
