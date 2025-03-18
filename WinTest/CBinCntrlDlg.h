#pragma once
#include "afxdialogex.h"


// CBinCntrlDlg 대화 상자

class CBinCntrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CBinCntrlDlg)

public:
	CBinCntrlDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CBinCntrlDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	int m_binValDisp;
	CSliderCtrl m_CtrlSlider;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
};
