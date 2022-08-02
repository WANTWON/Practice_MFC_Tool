
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "TextureMgr.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "MyForm.h"
#include "MapTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND	g_hWnd;


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)

	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
	: m_pDevice(CDevice::Get_Instance()), m_pTerrain(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	
}

CToolView::~CToolView()
{
}

#pragma region 불필요
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


#pragma endregion 불필요

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	//SetScrollSizes : 스크롤 바의 사이즈를 지정하는 CScrollView클래스의 멤버 함수
	//MM_TEXT : 픽셀 단위로 스크롤 바를 조정
	//2인자 : 스크롤 바의 사이즈 x,y 값
	//CSize : 사이즈를 저장하는 용도의 mfc 클래스

	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX, TILEY * TILECY / 2));

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// AfxGetMainWnd : 현재의 메인 윈도우의 정보를 반환하는 함수
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// GetWindowRect : 현재 창의 RECT 정보를 얻어오는 함수
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : 지정한 매개 변수로 RECT 값을 재 기록하는 함수
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rCScrollView{};

	// GetClientRect : 현재 view 정보를 얻어오는 함수
	GetClientRect(&rCScrollView);

	float	fRowFrm = float(rcWnd.right - rCScrollView.right);
	float	fColFrm = float(rcWnd.bottom- rCScrollView.bottom);

	/*SetWindowPos : 매개변수 대로 윈도우 위치와 크기를 조정하는 함수
	1. 배치할 윈도우의 Z순서에 대한 포인터
	2, 3. X,Y 좌표
	4, 5. 가로, 세로 크기
	6. SWP_NOZORDER : 현재 순서를 유지하겠다는 플래그(NULL인 경우 순서 변경을 하지 않겠다.)*/

	pMainFrm->SetWindowPos(nullptr, 0, 0, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);
	
	g_hWnd = m_hWnd;

	if (FAILED(m_pDevice->Init_Device()))
		AfxMessageBox(L"Init_Device Failed");

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Cube.png", TEX_SINGLE, L"Cube")))
	{
		AfxMessageBox(L"CUBE Image Insert Failed");
		return;
	}

	
	
	m_pTerrain = new CMyTerrain;
	m_pTerrain->Initialize();
	m_pTerrain->Set_MainView(this);

	m_pStage = new CStage;
	m_pStage->Initialize();
	m_pStage->Set_MainView(this);

	m_pConstruct = new CConstruct;
	dynamic_cast<CConstruct*>(m_pConstruct)->Set_Tile(dynamic_cast<CMyTerrain*>(m_pTerrain)->Get_VecTile());
	m_pConstruct->Initialize();
	m_pConstruct->Set_MainView(this);

	m_pNpc = new CNpc;
	dynamic_cast<CNpc*>(m_pNpc)->Set_Tile(dynamic_cast<CMyTerrain*>(m_pTerrain)->Get_VecTile());
	m_pNpc->Initialize();
	m_pNpc->Set_MainView(this);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	m_pDevice->Render_Begin();

	
	m_pStage->Render();
	m_pTerrain->Render();
	m_pConstruct->Render();
	m_pNpc->Render();
	

	if(LastClick == NPCTOOL)
		m_pNpc->Following_Render();
	else if ((LastClick == TREETOOL))
		m_pConstruct->Following_Render();



	m_pDevice->Render_End();
	
}


void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Safe_Delete(m_pTerrain);
	Safe_Delete(m_pConstruct);
	Safe_Delete(m_pStage);
	

	CTextureMgr::Get_Instance()->Destroy_Instance();
	m_pDevice->Destroy_Instance();

}

// 직선의 방정식을 이용하여 타일 피킹을 구현해와라

/*
y = ax + b 

a : 기울기
b : y절편*/

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);

	//Invalidate : 호출 시 윈도우에 WM_PAINT와 WM_ERASEBKGND 메시지를 발생시킴
	// On_Draw함수를 다시 한번 호출
	// WM_ERASEBKGND : 배경을 지우는 메세지

	//FALSE 일 때 : WM_PAINT 메세지만 발생
	//TRUE 일 때 : WM_PAINT, WM_ERASEBKGND 둘 다 발생
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	CMyForm*		pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));

	TOOLID			LastClick = pMyForm->Get_LastClick();

	if (LastClick == TAPTOOL)
	{
		CTabTool*		pTabTool = &(pMyForm->m_TabTool);
		//CMapTool*		pMapTool = &(pMyForm->m_MapTool);
		CMapTool*		pMapTool = pTabTool->m_pMapTool;

		if (nullptr == pMapTool)
			m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x) + GetScrollPos(0), float(point.y) + GetScrollPos(1), 0.f), 0);
		else
			m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x) + GetScrollPos(0), float(point.y) + GetScrollPos(1), 0.f), pMapTool->m_iDrawID);
	}

	else if (LastClick == TREETOOL)
	{
		CTreeTool*	pTreeTool = &(pMyForm->m_TreeTool);

		m_pConstruct->Add_Constructure(D3DXVECTOR3(float(point.x) + GetScrollPos(0), float(point.y) + GetScrollPos(1), 0.f), pTreeTool->m_strObjKey,pTreeTool->m_strOption, pTreeTool->m_iDrawID,
			pTreeTool->m_Horizontal.GetCheck(), pTreeTool->m_Vertical.GetCheck(), pTreeTool->m_fScaleX, pTreeTool->m_fScaleY);
	}

	else if (LastClick == NPCTOOL)
	{
		CObjTool* pObjTool = &(pMyForm->m_ObjTool);

		m_pNpc->Add_NPC(D3DXVECTOR3(float(point.x) + GetScrollPos(0), float(point.y) + GetScrollPos(1), 0.f),
			pObjTool->m_pObjKey, pObjTool->m_pStateLKey,
			pObjTool->iAlpha, pObjTool->iRed, pObjTool->iGreen, pObjTool->iBlue, 
			pObjTool->fOffSetX, pObjTool->fOffSetY);
	}


	pMiniView->Invalidate(FALSE);

	Invalidate(FALSE);

}


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	CMyForm*		pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	LastClick = pMyForm->Get_LastClick();

	CTreeTool*	pTreeTool = &(pMyForm->m_TreeTool);
	CObjTool*   pObjTool = &(pMyForm->m_ObjTool);

	float ptX = float(point.x + GetScrollPos(0));
	float ptY = float(point.y + GetScrollPos(1));

	int iTileIndex = 0;
	iTileIndex = m_pTerrain->Get_TileIndex(D3DXVECTOR3(ptX, ptY, 0.f));

	 //터지는 이유?
	

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		
		if (LastClick == TAPTOOL)
		{
			CTabTool*		pTabTool = &(pMyForm->m_TabTool);
			//CMapTool*		pMapTool = &(pMyForm->m_MapTool);
			CMapTool*		pMapTool = pTabTool->m_pMapTool;

			if (nullptr == pMapTool)
				m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x) + GetScrollPos(0), float(point.y) + GetScrollPos(1), 0.f), 0);
			else
				m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x) + GetScrollPos(0), float(point.y) + GetScrollPos(1), 0.f), pMapTool->m_iDrawID);
		}

	}



	if (LastClick == TREETOOL)
	{
		pMyForm->m_TreeTool.UpdateData(TRUE);

		m_pConstruct->Following(D3DXVECTOR3(float(point.x) + GetScrollPos(0), float(point.y) + GetScrollPos(1), 0.f),pTreeTool->m_strObjKey, pTreeTool->m_strOption, pTreeTool->m_iDrawID,
			pTreeTool->m_Horizontal.GetCheck(), pTreeTool->m_Vertical.GetCheck(), pTreeTool->m_fScaleX, pTreeTool->m_fScaleY);

		pTreeTool->m_iTileIndex = iTileIndex;
		pMyForm->m_TreeTool.UpdateData(FALSE); //업데이트 주의 다른데 함부로하면 터진다.
	}

	if (LastClick == NPCTOOL)
	{
		m_pNpc->Following(D3DXVECTOR3(float(point.x) + GetScrollPos(0), float(point.y) + GetScrollPos(1), 0.f),
			pObjTool->m_pObjKey , pObjTool->m_pStateLKey,
			pObjTool->fOffSetX, pObjTool->fOffSetY);
	}


	Invalidate(FALSE);
	pMiniView->Invalidate(FALSE);
	

}



void CToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnRButtonDown(nFlags, point);

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniView*		pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	CMyForm*		pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));

	TOOLID			LastClick = pMyForm->Get_LastClick();

	if (LastClick == TAPTOOL)
	{
		CTabTool*		pTabTool = &(pMyForm->m_TabTool);
		//CMapTool*		pMapTool = &(pMyForm->m_MapTool);
		CMapTool*		pMapTool = pTabTool->m_pMapTool;

		if (nullptr == pMapTool)
			m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x) + GetScrollPos(0), float(point.y) + GetScrollPos(1), 0.f), 0);
		else
			m_pTerrain->Tile_Change(D3DXVECTOR3(float(point.x) + GetScrollPos(0), float(point.y) + GetScrollPos(1), 0.f), 0);
	}

	else if (LastClick == TREETOOL)
	{
		CTreeTool*	pTreeTool = &(pMyForm->m_TreeTool);

		m_pConstruct->Delete_Constructure(D3DXVECTOR3(float(point.x) + GetScrollPos(0), float(point.y) + GetScrollPos(1), 0.f));
		
	}

	pMiniView->Invalidate(FALSE);

	Invalidate(FALSE);


}