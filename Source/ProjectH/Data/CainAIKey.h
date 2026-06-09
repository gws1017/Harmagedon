#pragma once

#define BBKEY_ROOMTARGET TEXT("TargetInRoom")
#define BBKEY_HOMEPOS TEXT("HomePos")
#define BBKEY_DISTANCE TEXT("Distance")

#define BBKEY_ATTACKPROB TEXT("RandomNumber")
#define BBKEY_SKILLPROB TEXT("SkillProb")
#define BBKEY_DASHPROB TEXT("DashProb")

#define BBKEY_RANDOMNUMBER TEXT("RandomNumber")

// Mode 1 AI 대기 중: BT 어떤 Prob/Dash 분기에도 매칭되지 않는 값 (strongkick <100, stomp2 <100 제외)
#define BB_IDLE_RANDOMNUMBER 100

#define BBKEY_ISBATTLECRYUSED TEXT("IsBattleCryUsed")
#define BBKEY_ISFIRSTPHASE TEXT("IsFirstPhase")