#pragma once
#include "CLog.h"

#define CheckNull(p){if(p == NULL) return;}
#define CheckNullMsg(p,msg){if(p == NULL) return CLog::Log(msg);}
#define CheckNullResult(p,result){if(p == NULL) return result;}

#define CheckTrue(p){if(p == true) return;}
#define CheckTrueResult(p,result){if(p == true) return result;}

#define CheckFalse(p){if(p == false) return;}
#define CheckFalseResult(p,result){if(p == false) return result;}