// ObjView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjView.h"
#include "Device.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "ObjTool.h"
#include "MyTexture.h"
#include "MyForm.h"


// CObjView

IMPLEMENT_DYNCREATE(CObjView, CView)

CObjView::CObjView()
{

}

CObjView::~CObjView()
{
}

BEGIN_MESSAGE_MAP(CObjView, CView)
END_MESSAGE_MAP()


// CObjView �׸����Դϴ�.

void CObjView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

	CDevice::Get_Instance()->Render_Begin();

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	//CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	CMyForm*		pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));

	CMyTexture*		pMyTexture = pMyForm->m_ObjTool.m_pMyTexture;

	/*if (pMyForm->m_ObjTool.Get_ObjKey() != L"" &&
		pMyForm->m_ObjTool.Get_StateKey() != L"")*/
	pMyTexture->Render(pMyForm->m_ObjTool.Get_ObjKey(), pMyForm->m_ObjTool.Get_StateKey(), pMyForm->m_ObjTool.Get_Pos(),
		pMyForm->m_ObjTool.Get_Alpha(), pMyForm->m_ObjTool.Get_Red(), pMyForm->m_ObjTool.Get_Green(), pMyForm->m_ObjTool.Get_Blue(),
		pMyForm->m_ObjTool.Get_OffSetX(), pMyForm->m_ObjTool.Get_OffSetY());

	CDevice::Get_Instance()->Render_End(m_hWnd);

	Invalidate(FALSE);
}


#pragma region ���ʿ�

// CObjView �����Դϴ�.

#ifdef _DEBUG
void CObjView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CObjView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CObjView �޽��� ó�����Դϴ�.

#pragma endregion ���ʿ�
