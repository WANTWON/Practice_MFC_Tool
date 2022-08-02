// MiniView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MiniView.h"
#include "Device.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Stage.h"


// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView �׸����Դϴ�.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

	CDevice::Get_Instance()->Render_Begin();

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	CStage*			pStage = pToolView->m_pStage;
	CMyTerrain*		pMyTerrain = pToolView->m_pTerrain;
	CConstruct*		pMyStruct = pToolView->m_pConstruct;
	CNpc*			pNpc = pToolView->m_pNpc;

	pStage->Set_MiniView(this);
	pMyTerrain->Set_MiniView(this);
	pMyStruct->Set_MiniView(this);
	pNpc->Set_MiniView(this);

	pStage->Mini_Render();
	pMyTerrain->Mini_Render();
	pMyStruct->Mini_Render();
	pNpc->Mini_Render();

	CDevice::Get_Instance()->Render_End(m_hWnd);


}

#pragma region ���ʿ�

// CMiniView �����Դϴ�.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView �޽��� ó�����Դϴ�.

#pragma endregion ���ʿ�

