#pragma once
#include "Include.h"
#include "afxwin.h"

// CPathFind ��ȭ �����Դϴ�.

class CPathFind : public CDialog
{
	DECLARE_DYNAMIC(CPathFind)

public:
	CPathFind(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathFind();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHFIND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void OnListBox();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	void HorizontalScroll(void);

public:
	CListBox m_ListBox;
	
	list<IMGPATH*>	m_PathInfoList;
};