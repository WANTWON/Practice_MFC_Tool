// PathFind.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFind.h"
#include "afxdialogex.h"
#include "FileInfo.h"


// CPathFind 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathFind, CDialog)

CPathFind::CPathFind(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHFIND, pParent)
{

}

CPathFind::~CPathFind()
{
}

void CPathFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathFind, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathFind::OnSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CPathFind::OnLoad)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPathFind::OnListBox)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPathFind 메시지 처리기입니다.


void CPathFind::OnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	wofstream    fout;

	fout.open(L"../Data/ImgPath.txt", ios::out);

	if (!fout.fail())	//파일 개방에 성공하면
	{
		for (auto& iter : m_PathInfoList)
		{
			fout << iter->wstrObjKey << L"|" << iter->wstrStateKey << L"|" << iter->iCount << L"|" << iter->wstrPath << endl;
		}

		fout.close();
	}
	HorizontalScroll();
	UpdateData(FALSE);
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
	//메모장 활성화 함수
}


void CPathFind::OnLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	wifstream	fin;

	fin.open(L"../Data/ImgPath.txt", ios::in);

	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR	szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MAX_STR] = L"";
		TCHAR	szPath[MAX_PATH] = L"";

		wstring	wstrCombined = L"";

		m_ListBox.ResetContent();

		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			wstrCombined = wstring(szObjKey) + L"|" + szStateKey + L"|" + szCount + L"|" + szPath;

			m_ListBox.AddString(wstrCombined.c_str());
		}
	}
	HorizontalScroll();
	UpdateData(FALSE);
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}


void CPathFind::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CPathFind::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnDropFiles(hDropInfo);

	UpdateData(TRUE);

	TCHAR	szFilePath[MAX_PATH] = L"";

	int	iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	TCHAR	szFileName[MAX_STR] = L"";

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);
		CFileInfo::DirInfoExtraction(szFilePath, m_PathInfoList);
	}

	m_ListBox.ResetContent();

	wstring	wstrCombined = L"";
	TCHAR	szBuf[MAX_PATH] = L"";

	for (auto& iter : m_PathInfoList)
	{
		//_itow_s : 정수를 문자열로 변환하는 함수
		// 10진수 형태의 문자로 변환을 의미
		_itow_s(iter->iCount, szBuf, 10);

		wstrCombined = iter->wstrObjKey + L"|" + iter->wstrStateKey + L"|" + szBuf + L"|" + iter->wstrPath;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	//Player|Dash|10|경로

	HorizontalScroll();
	UpdateData(FALSE);
}

void CPathFind::HorizontalScroll(void)
{
	CString		strName;
	CSize		size;

	int iCX = 0;

	CDC*	pDC = m_ListBox.GetDC();

	// GetCount : 리스트 박스 목록의 개수를 얻어옴
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		// GetTextExtent : 현재 문자열을 픽셀 단위로 변환
		size = pDC->GetTextExtent(strName);

		if (size.cx > iCX)
			iCX = size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	// GetHorizontalExtent : 리스트박스가 가로로 스크롤 할 수 있는 최대 범위를 얻어오는 함수
	if (iCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iCX);
}
