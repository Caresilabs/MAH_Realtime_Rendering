// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = nullptr; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete[](x); (x) = nullptr; }
#define PI (3.14159265358979323846f)



#include "targetver.h"
#include "LVP.h"
#include <string>





// TODO: reference additional headers your program requires here
