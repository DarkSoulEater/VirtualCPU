#pragma once

#include <string.h>

//#define DEBUG_OUT_FILE

#if defined(DEBUG_OUT_FILE)  
#   define DEB(CODE) CODE;
#else                         
#   define DEB(CODE) ;
#endif

//#define DEBUD_ACTIVATE_DUMP

const size_t kLenBreakPoint = 1;