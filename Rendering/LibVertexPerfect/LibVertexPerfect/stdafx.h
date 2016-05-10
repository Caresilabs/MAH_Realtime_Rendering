// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = nullptr; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete[](x); (x) = nullptr; }
#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = nullptr; }
#define ASSERT(x) if(FAILED(x)) { throw std::runtime_error("ASSERT failed\n"); }
#define PI (3.14159265358979323846f)

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "targetver.h"
#include <string>

#include "LVP.h"
#include "VPtr.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/Shaders/ShaderProgram.h"
