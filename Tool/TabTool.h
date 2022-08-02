#pragma once
#include "afxcmn.h"
#include "UnitTool.h"
#include "MapTool.h"
#include "Include.h"

// CTabTool 대화 상자입니다.

class CTabTool : public CDialog
{
	DECLARE_DYNAMIC(CTabTool)

public:
	CTabTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTabCtrl m_Tab;
	CUnitTool* m_pUnitTool;
	CMapTool*	m_pMapTool;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
};
