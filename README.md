# 🖼️ MFC 영상처리 툴 (Custom Image Processing Tool using MFC)

직접 구현한 영상처리 알고리즘을 기반으로 MFC 환경에서 작동하는 영상처리 툴입니다.  
기성 라이브러리 없이 필터 및 도형 검출 등의 알고리즘을 직접 코딩하여 구현하였으며,  
회로 이미지와 같은 특수 환경에서의 불량 검출 및 후처리에 활용 가능한 기능들을 포함하고 있습니다.

---

## 🔧 주요 기능

- ✅ **직접 구현한 영상처리 알고리즘**
  - 엣지 검출, 이진화, 스무딩 등 핵심 기법 직접 구현
- 🔍 **직선 및 원 검출 + 후처리를 통한 간소화**
  - 회로와 같은 구조에서 불필요한 요소 제거
- ⚠️ **불량 회로 이미지 전처리**
  - 회로 이미지 내 불량 유무 판단을 위한 전처리 기능

---

## 📷 주요 화면

### 1. 원본 이미지
![원본 이미지](https://github.com/user-attachments/assets/deb82e0c-77b5-4608-9363-35f46d98ee3b)

### 2. 직선 / 원 추출 후처리를 통한 간소화
![후처리 이미지1](https://github.com/user-attachments/assets/d24d0279-ea83-4f8e-abc4-f3f46cf3d6c7)
![후처리 이미지2](https://github.com/user-attachments/assets/cbe3c2e9-6a9c-4de2-8311-b10281d60d03)

### 3. 회로 이미지의 불량 검출을 위한 전처리 결과
![불량 검출](https://github.com/user-attachments/assets/275d7223-18a1-42aa-876b-644c111f7de9)

---

## 🛠️ 개발 환경

- **개발 언어**: C++
- **프레임워크**: MFC (Microsoft Foundation Class)
- **기능 구현**: OpenCV 없이 직접 알고리즘 구현

---

## 📌 프로젝트 목적

이 프로젝트는 영상처리의 핵심 알고리즘을 직접 구현하고, MFC 기반의 GUI 툴로 응용하는 것을 목표로 한다.  
특히, 산업현장의 회로 이미지와 같은 실제 데이터에 적용 가능한 **불량 검출을 위한 전처리/후처리 기술**을  
심플한 도구로 시각화할 수 있도록 설계되었다.

---

## 📬 문의

- **Author**: JiWoo Jung  
- **Email**: jju0531@naver.com  


