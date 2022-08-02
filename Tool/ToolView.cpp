
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)

	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
	: m_pDevice(CDevice::Get_Instance()), m_pTerrain(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	
}

CToolView::~CToolView()
{
}

#pragma region ���ʿ�
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


#pragma endregion ���ʿ�

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	//SetScrollSizes : ��ũ�� ���� ����� �����ϴ� CScrollViewŬ������ ��� �Լ�
	//MM_TEXT : �ȼ� ������ ��ũ�� �ٸ� ����
	//2���� : ��ũ�� ���� ������ x,y ��
	//CSize : ����� �����ϴ� �뵵�� mfc Ŭ����

	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX, TILEY * TILECY / 2));

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	// AfxGetMainWnd : ������ ���� �������� ������ ��ȯ�ϴ� �Լ�
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// GetWindowRect : ���� â�� RECT ������ ������ �Լ�
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : ������ �Ű� ������ RECT ���� �� ����ϴ� �Լ�
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rCScrollView{};

	// GetClientRect : ���� view ������ ������ �Լ�
	GetClientRect(&rCScrollView);

	float	fRowFrm = float(rcWnd.right - rCScrollView.right);
	float	fColFrm = float(rcWnd.bottom- rCScrollView.bottom);

	/*SetWindowPos : �Ű����� ��� ������ ��ġ�� ũ�⸦ �����ϴ� �Լ�
	1. ��ġ�� �������� Z������ ���� ������
	2, 3. X,Y ��ǥ
	4, 5. ����, ���� ũ��
	6. SWP_NOZORDER : ���� ������ �����ϰڴٴ� �÷���(NULL�� ��� ���� ������ ���� �ʰڴ�.)*/

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

// CToolView �׸���

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	Safe_Delete(m_pTerrain);
	Safe_Delete(m_pConstruct);
	Safe_Delete(m_pStage);
	

	CTextureMgr::Get_Instance()->Destroy_Instance();
	m_pDevice->Destroy_Instance();

}

// ������ �������� �̿��Ͽ� Ÿ�� ��ŷ�� �����ؿͶ�

/*
y = ax + b 

a : ����
b : y����*/

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonDown(nFlags, point);

	//Invalidate : ȣ�� �� �����쿡 WM_PAINT�� WM_ERASEBKGND �޽����� �߻���Ŵ
	// On_Draw�Լ��� �ٽ� �ѹ� ȣ��
	// WM_ERASEBKGND : ����� ����� �޼���

	//FALSE �� �� : WM_PAINT �޼����� �߻�
	//TRUE �� �� : WM_PAINT, WM_ERASEBKGND �� �� �߻�
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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

	 //������ ����?
	

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
		pMyForm->m_TreeTool.UpdateData(FALSE); //������Ʈ ���� �ٸ��� �Ժη��ϸ� ������.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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