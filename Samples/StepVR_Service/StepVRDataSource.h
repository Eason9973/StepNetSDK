//=============================================================================
// Copyright © 2010 NaturalPoint, Inc. All Rights Reserved.
// 
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall NaturalPoint, Inc. or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//=============================================================================


/*

SimpleServer.cpp

Illustrates the minimum code required to create and send marker data using
the NatNet server.

*/
#pragma once

#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <winsock2.h>
#include <math.h>
#include "NatNetTypes.h"
#include "NatNetServer.h"
#include "mmsystem.h"
#include "..\SampleClient3D\NATUtils.h"

//#include "stdafx.h"
//#include <iostream>
//#include <string>
#include <crtdbg.h>
#include <stdlib.h>
#include <windows.h>
#include <shellapi.h>

#define STREAM_RBS 1
#define STREAM_MARKERS 0
#define STREAM_SKELETONS 0
#define STREAM_LABELED_MARKERS 0

int StepVRDataSource_Start();
int StepVRDataSource_Stop();
void StepVRDataSource_Update();
