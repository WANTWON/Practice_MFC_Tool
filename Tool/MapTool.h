#pragma once
#include "afxwin.h"
#include "Include.h"

// CMapTool ��ȭ �����Դϴ�.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnListBox();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	void HorizontalScroll(void);
	afx_msg void OnSaveData();
	afx_msg void OnLoadTile();
	afx_msg void OnSaveTile();
	virtual BOOL OnInitDialog();

public:
	CListBox m_ListBox;
	CStatic m_Picture;
	
	/*��ó��Ʈ���� bmp�� �⺻ �����Ѵ�.
	cimage ��ü�� jpeg, gif, bmp, png������ ��� ����*/

	map<CString, CImage*>		m_MapPngImg;
	int							m_iDrawID = 0;
	CButton m_TileOption[5];
};
