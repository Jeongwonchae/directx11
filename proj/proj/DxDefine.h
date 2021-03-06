#pragma once

//LINKING

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dcompiler.lib")

//INCLUDES

#include <d3d11.h>
#include <dinput.h>
#include <mmsystem.h>
#include <dsound.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"
using namespace DirectX;

//warning C4316 처리용이라함
#include "AlignedAllocationPolicy.h"