#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Include.h"
#include <atlimage.h>
#include "ObjView.h"
#include "MyTexture.h"
// CObjTool 대화 상자입니다.

class CObjTool : public CDialog
{
	DECLARE_DYNAMIC(CObjTool)

public:
	CObjTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	map<CString, IMGPATH*>&		Get_PathInfo() { return m_mapPathInfo; }
	const TCHAR* Get_ObjKey() { return m_pObjKey; }
	const TCHAR* Get_StateKey() { return m_pStateLKey; }
	const int&	 Get_Pos() { return iPos; }

	int& Get_Alpha() { return iAlpha; }
	int& Get_Red() { return iRed; }
	int& Get_Green() { return iGreen; }
	int& Get_Blue() { return iBlue; }

	float& Get_OffSetX() { return fOffSetX; }
	float& Get_OffSetY() { return fOffSetY; }

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTreeTask(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSliderFrame(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditFrame();
	afx_msg void OnSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSliderRed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSliderGreen(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSliderBlue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditAlpha();
	afx_msg void OnEditRed();
	afx_msg void OnEditGreen();
	afx_msg void OnEditBlue();
	afx_msg void OnEditOffsetx();
	afx_msg void OnEditOffsety();
	afx_msg void OnSliderOffsetx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSliderOffsety(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();

public:
	CTreeCtrl m_TreeTask;
	CSliderCtrl m_Slider_Frame;
	CEdit m_EditSliderFrame;
	CEdit m_EditObjKey;
	CEdit m_EditStateKey;
	CEdit m_EditImageCount;
	CEdit m_Edit_OffSetX;
	CEdit m_Edit_OffSetY;
	CSliderCtrl m_Slider_OffSetX;
	CSliderCtrl m_Slider_OffSetY;
	CEdit m_Edit_Alpha;
	CEdit m_Edit_Red;
	CEdit m_Edit_Green;
	CEdit m_Edit_Blue;
	CSliderCtrl m_Slider_Alpha;
	CSliderCtrl m_Slider_Red;
	CSliderCtrl m_Slider_Green;
	CSliderCtrl m_Slider_Blue;


	map<CString, IMGPATH*>		m_mapPathInfo;
	map<CString, CImage*>		m_MapPngImg;

	//general
	int iCount = 0;
	int iPos = 0;
	const TCHAR* m_pObjKey;
	const TCHAR* m_pStateLKey;
	CString strCurState;

	CObjView*	m_pObjView;
	CMyTexture* m_pMyTexture;

	int iAlpha = 255;
	int iRed = 255;
	int iGreen = 255;
	int iBlue = 255;
	int iMirror = 1;

	float fOffSetX = 400.f;
	float fOffSetY = 500.f;
	
};
