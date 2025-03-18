// CBinCntrlDlg.cpp: 구현 파일
//

#include "pch.h"
#include "WinTest.h"
#include "afxdialogex.h"
#include "CBinCntrlDlg.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "WinTestDoc.h"


// CBinCntrlDlg 대화 상자

IMPLEMENT_DYNAMIC(CBinCntrlDlg, CDialog)

CBinCntrlDlg::CBinCntrlDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_binValDisp(0)
{

}

CBinCntrlDlg::~CBinCntrlDlg()
{
}

void CBinCntrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_binValDisp);
	DDV_MinMaxInt(pDX, m_binValDisp, 0, 255);
	DDX_Control(pDX, IDC_SLIDER1, m_CtrlSlider);
}


BEGIN_MESSAGE_MAP(CBinCntrlDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CBinCntrlDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CBinCntrlDlg::OnNMCustomdrawSlider1)
END_MESSAGE_MAP()


// CBinCntrlDlg 메시지 처리기


void CBinCntrlDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}


BOOL CBinCntrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_CtrlSlider.SetRange(0, 255);
	m_CtrlSlider.SetPos(100);

	m_binValDisp = m_CtrlSlider.GetPos();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CBinCntrlDlg::OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	CChildFrame* pChild = (CChildFrame*)pFrame->GetActiveFrame();
	ASSERT(pChild);
	CWinTestDoc* pDoc = (CWinTestDoc*)pChild->GetActiveDocument();
	ASSERT(pDoc);
	


	// Slider의 현재 위치를 가져옴  
	m_binValDisp = m_CtrlSlider.GetPos();
	UpdateData(FALSE); // 잡아온 데이터값으로 다이알로그박스의 표시부 갱신 

	// 현재 활성화 도큐멘트클래스 아래의 이치화 계산 함수를 호출 
	pDoc->m_BinThres(256, 256, m_binValDisp); // 슬라이드에서 설정한 이치화 계수치를 넘겨준다. 	
	*pResult = 0;
}
