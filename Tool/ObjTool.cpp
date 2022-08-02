// ObjTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjTool.h"
#include "afxdialogex.h"


// CObjTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjTool, CDialog)

CObjTool::CObjTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OBJTOOL, pParent)
{

}

CObjTool::~CObjTool()
{
}

void CObjTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeTask);
	DDX_Control(pDX, IDC_SLIDER_FRAME, m_Slider_Frame);
	DDX_Control(pDX, IDC_EDIT_FRAME, m_EditSliderFrame);
	DDX_Control(pDX, IDC_EDIT_OBJ_KEY, m_EditObjKey);
	DDX_Control(pDX, IDC_EDIT_STATE_KEY, m_EditStateKey);
	DDX_Control(pDX, IDC_EDIT_IMAGE_COUNT, m_EditImageCount);
	DDX_Control(pDX, IDC_EDIT_ALPHA, m_Edit_Alpha);
	DDX_Control(pDX, IDC_EDIT_RED, m_Edit_Red);
	DDX_Control(pDX, IDC_EDIT_GREEN, m_Edit_Green);
	DDX_Control(pDX, IDC_EDIT_BLUE, m_Edit_Blue);
	DDX_Control(pDX, IDC_SLIDER_ALPHA, m_Slider_Alpha);
	DDX_Control(pDX, IDC_SLIDER_RED, m_Slider_Red);
	DDX_Control(pDX, IDC_SLIDER_GREEN, m_Slider_Green);
	DDX_Control(pDX, IDC_SLIDER_BLUE, m_Slider_Blue);
	DDX_Control(pDX, IDC_EDIT_OFFSETX, m_Edit_OffSetX);
	DDX_Control(pDX, IDC_EDIT_OFFSETY, m_Edit_OffSetY);
	DDX_Control(pDX, IDC_SLIDER_OFFSETX, m_Slider_OffSetX);
	DDX_Control(pDX, IDC_SLIDER_OFFSETY, m_Slider_OffSetY);
}


BEGIN_MESSAGE_MAP(CObjTool, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CObjTool::OnTreeTask)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FRAME, &CObjTool::OnSliderFrame)
	ON_EN_CHANGE(IDC_EDIT_FRAME, &CObjTool::OnEditFrame)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_ALPHA, &CObjTool::OnEditAlpha)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ALPHA, &CObjTool::OnSliderAlpha)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_RED, &CObjTool::OnSliderRed)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_GREEN, &CObjTool::OnSliderGreen)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BLUE, &CObjTool::OnSliderBlue)
	ON_EN_CHANGE(IDC_EDIT_RED, &CObjTool::OnEditRed)
	ON_EN_CHANGE(IDC_EDIT_GREEN, &CObjTool::OnEditGreen)
	ON_EN_CHANGE(IDC_EDIT_BLUE, &CObjTool::OnEditBlue)
	ON_EN_CHANGE(IDC_EDIT_OFFSETX, &CObjTool::OnEditOffsetx)
	ON_EN_CHANGE(IDC_EDIT_OFFSETY, &CObjTool::OnEditOffsety)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_OFFSETX, &CObjTool::OnSliderOffsetx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_OFFSETY, &CObjTool::OnSliderOffsety)
END_MESSAGE_MAP()


// CObjTool 메시지 처리기입니다.


BOOL CObjTool::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_pObjView = new CObjView;
	m_pObjView->Create(NULL, L"", WS_CHILD | WS_BORDER | WS_VISIBLE, CRect(200, 40, 400, 240), this, 50001);
	m_pObjView->OnInitialUpdate();

	m_pMyTexture = new CMyTexture;
	m_pMyTexture->Initialize();

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

			//m_PathInfoList.push_back(IMGTemp);
		}

	}
	UpdateData(FALSE);

	//WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);

	m_Slider_Frame.SetRange(0, 30);
	//m_Slider_Frame.SetPos(0);
	m_Slider_Alpha.SetRange(0, 255);
	//m_Slider_Alpha.SetPos(255);
	m_Slider_Red.SetRange(0, 255);
	//m_Slider_Red.SetPos(255);
	m_Slider_Green.SetRange(0, 255);
	//m_Slider_Green.SetPos(255);
	m_Slider_Blue.SetRange(0, 255);
	//m_Slider_Blue.SetPos(255);
	m_Slider_OffSetX.SetRange(-500, 500);
	m_Slider_OffSetY.SetRange(-500, 500);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CObjTool::OnTreeTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM h_Root, h_Obj, h_State, h_CurState, h_Child;
	CString strRoot, strObj, strState/*, strCurState*/;

	h_CurState = pNMTreeView->itemNew.hItem;
	h_State = this->m_TreeTask.GetParentItem(h_CurState);
	h_Obj = this->m_TreeTask.GetParentItem(h_State);

	strCurState = m_TreeTask.GetItemText(h_CurState);
	strState = m_TreeTask.GetItemText(h_State);
	strObj = m_TreeTask.GetItemText(h_Obj);

	if (h_CurState != nullptr
		&& h_State != nullptr
		&& h_Obj != nullptr)
	{
		auto& iter = m_MapPngImg.find(strCurState);
		auto& iterPath = m_mapPathInfo.find(strCurState);

		if (iter == m_MapPngImg.end())
			return;

		//m_Picture.SetBitmap(*(iter->second));

		SetDlgItemText(IDC_EDIT_OBJ_KEY, (iterPath->second->wstrObjKey).c_str());
		SetDlgItemText(IDC_EDIT_STATE_KEY, (iterPath->second->wstrStateKey).c_str());
		SetDlgItemInt(IDC_EDIT_IMAGE_COUNT, iterPath->second->iCount);

		h_Child = m_TreeTask.GetChildItem(h_State);
		iCount = 0;

		while (h_Child)
		{
			++iCount;
			h_Child = m_TreeTask.GetNextSiblingItem(h_Child);
		}

		// 두 자리수 숫자 처리
		/*wstring wStr = L"";
		wStr = strCurState.GetString();
		wStr = wStr.substr(wStr.size() - 1, wStr.size());*/

		//int iPos = 0;
		int i = 0;
		CString cStrNum;
		cStrNum = strCurState;

		for (; cStrNum.GetLength(); ++i)
		{
			// isdigit : 매개 변수로 넘겨받는 단일 문자가 글자 타입의 문자인지 숫자 타입의 문자인지 판별해주는 함수
			// 숫자타입의 문자라고 판명되면 0이 아닌 값을 반환
			if (0 != isdigit(cStrNum[i]))
				break;
		}
		// Delete(index, count)
		// 인덱스 위치로부터 카운트만큼 문자를 삭제하는 함수
		cStrNum.Delete(0, i);

		// 문자를 정수형으로 변환하는 함수
		iPos = _tstoi(cStrNum);

		m_Slider_Frame.SetRange(0, iCount - 1);
		m_Slider_Frame.SetPos(iPos);

		m_pObjKey = (iterPath->second->wstrObjKey).c_str();
		m_pStateLKey = (iterPath->second->wstrStateKey).c_str();

		SetDlgItemInt(IDC_EDIT_ALPHA, iAlpha);
		SetDlgItemInt(IDC_EDIT_RED, iRed);
		SetDlgItemInt(IDC_EDIT_GREEN, iGreen);
		SetDlgItemInt(IDC_EDIT_BLUE, iBlue);

		m_Slider_Frame.SetPos(0);
		m_Slider_Alpha.SetPos(255);
		m_Slider_Red.SetPos(255);
		m_Slider_Green.SetPos(255);
		m_Slider_Blue.SetPos(255);
		m_Slider_OffSetX.SetPos(0);
		m_Slider_OffSetY.SetPos(0);
	}

	*pResult = 0;
}


void CObjTool::OnSliderFrame(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iEditPos;
	iEditPos = m_Slider_Frame.GetPos();

	// Edit Control 의 텍스트 값을 Slider Control 값으로 설정
	SetDlgItemInt(IDC_EDIT_FRAME, iEditPos);

	iPos = iEditPos;

	int i = 0;
	CString cStr;
	cStr = strCurState;

	for (; cStr.GetLength(); ++i)
	{
		// 만약 반복문 끝까지 문자열에 숫자가 없다면 탈출
		if (i == cStr.GetLength())
			break;
		// isdigit : 매개 변수로 넘겨받는 단일 문자가 글자 타입의 문자인지 숫자 타입의 문자인지 판별해주는 함수
		// 숫자타입의 문자라고 판명되면 0이 아닌 값을 반환
		if (0 != isdigit(cStr[i]))
			break;
	}

	// i == cStr.GetLength() 이면 문자열에 숫자가 없다는 뜻. 코드건너뜀
	if (i != cStr.GetLength())
	{
		// Delete(index, count)
		// 인덱스 위치로부터 카운트만큼 문자를 삭제하는 함수
		// 숫자부분을 지우는 작업
		cStr.Delete(i, cStr.GetLength());

		CString cStrNum = L"";
		cStrNum.Format(_T("%d"), iEditPos);

		// 이름 = 이름 + 숫자
		cStr = cStr + cStrNum;

		// 내일할 것 220712
		// 버튼 클릭시 BOOL 변수 TRUE FALSE 오가며 프레임 재생
		// ARGB 적용시키기
		// 속도변수를 주어서 프레임 재생속도 조절
		// 좌우 반전
		// 파일정보 (텍스쳐타입, 장수, 이미지크기, 애니메이션속도)

		auto& iter = m_MapPngImg.find(cStr);

		if (iter == m_MapPngImg.end())
			return;

		//m_Picture.SetBitmap(*(iter->second));
	}

	*pResult = 0;
}


void CObjTool::OnEditFrame()
{
	CString sPos;

	// Edit Control 의 텍스트 값을 불러옴
	m_EditSliderFrame.GetWindowText(sPos);

	int iFrame = 0;

	if (iCount != 0)
	{
		if (_ttoi(sPos) > iCount - 1)
			iFrame = iCount - 1;
		else
			iFrame = _ttoi(sPos);
	}

	m_Slider_Frame.SetPos(iFrame);

	iPos = iFrame;

	// 커서를 맨 끝으로 이동
	m_EditSliderFrame.SetSel(-1);
}


void CObjTool::OnSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iEditPos;
	iEditPos = m_Slider_Alpha.GetPos();

	// Edit Control 의 텍스트 값을 Slider Control 값으로 설정
	SetDlgItemInt(IDC_EDIT_ALPHA, iEditPos);

	iAlpha = iEditPos;

	*pResult = 0;
}


void CObjTool::OnSliderRed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iEditPos;
	iEditPos = m_Slider_Red.GetPos();

	// Edit Control 의 텍스트 값을 Slider Control 값으로 설정
	SetDlgItemInt(IDC_EDIT_RED, iEditPos);

	iRed = iEditPos;

	*pResult = 0;
}


void CObjTool::OnSliderGreen(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iEditPos;
	iEditPos = m_Slider_Green.GetPos();

	// Edit Control 의 텍스트 값을 Slider Control 값으로 설정
	SetDlgItemInt(IDC_EDIT_GREEN, iEditPos);

	iGreen = iEditPos;

	*pResult = 0;
}


void CObjTool::OnSliderBlue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iEditPos;
	iEditPos = m_Slider_Blue.GetPos();

	// Edit Control 의 텍스트 값을 Slider Control 값으로 설정
	SetDlgItemInt(IDC_EDIT_BLUE, iEditPos);

	iBlue = iEditPos;

	*pResult = 0;
}


void CObjTool::OnEditAlpha()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString sPos;

	// Edit Control 의 텍스트 값을 불러옴
	m_Edit_Alpha.GetWindowText(sPos);

	iAlpha = _ttoi(sPos);

	//m_Slider_Frame.SetPos(iAlpha);

	// 커서를 맨 끝으로 이동
	m_Edit_Alpha.SetSel(-1);
}

void CObjTool::OnEditRed()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString sPos;

	// Edit Control 의 텍스트 값을 불러옴
	m_Edit_Red.GetWindowText(sPos);

	iRed = _ttoi(sPos);

	//m_Slider_Frame.SetPos(iAlpha);

	// 커서를 맨 끝으로 이동
	m_Edit_Red.SetSel(-1);
}


void CObjTool::OnEditGreen()
{// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString sPos;

	// Edit Control 의 텍스트 값을 불러옴
	m_Edit_Green.GetWindowText(sPos);

	iGreen = _ttoi(sPos);

	//m_Slider_Frame.SetPos(iAlpha);

	// 커서를 맨 끝으로 이동
	m_Edit_Green.SetSel(-1);
}


void CObjTool::OnEditBlue()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.


	CString sPos;

	// Edit Control 의 텍스트 값을 불러옴
	m_Edit_Blue.GetWindowText(sPos);

	iBlue = _ttoi(sPos);

	//m_Slider_Frame.SetPos(iAlpha);

	// 커서를 맨 끝으로 이동
	m_Edit_Blue.SetSel(-1);
}


void CObjTool::OnEditOffsetx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sPos;

	// Edit Control 의 텍스트 값을 불러옴
	m_Edit_OffSetX.GetWindowText(sPos);

	fOffSetX = (float)_ttoi(sPos);

	//m_Slider_Frame.SetPos(iAlpha);

	// 커서를 맨 끝으로 이동
	m_Edit_OffSetX.SetSel(-1);
}


void CObjTool::OnEditOffsety()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString sPos;

	// Edit Control 의 텍스트 값을 불러옴
	m_Edit_OffSetY.GetWindowText(sPos);

	fOffSetY = (float)_ttoi(sPos);

	//m_Slider_Frame.SetPos(iAlpha);

	// 커서를 맨 끝으로 이동
	m_Edit_OffSetY.SetSel(-1);
}


void CObjTool::OnSliderOffsetx(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	float fEditPos;
	fEditPos = m_Slider_OffSetX.GetPos();

	// Edit Control 의 텍스트 값을 Slider Control 값으로 설정
	SetDlgItemInt(IDC_EDIT_OFFSETX, fEditPos);

	fOffSetX = fEditPos;

	*pResult = 0;
}


void CObjTool::OnSliderOffsety(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	float fEditPos;
	fEditPos = m_Slider_OffSetY.GetPos();

	// Edit Control 의 텍스트 값을 Slider Control 값으로 설정
	SetDlgItemInt(IDC_EDIT_OFFSETY, fEditPos);

	fOffSetY = fEditPos;

	*pResult = 0;
}


void CObjTool::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	delete m_pObjView;

	for_each(m_mapPathInfo.begin(), m_mapPathInfo.end(), CDeleteMap());
	m_mapPathInfo.clear();
	for_each(m_MapPngImg.begin(), m_MapPngImg.end(), CDeleteMap());
	m_MapPngImg.clear();
}
