// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MyTerrain.h"

// CMapTool ��ȭ �����Դϴ�.

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


// CMapTool �޽��� ó�����Դϴ�.


void CMapTool::OnListBox()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
		// isdigit : �Ű� ������ �Ѱܹ޴� ���� ���ڰ� ���� Ÿ���� �������� ���� Ÿ���� �������� �Ǻ����ִ� �Լ�
		// ����Ÿ���� ���ڶ�� �Ǹ�Ǹ� 0�� �ƴ� ���� ��ȯ
		if (0 != isdigit(strSelectName[i]))
			break;
	}
	// Delete(index, count)
	// �ε��� ��ġ�κ��� ī��Ʈ��ŭ ���ڸ� �����ϴ� �Լ�
	strSelectName.Delete(0, i);
	m_TileOption[iSelect].SetCheck(TRUE);

	// ���ڸ� ���������� ��ȯ�ϴ� �Լ�
	m_iDrawID = _tstoi(strSelectName);

	UpdateData(FALSE);

}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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

	// GetCount : ����Ʈ �ڽ� ����� ������ ����
	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		m_ListBox.GetText(i, strName);

		// GetTextExtent : ���� ���ڿ��� �ȼ� ������ ��ȯ
		size = pDC->GetTextExtent(strName);

		if (size.cx > iCX)
			iCX = size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	// GetHorizontalExtent : ����Ʈ�ڽ��� ���η� ��ũ�� �� �� �ִ� �ִ� ������ ������ �Լ�
	if (iCX > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iCX);
}

void CMapTool::OnSaveData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CFileDialog		Dlg(FALSE,	// TRUE(����), FALSE(�ٸ� �̸����� ����) ��� ����
		L"dat", // default Ȯ���ڸ�
		L"*.dat", // ��ȭ ���ڿ� ǥ�õ� ���� ���ϸ�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(�б����� üũ�ڽ� ����) / OFN_OVERWRITEPROMPT(�ߺ����� ���� �� ��� �޼��� ���)
		L"Data File(*.dat) | *.dat||", // ��ȭ ���ڿ� ǥ�õ� ���� ���� "�޺� �ڽ��� ��µ� ���ڿ� | ���� ����� ���͸� ���ڿ� ||"
		this); // �θ� ������ �ּ�


	TCHAR	szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath);

	// ��ü ��ο��� ���� �̸��� �߶󳻴� �Լ�
	// ��� �� ���ϸ��� ���ٸ� ���� ������ ��θ� �߶󳽴�
	PathRemoveFileSpec(szPath);

	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	// DoModal : ��ȭ ���ڸ� �����ϴ� �Լ�

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	CMyTerrain*			pMyTerrain = pToolView->m_pTerrain;

	pMyTerrain->Load_Tile();
}


void CMapTool::OnSaveTile()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
			ERR_MSG(L"���� ���� ����.");
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


BOOL CMapTool::OnInitDialog() //c++ ���� ��������� �ε��ϴ� ��
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

				//���ڿ��� ���ڷ� �ٲٱ�
				int	iCount = _ttoi(szCount);
				//��� �߰�.
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
						//���Ӱ� ���� �ٿ����Ѵ�.
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
