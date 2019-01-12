#pragma once

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>

#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )

#define initHeapMonitor() _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)