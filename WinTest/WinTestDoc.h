
// WinTestDoc.h: CWinTestDoc 클래스의 인터페이스
//


#pragma once
#include "MainFrm.h"


class CWinTestDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CWinTestDoc() noexcept;
	DECLARE_DYNCREATE(CWinTestDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CWinTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	unsigned char m_InImg[256][256];
	unsigned char m_InImg1[256][256];
	unsigned char m_InImg2[256][256];
	unsigned char m_OutImg[256][256];
	unsigned char m_OutImg1[256][256];
	void m_BinThres(int height, int width, int binThres); 
	void m_HistoEqual(int height, int width);
	void m_HIstoStretch(int height, int width);
	void m_HistoSpec_FileIn();
	void m_HistoSpec(int height, int width);
	void OnBinarOtzu();
	void Otzu_Threshold(unsigned char*orgImg,unsigned char*outImg,int height, int width);
	void m_SmoothingBox(int height, int width);
	void m_SharpeningLaplacian(int height, int width);
	void m_EdgePreWitt(int height, int width);
	void m_ZoomOut(int height, int width, float zoomoutfactor);
	void m_ZoomIn(int height, int width, float zoominfactor);
	void m_Rotation(int height, int width, int center_r, int center_c, float rotationAngle);
	void m_FFT1D(int NumData, float* pOneDRealImage, float* pOneDImImage, int Forward);
	void m_FFT2D(int height, int width, int Forward);
	void m_MorphologyBinaryErosion(int height, int width);

	typedef struct {
		int row, col; //좌표값 
	} StructureElementBinary, * pStructureElementBinary;
	pStructureElementBinary m_pSEBinary;

	void m_AllocStructureElementBinary(int HowMany);
	void m_FreeStructureElementBinary();
	void m_SetStructureElementBinary(int which, int row, int col);
	void m_MorphologyBinaryDilation(int height, int width);
	void m_MorphologyGrayErosion(int height, int width);
	void m_AllocStructureElementGray(int HowMany);

	typedef struct {
		int row, col;//좌표값 
		int grayval;//그레이 구조 요소의 값
	} StructureElementGray, * pStructureElementGray;
	pStructureElementGray m_pSEGray;
	void m_FreeStructureElementGray();
	void m_SetStructureElementGray(int which, int row, int col, int grayval);
	void grass_label(int height, int width);
	void grass(short* coloring, int height, int width, int i, int j, int curColor);
	int push(short* stackx, short* stacky, int arr_size, short vx, short vy, int* top);
	int pop(short* stackx, short* stacky, short* vx, short* vy, int* top);
	void m_BlobColoring(int height, int width);
	void m_BorderFollow(int height, int width);
	void ResultShowDlgBar(CString str);
	void m_TranslationImg();
	POINT m_MatchMAD1(int height, int width, unsigned char* m_TempImg, int tHeight, int tWidth);
	POINT m_MatchMSE1(int height, int width, unsigned char* m_TempImg, int tHeight, int tWidth);
//	unsigned char* m_TempImg;
//	BOOL m_flagTemplate;
//	int tHeight;
//	int tWidth;
	POINT m_MatchMAD(int height, int width, unsigned char* m_TempImg, int tHeight, int tWidth);
	POINT m_TemplateMatch(int height, int width, unsigned char* m_TempImg, int tHeight, int tWidth);
	void HT_Line(unsigned char* orgImg, unsigned char* outImg, int height, int width);
	void HT_Circle(unsigned char* orgImg, unsigned char* outImg, int height, int width);
	void Reverse(int height, int width);
	void Temper(int height, int width);

//	afx_msg void OnBmp();
//	afx_msg void OnBmpp();

	// 컬러영상부터터 여기임----------------------

	BITMAPFILEHEADER dibHf;
	BITMAPINFOHEADER dibHi;
	RGBQUAD palRGB[256];

	unsigned char* m_InImgg;
	unsigned char* m_OutImgg;
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	int height;
	int width;
	void HT_Line123(unsigned char* orgImg, unsigned char* outImg, int height, int width);
	void Temper123(int height, int width);
};
