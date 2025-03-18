
// WinTestView.cpp: CWinTestView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "WinTest.h"
#endif

#include "WinTestDoc.h"
#include "WinTestView.h"

#include "CBinCntrlDlg.h"

#include"MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinTestView

IMPLEMENT_DYNCREATE(CWinTestView, CView)

BEGIN_MESSAGE_MAP(CWinTestView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(IDM_BIN_DYNAMIC, &CWinTestView::OnBinDynamic)
	ON_COMMAND(IDM_HISTO_EQUAL, &CWinTestView::OnHistoEqual)
	ON_COMMAND(IDM_HISTO_STRETCH, &CWinTestView::OnHistoStretch)
	ON_COMMAND(IDM_HISTO_SPEC_FILEIN, &CWinTestView::OnHistoSpecFilein)
	ON_COMMAND(IDM_HISTO_SPEC_CAL, &CWinTestView::OnHistoSpecCal)
	ON_COMMAND(IDM_AUTO_LB, &CWinTestView::OnAutoLb)
	ON_COMMAND(IDM_SMOOTHING_BOX, &CWinTestView::OnSmoothingBox)
	ON_COMMAND(IDM_SHARPENING_LAPLACIAN, &CWinTestView::OnSharpeningLaplacian)
	ON_COMMAND(IDM_EDGE_PREWITT, &CWinTestView::OnEdgePrewitt)
	ON_COMMAND(IDM_ZOOMOUT, &CWinTestView::OnZoomout)
	ON_COMMAND(IDM_ZOOMIN, &CWinTestView::OnZoomin)
	ON_COMMAND(IDM_ROTATION, &CWinTestView::OnRotation)
	ON_COMMAND(IDM_2D_FFT, &CWinTestView::On2dFft)
	ON_COMMAND(IDM_MORPHOLOGY_BIN, &CWinTestView::OnMorphologyBin)
	ON_COMMAND(IDM_MORPHOLOGY_BINA, &CWinTestView::OnMorphologyBina)
	ON_COMMAND(IDM_GRAY_E, &CWinTestView::OnGrayE)
	ON_COMMAND(IDM_BIN_LABELLING, &CWinTestView::OnBinLabelling)
	ON_COMMAND(IDM_MY_LABELING, &CWinTestView::OnMyLabeling)
	ON_COMMAND(IDM_BORDER_FOLLOW, &CWinTestView::OnBorderFollow)
	ON_COMMAND(IDM_BLOCKMATCH_MAD, &CWinTestView::OnBlockmatchMad)
	ON_COMMAND(IDM_TRANSLATION, &CWinTestView::OnTranslation)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(IDM_NGC, &CWinTestView::OnNgc)
	ON_COMMAND(IDM_LINE, &CWinTestView::OnLine)
	ON_COMMAND(IDM_CIRCLE, &CWinTestView::OnCircle)
	ON_COMMAND(IDM_REVERSE, &CWinTestView::OnReverse)
	ON_COMMAND(IDM_TEMPER, &CWinTestView::OnTemper)
	ON_COMMAND(IDM_TEMPOR, &CWinTestView::OnTempor)
	ON_COMMAND(IDM_RE2TEM, &CWinTestView::OnRe2tem)
END_MESSAGE_MAP()

// CWinTestView 생성/소멸

CWinTestView::CWinTestView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	height = width = 256;
	int rwsize = (((width)+31) / 32 * 4);  // 4바이트의 배수여야 함
	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));

	BmInfo->bmiHeader.biBitCount = 8;
	BmInfo->bmiHeader.biClrImportant = 256;
	BmInfo->bmiHeader.biClrUsed = 256;
	BmInfo->bmiHeader.biCompression = 0;
	BmInfo->bmiHeader.biHeight = height;
	BmInfo->bmiHeader.biPlanes = 1;
	BmInfo->bmiHeader.biSize = 40;
	BmInfo->bmiHeader.biSizeImage = rwsize * height;
	BmInfo->bmiHeader.biWidth = width;
	BmInfo->bmiHeader.biXPelsPerMeter = 0;
	BmInfo->bmiHeader.biYPelsPerMeter = 0;
	for (int i = 0; i < 256; i++) // Palette number is 256
	{
		BmInfo->bmiColors[i].rgbRed = BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbBlue = i;
		BmInfo->bmiColors[i].rgbReserved = 0;
	}

	BufBmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	memcpy(BufBmInfo, BmInfo, sizeof(BITMAPINFO));
	memcpy(BufBmInfo->bmiColors, BmInfo->bmiColors, 256 * sizeof(RGBQUAD));
	m_flagMouse = FALSE;

}

CWinTestView::~CWinTestView()
{
	free(BmInfo);
	free(BufBmInfo);

}

BOOL CWinTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CWinTestView 그리기

void CWinTestView::OnDraw(CDC* pDC)
{
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) m_RevImg[i][j] = pDoc->m_InImg[height - i - 1][j];

	SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, width, height,
		0, 0, 0, height, m_RevImg, BmInfo, DIB_RGB_COLORS);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) m_RevImg[i][j] = pDoc->m_OutImg[height - i - 1][j];

	SetDIBitsToDevice(pDC->GetSafeHdc(), 300, 0, width, height,
		0, 0, 0, height, m_RevImg, BmInfo, DIB_RGB_COLORS);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) m_RevImg[i][j] = pDoc->m_OutImg1[height - i - 1][j];

	SetDIBitsToDevice(pDC->GetSafeHdc(), 600, 0, width, height,
		0, 0, 0, height, m_RevImg, BmInfo, DIB_RGB_COLORS);

	/*for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			unsigned char InVal = pDoc->m_InImg[i][j];
			unsigned char OutVal = pDoc->m_OutImg[i][j];

			pDC->SetPixel(j, i, RGB(InVal, InVal, InVal));
			pDC->SetPixel(j + 300, i, RGB(OutVal, OutVal, OutVal));
		}
	}*/

	//템플레이트 표시작업
	if (m_flagMouse == TRUE) 
		pDC->DrawEdge(&m_RectTrack, EDGE_ETCHED, BF_RECT);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);

	if (pFrame->m_flagTemplate == TRUE) // template가 설정되어 있는 경우 
	{
		pDC->DrawEdge(&m_RectTrack, EDGE_ETCHED, BF_RECT);
		m_DibDraw(pDC, 300, 0, pFrame->tHeight, pFrame->tWidth, pFrame->m_TempImg);
	}

	if (!(m_MatchPos.right == 0 && m_MatchPos.bottom == 0))
		pDC->DrawEdge(&m_MatchPos, EDGE_BUMP, BF_RECT);
}


// CWinTestView 인쇄

BOOL CWinTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CWinTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CWinTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CWinTestView 진단

#ifdef _DEBUG
void CWinTestView::AssertValid() const
{
	CView::AssertValid();
}

void CWinTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinTestDoc* CWinTestView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinTestDoc)));
	return (CWinTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CWinTestView 메시지 처리기


void CWinTestView::OnBinDynamic()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CBinCntrlDlg pbinCtrlDlg;
	pbinCtrlDlg.DoModal();
}



void CWinTestView::OnHistoEqual()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_HistoEqual(256, 256);
	Invalidate(FALSE);
}


void CWinTestView::OnHistoStretch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_HIstoStretch(256, 256);
	Invalidate(FALSE);
}


void CWinTestView::OnHistoSpecFilein()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_HistoSpec_FileIn();
	Invalidate(FALSE);

}


void CWinTestView::OnHistoSpecCal()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_HistoSpec(256, 256);
	Invalidate(FALSE);
}


void CWinTestView::OnAutoLb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBinarOtzu();
	Invalidate(FALSE);
}


void CWinTestView::OnSmoothingBox()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_SmoothingBox(256,256);
	Invalidate(FALSE);
}


void CWinTestView::OnSharpeningLaplacian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_SharpeningLaplacian(256, 256);
	Invalidate(FALSE);
}


void CWinTestView::OnEdgePrewitt()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_EdgePreWitt(256, 256);
	Invalidate(FALSE);
}


void CWinTestView::OnZoomout()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_ZoomOut(256, 256,0.7);
	Invalidate(FALSE);
}


void CWinTestView::OnZoomin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_ZoomIn(256, 256, 1.3);
	Invalidate(FALSE);
}


void CWinTestView::OnRotation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_Rotation(256, 256, (256 / 2), (256 / 2), 35);
	Invalidate(FALSE);
}


void CWinTestView::On2dFft()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_FFT2D(256,256,1);
	Invalidate(FALSE);
}


void CWinTestView::OnMorphologyBin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_AllocStructureElementBinary(4);
	pDoc->m_SetStructureElementBinary(0, 4, 0);
	pDoc->m_SetStructureElementBinary(1, 0, -1);
	pDoc->m_SetStructureElementBinary(2, 0, 0);
	pDoc->m_SetStructureElementBinary(3, 0, 1);


	pDoc->m_MorphologyBinaryErosion(256, 256);

	pDoc->m_FreeStructureElementBinary();
	Invalidate(FALSE);
}


void CWinTestView::OnMorphologyBina()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_AllocStructureElementBinary(4);
	pDoc->m_SetStructureElementBinary(0, 4, 0);
	pDoc->m_SetStructureElementBinary(1, 0, -1);
	pDoc->m_SetStructureElementBinary(2, 0, 0);
	pDoc->m_SetStructureElementBinary(3, 0, 1);


	pDoc->m_MorphologyBinaryDilation(256, 256);

	pDoc->m_FreeStructureElementBinary();
	Invalidate(FALSE);
}


void CWinTestView::OnMorphologyGrayErosion()
{
	// TODO: 여기에 구현 코드 추가.
	
}


void CWinTestView::OnGrayE()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
	ASSERT_VALID(pDoc);				   // 주소를 가져옴 

	//구조 요소를 생성후 값을 지정 
	pDoc->m_AllocStructureElementGray(6);
	pDoc->m_SetStructureElementGray(0, 6, 0, 0);
	pDoc->m_SetStructureElementGray(1, -1, 0, 1);
	pDoc->m_SetStructureElementGray(2, 0, -1, 1);
	pDoc->m_SetStructureElementGray(3, 0, 0, 2);
	pDoc->m_SetStructureElementGray(4, 0, 1, 1);
	pDoc->m_SetStructureElementGray(5, 1, 0, 1);

	pDoc->m_MorphologyGrayErosion(256, 256);

	//동적 할당 메모리 해제 
	pDoc->m_FreeStructureElementGray();

	Invalidate(FALSE); // 화면 갱신	
}


void CWinTestView::OnBinLabelling()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->grass_label(256, 256);

	Invalidate(FALSE);
}


void CWinTestView::OnMyLabeling()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_BlobColoring(256, 256);

	Invalidate(FALSE);

}


void CWinTestView::OnBorderFollow()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_BorderFollow(256, 256);

	Invalidate(FALSE);
}


void CWinTestView::OnTranslation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_TranslationImg();

	Invalidate(FALSE);
}


void CWinTestView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (point.y < 0 || point.y >= height)
		return;
	if (point.x < 0 || point.x >= width)
		return;

	m_RectTrack.left = m_RectTrack.right = point.x;
	m_RectTrack.top = m_RectTrack.bottom = point.y;
	m_flagMouse = TRUE;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);
	pFrame->m_flagTemplate = FALSE;


	CView::OnLButtonDown(nFlags, point);
}


void CWinTestView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_flagMouse == TRUE)
	{
		if (point.y < 0 || point.y >= height)
		{
			Invalidate(FALSE);	return;
		}

		if (point.x < 0 || point.x >= width)
		{
			Invalidate(FALSE);	return;
		}

		m_RectTrack.right = point.x;
		m_RectTrack.bottom = point.y;
		Invalidate(FALSE);
		 
	}

	CView::OnMouseMove(nFlags, point);
}


void CWinTestView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_flagMouse == TRUE)
	{
		CWinTestDoc* pDoc = GetDocument(); // 다큐멘트 클래스를 참조하기 위해 
		ASSERT_VALID(pDoc);				   // 인스턴스 주소를 가져옴 
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		ASSERT(pFrame);

		if (point.y < 0 || point.y >= height)  //영역 밖이면
		{
			m_flagMouse = pFrame->m_flagTemplate = FALSE;
			Invalidate(FALSE);
			return;
		}

		if (point.x < 0 || point.x >= width)  //영역 밖이면
		{
			m_flagMouse = pFrame->m_flagTemplate = FALSE;
			Invalidate(FALSE);
			return;
		}

		if (m_RectTrack.top >= m_RectTrack.bottom || m_RectTrack.left >= m_RectTrack.right) // 영역이 반대면
		{
			m_flagMouse = FALSE;
			pFrame->m_flagTemplate = TRUE;
			Invalidate(FALSE);
			return;
		}

		if (abs(m_RectTrack.right - m_RectTrack.left) < 10 && abs(m_RectTrack.bottom - m_RectTrack.top) < 10)//  영역이 너무 작으면  /  asb는 int형 절대값, fabs는 double형 절대값
		{
			m_flagMouse = FALSE;
			pFrame->m_flagTemplate = TRUE;
			Invalidate(FALSE);
			return;
		}

		///
		m_flagMouse = FALSE;
		pFrame->m_flagTemplate = TRUE;

		// 선택한 영역의 크기를 측정
		pFrame->tHeight = m_RectTrack.bottom - m_RectTrack.top;
		pFrame->tWidth = m_RectTrack.right - m_RectTrack.left;

		// 이전에 설정한 템플이 없으면 측정한 크기만큼의 템플이미지를 자동할당, 기존에 존재하면 없애고 새로 만들기
		if (pFrame->m_TempImg == NULL)
			pFrame->m_TempImg = new unsigned char[pFrame->tHeight * pFrame->tWidth];

		else
		{
			delete[]pFrame->m_TempImg;
			pFrame->m_TempImg = new unsigned char[pFrame->tHeight * pFrame->tWidth];
		}


		// 만든 템플 이미지자리에다가 선택한 영역의 이미지 넣기
		int i, j, index;
		for (i = 0; i < pFrame->tHeight; i++)
		{
			index = i * pFrame->tWidth;
			for (j = 0; j < pFrame->tWidth; j++)
				pFrame->m_TempImg[index + j] = pDoc->m_InImg[m_RectTrack.top + i][m_RectTrack.left + j];
		}

		Invalidate(FALSE);
	}


	CView::OnLButtonUp(nFlags, point);
}


void CWinTestView::m_DibDraw(CDC* pDC, int px, int py, int height, int width, BYTE* BufImg)
{
	// TODO: 여기에 구현 코드 추가.
	int rwsize = (((8 * width) + 31) / 32 * 4);  // 4바이트의 배수여야 함
	BufBmInfo->bmiHeader.biHeight = height;
	BufBmInfo->bmiHeader.biSizeImage = rwsize * height;
	BufBmInfo->bmiHeader.biWidth = width;

	unsigned char* BufRevImg = new unsigned char[height * rwsize];
	int index1, index2, i, j;
	for (i = 0; i < height; i++)
	{
		index1 = i * rwsize;
		index2 = (height - i - 1) * width;
		for (j = 0; j < width; j++) BufRevImg[index1 + j] = BufImg[index2 + j];
	}

	SetDIBitsToDevice(pDC->GetSafeHdc(), px, py, width, height,
		0, 0, 0, height, BufRevImg, BufBmInfo, DIB_RGB_COLORS);
	delete[]BufRevImg;
}


void CWinTestView::OnBlockmatchMad()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);

	if (pFrame->m_flagTemplate == FALSE)
		return;

	POINT OptPos = pDoc->m_MatchMAD(256, 256, pFrame->m_TempImg, pFrame->tHeight, pFrame->tWidth);
	m_MatchPos.left = OptPos.x;
	m_MatchPos.top = OptPos.y;
	m_MatchPos.right = OptPos.x + pFrame->tWidth;
	m_MatchPos.bottom = OptPos.y + pFrame->tHeight;

	Invalidate(FALSE);
}


void CWinTestView::OnNgc()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pFrame);

	if (pFrame->m_flagTemplate == FALSE)
		return;

	POINT OptPos = pDoc->m_TemplateMatch(256, 256, pFrame->m_TempImg, pFrame->tHeight, pFrame->tWidth);
	m_MatchPos.left = OptPos.x;
	m_MatchPos.top = OptPos.y;
	m_MatchPos.right = OptPos.x + pFrame->tWidth;
	m_MatchPos.bottom = OptPos.y + pFrame->tHeight;

	Invalidate(FALSE);
}


void CWinTestView::OnLine()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//가로 세로 크기정한거다
	int height = 256, widht = 256;

	//그냥 for문에 필요한 변수
	register int i, j;

	// org,out 이미지 new로 동적크기넣어줌
	unsigned char* orgImg = new unsigned char[height * width];
	unsigned char* outImg = new unsigned char[height * width];


	//256,256 for문 돌려서 투입 이미지를 org 이미지에 박아버림
	for (i = 0; i < height; i++) 
		for (j = 0; j < width; j++) 
			orgImg[i * width + j] = pDoc->m_InImg[i][j];

	// 여기서 in 이미지를 org에 넣고
	pDoc->HT_Line(orgImg, outImg, height, width);
	//함수 끝나면 out이미지에 답안지가 있어서

	//여기서 out 이미지를 m_out이미지에 넣고 출력하는거임
	for (i = 0; i < height; i++) 
		for (j = 0; j < width; j++) 
			pDoc->m_OutImg[i][j] = outImg[i * width + j];

	delete[]orgImg;
	delete[]outImg;


	Invalidate(FALSE);
}


void CWinTestView::OnCircle()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//가로 세로 크기정한거다
	int height = 256, widht = 256;

	//그냥 for문에 필요한 변수
	register int i, j;

	// org,out 이미지 new로 동적크기넣어줌
	unsigned char* orgImg = new unsigned char[height * width];
	unsigned char* outImg = new unsigned char[height * width];


	//256,256 for문 돌려서 투입 이미지를 org 이미지에 박아버림
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			orgImg[i * width + j] = pDoc->m_InImg[i][j];

	// 여기서 in 이미지를 org에 넣고
	pDoc->HT_Circle(orgImg, outImg, height, width);
	//함수 끝나면 out이미지에 답안지가 있어서

	//여기서 out 이미지를 m_out이미지에 넣고 출력하는거임
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			pDoc->m_OutImg[i][j] = outImg[i * width + j];

	delete[]orgImg;
	delete[]outImg;


	Invalidate(FALSE);
}


void CWinTestView::OnReverse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Reverse(256, 256);
	Invalidate(FALSE);
}


void CWinTestView::OnTemper()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Temper(256, 256);
	Invalidate(FALSE);
}


void CWinTestView::OnTempor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//가로 세로 크기정한거다
	int height = 256, widht = 256;

	//그냥 for문에 필요한 변수
	register int i, j;

	// org,out 이미지 new로 동적크기넣어줌
	unsigned char* orgImg = new unsigned char[height * width];
	unsigned char* outImg = new unsigned char[height * width];


	//256,256 for문 돌려서 투입 이미지를 org 이미지에 박아버림
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			orgImg[i * width + j] = pDoc->m_InImg[i][j];

	// 여기서 in 이미지를 org에 넣고
	pDoc->HT_Line123(orgImg, outImg, height, width);
	//함수 끝나면 out이미지에 답안지가 있어서

	//여기서 out 이미지를 m_out이미지에 넣고 출력하는거임
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			pDoc->m_OutImg[i][j] = outImg[i * width + j];

	delete[]orgImg;
	delete[]outImg;


	Invalidate(FALSE);

}


void CWinTestView::OnRe2tem()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->Temper123(256, 256);
	Invalidate(FALSE);
}
