// ObjTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjTool.h"
#include "afxdialogex.h"


// CObjTool ��ȭ �����Դϴ�.

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


// CObjTool �޽��� ó�����Դϴ�.


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

			//���ڿ��� ���ڷ� �ٲٱ�
			int	iCount = _ttoi(szCount);
			//��� �߰�.
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
					//���Ӱ� ���� �ٿ����Ѵ�.
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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CObjTool::OnTreeTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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

		// �� �ڸ��� ���� ó��
		/*wstring wStr = L"";
		wStr = strCurState.GetString();
		wStr = wStr.substr(wStr.size() - 1, wStr.size());*/

		//int iPos = 0;
		int i = 0;
		CString cStrNum;
		cStrNum = strCurState;

		for (; cStrNum.GetLength(); ++i)
		{
			// isdigit : �Ű� ������ �Ѱܹ޴� ���� ���ڰ� ���� Ÿ���� �������� ���� Ÿ���� �������� �Ǻ����ִ� �Լ�
			// ����Ÿ���� ���ڶ�� �Ǹ�Ǹ� 0�� �ƴ� ���� ��ȯ
			if (0 != isdigit(cStrNum[i]))
				break;
		}
		// Delete(index, count)
		// �ε��� ��ġ�κ��� ī��Ʈ��ŭ ���ڸ� �����ϴ� �Լ�
		cStrNum.Delete(0, i);

		// ���ڸ� ���������� ��ȯ�ϴ� �Լ�
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int iEditPos;
	iEditPos = m_Slider_Frame.GetPos();

	// Edit Control �� �ؽ�Ʈ ���� Slider Control ������ ����
	SetDlgItemInt(IDC_EDIT_FRAME, iEditPos);

	iPos = iEditPos;

	int i = 0;
	CString cStr;
	cStr = strCurState;

	for (; cStr.GetLength(); ++i)
	{
		// ���� �ݺ��� ������ ���ڿ��� ���ڰ� ���ٸ� Ż��
		if (i == cStr.GetLength())
			break;
		// isdigit : �Ű� ������ �Ѱܹ޴� ���� ���ڰ� ���� Ÿ���� �������� ���� Ÿ���� �������� �Ǻ����ִ� �Լ�
		// ����Ÿ���� ���ڶ�� �Ǹ�Ǹ� 0�� �ƴ� ���� ��ȯ
		if (0 != isdigit(cStr[i]))
			break;
	}

	// i == cStr.GetLength() �̸� ���ڿ��� ���ڰ� ���ٴ� ��. �ڵ�ǳʶ�
	if (i != cStr.GetLength())
	{
		// Delete(index, count)
		// �ε��� ��ġ�κ��� ī��Ʈ��ŭ ���ڸ� �����ϴ� �Լ�
		// ���ںκ��� ����� �۾�
		cStr.Delete(i, cStr.GetLength());

		CString cStrNum = L"";
		cStrNum.Format(_T("%d"), iEditPos);

		// �̸� = �̸� + ����
		cStr = cStr + cStrNum;

		// ������ �� 220712
		// ��ư Ŭ���� BOOL ���� TRUE FALSE ������ ������ ���
		// ARGB �����Ű��
		// �ӵ������� �־ ������ ����ӵ� ����
		// �¿� ����
		// �������� (�ؽ���Ÿ��, ���, �̹���ũ��, �ִϸ��̼Ǽӵ�)

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

	// Edit Control �� �ؽ�Ʈ ���� �ҷ���
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

	// Ŀ���� �� ������ �̵�
	m_EditSliderFrame.SetSel(-1);
}


void CObjTool::OnSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int iEditPos;
	iEditPos = m_Slider_Alpha.GetPos();

	// Edit Control �� �ؽ�Ʈ ���� Slider Control ������ ����
	SetDlgItemInt(IDC_EDIT_ALPHA, iEditPos);

	iAlpha = iEditPos;

	*pResult = 0;
}


void CObjTool::OnSliderRed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int iEditPos;
	iEditPos = m_Slider_Red.GetPos();

	// Edit Control �� �ؽ�Ʈ ���� Slider Control ������ ����
	SetDlgItemInt(IDC_EDIT_RED, iEditPos);

	iRed = iEditPos;

	*pResult = 0;
}


void CObjTool::OnSliderGreen(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int iEditPos;
	iEditPos = m_Slider_Green.GetPos();

	// Edit Control �� �ؽ�Ʈ ���� Slider Control ������ ����
	SetDlgItemInt(IDC_EDIT_GREEN, iEditPos);

	iGreen = iEditPos;

	*pResult = 0;
}


void CObjTool::OnSliderBlue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int iEditPos;
	iEditPos = m_Slider_Blue.GetPos();

	// Edit Control �� �ؽ�Ʈ ���� Slider Control ������ ����
	SetDlgItemInt(IDC_EDIT_BLUE, iEditPos);

	iBlue = iEditPos;

	*pResult = 0;
}


void CObjTool::OnEditAlpha()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString sPos;

	// Edit Control �� �ؽ�Ʈ ���� �ҷ���
	m_Edit_Alpha.GetWindowText(sPos);

	iAlpha = _ttoi(sPos);

	//m_Slider_Frame.SetPos(iAlpha);

	// Ŀ���� �� ������ �̵�
	m_Edit_Alpha.SetSel(-1);
}

void CObjTool::OnEditRed()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString sPos;

	// Edit Control �� �ؽ�Ʈ ���� �ҷ���
	m_Edit_Red.GetWindowText(sPos);

	iRed = _ttoi(sPos);

	//m_Slider_Frame.SetPos(iAlpha);

	// Ŀ���� �� ������ �̵�
	m_Edit_Red.SetSel(-1);
}


void CObjTool::OnEditGreen()
{// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString sPos;

	// Edit Control �� �ؽ�Ʈ ���� �ҷ���
	m_Edit_Green.GetWindowText(sPos);

	iGreen = _ttoi(sPos);

	//m_Slider_Frame.SetPos(iAlpha);

	// Ŀ���� �� ������ �̵�
	m_Edit_Green.SetSel(-1);
}


void CObjTool::OnEditBlue()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


	CString sPos;

	// Edit Control �� �ؽ�Ʈ ���� �ҷ���
	m_Edit_Blue.GetWindowText(sPos);

	iBlue = _ttoi(sPos);

	//m_Slider_Frame.SetPos(iAlpha);

	// Ŀ���� �� ������ �̵�
	m_Edit_Blue.SetSel(-1);
}


void CObjTool::OnEditOffsetx()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sPos;

	// Edit Control �� �ؽ�Ʈ ���� �ҷ���
	m_Edit_OffSetX.GetWindowText(sPos);

	fOffSetX = (float)_ttoi(sPos);

	//m_Slider_Frame.SetPos(iAlpha);

	// Ŀ���� �� ������ �̵�
	m_Edit_OffSetX.SetSel(-1);
}


void CObjTool::OnEditOffsety()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialog::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString sPos;

	// Edit Control �� �ؽ�Ʈ ���� �ҷ���
	m_Edit_OffSetY.GetWindowText(sPos);

	fOffSetY = (float)_ttoi(sPos);

	//m_Slider_Frame.SetPos(iAlpha);

	// Ŀ���� �� ������ �̵�
	m_Edit_OffSetY.SetSel(-1);
}


void CObjTool::OnSliderOffsetx(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	float fEditPos;
	fEditPos = m_Slider_OffSetX.GetPos();

	// Edit Control �� �ؽ�Ʈ ���� Slider Control ������ ����
	SetDlgItemInt(IDC_EDIT_OFFSETX, fEditPos);

	fOffSetX = fEditPos;

	*pResult = 0;
}


void CObjTool::OnSliderOffsety(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	float fEditPos;
	fEditPos = m_Slider_OffSetY.GetPos();

	// Edit Control �� �ؽ�Ʈ ���� Slider Control ������ ����
	SetDlgItemInt(IDC_EDIT_OFFSETY, fEditPos);

	fOffSetY = fEditPos;

	*pResult = 0;
}


void CObjTool::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	delete m_pObjView;

	for_each(m_mapPathInfo.begin(), m_mapPathInfo.end(), CDeleteMap());
	m_mapPathInfo.clear();
	for_each(m_MapPngImg.begin(), m_MapPngImg.end(), CDeleteMap());
	m_MapPngImg.clear();
}
