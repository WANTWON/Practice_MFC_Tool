#pragma once
#include "afxwin.h"
#include "Include.h"

// CUnitTool ��ȭ �����Դϴ�.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnitTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAddPlayer();
	afx_msg void OnListBox();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void OnSearch();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
public:
	CString m_strName;
	CString m_strSearchName;

	int m_iHp;
	int m_iAttack;

	CListBox m_ListBox;
	CButton m_Radio[3];
	CButton m_Check[3];

	map<CString, UNITDATA*> m_mapUnitData;
	
};
