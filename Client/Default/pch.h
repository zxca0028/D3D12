
#ifndef PCH_H
#define PCH_H

#include "framework.h"

#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>

/* DX12 */
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#include <DirectXColors.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include "d3dx12.h"
#include "d3d12shader.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "Dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#include <memory>
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <string>
#include <comdef.h>

#include "Exception.h"

#endif