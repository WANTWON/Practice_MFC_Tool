#pragma once

#include "UnitTool.h"
#include "MapTool.h"
#include "TabTool.h"
#include "PathFind.h"
#include "TreeTool.h"
#include "ObjTool.h"

// CMyForm 폼 뷰입니다.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUnitTool();
	virtual void OnInitialUpdate();
	afx_msg void OnMapTool();
	afx_msg void OnTabTool();
	afx_msg void OnPathFind();
	const TOOLID& Get_LastClick(void) { return m_eLastClick; }
public:
	CFont		m_Font;
	CUnitTool  m_UnitTool;
	CMapTool   m_MapTool;
	CTabTool   m_TabTool;
	CPathFind  m_PathFind;
	CTreeTool  m_TreeTool;
	CObjTool   m_ObjTool;

	//value


	//General
	TOOLID		m_eLastClick;
	afx_msg void OnTreeTool();
	
	afx_msg void OnObjTool();
};


