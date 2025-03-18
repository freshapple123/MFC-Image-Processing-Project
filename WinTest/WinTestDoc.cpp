
// WinTestDoc.cpp: CWinTestDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "WinTest.h"
#endif

#include "WinTestDoc.h"
#include <stdio.h>
#include <propkey.h>

#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWinTestDoc

IMPLEMENT_DYNCREATE(CWinTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CWinTestDoc, CDocument)
	//ON_COMMAND(IDM_BMP, &CWinTestDoc::OnBmp)
//	ON_COMMAND(IDM_BMPp, &CWinTestDoc::OnBmpp)
END_MESSAGE_MAP()


// CWinTestDoc 생성/소멸

CWinTestDoc::CWinTestDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.


	

}

CWinTestDoc::~CWinTestDoc()
{
}

BOOL CWinTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CWinTestDoc serialization

void CWinTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
		ar.Write(m_OutImg, 256 * 256); // 처리된 영상배열 m_OutImg를 파일로 저장 

	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		CFile* infile = ar.GetFile(); // 입력할 화일의 포인트를 가져옴.
		if (infile->GetLength() != 256 * 256) // 화일 사이즈를 검사함 
		{
			AfxMessageBox(_T("파일 크기가 256x256사이즈가 아닙니다."));
			return;
		}
		ar.Read(m_InImg, infile->GetLength()); // 영상파일을 읽어 m_InImg배열에 저장 

	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CWinTestDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CWinTestDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWinTestDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWinTestDoc 진단

#ifdef _DEBUG
void CWinTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWinTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWinTestDoc 명령


void CWinTestDoc::m_BinThres(int height, int width, int binThres)
{
	// TODO: 여기에 구현 코드 추가.
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (m_InImg[i][j] > binThres) m_OutImg[i][j] = 255;
			else m_OutImg[i][j] = 0;
		}
	}
	UpdateAllViews(FALSE);
}


void CWinTestDoc::m_HistoEqual(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int i, j;
	unsigned int* histogram = new unsigned int[256];
	unsigned int* sum_hist = new unsigned int[256];

	//초기화
	for (i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}

	//히스토그램 만드는 공식임
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			histogram[m_InImg[i][j]]++;
		}
	}

	// 여기서 부터 이건 평활화 위한 공식 준비
	int sum = 0;
	float scale_factor = 255.0f / (float)(height * width);
	

	// 여기서 
	for (i = 0; i < 256; i++)
	{
		sum += histogram[i];
		sum_hist[i] = (int)((sum * scale_factor) + 0.5);
	}


	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			m_OutImg[i][j] = sum_hist[m_InImg[i][j]];
		}
	}

	delete[]histogram; delete[]sum_hist;




}


void CWinTestDoc::m_HIstoStretch(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int i, j;
	int lowvalue = 255, hightvalue = 0;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (m_InImg[i][j] < lowvalue)
				lowvalue = m_InImg[i][j];
		}
	}

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if (m_InImg[i][j] > hightvalue)
				hightvalue = m_InImg[i][j];
		}
	}

	float mult = 255.0f / (float)(hightvalue - lowvalue);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			m_OutImg[i][j] = (unsigned char)((m_InImg[i][j] - lowvalue) * mult);
		}
	}
}


void CWinTestDoc::m_HistoSpec_FileIn()
{
	// TODO: 여기에 구현 코드 추가.
	CFileDialog openingdlg1(TRUE);
	CFile file;

	if (openingdlg1.DoModal() == IDOK)
	{
		file.Open(openingdlg1.GetFileName(), CFile::modeRead);
		file.Read(m_InImg1, sizeof(m_InImg1));
		file.Close();
	}

	int i, j;

	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 256; j++)
		{
			m_OutImg[i][j] = m_InImg1[i][j];
		}
	}
}


void CWinTestDoc::m_HistoSpec(int height, int width)
{
	int i, j;
	// histogram연산을 위해 사용할 배열을 할당 
	unsigned int* histogram = new unsigned int[256];
	unsigned int* sum_hist = new unsigned int[256];
	unsigned int* desired_histogram = new unsigned int[256];
	unsigned int* desired_sum_hist = new unsigned int[256];

	// histogram배열을 초기화 
	for (i = 0; i < 256; i++) histogram[i] = desired_histogram[i] = 0;

	// 영상의 histogram을 계산하라 
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			histogram[m_InImg[i][j]]++;  // 입력 영상의 히스토그램 
			desired_histogram[m_InImg1[i][j]]++;  // 지정 영상의 히스토그램 
		}
	}


	// histogram의 정규화된 합을 계산하라 
	int sum = 0;
	float scale_factor = 255.0f / (float)(height * width);

	for (i = 0; i < 256; i++)
	{
		sum += histogram[i];
		sum_hist[i] = (int)((sum * scale_factor) + 0.5);
	}

	// desired histogram에 대한 정규화된 합을 계산
	sum = 0;
	for (i = 0; i < 256; i++)
	{
		sum += desired_histogram[i];
		desired_sum_hist[i] = (int)(sum * scale_factor);
	}

	// 가장 가까운 정규화합 히스토그램값을 주는 index를 찾음   
	int* inv_hist = new int[256];
	int hist_min, hist_index, hist_diff;
	for (i = 0; i < 256; i++)
	{
		hist_min = 1000;
		for (j = 0; j < 256; j++)

		{
			hist_diff = abs((int)sum_hist[i] - (int)desired_sum_hist[j]);
			if (hist_diff < hist_min)
			{
				hist_min = hist_diff;
				hist_index = j;
			}
		}

		inv_hist[i] = hist_index;
	}

	// 역 히스토그램 변환 
	for (i = 0; i < height; i++) for (j = 0; j < width; j++) m_OutImg[i][j] = inv_hist[m_InImg[i][j]];

	// 메모리 해제 
	delete[]inv_hist; delete[]histogram; delete[]desired_histogram;
	delete[]sum_hist; delete[]desired_sum_hist;
}


void CWinTestDoc::OnBinarOtzu()
{
	// TODO: 여기에 구현 코드 추가.
	int height = 256, width = 256;
	register int i, j;

	unsigned char* orgImg = new unsigned char[height * width];
	unsigned char* outImg = new unsigned char[height * width];

	for (i = 0; i < height; i++) for (j = 0; j < width; j++) orgImg[i * width + j] = m_InImg[i][j];

	Otzu_Threshold(orgImg, outImg, height, width);

	for (i = 0; i < height; i++) for (j = 0; j < width; j++) m_OutImg[i][j] = outImg[i * width + j];

	delete[]orgImg;
	delete[]outImg;

	UpdateAllViews(NULL);
}


void CWinTestDoc::Otzu_Threshold(unsigned char* orgImg, unsigned char* outImg, int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, t;

	// Histogram set
	int		hist[256];
	float   prob[256];
	for (i = 0; i < 256; i++) { hist[i] = 0; prob[i] = 0.0f; }
	for (i = 0; i < height * width; i++) hist[(int)orgImg[i]]++;
	for (i = 0; i < 256; i++) prob[i] = (float)hist[i] / (float)(height * width);


	float wsv_min = 1000000.0f;
	float wsv_u1, wsv_u2, wsv_s1, wsv_s2;
	int wsv_t;

	for (t = 0; t < 256; t++)
	{
		// q1, q2 계산 
		float q1 = 0.0f, q2 = 0.0f;

		for (i = 0; i < t; i++)   q1 += prob[i];
		for (i = t; i < 256; i++) q2 += prob[i];

		if (q1 == 0 || q2 == 0) continue;

		// u1, u2 계산 
		float u1 = 0.0f, u2 = 0.0f;
		for (i = 0; i < t; i++)   u1 += i * prob[i]; u1 /= q1;
		for (i = t; i < 256; i++) u2 += i * prob[i]; u2 /= q2;

		// s1, s2 계산 
		float s1 = 0.0f, s2 = 0.0f;
		for (i = 0; i < t; i++)   s1 += (i - u1) * (i - u1) * prob[i]; s1 /= q1;
		for (i = t; i < 256; i++) s2 += (i - u2) * (i - u2) * prob[i]; s2 /= q2;

		float wsv = q1 * s1 + q2 * s2;

		if (wsv < wsv_min)
		{
			wsv_min = wsv; wsv_t = t;
			wsv_u1 = u1; wsv_u2 = u2;
			wsv_s1 = s1; wsv_s2 = s2;
		}
	}

	// thresholding
	for (i = 0; i < height * width; i++) if (orgImg[i] < wsv_t) outImg[i] = 0; else outImg[i] = 255;
}


void CWinTestDoc::m_SmoothingBox(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int MaskBox[3][3] = { {1,1,1},{1,1,1} ,{1,1,1} };
	int heightm1 = height - 1;
	int widthm1 = width - 1;
	int mr, mc;
	int newValue;
	int i, j;

	// 우선 결과 출력될 이미지 배열 0초기화 시키기
	/*for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)*/
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			m_OutImg[i][j] = 0;

	for (i = 1; i < heightm1; i++)
		for (j = 1; j < widthm1; j++)
		{
			newValue = 0;
			for (mr = 0; mr < 3; mr++)
				for (mc = 0; mc < 3; mc++)
					newValue += (MaskBox[mr][mc] * m_InImg[i + mr - 1][j + mr - 1]);
			newValue /= 9;

			m_OutImg[i][j] = (BYTE)newValue;
		}

}


void CWinTestDoc::m_SharpeningLaplacian(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	

	int MaskBox[3][3] = { {-1,-1,-1},{-1,8,-1} ,{-1,-1,-1} };
	int heightm1 = height - 1;
	int widthm1 = width - 1;
	int mr, mc, newValue, i, j, min, max;
	int* pTmpImg;
	float constVal1, constVal2;


	pTmpImg = new int[height * width];

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			m_OutImg[i][j] = 0;
			pTmpImg[i * width + j] = 0;
		}

	for (i = 1; i < heightm1; i++)
	{
		for (j = 1; j < widthm1; j++)
		{
			newValue = 0;
			for (mr = 0; mr < 3; mr++)
				for (mc = 0; mc < 3; mc++)
				{
					newValue += (MaskBox[mr][mc] * m_InImg[i + mr - 1][j + mc - 1]);
				}
					//값을 양수로 변환
				if (newValue < 0)
					newValue = -newValue;
				pTmpImg[i * width + j] = newValue;
				
		}
	}

	min = (int)10e10;
	max = (int)-10e10;
	for (i = 1; i < heightm1; i++)
		for (j = 1; j < widthm1; j++)
		{
			newValue = pTmpImg[i * width + j];
			if (newValue < min)
				min = newValue;
			if (newValue > max)
				max = newValue;
		}

	constVal1 = (float)(255.0 / (max - min));
	constVal2 = (float)(-255.0 * min / (max - min));
	for (i = 1; i < heightm1; i++)
		for (j = 1; j < widthm1; j++)
		{
			//min 과 max 사이의 값을 0,255로 바꾸기
			newValue = pTmpImg[i * width + j];
			newValue = constVal1 * newValue + constVal2;
			m_OutImg[i][j] = (BYTE)newValue;
		}

	delete[] pTmpImg;


}


void CWinTestDoc::m_EdgePreWitt(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int MaskPrewittX[3][3] = { {-1,0,1},
						{-1,0,1},
						{-1,0,1} };
	int MaskPrewittY[3][3] = { {1,1,1},
							{0,0,0},
							{-1,-1,-1} };
	int heightm1 = height - 1;
	int widthm1 = width - 1;
	int mr, mc;
	int newValue;
	int i, j;
	int* pImgPrewittX, * pImgPrewittY;
	int min, max, where;
	float constVal1, constVal2;

	//정수값을 갖는 이미지 동적 메모리 할당
	pImgPrewittX = new int[height * width];
	pImgPrewittY = new int[height * width];

	//결과 이미지 0으로 초기화
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			m_OutImg[i][j] = 0;
			where = i * width + j;
			pImgPrewittX[where] = 0;
			pImgPrewittY[where] = 0;
		}

	//X 방향 에지 강도 계산 
	for (i = 1; i < heightm1; i++)
	{
		for (j = 1; j < widthm1; j++)
		{
			newValue = 0; //0으로 초기화
			for (mr = 0; mr < 3; mr++)
				for (mc = 0; mc < 3; mc++)
					newValue += (MaskPrewittX[mr][mc] * m_InImg[i + mr - 1][j + mc - 1]);
			pImgPrewittX[i * width + j] = newValue;
		}
	}
	//Y 방향 에지 강도 계산 
	for (i = 1; i < heightm1; i++)
	{
		for (j = 1; j < widthm1; j++)
		{
			newValue = 0; //0으로 초기화
			for (mr = 0; mr < 3; mr++)
				for (mc = 0; mc < 3; mc++)
					newValue += (MaskPrewittY[mr][mc] * m_InImg[i + mr - 1][j + mc - 1]);
			pImgPrewittY[i * width + j] = newValue;
		}
	}
	//에지 강도 계산 절대값(X)+절대값(Y)후 pImgPrewittX[]에 저장
	for (i = 1; i < heightm1; i++)
		for (j = 1; j < widthm1; j++)
		{
			where = i * width + j;
			constVal1 = pImgPrewittX[where];
			constVal2 = pImgPrewittY[where];
			if (constVal1 < 0)
				constVal1 = -constVal1;
			if (constVal2 < 0)
				constVal2 = -constVal2;
			pImgPrewittX[where] = constVal1 + constVal2;
		}
	//디스플레이를 위해 0에서 255사이로 값의 범위를 매핑
	//이를 위해 먼저 최대,최소값을 찾은후 이를 이용하여 매핑한다.
	min = (int)10e10;
	max = (int)-10e10;
	for (i = 1; i < heightm1; i++)
	{
		for (j = 1; j < widthm1; j++)
		{
			newValue = pImgPrewittX[i * width + j];
			if (newValue < min)
				min = newValue;
			if (newValue > max)
				max = newValue;
		}
	}
	//변환시 상수값을 미리 계산 
	constVal1 = (float)(255.0 / (max - min));
	constVal2 = (float)(-255.0 * min / (max - min));
	for (i = 1; i < heightm1; i++)
	{
		for (j = 1; j < widthm1; j++)
		{
			//[min,max]사이의 값을 [0,255]값으로 변환 
			newValue = pImgPrewittX[i * width + j];
			newValue = constVal1 * newValue + constVal2;
			m_OutImg[i][j] = (BYTE)newValue;
		}
	}

	//동적 할당 메모리 해제
	delete[] pImgPrewittX;
	delete[] pImgPrewittY;
}


void CWinTestDoc::m_ZoomOut(int height, int width, float zoomoutfactor)
{
	// TODO: 여기에 구현 코드 추가.
	BYTE* pZoomImg;
	BYTE newValue;
	int new_height = (int)(height * zoomoutfactor);//새로운 이미지의 높이 계산 
	int new_width = (int)(width * zoomoutfactor);//새로운 이미지의 폭 계산 
	int heightm1 = height - 1;
	int widthm1 = width - 1;
	int where, org_where;
	int r, c;//타겟 이미지 좌표 
	float r_orgr, r_orgc;//원 이미지 상의 해당 좌표 (실수값)
	int i_orgr, i_orgc;//원 이미지 상의 해당 좌표 (정수값)
	float sr, sc;// 예 1.24-1=0.24
	float I1, I2, I3, I4;

	pZoomImg = new BYTE[new_height * new_width];

	for (r = 0; r < new_height; r++)
		for (c = 0; c < new_width; c++)
		{
			r_orgr = r / zoomoutfactor;
			r_orgc = c / zoomoutfactor;
			i_orgr = floor(r_orgr);//예: floor(2.8)=2.0 ,즉 내림함수이다.
			i_orgc = floor(r_orgc);
			sr = r_orgr - i_orgr;
			sc = r_orgc - i_orgc;
			//범위 조사 
			//원이미지의 범위를 벗어나는 경우 0값 할당 
			if (i_orgr<0 || i_orgr>heightm1 || i_orgc<0 || i_orgc>widthm1)
			{
				where = r * new_width + c;
				pZoomImg[where] = 0;
			}
			//원 이미지의 범위 내에 존재 이중 선형 보간 수행 
			else
			{
				I1 = (float)m_InImg[i_orgr][i_orgc];//(org_r,org_c)
				I2 = (float)m_InImg[i_orgr][i_orgc + 1];//(org_r,org_c+1)
				I3 = (float)m_InImg[i_orgr + 1][i_orgc + 1];//(org_r+1,org_c+1)
				I4 = (float)m_InImg[i_orgr + 1][i_orgc];//(org_r+1,org_c)

				//이중 선형 보간을 통한 새로운 밝기값 계산
				newValue = (BYTE)(I1 * (1 - sc) * (1 - sr) + I2 * sc * (1 - sr) + I3 * sc * sr + I4 * (1 - sc) * sr);
				where = r * new_width + c;
				pZoomImg[where] = newValue;
			}
		}
	//일단 영상의 일부만 복사하는 것으로 함.
	for (r = 0; r < new_height; r++)
		for (c = 0; c < new_width; c++)
		{
			m_OutImg[r][c] = pZoomImg[r * new_width + c];
		}
	//동적 할당 메모리 해제 
	delete[] pZoomImg;
}


void CWinTestDoc::m_ZoomIn(int height, int width, float zoominfactor)
{
	// TODO: 여기에 구현 코드 추가.
	BYTE* pZoomImg;
	BYTE newValue;
	int new_height = (int)(height * zoominfactor);//새로운 이미지의 높이 계산 
	int new_width = (int)(width * zoominfactor);//새로운 이미지의 폭 계산 
	int heightm1 = height - 1;
	int widthm1 = width - 1;
	int where, org_where;
	int r, c;//타겟 이미지 좌표 
	float r_orgr, r_orgc;//원 이미지 상의 해당 좌표 (실수값)
	int i_orgr, i_orgc;//원 이미지 상의 해당 좌표 (정수값)
	float sr, sc;// 예 1.24-1=0.24
	float I1, I2, I3, I4;


	//Zoom Image를 위한 동적 메모리 할당
	pZoomImg = new BYTE[new_height * new_width];

	for (r = 0; r < new_height; r++)
		for (c = 0; c < new_width; c++)
		{
			r_orgr = r / zoominfactor;
			r_orgc = c / zoominfactor;
			i_orgr = floor(r_orgr);//예: floor(2.8)=2.0
			i_orgc = floor(r_orgc);
			sr = r_orgr - i_orgr;
			sc = r_orgc - i_orgc;
			//범위 조사 
			//원이미지의 범위를 벗어나는 경우 0값 할당 
			if (i_orgr<0 || i_orgr>heightm1 || i_orgc<0 || i_orgc>widthm1)
			{
				where = r * new_width + c;
				pZoomImg[where] = 0;
			}
			//원 이미지의 범위 내에 존재 이중 선형 보간 수행 
			else
			{
				I1 = (float)m_InImg[i_orgr][i_orgc];//(org_r,org_c)
				I2 = (float)m_InImg[i_orgr][i_orgc + 1];//(org_r,org_c+1)
				I3 = (float)m_InImg[i_orgr + 1][i_orgc + 1];//(org_r+1,org_c+1)
				I4 = (float)m_InImg[i_orgr + 1][i_orgc];//(org_r+1,org_c)

				//이중 선형 보간을 통한 새로운 밝기값 계산
				newValue = (BYTE)(I1 * (1 - sc) * (1 - sr) + I2 * sc * (1 - sr) + I3 * sc * sr + I4 * (1 - sc) * sr);
				where = r * new_width + c;
				pZoomImg[where] = newValue;
			}
		}

	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
		{
			m_OutImg[r][c] = pZoomImg[r * new_width + c];
		}
	//동적 할당 메모리 해제 
	delete[] pZoomImg;
}


void CWinTestDoc::m_Rotation(int height, int width, int center_r, int center_c, float rotationAngle )
{
	// TODO: 여기에 구현 코드 추가.
	BYTE* pRotationImg;
	BYTE newValue;
	int heightm1 = height - 1;//계산의 중복을 피하기 위해 사용
	int widthm1 = width - 1;//계산의 중복을 피하기 위해 사용
	int where, org_where;
	int r, c;//타겟 이미지 좌표 
	float r_orgr, r_orgc;//원 이미지 상의 해당 좌표 (실수값)
	int i_orgr, i_orgc;//원 이미지 상의 해당 좌표 (정수값)
	float sr, sc;// 예 1.24-1=0.24
	float I1, I2, I3, I4;
	float cosAngle, sinAngle;
	float rotationAngleRad = (float)(rotationAngle * 3.14159265 / 180);//angle->radian으로 변환

	//Rotation Image를 위한 동적 메모리 할당
	pRotationImg = new BYTE[height * width];

	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
		{
			cosAngle = (float)cos(rotationAngleRad);
			sinAngle = (float)sin(rotationAngleRad);
			//회전전의 원 이미지상의 좌표 구함 
			r_orgr = -sinAngle * (c - center_c) + cosAngle * (r - center_r) + center_r;
			r_orgc = cosAngle * (c - center_c) + sinAngle * (r - center_r) + center_c;
			i_orgr = floor(r_orgr);//예: floor(2.8)=2.0
			i_orgc = floor(r_orgc);
			sr = r_orgr - i_orgr;
			sc = r_orgc - i_orgc;
			//범위 조사 
			//원이미지의 범위를 벗어나는 경우 0값 할당 
			if (i_orgr<0 || i_orgr>heightm1 || i_orgc<0 || i_orgc>widthm1)
			{
				where = r * width + c;
				pRotationImg[where] = 0;
			}
			//원 이미지의 범위 내에 존재 이중 선형 보간 수행 
			else
			{
				I1 = (float)m_InImg[i_orgr][i_orgc];//(org_r,org_c)
				I2 = (float)m_InImg[i_orgr][i_orgc + 1];//(org_r,org_c+1)
				I3 = (float)m_InImg[i_orgr + 1][i_orgc + 1];//(org_r+1,org_c+1)
				I4 = (float)m_InImg[i_orgr + 1][i_orgc];//(org_r+1,org_c)

				//이중 선형 보간을 통한 새로운 밝기값 계산
				newValue = (BYTE)(I1 * (1 - sc) * (1 - sr) + I2 * sc * (1 - sr) + I3 * sc * sr + I4 * (1 - sc) * sr);
				where = r * width + c;
				pRotationImg[where] = newValue;
			}
		}
	//결과 이미지로 영상 복사 
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
		{
			m_OutImg[r][c] = pRotationImg[r * width + c];
		}
	//동적 할당 메모리 해제 
	delete[] pRotationImg;

}


void CWinTestDoc::m_FFT1D(int NumData, float* pOneDRealImage, float* pOneDImImage, int Forward)
{
	// TODO: 여기에 구현 코드 추가.
	int log2NumData;
	int HalfNumData;
	int i, j, k, mm;
	int step;
	int ButterFly, ButterFlyHalf;
	float RealValue, ImValue;
	double AngleRadian;
	float CosineValue, SineValue;
	float ArcRe, ArcIm, ReBuf, ImBuf, ArcBuf;

	//scramble 과정 수행 
	//입력 데이터의 순서를 바꿈 
	log2NumData = 0;
	while (NumData != (1 << log2NumData))
		log2NumData++;
	HalfNumData = NumData >> 1;
	j = 1;
	for (i = 1; i < NumData; i++)
	{
		if (i < j)
		{
			RealValue = pOneDRealImage[j - 1];
			ImValue = pOneDImImage[j - 1];
			pOneDRealImage[j - 1] = pOneDRealImage[i - 1];
			pOneDImImage[j - 1] = pOneDImImage[i - 1];
			pOneDRealImage[i - 1] = RealValue;
			pOneDImImage[i - 1] = ImValue;
		}
		k = HalfNumData;
		while (k < j)
		{
			j -= k;
			k = k >> 1;
		}
		j += k;
	}
	//butterfly 과정 수행 
	for (step = 1; step <= log2NumData; step++)
	{
		ButterFly = 1 << step;
		ButterFlyHalf = ButterFly >> 1;
		ArcRe = 1;
		ArcIm = 0;
		AngleRadian = (double)(3.141592 / ButterFlyHalf);
		CosineValue = (float)cos(AngleRadian);
		SineValue = (float)sin(AngleRadian);
		if (Forward) //Foward
			SineValue = -SineValue;
		for (j = 1; j <= ButterFlyHalf; j++)
		{
			i = j;
			while (i <= NumData)
			{
				mm = i + ButterFlyHalf;
				ReBuf = pOneDRealImage[mm - 1] * ArcRe - pOneDImImage[mm - 1] * ArcIm;
				ImBuf = pOneDRealImage[mm - 1] * ArcIm + pOneDImImage[mm - 1] * ArcRe;
				pOneDRealImage[mm - 1] = pOneDRealImage[i - 1] - ReBuf;
				pOneDImImage[mm - 1] = pOneDImImage[i - 1] - ImBuf;
				pOneDRealImage[i - 1] = pOneDRealImage[i - 1] + ReBuf;
				pOneDImImage[i - 1] = pOneDImImage[i - 1] + ImBuf;
				i += ButterFly;
			}
			ArcBuf = ArcRe;
			ArcRe = ArcRe * CosineValue - ArcIm * SineValue;
			ArcIm = ArcBuf * SineValue + ArcIm * CosineValue;
		}
	}
	if (Forward) //Forward
	{
		for (j = 0; j < NumData; j++)
		{
			pOneDRealImage[j] /= NumData;
			pOneDImImage[j] /= NumData;
		}
	}
}


void CWinTestDoc::m_FFT2D(int height, int width, int Forward)
{
	// TODO: 여기에 구현 코드 추가.
	int r, c, where;
	float* pTwoDRealImage; //FFT연산의 실수부에 해당 
	float* pTwoDImImage; //FFT연산의 허수부에 해당 
	float* pOneDRealImage; //한행 또는 한열에 대한 FFT의 실수부에 해당 
	float* pOneDImImage;//한행 또는 한열에 대한 FFT의 허수부에 해당

	float magnitude, phase, real, imag;//직교 좌표계상의 정보를 Polar Coordinate으로 변경 
	float max, Const, BVal;

	//동적 메모리 할당
	pOneDRealImage = new float[height];
	pOneDImImage = new float[height];
	pTwoDRealImage = new float[height * width];
	pTwoDImImage = new float[height * width];

	//원 이미지로부터 FFT연산을 위한 초기값 설정
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
		{
			where = r * width + c;
			pTwoDRealImage[where] = m_InImg[r][c];
			pTwoDImImage[where] = 0;
		}
	//->방향으로 1차의 FFT 수행 
	for (r = 0; r < height; r++)
	{
		//해당하는 행의 데이터 복사 
		for (c = 0; c < width; c++)
		{
			where = r * width + c;
			pOneDRealImage[c] = pTwoDRealImage[where];
			pOneDImImage[c] = pTwoDImImage[where];
		}
		//1차의 FFT 수행 
		m_FFT1D(height, pOneDRealImage, pOneDImImage, Forward);
		//1차의 FFT 수행 결과 저장
		for (c = 0; c < width; c++)
		{
			where = r * width + c;
			pTwoDRealImage[where] = pOneDRealImage[c];
			pTwoDImImage[where] = pOneDImImage[c];
		}

	}
	//수직 방향으로 1차의 FFT 수행 
	for (c = 0; c < width; c++)
	{
		//해당하는 행의 데이터 복사 
		for (r = 0; r < height; r++)
		{
			where = r * width + c;
			pOneDRealImage[r] = pTwoDRealImage[where];
			pOneDImImage[r] = pTwoDImImage[where];
		}
		//1차의 FFT 수행 
		m_FFT1D(height, pOneDRealImage, pOneDImImage, Forward);
		//1차의 FFT 수행 결과 저장
		for (r = 0; r < width; r++)
		{
			where = r * width + c;
			pTwoDRealImage[where] = pOneDRealImage[r];
			pTwoDImImage[where] = pOneDImImage[r];
		}

	}
	//직교 좌표계상의 정보를 Polar Coordinate으로 변경 
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
		{
			where = r * width + c;
			real = pTwoDRealImage[where];
			imag = pTwoDImImage[where];
			magnitude = (float)sqrt((real * real + imag * imag));
			pTwoDRealImage[where] = magnitude;
		}

	//값의 범위를 [0~255]로 맞추기 위해 최대값 찾음 
	max = -1;
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
		{
			magnitude = pTwoDRealImage[r * width + c];
			if (magnitude > max)
				max = magnitude;
		}
	Const = (float)(255.0 / log10((1 + max)));//값의 범위를 [0~255]로 변환하기 위한 상수 

	//변환 상수값을 이용하여 변환 
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
		{
			magnitude = pTwoDRealImage[r * width + c];
			BVal = Const * log10((1 + magnitude));
			m_OutImg[r][c] = (BYTE)BVal;
		}

	//주파수의 재배치: 저주파 성분을 영상의 중앙부에 오도록 배치
	int half_r = height / 2, half_c = width / 2;
	int half_r_m1 = half_r - 1, half_c_m1 = half_c - 1;
	int rr, cc;
	for (r = 0; r < height; r += half_r)
		for (c = 0; c < width; c += half_c)
		{
			for (rr = 0; rr < half_r; rr++)
				for (cc = 0; cc < half_c; cc++)
					pTwoDRealImage[(half_r_m1 - rr + r) * width + (half_c_m1 - cc + c)] =
					m_OutImg[r + rr][c + cc];
		}
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
			m_OutImg[r][c] = (BYTE)pTwoDRealImage[r * width + c];

	//동적 메모리 할당 해제 
	delete[] pOneDRealImage;
	delete[] pOneDImImage;
	delete[] pTwoDRealImage;
	delete[] pTwoDImImage;
}
//입력 영상은 이치화된 이미지를 대상으로 함.
//입력 영상에서 0: 대상 물체 
//입력 영상에서 255: 배경 물체
#define BACKGROUND 255
#define FOREGROUND 0
void CWinTestDoc::m_MorphologyBinaryErosion(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int flagPassed;
	int r, c, i;
	int mx, my;
	float mean, sum;

	//먼저 입력 영상을 이치화함 
	//영상의 평균을 구한후 평균보다 큰 값은 0으로 
	//평균보다 작은 값은 255로 지정 
	sum = 0;
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
			sum += m_InImg[r][c];
	mean = sum / (height * width);
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
			if (m_InImg[r][c] > mean)
				m_InImg[r][c] = FOREGROUND;
			else
				m_InImg[r][c] = BACKGROUND;

	//결과 이미지를 BACKGROUND=255값으로 모두 초기화 
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
			m_OutImg[r][c] = BACKGROUND;
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
		{
			flagPassed = 1;
			for (i = 1; i < m_pSEBinary[0].row; i++)
			{
				mx = c + m_pSEBinary[i].col;
				my = r + m_pSEBinary[i].row;
				//범위 검사
				if (mx >= 0 && mx < width && my >= 0 && my < height)
					if (m_InImg[my][mx] == BACKGROUND) //하나라도 BACKGROUND=255값을 
						//포함하면 제일 안쪽 for loop를 빠져나감.
					{
						flagPassed = 0;
						break;
					}
			}
			if (flagPassed)
				m_OutImg[r][c] = FOREGROUND;
		}
}


void CWinTestDoc::m_AllocStructureElementBinary(int HowMany)
{
	// TODO: 여기에 구현 코드 추가.
	m_pSEBinary = new StructureElementBinary[HowMany];

}


void CWinTestDoc::m_FreeStructureElementBinary()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_pSEBinary != NULL)
		delete[] m_pSEBinary;
}


void CWinTestDoc::m_SetStructureElementBinary(int which, int row, int col)
{
	// TODO: 여기에 구현 코드 추가.
	m_pSEBinary[which].row = row;
	m_pSEBinary[which].col = col;
}


void CWinTestDoc::m_MorphologyBinaryDilation(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int flagPassed;
	int r, c, i;
	int mx, my;
	float mean, sum;

	//먼저 입력 영상을 이치화함 
	//영상의 평균을 구한후 평균보다 큰 값은 0으로 
	//평균보다 작은 값은 255로 지정 
	sum = 0;
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
			sum += m_InImg[r][c];
	mean = sum / (height * width);
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
			if (m_InImg[r][c] > mean)
				m_InImg[r][c] = FOREGROUND;
			else
				m_InImg[r][c] = BACKGROUND;
	//결과 이미지를 BACKGROUND=255값으로 모두 초기화 
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
			m_OutImg[r][c] = BACKGROUND;
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
		{
			flagPassed = 0;
			for (i = 1; i < m_pSEBinary[0].row; i++)
			{
				mx = c + m_pSEBinary[i].col;
				my = r + m_pSEBinary[i].row;
				//범위 검사
				if (mx >= 0 && mx < width && my >= 0 && my < height)
					if (m_InImg[my][mx] == FOREGROUND) //하나라도 FOREGROUND=0값을 
						//포함하면 제일 안쪽 for loop를 빠져나감.
					{
						flagPassed = 1;
						break;
					}
			}
			if (flagPassed)
				m_OutImg[r][c] = FOREGROUND;
		}
}


void CWinTestDoc::m_MorphologyGrayErosion(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int r, c, i;
	int mx, my;
	int min, max, diff, where;
	int* pTmpImg;
	float constVal1, constVal2, newValue;

	//동적 이미지 할당 
	pTmpImg = new int[height * width];

	//결과 이미지를 0으로 모두 초기화 
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
			pTmpImg[r * width + c] = 0;

	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
		{
			min = m_InImg[r][c];
			for (i = 1; i < m_pSEGray[0].row; i++)//pSE[0].row: Structure Element의 크기
			{
				mx = c + m_pSEGray[i].col;
				my = r + m_pSEGray[i].row;
				//범위 검사
				if (mx >= 0 && mx < width && my >= 0 && my < height) // 0 ~~ 256 안에 있는거
				{


					diff = m_InImg[my][mx] - m_pSEGray[i].grayval;
					if (diff < min)
						min = diff;
				}
			}
			pTmpImg[r * width + c] = min;
		}
	//결과 이미지를 0에서 255사이로 변환 
	//최소, 최대값을 찾은후 변환 
	min = (int)10e10;
	max = (int)-10e10;
	for (r = 0; r < height; r++)
		for (c = 0; c < width; c++)
		{
			diff = pTmpImg[r * width + c];
			if (diff < min)
				min = diff;
			if (diff > max)
				max = diff;
		}
	//변환시 상수값을 미리 계산 
	constVal1 = (float)(255.0 / (max - min));
	constVal2 = (float)(-255.0 * min / (max - min));
	for (r = 1; r < height; r++)
		for (c = 1; c < width; c++)
		{
			//[min,max]사이의 값을 [0,255]값으로 변환 
			newValue = pTmpImg[r * width + c];
			newValue = constVal1 * newValue + constVal2;
			m_OutImg[r][c] = (BYTE)newValue;
		}

	//동적 메모리 할당 해제 
	delete[] pTmpImg;
}


void CWinTestDoc::m_AllocStructureElementGray(int HowMany)
{
	// TODO: 여기에 구현 코드 추가.
	m_pSEGray = new StructureElementGray[HowMany];

}


void CWinTestDoc::m_FreeStructureElementGray()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_pSEGray != NULL)
		delete[] m_pSEGray;
}


void CWinTestDoc::m_SetStructureElementGray(int which, int row, int col, int grayval)
{
	// TODO: 여기에 구현 코드 추가.

	m_pSEGray[which].row = row;
	m_pSEGray[which].col = col;
	m_pSEGray[which].grayval = grayval;
}

void CWinTestDoc::grass_label(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	//라벨링된 영상을 저장하기 위한 배열의 메모리 할당
	short* coloring = new short[height * width];
	int f = 0; // 이거 농담삼아하는거
	int i, j, curColor = 0;
	for (i = 0; i < height * width; i++)
		coloring[i] = 0; //모든 화소를 일단 0으로 마킹(이를 미 방문점이라고 한다고 한다.)
	CString message;//농담
	//입력영상의 라벨링
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++) //이중 for문으로 전체로 다 돌리고	
		{ //물체영역이 255이고 미 방문점이라면 라벨링 시작(
			if (m_InImg[i][j] == 255 && coloring[i * width + j] == 0)
			{
				curColor++;
				f++;

				CString strMessage;
				strMessage.Format(_T("This is a message: %d\n"), f);
				TRACE(strMessage);

				if (f % 255 == 0)
				{
					strMessage.Format(_T("Congratulation: %d\n"), f);
					TRACE(strMessage);
				}

				grass(coloring, height, width, i, j, curColor);
			}
			
		}
	}

	float grayGap = 250.0f / (float)curColor;

	//라벨링된 데이터를 m_OutImg 배열을 이용해서 출력
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++) 
		{
			int value = (int)(coloring[i * width + j] * grayGap);
			if (value == 0)
				m_OutImg[i][j] = 255;
			else
				m_OutImg[i][j] = value;
		}
	}
	
	delete[]coloring;
}

int h = 0;

void CWinTestDoc::grass(short* coloring, int height, int width, int i, int j, int curColor)
{
	// TODO: 여기에 구현 코드 추가.
	int k, l, index;
	

	for (k = i-1; k <= i + 1; k++) 
	{

			for (l = j - 1; l <= j + 1; l++) //여기서 L++대신에 j++넣어서 오류뜬거였음
			{
					if (k<0 || k>height || l < 0 || l >= width)continue;

					index = k * width + l;
					if (m_InImg[k][l] == 255 && coloring[index] == 0)
					{
						coloring[index] = curColor;
						grass(coloring, height, width, k, l, curColor);
					}
			}
	}
}


int CWinTestDoc::push(short* stackx, short* stacky, int arr_size, short vx, short vy, int* top) //스택에 데이터 저장하는 함수
{
	// TODO: 여기에 구현 코드 추가.
	if (*top >= arr_size) // 이건 스택의 용량보다 더 저장되는걸 막기위한거다.
		return(-1);

	(*top)++;
	stackx[*top] = vx;
	stacky[*top] = vy;
	return(1);
}


int CWinTestDoc::pop(short* stackx, short* stacky, short* vx, short* vy, int* top) // 스택에 데이터 반환함수
{
	// TODO: 여기에 구현 코드 추가.
	if (*top == 0) // 스택에 더 이상 저장된게 없으면 -1 리턴한다.
		return(-1);

	*vx = stackx[*top]; //리턴하는 작업
	*vy = stacky[*top];
	(*top)--;
	return (1);
}


void CWinTestDoc::m_BlobColoring(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	int i, j, m, n, top, area, BlobArea[1000];
	short curColor = 0, r, c;

	//스택으로 사용할 메모리 할당
	short* stackx = new short[height * width];
	short* stacky = new short[height * width];
	int arr_size = height * width;

	//라벨링된 픽셀을 저장하기 위해 메모리 할당
	short* coloring = new short[height * width];
	for (i = 0; i < height * width; i++)
		coloring[i] = 0; //0으로 초기화

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			// 이미 방문한 점이거나 픽셀값이 255가 아니라면 처리 안함 
			if (coloring[i * width + j] != 0 || m_InImg[i][j] != 255) continue;

			r = i;
			c = j;
			top = 0; area = 1;
			curColor++;

			while (1)
			{
GRASSFIRE:
				for (m = r - 1; m <= r + 1; m++)
				{
					for (n = c - 1; n <= c + 1; n++)
					{
						// 관심 픽셀이 영상경계를 벗어나면 처리 안함  
						if (m < 0 || m >= height || n < 0 || n >= width) continue;

						if ((int)m_InImg[m][n] == 255 && coloring[m * width + n] == 0)
						{
							coloring[m * width + n] = curColor;  // 현재 라벨로 마크 
							if (push(stackx, stacky,arr_size, (short)m, (short)n, &top) == -1) continue;
							r = m;
							c = n;
							area++;
							goto GRASSFIRE;
						}
					}
				}
				if (pop(stackx, stacky, &r, &c, &top) == -1) break;
			}
			if (curColor < 1000) BlobArea[curColor] = area;
		}
	}

	float grayGap = 250.0f / (float)curColor;

	//영역의 면적을 파일로 출력한데.. 나도 뭔지 모름
	FILE* fout = fopen("blobarea.out", "w");
	if (fout != NULL)
	{
		for (i = 1; i < curColor; i++)
			fprintf(fout, "%i: %d\n", BlobArea[i]);
		fclose(fout);
	}

	CString tempStr;
	tempStr.Format(_T("\r\n\r\n[ Blob Coloring(2) ]\r\n-----------------------------------------\r\n"));
	ResultShowDlgBar(tempStr);
	for (i = 1; i <= curColor; i++)
	{
		tempStr.Format(_T("%i: %d\r\n"), i, BlobArea[i]);
		ResultShowDlgBar(tempStr);
	}


	// 라벨링된 데이터를 m_OutImg배열을 이용하여 화면출력 
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			int value = (int)(coloring[i * width + j] * grayGap);
			if (value == 0) m_OutImg[i][j] = 255;
			else m_OutImg[i][j] = value;
		}
	}

	delete[]coloring; delete[]stackx; delete[]stacky;


}


void CWinTestDoc::m_BorderFollow(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.

	// 영역의 경계정보를 저장하기 위한 구조체 메모리 
	typedef struct tagBORDERINFO { short* x, * y; short n, dn; } BORDERINFO;
	BORDERINFO stBorderInfo[1000];

	// 영상에 있는 픽셀이 방문된 점인지를 마크하기 위해 영상 메모리 할당 
	unsigned char* visited = new unsigned char[height * width];
	memset(visited, 0, height* width * sizeof(char)); // 메모리 초기화 

	// 추적점을 임시로 저장하기 위한 메모리 
	short* xchain = new short[10000];
	short* ychain = new short[10000];

	// 관심 픽셀의 시계방향으로 주위점을 나타내기 위한 좌표 설정 
	const POINT nei[8] =           // clockwise neighbours
	{
		{1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1}, {1,1}
	};

	int x0, y0, x, y, k, n;
	int numberBorder = 0, border_count, diagonal_count;
	unsigned char c0, c1;


	for (x = 1; x < height; x++)
	{
		for (y = 1; y < width; y++)
		{
			c0 = m_InImg[x][y];
			c1 = m_InImg[x - 1][y];

			if (c0 != c1 && c0 == (unsigned char)255 && visited[x * width + y] == 0)  //c0!=c1(경계이고)
			{
				border_count = 0;  // 경계점의 갯수를 세기 위한 카운터
				diagonal_count = 0;  // 대각선 방향의 경계점 개수를 세는 카운터

				x0 = x;
				y0 = y;
				n = 4;
				do
				{
					// 관심점 주위에서 같은 칼라를 가진 경계점을 찾기 위함 
					for (k = 0; k < 8; k++, n = ((n + 1) & 7))  // 01234567
					{									 // 12345670
						short  u = (short)(x + nei[n].x);
						short  v = (short)(y + nei[n].y);
						if (u < 0 || u >= height || v < 0 || v >= width) continue;
						if (m_InImg[u][v] == c0) break; // 관심점의 주위를 돌다가 같은 밝기의     
						// 경계를 만나면 다음으로 추적할 점이 된다. 
					}
					if (k == 8) break;    // isolated point occurs

					visited[x * width + y] = 255;  // 방문한 점으로 마크 
					xchain[border_count] = x;
					ychain[border_count++] = y;

					if (border_count >= 10000) break;

					x = x + nei[n].x;
					y = y + nei[n].y;

					if (n % 2 == 1) diagonal_count++;
					// 01234567 
					n = (n + 5) & 7; // 56701234
				} while (!(x == x0 && y == y0));

				if (k == 8) continue;    // isolated point occurs

				// 경계정보를 저장 
				if (border_count < 10) continue; // 너무작은 영역의 경계이면 무시한다.  

				stBorderInfo[numberBorder].x = new short[border_count];
				stBorderInfo[numberBorder].y = new short[border_count];

				for (k = 0; k < border_count; k++)
				{
					stBorderInfo[numberBorder].x[k] = xchain[k];
					stBorderInfo[numberBorder].y[k] = ychain[k];
				}
				stBorderInfo[numberBorder].n = border_count;
				stBorderInfo[numberBorder++].dn = diagonal_count;

				if (numberBorder >= 1000) break;
			}
		}
	}

	// 화면에 경계를 출력하기 위해 m_OutImg배열을 이용하자. 
	memset(m_OutImg, 255, height* width * sizeof(char));
	for (k = 0; k < numberBorder; k++)
	{
		TRACE("(%d: %d %d, %d)\r\n", k, stBorderInfo[k].n, stBorderInfo[k].dn, (int)(sqrt(2) * stBorderInfo[k].dn) + (stBorderInfo[k].n - stBorderInfo[k].dn));
		
		for (int i = 0; i < stBorderInfo[k].n; i++)
		{
			x = stBorderInfo[k].x[i];
			y = stBorderInfo[k].y[i];
			m_OutImg[x][y] = 0;
		}
	}

	for (k = 0; k < numberBorder; k++) { delete[]stBorderInfo[k].x; delete[]stBorderInfo[k].y; }
	delete[]visited; delete[]xchain; delete[]ychain;


}


void CWinTestDoc::ResultShowDlgBar(CString str)
{
	// TODO: 여기에 구현 코드 추가.

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CRect rect;
	pFrame->GetWindowRect(&rect);

	if (!pFrame->m_ResultShowBar.IsWindowVisible())
	{
		pFrame->DockControlBar(&pFrame->m_ResultShowBar);
		pFrame->m_ResultShowBar.ShowWindow(SW_SHOW);
		pFrame->FloatControlBar(&pFrame->m_ResultShowBar, CPoint(rect.right - 324, rect.bottom - 125));
	}

	CEdit* pEdit = (CEdit*)pFrame->m_ResultShowBar.GetDlgItem(IDC_RESULTSHOW);
	int nLength = pEdit->GetWindowTextLength();

	if (nLength < 10000) pEdit->SetSel(nLength, nLength);
	else pEdit->SetSel(nLength - 10000, nLength);
	pEdit->ReplaceSel(str);
	pFrame->RecalcLayout();

}


void CWinTestDoc::m_TranslationImg()
{
	int i, j;
	// TODO: 여기에 구현 코드 추가.
	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 256; j++)
		{
			m_InImg[i][j] = m_OutImg[i][j]; 
		}
	}
}


POINT CWinTestDoc::m_MatchMAD1(int height, int width, unsigned char* m_TempImg, int tHeight, int tWidth)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j, m, n, index;
	float SumD, MinCorr = 255.0f * tHeight * tWidth;
	POINT OptPos;

	for (m = 0; m < height - tHeight; m++)
	{
		for (n = 0; n < width - tWidth; n++)
		{
			SumD = 0.0f;
			for (i = 0; i < tHeight; i++)
			{
				index = i * tWidth;
				for (j = 0; j < tWidth; j++) 
					SumD += (float)fabs((int)m_InImg[m + i][n + j] - (int)m_TempImg[index + j]);
			}

			if (SumD < MinCorr)
			{
				MinCorr = SumD;
				OptPos.y = m;
				OptPos.x = n;
			}
		}
	}
	MinCorr /= (float)(tHeight * tWidth);

	return OptPos;
}


POINT CWinTestDoc::m_MatchMSE1(int height, int width, unsigned char* m_TempImg, int tHeight, int tWidth)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j, m, n, index;
	int diff;
	float SumD, MinCorr = 255.0f * 255.0f * tHeight * tWidth;
	POINT OptPos;

	for (m = 0; m < height - tHeight; m++)
	{
		for (n = 0; n < width - tWidth; n++)
		{
			SumD = 0.0f;
			for (i = 0; i < tHeight; i++)
			{
				index = i * tWidth;
				for (j = 0; j < tWidth; j++)
				{
					diff = m_InImg[m + i][n + j] - m_TempImg[index + j];
					SumD += (float)(diff * diff);
				}
			}

			if (SumD < MinCorr)
			{
				MinCorr = SumD;
				OptPos.y = m;
				OptPos.x = n;
			}
		}
	}

	MinCorr /= (float)(tHeight * tWidth);

	return OptPos;

}


POINT CWinTestDoc::m_MatchMAD(int height, int width, unsigned char* m_TempImg, int tHeight, int tWidth)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j, m, n, index;
	float SumD, MinCorr = 255.0f * tHeight * tWidth;
	POINT OptPos;


	//프로그래스 때매 넣은거다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_pProgressBar.SetRange(0, height - tHeight);
	pFrame->m_pProgressBar.SetStep(1);

	CRect rc;
	pFrame->pStatusBar->GetItemRect(0, rc);
	pFrame->m_pProgressBar.MoveWindow(&rc);















	CString tempStr;
	tempStr.Format(_T("\r\n[ MAD Matching Started ]\r\n----------------------------\r\n"));
	ResultShowDlgBar(tempStr);

	for (m = 0; m < height - tHeight; m++)
	{
		for (n = 0; n < width - tWidth; n++)
		{
			SumD = 0.0f;
			for (i = 0; i < tHeight; i++)
			{
				index = i * tWidth;

				for (j = 0; j < tWidth; j++) 
					SumD += (float)fabs((int)m_InImg[m + i][n + j] - (int)m_TempImg[index + j]);
			}

			if (SumD < MinCorr)
			{
				MinCorr = SumD;
				OptPos.y = m;
				OptPos.x = n;
			}
		}
	}
	MinCorr /= (float)(tHeight * tWidth);
	tempStr.Format(_T("Optimal Position: (%d, %d)\r\n Minimum MAD Value: %7.3f\r\n"), OptPos.y, OptPos.x, MinCorr);
	ResultShowDlgBar(tempStr);

	return OptPos;
}




POINT CWinTestDoc::m_TemplateMatch(int height, int width, unsigned char* m_TempImg, int tHeight, int tWidth)
{
	// TODO: 여기에 구현 코드 추가.
	float ST = 0.0f, temp;
	float SumT = 0.0f;
	int NoT = 0;

	register int i, j, m, n, index; //register은 그저 속도빠르게 하기위한 변수

	for (i = 0; i < tHeight; i++)
	{
		index = i * tWidth; // 그냥 2차원을 1차원 표현
		for (j = 0; j < tWidth; j++)
		{
			temp = (float)m_TempImg[index + j];
			SumT += temp; //SumT 에는 템플에 있는 밝기값 다 더한거
			ST += temp * temp; // ST는 템플에 있는 밝기값 제곱을 다 던한거
			NoT++; // Not 은 횟수마다 1더해서 0에서 256*256
		}
	}

	// ProgressBar표시를 위한 부분 
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_pProgressBar.SetRange(0, height - tHeight);
	pFrame->m_pProgressBar.SetStep(1);
	CRect rc;
	pFrame->pStatusBar->GetItemRect(0, rc);
	pFrame->m_pProgressBar.MoveWindow(&rc);


	float MaxCorr = 0.0f, CurCorr, a1, a2, DeNomi, Nomi;
	float SumI, SI, ScT, tpi;
	POINT OptPos;

	
	CString tempStr;
	tempStr.Format(_T("\r\n[ Template Matching Started ]\r\n----------------------------\r\n"));
	ResultShowDlgBar(tempStr);


	for (m = 0; m < height - tHeight; m++)
	{
		for (n = 0; n < width - tWidth; n++)
		{
			SumI = SI = ScT = 0.0f;
			for (i = 0; i < tHeight; i++)
			{
				index = i * tWidth;
				for (j = 0; j < tWidth; j++)
				{
					temp = (float)m_InImg[m + i][n + j];
					tpi = (float)m_TempImg[index + j];
					SumI += temp;
					SI += temp * temp;
					ScT += tpi * temp;
				}
			}

			a1 = NoT * SI - SumI * SumI;
			a2 = NoT * ST - SumT * SumT;
			DeNomi = (float)(a1 * a2);
			Nomi = (NoT * ScT - SumI * SumT);

			if (DeNomi < 0.0001) CurCorr = 0;
			else                  CurCorr = Nomi * Nomi / DeNomi;

			if (CurCorr > MaxCorr)
			{
				MaxCorr = CurCorr;
				OptPos.y = m;
				OptPos.x = n;
			}
		}

		pFrame->m_pProgressBar.StepIt();
	}
	tempStr.Format(_T("Optimal Position: (%d, %d)\r\n Maximum NGC Value: %7.4f\r\n"), OptPos.y, OptPos.x, sqrt(MaxCorr));
	ResultShowDlgBar(tempStr);

	return OptPos;
}


void CWinTestDoc::HT_Line(unsigned char* orgImg, unsigned char* outImg, int height, int width)
{
	// TODO: 여기에 구현 코드 추가.

	// 오리지널임 1

	/*
	register int i, j, k;
	int d, index;
	float p2d = 3.141592654f / 180.0f; //60분법( 1도, 24도 ...) 에서 라디안(3.14)같은거로 바꾸려는거임

	int H[360][362] = { 0 }; //투표하기위한 배열
	int thres = 60;

	float* LUT_COS = new float[360];
	float* LUT_SIN = new float[360];

	//룩업테이블 만드는거임 1도마다 모든 sin,cos 구해서 배열에 저장하는거임
	for (i = 0; i < 360; i++)
	{
		LUT_COS[i] = (float)cos(i * p2d);
		LUT_SIN[i] = (float)sin(i * p2d);
	}

	// our 이미지는 im이미지랑 같고 그걸 다시 out에다가 넣어서 한마디로 out=org=in 된거임
	for (i = 0; i < height * width; i++)
		outImg[i] = orgImg[i];

	// For voting
	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			if (orgImg[index + j] == 255)
			{
				for (k = 0; k < 360; k++)
				{
					d = (int)(i * LUT_COS[k] + j * LUT_SIN[k]);
					if (d >= 4 && d <= 360) H[k][d]++;
				}
			}
		}
	}

	// For display
	for (d = 4; d <= 360; d++)
	{
		for (k = 0; k < 360; k++)
		{
			if (H[k][d] > thres) //H배열에다가 히스토그램처럼 투표해놨고 그기다가 th값 (60) 이상 투표되었으면 선 그리는거임
			{
				i = j = 2;

				for (j = 2; j < height; j++) // vertical pixel
				{
					i = (int)((d - j * LUT_SIN[k]) / LUT_COS[k]);
					if (i < height && i>0) outImg[i * width + j] = 255;
				}

				for (i = 2; i < width; i++) // horizontal pixel
				{
					j = (int)((d - i * LUT_COS[k]) / LUT_SIN[k]);
					if (j < height && j>0) outImg[i * width + j] = 255;
				}
			}
		}
	}

	// 왜 x,y축을 따로 처리하는가에 관한 대답이다.
	// 
	// 정확성: 각 방정식이 적용되는 좌표 범위가 다를 수 있습니다. 한 방향으로만 계산하면 해상도가 낮은 경우 정확하지 않을 수 있습니다.
	// 경우의 수 다루기: cos(θ)나 sin(θ) 값이 0에 가까워질 때, 계산에서 분모가 0에 가까워져 불안정해질 수 있습니다. 이를 방지하기 위해 두 방향을 분리하여 처리합니다.
	// 완전성: 이미지의 모든 픽셀을 정확하게 커버하기 위해, 두 방향을 모두 고려하여 처리하는 것이 필요합니다. 이는 모든 가능한 픽셀 좌표를 포함할 수 있도록 보장합니다.
	delete[]LUT_COS;
	delete[]LUT_SIN;
	*/

	// 1- end

	// 중복되는거 뭉침(+-10) + 임계 조정함
	// 2 - start

	/*
	register int i, j, k;
	int d, index;
	float p2d = 3.141592654f / 180.0f;

	int H[360][362] = { 0 };
	int thres = 64;

	float* LUT_COS = new float[360];
	float* LUT_SIN = new float[360];

	for (i = 0; i < 360; i++)
	{
		LUT_COS[i] = (float)cos(i * p2d);
		LUT_SIN[i] = (float)sin(i * p2d);
	}

	for (i = 0; i < height * width; i++) outImg[i] = orgImg[i];

	// For voting
	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			if (orgImg[index + j] == 255)
			{
				for (k = 0; k < 360; k++)
				{
					d = (int)(i * LUT_COS[k] + j * LUT_SIN[k]);
					if (d >= 4 && d <= 360) H[k][d]++;
				}
			}
		}
	}

	// 합치기
	for (d = 4; d <= 360; d++)
	{
		for (k = 0; k < 360; k++)
		{
			if (H[k][d] > thres)
			{
				for (int d2 = d - 10; d2 <= d + 10; d2++)
				{
					for (int k2 = k - 10; k2 <= k + 10; k2++)
					{
						int k_mod = (k2 + 360) % 360;
						if (H[k_mod][d2] > thres && !(d == d2 && k == k_mod))
						{
							H[k][d] += H[k_mod][d2];
							H[k_mod][d2] = 0;
						}
					}
				}
			}
		}
	}

	// For display
	for (d = 4; d <= 360; d++)
	{
		for (k = 0; k < 360; k++)
		{
			if (H[k][d] > thres)
			{
				i = j = 2;

				for (j = 2; j < height; j++) // vertical pixel
				{
					i = (int)((d - j * LUT_SIN[k]) / LUT_COS[k]);
					if (i < height && i > 0) outImg[i * width + j] = 255;
				}

				for (i = 2; i < width; i++) // horizontal pixel
				{
					j = (int)((d - i * LUT_COS[k]) / LUT_SIN[k]);
					if (j < height && j > 0) outImg[i * width + j] = 255;
				}
			}
		}
	}

	delete[]LUT_COS;
	delete[]LUT_SIN;
	*/

	// 2-end

	// 수평에 가가운거 없앤거
	// 3-start

	/*
	register int i, j, k;
	int d, index;
	float p2d = 3.141592654f / 180.0f;

	int H[360][362] = { 0 };
	int thres = 64;

	float* LUT_COS = new float[360];
	float* LUT_SIN = new float[360];

	for (i = 0; i < 360; i++)
	{
		LUT_COS[i] = (float)cos(i * p2d);
		LUT_SIN[i] = (float)sin(i * p2d);
	}

	for (i = 0; i < height * width; i++) outImg[i] = orgImg[i];

	// For voting
	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			if (orgImg[index + j] == 255)
			{
				for (k = 0; k < 360; k++)
				{
					// 수평에 가까운 각도는 무시
					if (k <= 10 || k >= 350) continue;

					d = (int)(i * LUT_COS[k] + j * LUT_SIN[k]);
					if (d >= 4 && d <= 360) H[k][d]++;
				}
			}
		}
	}

	// 합치기
	for (d = 4; d <= 360; d++)
	{
		for (k = 0; k < 360; k++)
		{
			if (H[k][d] > thres)
			{
				for (int d2 = d - 10; d2 <= d + 10; d2++)
				{
						for (int k2 = k - 10; k2 <= k + 10; k2++)
						{
							int k_mod = k2;
							if (H[k_mod][d2] > thres && !(d == d2 && k == k_mod))
							{
								H[k][d] += H[k_mod][d2];
								H[k_mod][d2] = 0;
							}
						}
				}
			}
		}
	}

	// For display
	for (d = 4; d <= 360; d++)
	{
		for (k = 0; k < 360; k++)
		{
			if (H[k][d] > thres)
			{
				i = j = 2;

				for (j = 2; j < height; j++) // vertical pixel
				{
					i = (int)((d - j * LUT_SIN[k]) / LUT_COS[k]);
					if (i < height && i > 0) outImg[i * width + j] = 255;
				}

				for (i = 2; i < width; i++) // horizontal pixel
				{
					j = (int)((d - i * LUT_COS[k]) / LUT_SIN[k]);
					if (j < height && j > 0) outImg[i * width + j] = 255;
				}
			}
		}
	}

	delete[] LUT_COS;
	delete[] LUT_SIN;
	*/

	//3-end

	//차도에 가까운거만 살린거임

	
	register int i, j, k;
	int d, index;
	float p2d = 3.141592654f / 180.0f;
	
	int H[360][362] = { 0 };
	int thres = 64;
	
	float* LUT_COS = new float[360];
	float* LUT_SIN = new float[360];
	
	for (i = 0; i < 360; i++)
	{
		LUT_COS[i] = (float)cos(i * p2d);
		LUT_SIN[i] = (float)sin(i * p2d);
	}
	
	for (i = 0; i < height * width; i++) outImg[i] = orgImg[i];
	
	// For voting
	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			if (orgImg[index + j] == 255)
			{
				for (k = 0; k < 360; k++)
				{
					// 수평에 가까운 각도는 무시
					if (k <= 10 || k >= 350) continue;
	
					d = (int)(i * LUT_COS[k] + j * LUT_SIN[k]);
					if (d >= 4 && d <= 360) H[k][d]++;
				}
			}
		}
	}
	
	// 합치기
	for (d = 4; d <= 360; d++)
	{
		for (k = 0; k < 360; k++)
		{
			if (H[k][d] > thres)
			{
				for (int d2 = d - 10; d2 <= d + 10; d2++)
				{
					if (d2 >= 4 && d2 <= 360)
					{
						for (int k2 = k - 10; k2 <= k + 10; k2++)
						{
							if (k2 >= 0 && k2 <= 360)
							{
								if (H[k2][d2] > thres && !(d == d2 && k == k2))
								{
									H[k][d] += H[k2][d2];
									H[k2][d2] = 0;
								}
							}
						}
					}
				}
			}
		}
	}
	
	// For display
	// 우선 0~180사이
	int max_k = -1;
	int dd = -1;
	for (d = 4; d <= 360; d++)
	{
		for (k = 0; k < 180; k++)
		{
			if (H[k][d] > thres)
			{
				
				if (k > max_k)
				{
					max_k = k;
					dd = d;
				}

				
			}
		}
	}

	i = j = 2;

	for (j = 2; j < height; j++) // vertical pixel
	{
		i = (int)((dd - j * LUT_SIN[max_k]) / LUT_COS[max_k]);
		if (i < height && i > 0)
			outImg[i * width + j] = 255;
	}

	for (i = 2; i < width; i++) // horizontal pixel
	{
		j = (int)((dd - i * LUT_COS[max_k]) / LUT_SIN[max_k]);
		if (j < height && j > 0)
			outImg[i * width + j] = 255;
	}


	//다음 180~360사이
	int max_kk = 1000;
	int ddd = -1;
	for (d = 4; d <= 360; d++)
	{
		for (k = 180; k < 360; k++)
		{
			if (H[k][d] > thres)
			{

				if (k < max_kk)
				{
					max_kk = k;
					ddd = d;
				}
			}
		}
	}

	i = j = 2;

	for (j = 2; j < height; j++) // vertical pixel
	{
		i = (int)((ddd - j * LUT_SIN[max_kk]) / LUT_COS[max_kk]);
		if (i < height && i > 0)
			outImg[i * width + j] = 255;
	}

	for (i = 2; i < width; i++) // horizontal pixel
	{
		j = (int)((ddd - i * LUT_COS[max_kk]) / LUT_SIN[max_kk]);
		if (j < height && j > 0)
			outImg[i * width + j] = 255;
	}
	
	delete[] LUT_COS;
	delete[] LUT_SIN;
	
}
#include <vector>
#include <cmath>
struct Circle
{
	int i; // center y-coordinate
	int j; // center x-coordinate
	int k; // radius
};

void CWinTestDoc::HT_Circle(unsigned char* orgImg, unsigned char* outImg, int height, int width)
{
	// TODO: 여기에 구현 코드 추가.

	//오리지널 시작
	/*
	register int i, j, k, ang;
	int index, rr, cc;
	int r_min = 30, r_max = 100;
	float p2d = 3.141592654f / 180.0f;

	unsigned int thres = 90;

	// Memory allocation 3차원 배열 동적으로 지정한거임
	unsigned int*** H = new unsigned int** [height];
	for (i = 0; i < height; i++)
	{
		H[i] = new unsigned int* [width];
		for (j = 0; j < width; j++)
		{
			H[i][j] = new unsigned int[r_max];
			for (k = 0; k < r_max; k++) H[i][j][k] = 0;
		}
	}

	// cos sin동적지정
	float* LUT_COS = new float[360];
	float* LUT_SIN = new float[360];

	// sin cos 룩업테이블 지정한거임
	for (i = 0; i < 360; i++)
	{
		LUT_COS[i] = (float)cos(i * p2d);
		LUT_SIN[i] = (float)sin(i * p2d);
	}

	//우선 in이미지를 out에 넣은거임
	for (i = 0; i < height * width; i++) outImg[i] = orgImg[i];


	// For vote
	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			if (orgImg[index + j] == 255)
			{
				for (k = r_min; k < r_max; k++) //k는 반지름 길이라서 최소 반지름 을 30~100으로 지정한거임
				{
					for (ang = 0; ang < 360; ang++)
					{
						rr = (int)(i - k * LUT_COS[ang]);
						cc = (int)(j - k * LUT_SIN[ang]);

						if (rr < height && rr>0 && cc < width && cc>0) H[rr][cc][k]++; //모든 경우의수 3차원에다가 투표한거임
					}
				}
			}
		}
	}


	// For display
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			for (k = r_min; k < r_max; k++)
			{
				if (H[i][j][k] > thres) //th값(90)이상인것 들만 실제로 구현하자는것임
				{
					for (ang = 0; ang < 360; ang++)
					{
						rr = (int)(i + k * LUT_COS[ang]);
						cc = (int)(j + k * LUT_SIN[ang]);

						if (rr > 0 && rr < height && cc>0 && cc < width) outImg[rr * width + cc] = 255;
					}
				}
			}
		}
	}


	// Delete arrays 3차원 메모리 해제하는거임
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++) delete[] H[i][j];
		delete[] H[i];
	}
	delete[]H;

	delete[]LUT_COS;
	delete[]LUT_SIN;
	*/
	// 오리지널 끝

	// 2-start
	// 중복 합치는거임

	/*
	register int i, j, k, ang;
	int index, rr, cc;
	int r_min = 30, r_max = 100;
	float p2d = 3.141592654f / 180.0f;

	unsigned int thres = 90;

	// Memory allocation 3차원 배열 동적으로 지정한거임
	unsigned int*** H = new unsigned int** [height];
	for (i = 0; i < height; i++)
	{
		H[i] = new unsigned int* [width];
		for (j = 0; j < width; j++)
		{
			H[i][j] = new unsigned int[r_max];
			for (k = 0; k < r_max; k++) H[i][j][k] = 0;
		}
	}

	// cos sin동적지정
	float* LUT_COS = new float[360];
	float* LUT_SIN = new float[360];

	// sin cos 룩업테이블 지정한거임
	for (i = 0; i < 360; i++)
	{
		LUT_COS[i] = (float)cos(i * p2d);
		LUT_SIN[i] = (float)sin(i * p2d);
	}

	//우선 in이미지를 out에 넣은거임
	for (i = 0; i < height * width; i++) outImg[i] = orgImg[i];


	// For vote
	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			if (orgImg[index + j] == 255)
			{
				for (k = r_min; k < r_max; k++) //k는 반지름 길이라서 최소 반지름 을 30~100으로 지정한거임
				{
					for (ang = 0; ang < 360; ang++)
					{
						rr = (int)(i - k * LUT_COS[ang]);
						cc = (int)(j - k * LUT_SIN[ang]);

						if (rr < height && rr>0 && cc < width && cc>0) H[rr][cc][k]++; //모든 경우의수 3차원에다가 투표한거임
					}
				}
			}
		}
	}

	//합치기
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			for (k = r_min; k < r_max; k++)
			{
				if (H[i][j][k] > thres) //th값(90)이상인것 들만 실제로 구현하자는것임
				{
					for (int i2 = i - 10; i2 < i + 10; i2++)
					{
						if (i2 >= 0 && i2 <= height)
						{
							for (int j2 = j - 10; j2 < j + 10; j2++)
							{
								if (j2 >= 0 && j2 <= width)
								{
									for (int k2 = k - 10; k2 < k + 10; k2++)
									{
										if (k2 >= r_min && k2 <= r_max)
										{
											if (H[i][j][k] > thres && !(i == i2 && j == j2 && k == k2))
											{
												H[i][j][k] += H[i2][j2][k2];
												H[i2][j2][k2] = 0;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	// 원 안에 또다른 원 있으면 제거
	//for (i = 0; i < height; i++)
	//{
	//	for (j = 0; j < width; j++)
	//	{
	//		for (k = r_min; k < r_max; k++)
	//		{
	//			if (H[i][j][k] > thres) //th값(90)이상인것 들만 실제로 구현하자는것임
	//			{
	//				for (ang = 0; ang < 360; ang++)
	//				{
	//					rr = (int)(i + k * LUT_COS[ang]);
	//					cc = (int)(j + k * LUT_SIN[ang]);

	//					
	//					
	//				}
	//			}
	//		}
	//	}
	//}

	

	

	// For display
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			for (k = r_min; k < r_max; k++)
			{
				if (H[i][j][k] > thres) //th값(90)이상인것 들만 실제로 구현하자는것임
				{
					for (ang = 0; ang < 360; ang++)
					{
						rr = (int)(i + k * LUT_COS[ang]);
						cc = (int)(j + k * LUT_SIN[ang]);

						if (rr > 0 && rr < height && cc>0 && cc < width) 
							outImg[rr * width + cc] = 255;
					}
				}
			}
		}
	}


	// Delete arrays 3차원 메모리 해제하는거임
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++) delete[] H[i][j];
		delete[] H[i];
	}
	delete[]H;

	delete[]LUT_COS;
	delete[]LUT_SIN;
	*/
	// 2-end


	register int i, j, k, ang;
    int index, rr, cc;
    int r_min = 30, r_max = 100;
    float p2d = 3.141592654f / 180.0f;

    unsigned int thres = 90;

    // Memory allocation 3차원 배열 동적으로 지정한거임
    unsigned int*** H = new unsigned int** [height];
    for (i = 0; i < height; i++)
    {
        H[i] = new unsigned int* [width];
        for (j = 0; j < width; j++)
        {
            H[i][j] = new unsigned int[r_max];
            for (k = 0; k < r_max; k++) H[i][j][k] = 0;
        }
    }

    // cos sin동적지정
    float* LUT_COS = new float[360];
    float* LUT_SIN = new float[360];

    // sin cos 룩업테이블 지정한거임
    for (i = 0; i < 360; i++)
    {
        LUT_COS[i] = (float)cos(i * p2d);
        LUT_SIN[i] = (float)sin(i * p2d);
    }

    //우선 in이미지를 out에 넣은거임
    for (i = 0; i < height * width; i++) outImg[i] = orgImg[i];

    // For vote
    for (i = 0; i < height; i++)
    {
        index = i * width;
        for (j = 0; j < width; j++)
        {
            if (orgImg[index + j] == 255)
            {
                for (k = r_min; k < r_max; k++) //k는 반지름 길이라서 최소 반지름 을 30~100으로 지정한거임
                {
                    for (ang = 0; ang < 360; ang++)
                    {
                        rr = (int)(i - k * LUT_COS[ang]);
                        cc = (int)(j - k * LUT_SIN[ang]);

                        if (rr < height && rr>0 && cc < width && cc>0) H[rr][cc][k]++; //모든 경우의수 3차원에다가 투표한거임
                    }
                }
            }
        }
    }

    // Circle storage vector
    std::vector<Circle> circles; //벡터 선언

    //합치기
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            for (k = r_min; k < r_max; k++)
            {
                if (H[i][j][k] > thres) //th값(90)이상인것 들만 실제로 구현하자는것임
                {
                    for (int i2 = i - 10; i2 < i + 10; i2++)
                    {
                        if (i2 >= 0 && i2 <= height)
                        {
                            for (int j2 = j - 10; j2 < j + 10; j2++)
                            {
                                if (j2 >= 0 && j2 <= width)
                                {
                                    for (int k2 = k - 10; k2 < k + 10; k2++)
                                    {
                                        if (k2 >= r_min && k2 <= r_max)
                                        {
                                            if (H[i][j][k] > thres && !(i == i2 && j == j2 && k == k2))
                                            {
                                                H[i][j][k] += H[i2][j2][k2];
                                                H[i2][j2][k2] = 0;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // 임계점 이상의 원을 저장해라
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            for (k = r_min; k < r_max; k++)
            {
                if (H[i][j][k] > thres) //th값(90)이상인것 들만 실제로 구현하자는것임
                {
                    Circle circle = { i, j, k };
                    circles.push_back(circle); //만들어진 객체를 저장하는 함수
                }
            }
        }
    }

    // 큰원안에 작은원 있으면 제거할거임
    for (size_t m = 0; m < circles.size(); m++) //m은 비교의 기준이 되는 원이고 , n은 비교대상인 원
    {
        for (size_t n = 0; n < circles.size(); n++)
        {
            if (m != n)// 자기자신과 비교하지 않도록
            {
				// 두 원의 중심 사이의 거리의 제곱
                int dist_sq = (circles[m].i - circles[n].i) * (circles[m].i - circles[n].i) + 
                              (circles[m].j - circles[n].j) * (circles[m].j - circles[n].j);
				// 작은 원이 큰 원 안에 있는지 확인
                if (dist_sq < circles[m].k * circles[m].k && circles[m].k > circles[n].k)
                {
					// 작은 원을 벡터에서 제거
                    circles.erase(circles.begin() + n);
                    n--;// 벡터에서 원소를 제거했으므로 인덱스를 하나 줄임
                }
            }
        }
    }

    // For display
    for (const auto& circle : circles)
    {
        for (ang = 0; ang < 360; ang++)
        {
            rr = (int)(circle.i + circle.k * LUT_COS[ang]);
            cc = (int)(circle.j + circle.k * LUT_SIN[ang]);

            if (rr > 0 && rr < height && cc > 0 && cc < width) 
                outImg[rr * width + cc] = 255;
        }
    }

    // Delete arrays 3차원 메모리 해제하는거임
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++) delete[] H[i][j];
        delete[] H[i];
    }
    delete[] H;

    delete[] LUT_COS;
    delete[] LUT_SIN;
	
}


void CWinTestDoc::Reverse(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			m_OutImg[i][j] = 255 - m_InImg[i][j];
		}
	}
	UpdateAllViews(NULL);
}


void CWinTestDoc::Temper(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			m_OutImg1[i][j] =  m_InImg[i][j];
		}
	}
	UpdateAllViews(NULL);
}



//void CWinTestDoc::OnBmp()
//{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//}


	

//void CWinTestDoc::OnBmpp()
//{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
//	CFileDialog openingdlg1(TRUE);
//	CFile file;
//
//	
//}




BOOL CWinTestDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	

	return TRUE;
}


void CWinTestDoc::HT_Line123(unsigned char* orgImg, unsigned char* outImg, int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	register int i, j, k, ang;
	int index, rr, cc;
	int r_min = 30, r_max = 100;
	float p2d = 3.141592654f / 180.0f;

	unsigned int thres = 90;

	// Memory allocation 3차원 배열 동적으로 지정한거임
	unsigned int*** H = new unsigned int** [height];
	for (i = 0; i < height; i++)
	{
		H[i] = new unsigned int* [width];
		for (j = 0; j < width; j++)
		{
			H[i][j] = new unsigned int[r_max];
			for (k = 0; k < r_max; k++) H[i][j][k] = 0;
		}
	}

	// cos sin동적지정
	float* LUT_COS = new float[360];
	float* LUT_SIN = new float[360];

	// sin cos 룩업테이블 지정한거임
	for (i = 0; i < 360; i++)
	{
		LUT_COS[i] = (float)cos(i * p2d);
		LUT_SIN[i] = (float)sin(i * p2d);
	}

	//우선 in이미지를 out에 넣은거임
	for (i = 0; i < height * width; i++) outImg[i] = orgImg[i];


	// For vote
	for (i = 0; i < height; i++)
	{
		index = i * width;
		for (j = 0; j < width; j++)
		{
			if (orgImg[index + j] == 255)
			{
				for (k = r_min; k < r_max; k++) //k는 반지름 길이라서 최소 반지름 을 30~100으로 지정한거임
				{
					for (ang = 0; ang < 360; ang++)
					{
						rr = (int)(i - k * LUT_COS[ang]);
						cc = (int)(j - k * LUT_SIN[ang]);

						if (rr < height && rr>0 && cc < width && cc>0) H[rr][cc][k]++; //모든 경우의수 3차원에다가 투표한거임
					}
				}
			}
		}
	}

	//합치기
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			for (k = r_min; k < r_max; k++)
			{
				if (H[i][j][k] > thres) //th값(90)이상인것 들만 실제로 구현하자는것임
				{
					for (int i2 = i - 10; i2 < i + 10; i2++)
					{
						if (i2 >= 0 && i2 <= height)
						{
							for (int j2 = j - 10; j2 < j + 10; j2++)
							{
								if (j2 >= 0 && j2 <= width)
								{
									for (int k2 = k - 10; k2 < k + 10; k2++)
									{
										if (k2 >= r_min && k2 <= r_max)
										{
											if (H[i][j][k] > thres && !(i == i2 && j == j2 && k == k2))
											{
												H[i][j][k] += H[i2][j2][k2];
												H[i2][j2][k2] = 0;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	// 원 안에 또다른 원 있으면 제거
	//for (i = 0; i < height; i++)
	//{
	//	for (j = 0; j < width; j++)
	//	{
	//		for (k = r_min; k < r_max; k++)
	//		{
	//			if (H[i][j][k] > thres) //th값(90)이상인것 들만 실제로 구현하자는것임
	//			{
	//				for (ang = 0; ang < 360; ang++)
	//				{
	//					rr = (int)(i + k * LUT_COS[ang]);
	//					cc = (int)(j + k * LUT_SIN[ang]);

	//					
	//					
	//				}
	//			}
	//		}
	//	}
	//}





	// For display
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			for (k = r_min; k < r_max; k++)
			{
				if (H[i][j][k] > thres) //th값(90)이상인것 들만 실제로 구현하자는것임
				{
					for (ang = 0; ang < 360; ang++)
					{
						rr = (int)(i + k * LUT_COS[ang]);
						cc = (int)(j + k * LUT_SIN[ang]);

						if (rr > 0 && rr < height && cc>0 && cc < width)
							outImg[rr * width + cc] = 255;
					}
				}
			}
		}
	}


	// Delete arrays 3차원 메모리 해제하는거임
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++) delete[] H[i][j];
		delete[] H[i];
	}
	delete[]H;

	delete[]LUT_COS;
	delete[]LUT_SIN;
}


void CWinTestDoc::Temper123(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			m_OutImg1[i][j] = m_OutImg[i][j];
		}
	}
	UpdateAllViews(NULL);
}
