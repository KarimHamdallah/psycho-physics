#pragma once
#include "pLogger.h"
#include <intrin.h>


// TODO: Change __debugbreak according to the compiler used
#define PSYCHO_ASSERT(cond) if(cond) {} else {__debugbreak();}
#define PSYCHO_ASSERT_MSG(cond,msg) if(cond) {} else {PSYCHO_CRITICAL("Assertion Failed: File {0} on Line {1} : {2}",__FILE__,__LINE__,msg); __debugbreak();}