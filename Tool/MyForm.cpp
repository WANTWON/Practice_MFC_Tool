// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnUnitTool)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnMapTool)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyForm::OnTabTool)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyForm::OnPathFind)
	ON_BN_CLICKED(IDC_BUTTON6, &CMyForm::OnTreeTool)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyForm::OnObjTool)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.


void CMyForm::OnUnitTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	/*if (nullptr == m_UnitTool.GetSafeHwnd())
		m_UnitTool.Create(IDD_UNITTOOL);

	m_UnitTool.ShowWindow(SW_SHOW);*/
}


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_Font.CreatePointFont(180, L"궁서"); 

	//GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	//GetDlgItem(IDC_BUTTON2)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON3)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON5)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON6)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON7)->SetFont(&m_Font);


}


void CMyForm::OnMapTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*if (nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_MAPTOOL);

	m_MapTool.ShowWindow(SW_SHOW);*/
}


void CMyForm::OnTabTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_TabTool.GetSafeHwnd())
		m_TabTool.Create(IDD_TABTOOL);

	m_TabTool.ShowWindow(SW_SHOW);

	m_eLastClick = TAPTOOL;
}


void CMyForm::OnPathFind()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_PathFind.GetSafeHwnd())
		m_PathFind.Create(IDD_PATHFIND);

	m_PathFind.ShowWindow(SW_SHOW);
}


void CMyForm::OnTreeTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_TreeTool.GetSafeHwnd())
		m_TreeTool.Create(IDD_TREETOOL);

	m_TreeTool.ShowWindow(SW_SHOW);

	m_eLastClick = TREETOOL;
}


void CMyForm::OnObjTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.if (nullptr == m_PathFind.GetSafeHwnd())
	if (nullptr == m_ObjTool.GetSafeHwnd())
		m_ObjTool.Create(IDD_OBJTOOL);		// 해당 id에 맞는 다이얼로그 생성

	m_ObjTool.ShowWindow(SW_SHOW);	// 창 모양으로 출력
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_eLastClick = NPCTOOL;
}
