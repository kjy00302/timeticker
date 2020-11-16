# 전광판 시계

아두이노와 MAX7219 기반 매트릭스를 사용해서 전광판 시계를 만든다.

### 작업 목록
  1. [x] 참조 구현체 만들기 (python_poc/scroll.py)
  2. [ ] 각 역할 부분 만들기
     - [ ] 디스플레이 동작
       - [x] 기초 동작
       - [ ] 스크롤 동작
       - [ ] 주변 환경에 따라 밝기 자동 제어
     - [ ] 시간 동기화
       - [ ] GPS에서 시간 가져오기
       - [ ] RTC에 시간 읽고 쓰기
       - [ ] 1초마다 동작하는 함수 만들기
     - [ ] 온/습도 측정
     - [ ] 폰트 읽기/쓰기
       - [ ] 영문 폰트
       - [ ] 한글 폰트
       - [ ] 사용자 지정 폰트
  3. [ ] 최종 스케치 작성 
  4. [ ] 제어 클라이언트 작성
