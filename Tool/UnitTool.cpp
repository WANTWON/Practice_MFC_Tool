// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strName(_T(""))
	, m_iHp(0)
	, m_iAttack(0)
	, m_strSearchName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Text(pDX, IDC_EDIT2, m_iHp);
	DDX_Text(pDX, IDC_EDIT3, m_iAttack);
	DDX_Control(pDX, IDC_LIST2, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);
	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);
	DDX_Text(pDX, IDC_EDIT4, m_strSearchName);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnAddPlayer)
	ON_LBN_SELCHANGE(IDC_LIST2, &CUnitTool::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnSave)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnLoad)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnSearch)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CUnitTool 메시지 처리기입니다.


void CUnitTool::OnAddPlayer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	UNITDATA* pUnit = new UNITDATA;
	pUnit->strName = m_strName;
	pUnit->iHp = m_iHp;
	pUnit->iAttack = m_iAttack;

	m_ListBox.AddString(pUnit->strName);

	for (int i = 0; i < 3; ++i)
	{
		if (m_Radio[i].GetCheck())
		{
			pUnit->byJobIndex = i;
			break;
		}
	}

	pUnit->byItem = 0x00;

	if (m_Check[0].GetCheck())
		pUnit->byItem |= RUBY;

	if (m_Check[1].GetCheck())
		pUnit->byItem |= DIAMOND;

	if (m_Check[1].GetCheck())
		pUnit->byItem |= SAPPHIRE;


	m_mapUnitData.insert({ pUnit->strName, pUnit });

	UpdateData(FALSE);

}


void CUnitTool::OnListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString strFindName;

	int iSelect = m_ListBox.GetCurSel();

	if (-1 == iSelect)
		return;

	m_ListBox.GetText(iSelect, strFindName);

	auto	iter = m_mapUnitData.find(strFindName);

	if (iter == m_mapUnitData.end())
		return;

	m_strName = iter->second->strName;
	m_iHp = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
		m_Check[i].SetCheck(FALSE);
	}

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);

	UpdateData(FALSE);
}


void CUnitTool::OnSave()
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
		for (auto& rPair : m_mapUnitData)
		{
			dwStringSize = sizeof(wchar_t)*(rPair.second->strName.GetLength() + 1);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, rPair.second->strName.GetString(), dwStringSize, &dwByte, nullptr);
			WriteFile(hFile, &rPair.second->iAttack, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &rPair.second->iHp, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &rPair.second->byItem, sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &rPair.second->byJobIndex, sizeof(BYTE), &dwByte, nullptr);


		}

		CloseHandle(hFile);
	}
	
}


void CUnitTool::OnLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (auto& rPair : m_mapUnitData)
		Safe_Delete(rPair.second);

	m_mapUnitData.clear();
	m_ListBox.ResetContent();


	CFileDialog Dlg(TRUE,
		L"dat",
		L"*.dat",
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
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			ERR_MSG(L"파일 개방 실패.");
			return;
		}
		DWORD dwByte = 0;
		DWORD dwStringSize = 0;
		UNITDATA* pUnitInfo = nullptr;
		while (true)
		{
			pUnitInfo = new UNITDATA;
			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwByte, nullptr);
			TCHAR* pTemp = new TCHAR[dwStringSize];
			ReadFile(hFile, pTemp, dwStringSize, &dwByte, nullptr);
			ReadFile(hFile, &pUnitInfo->iAttack, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &pUnitInfo->iHp, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &pUnitInfo->byItem, sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &pUnitInfo->byJobIndex, sizeof(BYTE), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Delete(pUnitInfo);
				if (pTemp)
				{
					delete[] pTemp;
					pTemp = nullptr;
				}
				break;
			}
			pUnitInfo->strName = pTemp;
			m_mapUnitData.emplace(pUnitInfo->strName, pUnitInfo);
			m_ListBox.AddString(pUnitInfo->strName);
		}
		CloseHandle(hFile);
	}
	UpdateData(FALSE);
}


void CUnitTool::OnSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	auto	iter = m_mapUnitData.find(m_strSearchName);

	if (iter == m_mapUnitData.end())
		return;

	m_strName = iter->second->strName;
	m_iHp = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);
		m_Check[i].SetCheck(FALSE);
	}

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);

	UpdateData(FALSE);
}


void CUnitTool::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	for_each(m_mapUnitData.begin(), m_mapUnitData.end(), CDeleteMap());
	m_mapUnitData.clear();
}


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_Radio[0].SetCheck(TRUE);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
