# Harmagedon

> **Unreal Engine 5, C++**, BP, Jira

### **개발 인원** 

- (5명) 클라이언트 2명,  기획 2명, 그래픽 1명

### **개발 기간**

- 팀 개발 : 2024.01.03 ~ 2024.06.16
- 기능 업데이트 : 2024.06.16 ~ 진행중

>  💡 스팀 게임 출시를 목표로 **소울라이크 액션 게임**을 좋아하는 사람을 모아 시작한 프로젝트입니다.
개인 프로젝트인 [Unreal Action RPG](https://www.notion.so/Unreal-Action-RPG-c93cb31cfee14571a270b5ead9887d5f?pvs=21)  프로젝트를 기반으로 시작해서 좀 더 기능을 추가하고 다듬었습니다.
개발 기간은 최대 6개월로 잡았으며(초반 튜토리얼 보스까지), 개발 초기에는 다뤄보고 싶었던 기술이나 기능을 개발하고 실험하는데 시간을 쏟았습니다.
현재 **스팀에 데모버전을 출시**했으며, 조만간 **정식 버전도 출시할 예정**입니다.

### 구현 내용

---

- 추가된 기능
  - 플레이어
    - **몬스터 타게팅(락온)**
    - **패링**
    - **가드**
    - 구르기
  - 장비
    - **인벤토리 컴포넌트**
    - 검, 방패
    - [**방어구 (모듈식 캐릭터)**](https://www.notion.so/585c3898df7146efa24656cc8616aae9?pvs=21)
  - 근접 몬스터
    - Behavior Tree

### **개발 문서 (개발 일지)**

- 기능별 개발 과정을 보고 싶으면 [**여기**](https://www.notion.so/b67b5069afb9481ea05498e60245b358?pvs=21)를 눌러주세요

### **Steam 상점페이지**

- 정식 상점페이지 (플레이 불가능)


  [![Harmagedon](https://github.com/user-attachments/assets/24c29b52-4bd8-4cbf-976e-93aac210210c)](https://store.steampowered.com/app/3161990/Harmagedon/)

- 데모 (플레이 가능)


  [![Harmagedon Demo](https://github.com/user-attachments/assets/052af74e-72c4-43c1-95f7-cfad98d3557c)](https://store.steampowered.com/app/3166790/Harmagedon_Demo/)



### **Youtube**
[![Youtube Link](http://img.youtube.com/vi/xjUVG3ykJtA/0.jpg)](https://www.youtube.com/watch?v=xjUVG3ykJtA)

### 사진

---
![image](https://github.com/user-attachments/assets/c693ed70-13de-4cb1-81b3-ec6b8df41479)

![GIF_2024-08-29_EC98A4ED9B84_5-01-08](https://github.com/user-attachments/assets/be25d44c-249d-4075-a08f-3a8a09ee9239)
![GIF_2024-08-29_EC98A4ED9B84_5-18-01](https://github.com/user-attachments/assets/5445d837-cad2-4846-b5c6-a8597ea95717)
![GIF_2024-08-29_EC98A4ED9B84_5-15-03](https://github.com/user-attachments/assets/507b8925-560c-40b0-a8fd-91a5a62d1690)
![GIF_2024-08-29_EC98A4ED9B84_5-11-19](https://github.com/user-attachments/assets/f53b317b-06f7-4e25-aece-e134cb38a9ef)


### 개발 중단된 기능

---

[**Cel Shading**](https://www.notion.so/f6cb567c89c940ca9189e5e1f3e6efd9?pvs=21)

- **카툰 렌더링**을 표현하는데 기본적인 것들을 몇가지 구현해보고, 테스트 완료했지만, 
  만드려는 게임과 어울리지 않다고 판단되어 사용하지 않게 되었습니다.

![Untitled](https://github.com/user-attachments/assets/60d77b2f-705b-4a1e-8dbb-8d59dffb931d)
![Untitled 1](https://github.com/user-attachments/assets/266e1187-c51d-4822-84fb-dba9efd2b394)
![Untitled 2](https://github.com/user-attachments/assets/f484cec8-e375-43c0-a9c3-1b19fa69fd2d)


[**캐릭터 커스터마이징**](https://www.notion.so/24c1866e001c4397b76ac2628ee3d392?pvs=21) & [**메타휴먼**](https://www.notion.so/b1a2780448f943f0b9de8c5b7b11e327?pvs=21)

![GIF_2023-12-26_EC98A4ECA084_6-08-04](https://github.com/user-attachments/assets/72944e88-1e22-4657-8afe-978a26e50dfd)
![GIF_2023-12-19_EC98A4ED9B84_10-22-25](https://github.com/user-attachments/assets/841d1b32-b9c3-4de6-b3cd-7a93dcd0f8a9)
![GIF_2023-12-14_EC98A4ED9B84_4-09-36](https://github.com/user-attachments/assets/d4ccd9e2-4898-42fa-8510-4fb463210547)


- 간단하게 기능을 구현해 보았지만, 해당 기능을 구현하려면 **모프 타겟이 포함된 모델이 필요**한데, 그래픽 팀원 확인 결과 모프 타겟을 넣으려면 얼굴에 수 많은 뼈가 필요하나 기간적으로도 상당히 오랜 시간이 걸리고, 그래픽 팀원이 한 명일 뿐더러 그 정도면 **리깅을 좀 더 전문적으로 다뤄본 경험이 필요**하다고 생각했습니다.
- 모프 타겟을 직접 만들지 않는 방법으로 **메타휴먼을 이용**하는 방법이 있는데, 그래픽 팀원이 **블렌더에서 메타휴먼을 다루는 것이 쉽지 않아서** 결국 기능 개발을 중단하게 되었습니다.
