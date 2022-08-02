// TabTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "TabTool.h"
#include "afxdialogex.h"



// CTabTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabTool, CDialog)

CTabTool::CTabTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TABTOOL, pParent)
{

}

CTabTool::~CTabTool()
{
}

void CTabTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CTabTool, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTabTool::OnTcnSelchangeTab)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTabTool 메시지 처리기입니다.


BOOL CTabTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Tab.InsertItem(0, L"Unit");
	m_Tab.InsertItem(1, L"Tile");

	m_Tab.SetCurSel(0);

	CRect rect = {};
	m_Tab.GetWindowRect(&rect);


	m_pUnitTool = new CUnitTool;
	m_pUnitTool->Create(IDD_UNITTOOL, &m_Tab);
	m_pUnitTool->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pUnitTool->ShowWindow(SW_SHOW);

	m_pMapTool = new CMapTool;
	m_pMapTool->Create(IDD_MAPTOOL, &m_Tab);
	m_pMapTool->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pMapTool->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTabTool::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iSelect = m_Tab.GetCurSel();

	switch (iSelect)
	{
	case 0:
		m_pUnitTool->ShowWindow(SW_SHOW);
		m_pMapTool->ShowWindow(SW_HIDE);
		break;

	case 1:
		m_pUnitTool->ShowWindow(SW_HIDE);
		m_pMapTool->ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}


void CTabTool::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_pUnitTool = nullptr;
	delete m_pMapTool;
	m_pMapTool = nullptr;
}
