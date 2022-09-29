0. 실행환경 
설치목록 : mysql, mysql-connector-odbc, mysql-workbench-community (all x64 version)
windows 10, visual studio 2019
그 외 환경설정은 공지사항의 프로젝트 환경 설정 관련 REFERENCE를 따름.

1. 실행방법
압축파일의 압축을 푼 뒤, 폴더 내의 Project1.sln 실행
이후 Debug, x64 환경에서 로컬 Windows 디버거를 사용해 실행
(프로젝트 공지사항의 파일구조와 달리 학번.c, 학번.txt는 Project1 폴더 내부에 위치함)

2. QUERY
실행 후 숫자 1~7을 입력해 해당하는 TYPE의 query를 실행
SELECT QUERY TYPES 메뉴에서 0을 입력시 프로그램이 종료됨.
SUBQUERY 메뉴에서 0을 입력시 SELECT QUERY TYPES 메뉴로 이동함.
* QUERY TYPE 1의 경우, tracking number가 7014인 shipment만 stopped by accident 상태이므로, 7014를 입력해야만 정상적으로 작동. 다른수를 입력할경우 올바른 track num을 입력하라고 메시지를 출력하도록 구현함. *
* 0 입력을 통해 종료해야만 DROP TABLE이 정상적으로 일어나므로 테스트 종료시 반드시 0 입력으로 프로그램을 종료할것. *

3. 각종오류
(1) libmysql.dll 오류 해결
C:\Program Files\MySQL\MySQL Server 8.0\lib 폴더에서 libmysql.dll 파일을 복사
C:\Windows\System32 및 프로젝트 폴더에 붙여넣기

(2) could not acquire management access for administration 오류 해결
MySQL Workbench 설치된 최신버전 삭제 후 archive에서 8.0.22 version으로 다운그레이드

20171645 박찬우