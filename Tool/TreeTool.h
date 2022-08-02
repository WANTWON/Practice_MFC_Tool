#pragma once
#include "Include.h"
#include "afxcmn.h"
#include "Include.h"
#include "afxwin.h"
#include <atlimage.h>

// CTreeTool 대화 상자입니다.

class CTreeTool : public CDialog
{
	DECLARE_DYNAMIC(CTreeTool)

public:
	CTreeTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTreeTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TREETOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTreeTask(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
public:
	//control
	CTreeCtrl m_TreeTask;
	CStatic m_Picture;

	CSliderCtrl m_ScaleX_Slider;
	CSliderCtrl m_ScaleY_Slider;
	//value
	CString m_strSize;
	float m_fScaleX = 0.f;
	float m_fScaleY = 0.f;
	float m_fScale = 0.f;

	int m_iTileIndex = 0;
	//general
	map<CString, IMGPATH*>		m_mapPathInfo;
	map<CString, CImage*>		m_MapPngImg;

	int m_iDrawID = 0;
	CString m_strOption;

	CButton m_Horizontal;
	CButton m_Vertical;
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
	
	afx_msg void OnScaleXSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnScaleYSlider(NMHDR *pNMHDR, LRESULT *pResult);

	
	
	afx_msg void OnScaleAll_Slider(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_Scale_Slider;
	afx_msg void OnScaleReset_Button();
	CString m_strObjKey;
	CString m_strStateKey;
	
};
