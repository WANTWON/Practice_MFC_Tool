// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MyTerrain.h"

// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{
	
}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_RADIO1, m_TileOption[0]);
	DDX_Control(pDX, IDC_RADIO2, m_TileOption[1]);
	DDX_Control(pDX, IDC_RADIO3, m_TileOption[2]);
	DDX_Control(pDX, IDC_RADIO4, m_TileOption[3]);
	DDX_Control(pDX, IDC_RADIO5, m_TileOption[4]);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnListBox)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnLoadTile)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTool::OnSaveTile)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.


void CMapTool::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString		strSelectName;
	for (int i = 0; i < 5; ++i)
	{
		m_TileOption[i].SetCheck(FALSE);
	}

	int	iSelect = m_ListBox.GetCurSel();

	if (-1 == iSelect)
		return;

	m_ListBox.GetText(iSelect, strSelectName);

	auto iter = m_MapPngImg.find(strSelectName);

	if (iter == m_MapPngImg.end())
		return;

	m_Picture.SetBitmap(*(iter->second));
	
	int i = 0;
	for (; strSelectName.GetLength(); ++i)
	{
		// isdigit : 매개 변수로 넘겨받는 단일 문자가 글자 타입의 문자인지 숫자 타입의 문자인지 판별해주는 함수
		// 숫자타입의 문자라고 판명되면 0이 아닌 값을 반환
		if (0 != isdigit(strSelectName[i]))
			break;
	}
	// Delete(index, count)
	// 인덱스 위치로부터 카운트만큼 문자를 삭제하는 함수
	strSelectName.Delete(0, i);
	m_TileOption[iSelect].SetCheck(TRUE);

	// 문자를 정수형으로 변환하는 함수
	m_iDrawID = _tstoi(strSelectName);

	UpdateData(FALSE);

}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);

	TCHAR szFilePath[MAX_PATH] = L"";

	int iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	TCHAR szFileName[MAX_STR] = L"";

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString strRelative = CFileInfo::ConvertRelativePath(szFilePath);
		CString	strFileName = PathFindFileName(strRelative);

		lstrcpy(szFileName, strFileName.GetString());

		PathRemoveExtension(szFileName);
	
		strFileName = szFileName;

		auto iter = m_MapPngImg.find(strFileName);

		if (iter == m_MapPngImg.end())
		{
			CImage*			pPngImage = new CImage;
			pPngImage->Load(strRelative);

			m_MapPngImg.insert({ strFileName, pPngImage });
			m_ListBox.AddString(strFileName);
		}
	}

	HorizontalScroll();

	UpdateData(FALSE);
}


void CMapTool::HorizontalScroll(void)
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

void CMapTool::OnSaveData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog		Dlg(FALSE,	// TRUE(열기), FALSE(다른 이름으로 저장) 모드 지정
		L"dat", // default 확장자명
		L"*.dat", // 대화 상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(읽기전용 체크박스 숨김) / OFN_OVERWRITEPROMPT(중복파일 저장 시 경고 메세지 띄움)
		L"Data File(*.dat) | *.dat||", // 대화 상자에 표시될 파일 형식 "콤보 박스에 출력될 문자열 | 실제 사용할 필터링 문자열 ||"
		this); // 부모 윈도우 주소


	TCHAR	szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);

	// 전체 경로에서 파일 이름만 잘라내는 함수
	// 경로 상에 파일명이 없다면 가장 마지막 경로를 잘라낸다
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	// DoModal : 대화 상자를 실행하는 함수

	if (IDOK == Dlg.DoModal())
	{
		CString	str = Dlg.GetPathName();

		const TCHAR*	pGetPath = str.GetString();

		HANDLE		hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		CMainFrame*		pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrame->m_MainSplitter.GetPane(0, 1));
		CMyTerrain*		pTerrain = pToolView->m_pTerrain;

		vector<TILE*>&  vecTile = pTerrain->Get_VecTile(); 

		DWORD	dwByte = 0;


		for (auto& iter : vecTile)
			WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);

		CloseHandle(hFile);
	}
}


void CMapTool::OnLoadTile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	CMyTerrain*			pMyTerrain = pToolView->m_pTerrain;

	pMyTerrain->Load_Tile();
}


void CMapTool::OnSaveTile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog Dlg(FALSE,
		L"dat",
		L"*dat",
		OFN_OVERWRITEPROMPT,
		L"Data File(*.dat) | *.dat||",
		this);

	TCHAR szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"파일 개방 실패.");
			return;
		}

		DWORD dwByte = 0;
		DWORD dwStringSize = 0;

		for (auto& rPair : m_MapPngImg)
		{
			dwStringSize = sizeof(wchar_t)*(rPair.first.GetLength() + 1);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, rPair.first.GetString(), dwStringSize, &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}


}


BOOL CMapTool::OnInitDialog() //c++ 파일 입출력으로 로드하는 법
{
	CDialog::OnInitDialog();

	UpdateData(TRUE);
	//Load Obj Data
	wifstream		fin;

	fin.open(L"../Data/ImgPath.txt", ios::in);

	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR	szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MAX_STR] = L"";
		TCHAR	szPath[MAX_PATH] = L"";

		//wstring	wstrCombined = L"";

		CString  strObj, strState, strStateNum;

		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			if (  !lstrcmp(szObjKey, L"Object") && !lstrcmp(szStateKey, L"Tile") )
			{
				if (strObj != szObjKey)
				{
					strObj.Format(szObjKey);
				}
				if (strState != szStateKey)
				{
					strState.Format(szStateKey);
				}

				int i = 0;

				//문자열을 숫자로 바꾸기
				int	iCount = _ttoi(szCount);
				//노드 추가.
				for (; i < iCount;)
				{
					TCHAR szCount[5];
					_itot_s(i, szCount, 10);
					CString strCombined = CString(szStateKey) + szCount;
					strStateNum.Format(strCombined);

					auto		iter = m_MapPngImg.find(strStateNum);

					if (iter == m_MapPngImg.end())
					{
						CImage*		pPngImage = new CImage;
						//새롭게 만들어서 붙여야한다.
						CString CombinePath;
						CombinePath.Format(szPath, i);

						pPngImage->Load(CombinePath);

						m_MapPngImg.insert({ strStateNum, pPngImage });
						m_ListBox.AddString(strStateNum);
					}
					++i;
				}
			}
		}
	}
	UpdateData(FALSE);
	return TRUE; 
}
