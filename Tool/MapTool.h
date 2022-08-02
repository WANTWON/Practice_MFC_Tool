#pragma once
#include "afxwin.h"
#include "Include.h"

// CMapTool 대화 상자입니다.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
	
	/*픽처컨트롤은 bmp를 기본 지원한다.
	cimage 객체가 jpeg, gif, bmp, png형식을 모두 지원*/

	map<CString, CImage*>		m_MapPngImg;
	int							m_iDrawID = 0;
	CButton m_TileOption[5];
};
