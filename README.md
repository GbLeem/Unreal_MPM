# Unreal_MPM
Material Point Method in Unreal Engine 5
## 2023-1  Software Capstone Design
## 개요
* 2023-1 소프트웨어융합캡스톤디자인
* Unreal Engine 5에 MPM을 이용한 물리 시뮬레이션 구현
* 대표 참고 논문 : A Moving Least Squares Material Point Method with Displacement Discontinuity and Two-Way Rigid Body Coupling (SIGGRAPH 2018)
* 결과 영상
https://www.youtube.com/watch?v=Ec28TGyPb-g
## 프로젝트 설명
* 개발 툴 및 사용 언어
    * Unreal Engine 5
    * Visual Studio (C++)
* 개발 방법
    * MPM
        * 
    * Unreal C++
        * Instanced Static Mesh Component를 이용하여 particle의 정보 update
        * Chaos System의 PMatrix 타입 사용하여 matrix 연산 수행
## 실행 방법
* branch를 3d branch로 바꾼 후 다운로드
* 프로젝트를 열고 Content\MY\Final 폴더 안에 존재하는 6가지의 블루프린트를 map에 배치 후 실행
## 프로젝트 결과
* 구현된 시뮬레이션은 2D,3D 상황에서의 no deforamtionn gradient, hyper elatsic gradient, fluid
* No Deformation Gradient
    * 2D
    * 3D
* Hyper Elastic Gradient
    * 2D
    * 3D
* Fluid
    * 2D
    * 3D
## 참고 자료
* MPM Course : The Material Point Method for Simulating Continuum Materials (SIGGRAPH 2016)
* MPM Snow : A material point method for snow simulation (SIGGRAPH 2013)
* MLS-MPM : A Moving Least Squares Material Point Method with Displacement Discontinuity and Two-Way Rigid Body Coupling (SIGGRAPH 2018)
* MPM in Unity : https://nialltl.neocities.org/articles/mpm_guide
