// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#include <windows.h>


// TODO:  在此处引用程序需要的其他头文件
#pragma warning(push)
#pragma warning(disable: 4005) // possible loss of data
#include <d3dx11.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#pragma warning(pop)
#include <xnamath.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <array>
#include <list>
#include <set>
#include <regex>
#include <mmsystem.h>

#include "gfTypes.h"