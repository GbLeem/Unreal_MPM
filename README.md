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
    * MPM pipeline
        * BeginPlay
           * Initialize
        * Tick
          * Clear Grid
          * P2G
          * Update Grid
          * G2P
    * Unreal C++
        * Instanced Static Mesh Component를 이용하여 particle의 정보 update (https://docs.unrealengine.com/5.1/enUS/API/Runtime/Engine/Components/UInstancedStaticMeshComponent)
        * Chaos System의 PMatrix 타입 사용하여 matrix 연산 수행
(https://docs.unrealengine.com/5.2/en-US/API/Runtime/ChaosCore/Chaos/PMatrix_FReal_3_3/PMatrix/)
## 실행 방법
* branch를 3d branch로 바꾼 후 다운로드
* 프로젝트를 열고 Content\MY\Final 폴더 안에 존재하는 7가지의 블루프린트를 map에 배치 후 실행
* _Test : no deformation 상황에서의 시뮬레이션
* _NeoHookean : hyper elastic 상황에서의 시뮬레이션
* _Fluid : fluid 시뮬레이션
* _Interaction : 두개의 물체가 상호작용하는 상황에서의 시뮬레이션
## 프로젝트 결과
* 구현된 시뮬레이션은 2D,3D 상황에서의 no deformation gradient, hyper elatsic gradient, fluid
* 구현 모습 일부
   * 2D No Deformation Gradient <br/> ![no_deformation_AdobeExpress (1)](https://github.com/GbLeem/Unreal_MPM/assets/86725870/24c84ebd-0da9-4b06-83f4-9ee52a7a160a)
   * 2D Fluid <br/> ![2d_flui_AdobeExpress](https://github.com/GbLeem/Unreal_MPM/assets/86725870/5fc8f5c3-f24f-4682-ad94-7ed5eb45453a)
   * 3D Hyper Elastic Gradient <br/> ![3d_ela_AdobeExpress](https://github.com/GbLeem/Unreal_MPM/assets/86725870/43e81ce6-f014-4e4a-bc66-32ad3a656172)
   * 3D Interaction <br/> ![3d_ela_int_AdobeExpress](https://github.com/GbLeem/Unreal_MPM/assets/86725870/2d0fdc33-a8a7-4f04-ba89-c6bf2bcb22ab)
## 참고 자료
* MPM Course : The Material Point Method for Simulating Continuum Materials (SIGGRAPH 2016)
* MPM Snow : A material point method for snow simulation (SIGGRAPH 2013)
* MLS-MPM : A Moving Least Squares Material Point Method with Displacement Discontinuity and Two-Way Rigid Body Coupling (SIGGRAPH 2018)
* MPM in Unity : https://nialltl.neocities.org/articles/mpm_guide
