#ifndef ENGINE_INCLUDE_H__
#define ENGINE_INCLUDE_H__

#pragma warning (disable : 4251)

#include <d3dx9.h>
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <assert.h>
#include <random>
#include <chrono>
#include <limits>

#include "iostream"
using namespace std;

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include "Engine_Typedef.h"
#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Enum.h"
#include "Engine_Struct.h"

//#include "KeyMgr.h"


#endif // ENGINE_INCLUDE_H__


