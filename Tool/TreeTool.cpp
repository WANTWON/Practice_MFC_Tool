// TreeTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "TreeTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Construct.h"
#include "TextureMgr.h"
// CTreeTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTreeTool, CDialog)

CTreeTool::CTreeTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TREETOOL, pParent)
	, m_strSize(_T(""))
	, m_fScaleX(0)
	, m_fScaleY(0)
	, m_fScale(0)
	, m_strObjKey(_T(""))
	, m_strStateKey(_T(""))
	, m_iTileIndex(0)
{

}

CTreeTool::~CTreeTool()
{
}

void CTreeTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeTask);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_CHECK1, m_Horizontal);
	DDX_Control(pDX, IDC_CHECK2, m_Vertical);
	DDX_Text(pDX, IDC_EDIT1, m_strSize);
	DDX_Control(pDX, IDC_SCALEX_SLIDER, m_ScaleX_Slider);
	DDX_Control(pDX, IDC_SCALEY_SLIDER, m_ScaleY_Slider);
	DDX_Text(pDX, IDC_SCALEX_EDIT, m_fScaleX);
	DDX_Text(pDX, IDC_SCALEY_EDIT, m_fScaleY);
	DDX_Text(pDX, IDC_SCALE_EDIT, m_fScale);
	DDX_Control(pDX, IDC_SCALE_SLIDER, m_Scale_Slider);
	DDX_Text(pDX, IDC_EDIT4, m_strObjKey);
	DDX_Text(pDX, IDC_EDIT5, m_strStateKey);
	DDX_Text(pDX, IDC_EDIT3, m_iTileIndex);
}


BEGIN_MESSAGE_MAP(CTreeTool, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CTreeTool::OnTreeTask)
	ON_BN_CLICKED(IDC_BUTTON1, &CTreeTool::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON2, &CTreeTool::OnLoadData)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SCALEX_SLIDER, &CTreeTool::OnScaleXSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SCALEY_SLIDER, &CTreeTool::OnScaleYSlider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SCALE_SLIDER, &CTreeTool::OnScaleAll_Slider)
	ON_BN_CLICKED(IDC_BUTTON3, &CTreeTool::OnScaleReset_Button)
END_MESSAGE_MAP()


// CTreeTool 메시지 처리기입니다.


void CTreeTool::OnTreeTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	int i = 0;
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM h_Root, h_Obj, h_State, h_CurState;
	CString strRoot, strObj, strState, strCurState, strSelectNum;

	h_CurState = pNMTreeView->itemNew.hItem;
	h_State = this->m_TreeTask.GetParentItem(h_CurState);
	h_Obj = this->m_TreeTask.GetParentItem(h_State);

	strCurState = m_TreeTask.GetItemText(h_CurState);
	strState = m_TreeTask.GetItemText(h_State);
	strObj = m_TreeTask.GetItemText(h_Obj);
	strSelectNum = strCurState;

	if (h_CurState != nullptr
		&& h_State != nullptr
		&& h_Obj != nullptr)
	{
		m_strObjKey = strObj;
		m_strStateKey = strState;

		int i = 0;
		//숫자만 남기기
		for (; strSelectNum.GetLength(); ++i)
		{
			if (0 != isdigit(strSelectNum[i]))
				break;
		}
		strSelectNum.Delete(0, i);
		m_iDrawID = _tstoi(strSelectNum);

		m_strOption = strState;

		auto& iter = m_MapPngImg.find(strCurState);
		if (iter == m_MapPngImg.end())
			return;
		m_Picture.SetBitmap(*(iter->second));

		/////분리예정
		//사이즈 띄우기
		//주의사항 : 현재 폴더 구조가 스테이트 ->스테이트->아이디임.
		if (CTextureMgr::Get_Instance()->Get_Texture(strState, strState, m_iDrawID))
		{
			int Width = CTextureMgr::Get_Instance()->Get_Texture(strState, strState, m_iDrawID)->tImgInfo.Width;
			int Height = CTextureMgr::Get_Instance()->Get_Texture(strState, strState, m_iDrawID)->tImgInfo.Height;
		
			TCHAR szCountW[10] = { 0 };
			_itot_s(Width, szCountW, 10);

			TCHAR szCountH[10] = { 0 };
			_itot_s(Height, szCountH, 10);

			m_strSize = CString(szCountW) + L"*" + szCountH;
		}

	}
	UpdateData(FALSE);
	*pResult = 0;
}


BOOL CTreeTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(TRUE);
	//Load Obj Data
	wifstream		fin;

	fin.open(L"../Data/ImgPath.txt", ios::in);

	CString	strRoot;
	HTREEITEM h_Root;
	strRoot.Format(L"OBJECT");
	h_Root = m_TreeTask.InsertItem(strRoot, NULL, NULL);
	if (!fin.fail())
	{
		TCHAR	szObjKey[MAX_STR] = L"";
		TCHAR	szStateKey[MAX_STR] = L"";
		TCHAR	szCount[MAX_STR] = L"";
		TCHAR	szPath[MAX_PATH] = L"";

		//wstring	wstrCombined = L"";

		HTREEITEM  h_Obj, h_State, h_StateNum;

		CString  strObj, strState, strStateNum;

		while (true)
		{
				fin.getline(szObjKey, MAX_STR, '|');
				fin.getline(szStateKey, MAX_STR, '|');
				fin.getline(szCount, MAX_STR, '|');
				fin.getline(szPath, MAX_PATH);

				if (fin.eof())
					break;
				if (strObj != szObjKey)
				{
					strObj.Format(szObjKey);
					h_Obj = m_TreeTask.InsertItem(strObj, NULL, NULL);
				}
				if (strState != szStateKey)
				{
					strState.Format(szStateKey);
					h_State = m_TreeTask.InsertItem(strState, h_Obj, NULL);
				}

				int i = 0;

				//문자열을 숫자로 바꾸기
				int	iCount = _ttoi(szCount);
				//노드 추가.
				for (; i < iCount;)
				{
					//TCHAR szCombine = szStateKey + szCount;
					TCHAR szCount[10] = { 0 };
					_itot_s(i, szCount, 10);
					CString strCombined = CString(szStateKey) + szCount;
					strStateNum.Format(strCombined);
					h_StateNum = m_TreeTask.InsertItem(strStateNum, h_State, NULL);

					IMGPATH* IMGTemp = new IMGPATH;
					IMGTemp->wstrObjKey = wstring(szObjKey);
					IMGTemp->wstrStateKey = wstring(szStateKey);
					IMGTemp->iCount = iCount;
					IMGTemp->wstrPath = wstring(szPath);
					m_mapPathInfo.emplace(strStateNum, IMGTemp);

					auto		iter = m_MapPngImg.find(strStateNum);

					if (iter == m_MapPngImg.end())
					{
						CImage*		pPngImage = new CImage;
						//새롭게 만들어서 붙여야한다.
						CString CombinePath;
						CombinePath.Format(szPath, i);

						pPngImage->Load(CombinePath);

						m_MapPngImg.insert({ strStateNum, pPngImage });
					}
					++i;
				}
			}
	}
	UpdateData(FALSE);

	//Slider 초기화
	m_ScaleX_Slider.SetRange(1, 20);
	m_ScaleX_Slider.SetPos(10);
	m_ScaleY_Slider.SetRange(1, 20);
	m_ScaleY_Slider.SetPos(10);
	m_Scale_Slider.SetRange(1, 20);
	m_Scale_Slider.SetPos(10);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}




void CTreeTool::OnSaveData()
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
		CConstruct*		pConstruct = pToolView->m_pConstruct;

		vector<CONSTRUCT*>  vecConstruct = pConstruct->Get_Construct();

		DWORD	dwByte = 0;


		for (auto& iter : vecConstruct)
			WriteFile(hFile, iter, sizeof(CONSTRUCT), &dwByte, nullptr);

		CloseHandle(hFile);
	}
}


void CTreeTool::OnLoadData()
{
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	CConstruct*			pMyStruct = pToolView->m_pConstruct;

	pMyStruct->Load_Data();
}


void CTreeTool::OnScaleXSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(TRUE);
	int position;
	position = m_ScaleX_Slider.GetPos();
	//SetDlgItemInt(IDC_SCALEY_EDIT, position * 0.01f);
	m_fScaleX = position * 0.1f;

	UpdateData(FALSE);
	*pResult = 0;
}


void CTreeTool::OnScaleYSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int position;
	position = m_ScaleY_Slider.GetPos();
	//SetDlgItemInt(IDC_SCALEY_EDIT, position * 0.01f);
	m_fScaleY = position * 0.1f;

	UpdateData(FALSE);
	*pResult = 0;
}


void CTreeTool::OnScaleAll_Slider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if (GetAsyncKeyState(VK_LBUTTON))
	{// 현재 문제점 슬라이드에 갖다대기만해도 같이움직이려한다.
		UpdateData(TRUE);

		int position;
		
		position = m_Scale_Slider.GetPos();
		m_ScaleX_Slider.SetPos(m_Scale_Slider.GetPos());
		m_ScaleY_Slider.SetPos(m_Scale_Slider.GetPos());
		m_fScale = position * 0.1f;
		m_fScaleX = m_fScale;
		m_fScaleY = m_fScale;

		
		UpdateData(FALSE);
	}
	

	*pResult = 0;
}


void CTreeTool::OnScaleReset_Button()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Scale_Slider.SetPos(10);
	m_ScaleX_Slider.SetPos(10);
	m_ScaleY_Slider.SetPos(10);

	m_fScale = 1.f;
	m_fScaleX = 1.f;
	m_fScaleY = 1.f;
}

