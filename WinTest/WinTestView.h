
// WinTestView.h: CWinTestView 클래스의 인터페이스
//

#pragma once


class CWinTestView : public CView
{
protected: // serialization에서만 만들어집니다.
	CWinTestView() noexcept;
	DECLARE_DYNCREATE(CWinTestView)

// 특성입니다.
public:
	CWinTestDoc* GetDocument() const;

// 작업입니다.
public:
	unsigned char m_RevImg[256][256];
	int width;
	int height;
	BITMAPINFO* BmInfo;

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CWinTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBinDynamic();
	afx_msg void OnHistoEqual();
	afx_msg void OnHistoStretch();
	afx_msg void OnHistoSpecFilein();
	afx_msg void OnHistoSpecCal();
	afx_msg void OnAutoLb();
	afx_msg void OnSmoothingBox();
	afx_msg void OnSharpeningLaplacian();
	afx_msg void OnEdgePrewitt();
	afx_msg void OnZoomout();
	afx_msg void OnZoomin();
	afx_msg void OnRotation();
	afx_msg void On2dFft();
	afx_msg void OnMorphologyBin();
	afx_msg void OnMorphologyBina();
	void OnMorphologyGrayErosion();
	afx_msg void OnGrayE();
	afx_msg void OnBinLabelling();
	afx_msg void OnMyLabeling();
	afx_msg void OnBorderFollow();
	afx_msg void OnTranslation();
	BOOL m_flagMouse;
	BITMAPINFO* BufBmInfo;
	CRect m_RectTrack;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void m_DibDraw(CDC* pDC, int px, int py, int height, int width, BYTE* BufImg);
	CRect m_MatchPos;
	afx_msg void OnBlockmatchMad();
	afx_msg void OnNgc();
	afx_msg void OnLine();
	afx_msg void OnCircle();
	afx_msg void OnReverse();
	afx_msg void OnTemper();
	afx_msg void OnTempor();
	afx_msg void OnRe2tem();
};

#ifndef _DEBUG  // WinTestView.cpp의 디버그 버전
inline CWinTestDoc* CWinTestView::GetDocument() const
   { return reinterpret_cast<CWinTestDoc*>(m_pDocument); }
#endif

