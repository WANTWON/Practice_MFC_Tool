#pragma once
#include "afxcmn.h"
#include "UnitTool.h"
#include "MapTool.h"
#include "Include.h"

// CTabTool ��ȭ �����Դϴ�.

class CTabTool : public CDialog
{
	DECLARE_DYNAMIC(CTabTool)

public:
	CTabTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTabTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTabCtrl m_Tab;
	CUnitTool* m_pUnitTool;
	CMapTool*	m_pMapTool;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
};
