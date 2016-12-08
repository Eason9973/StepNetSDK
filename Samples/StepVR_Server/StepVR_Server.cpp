//=============================================================================
// Copyright ?2010 NaturalPoint, Inc. All Rights Reserved.
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

#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <winsock2.h>
#include <math.h>
#include "NatNetTypes.h"
#include "NatNetServer.h"

//#include "stdafx.h"
#include <crtdbg.h>
#include <stdlib.h>
#include <windows.h>
#include <shellapi.h>
//#include <iostream>
//#include <string>
#include "StepVR.h"
#include "mmsystem.h"
#include "..\SampleClient3D\NATUtils.h"

#pragma comment(lib,"StepVR.lib")
#define _CRTDBG_MAP_ALLOC 
#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)



#pragma warning( disable : 4996 )

void BuildDescription(sDataDescriptions* pDescription);
void SendDescription(sDataDescriptions* pDescription);
void FreeDescription(sDataDescriptions* pDescription);
void BuildFrame(long FrameNumber, sDataDescriptions* pDataDescriptions, sFrameOfMocapData* pOutFrame);
void SendFrame(sFrameOfMocapData* pFrame);
void FreeFrame(sFrameOfMocapData* pFrame);
void resetServer();
int CreateServer(int iConnectionType);
int HiResSleep(int msecs);

// callbacks
void __cdecl MessageHandler(int msgType, char* msg);
int __cdecl RequestHandler(sPacket* pPacketIn, sPacket* pPacketOut, void* pUserData);

// playing thread
int StartPlayingThread();
int StopPlayingThread();
DWORD WINAPI PlayingThread_Func(void *dummy);

NatNetServer* theServer;    					// The NatNet Server 
sDataDescriptions descriptions; 				// Describes what is sent (Marker sets, rigid bodies, etc)
long g_lCurrentFrame = 0;
bool g_bPlaying = false;
DWORD PlayingThread_ID = NULL;
HANDLE PlayingThread_Handle = NULL;
int counter = 0;
int counter2 = 0;
float fCounter = 0.0f;

unsigned int MyDataPort = 3130;
unsigned int MyCommandPort = 3131;
unsigned long lAddresses[10];

#define STREAM_RBS 1
#define STREAM_MARKERS 1
#define STREAM_SKELETONS 1
#define STREAM_LABELED_MARKERS 1

float rd_p[3], rd_o[4];

StepVR::Manager* manager = new StepVR::Manager();


//using namespace StepVR;
int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//std::cout << "This is MoCap CPlusPlus main function test." << std::endl;
	int fd = manager->Start();
	printf("fd: %d\n", fd);
	if (0 != manager->Start())	{
		//exit(-1);
	}
	
	// Create a NatNet server
	int iConnectionType = ConnectionType_Multicast;
	//int iConnectionType = ConnectionType_Unicast;
	int iResult = CreateServer(iConnectionType);
	if (iResult != ErrorCode_OK)
	{
		printf("Error initializing server.  See log for details.  Exiting");
		return 1;
	}

	// Create a MarkerSet description
	BuildDescription(&descriptions);

	// OK! Ready to stream data.  Listen for request from clients (RequestHandler())
	printf("\n\nCommands:\nn\tnext frame\ns\tstream frames\nr\treset server\nq\tquit\n\r\tmulticast\nu\tunicast\n\n");
	bool bExit = false;
	while (int c = _getch())
	{
		switch (c)
		{
			/*
			case 'n':							// next frame
			{
			sFrameOfMocapData frame;
			BuildFrame(g_lCurrentFrame++, &descriptions, &frame);
			SendFrame(&frame);
			FreeFrame(&frame);
			}
			break;	*/
		case 'q':                           // quit
			bExit = true;
			break;
		case 's':							// play continuously
			g_lCurrentFrame = 0;
			if (g_bPlaying)
				StopPlayingThread();
			else
				StartPlayingThread();
			break;
		case 'r':	                        // reset server
			resetServer();
			break;
		case 'm':	                        // change to multicast
			iResult = CreateServer(ConnectionType_Multicast);
			if (iResult == ErrorCode_OK)
				printf("Server connection type changed to Multicast.\n\n");
			else
				printf("Error changing server connection type to Multicast.\n\n");
			break;
		case 'u':	                        // change to unicast
			iResult = CreateServer(ConnectionType_Unicast);
			if (iResult == ErrorCode_OK)
				printf("Server connection type changed to Unicast.\n\n");
			else
				printf("Error changing server connection type to Unicast.\n\n");
			break;

		default:
			break;
		}

		if (bExit)
		{
			theServer->Uninitialize();
			FreeDescription(&descriptions);
			break;
		}
	}
	manager->Stop();
	delete manager;
	_CrtDumpMemoryLeaks();
	getchar();

	return ErrorCode_OK;
}

int CreateServer(int iConnectionType)
{
	// release previous server
	if (theServer)
	{
		theServer->Uninitialize();
		delete theServer;
	}

	// create new NatNet server
	theServer = new NatNetServer(iConnectionType);

	// optional - use old multicast group
	//theServer->SetMulticastAddress("224.0.0.1");

	// print version info
	unsigned char ver[4];
	theServer->NatNetVersion(ver);
	printf("NatNet Simple Server (NatNet ver. %d.%d.%d.%d)\n", ver[0], ver[1], ver[2], ver[3]);

	// set callbacks
	theServer->SetErrorMessageCallback(MessageHandler);
	theServer->SetVerbosityLevel(Verbosity_Debug);
	theServer->SetMessageResponseCallback(RequestHandler);	    // Handles requests from the Client

	// get local ip addresses
	char szIPAddress[128];
	in_addr MyAddress[10];
	int nAddresses = 3;// NATUtils::GetLocalIPAddresses2((unsigned long *)&MyAddress, 10);
	if (nAddresses < 1)
	{
		printf("Unable to detect local network.  Exiting");
		return 1;
	}
	for (int i = 0; i < nAddresses; i++)
	{
		sprintf(szIPAddress, "%d.%d.%d.%d", MyAddress[i].S_un.S_un_b.s_b1, MyAddress[i].S_un.S_un_b.s_b2, MyAddress[i].S_un.S_un_b.s_b3, MyAddress[i].S_un.S_un_b.s_b4);
		printf("IP Address : %s\n", szIPAddress);
	}
	// select first
	sprintf(szIPAddress, "%d.%d.%d.%d", MyAddress[0].S_un.S_un_b.s_b1, MyAddress[0].S_un.S_un_b.s_b2, MyAddress[0].S_un.S_un_b.s_b3, MyAddress[0].S_un.S_un_b.s_b4);

	// Initialize NatNet server with first detected ip address - use NatNet default port assignments
	int retCode = theServer->Initialize(szIPAddress);
	// to use a different port for commands and/or data:
	//int retCode = theServer->Initialize(szIPAddress, MyCommandPort, MyDataPort);
	if (retCode != 0)
	{
		printf("Error initializing server.  See log for details.  Exiting");
		return 1;
	}
	else
	{
		printf("Server initialized on %s.\n", szIPAddress);
	}

	// print address/port info
	char szCommandIP[24];
	char szDataIP[24];
	char szMulticastGroup[24];
	int iDataPort, iCommandPort, iMulticastPort;
	theServer->GetSocketInfo(szDataIP, &iDataPort, szCommandIP, &iCommandPort, szMulticastGroup, &iMulticastPort);
	printf("Command Socket  : %s:%d\n", szCommandIP, iCommandPort);
	printf("Data Socket     : %s:%d\n", szDataIP, iDataPort);
	if (iConnectionType == ConnectionType_Multicast)
	{
		printf("Connection Type : Multicast\n");
		printf("Multicast Group : %s:%d\n", szMulticastGroup, iMulticastPort);
	}
	else
		printf("Connection Type : Unicast\n");


	return ErrorCode_OK;

}

// RequestHandler receives requests from the Client.  Server should
// build and return "request responses" packets in this same thread.
//
//   pPacketIn  - data packet from Client
//   pPacketOut - empty packet, to be filled in and returned to the
//				  Client as the "request response".
//
int __cdecl RequestHandler(sPacket* pPacketIn, sPacket* pPacketOut, void* pUserData)
{
	int iHandled = 1;	// handled

	switch (pPacketIn->iMessage)
	{
	case NAT_PING:
		printf("[SampleServer] received ping from Client.\n");
		printf("[SampleServer] Client App Name : %s\n", pPacketIn->Data.Sender.szName);
		printf("[SampleServer] Client App Version : %d.%d.%d.%d\n", pPacketIn->Data.Sender.Version[0],
			pPacketIn->Data.Sender.Version[1], pPacketIn->Data.Sender.Version[2], pPacketIn->Data.Sender.Version[3]);
		printf("[SampleServer] Client App NatNet Version : %d.%d.%d.%d\n", pPacketIn->Data.Sender.NatNetVersion[0],
			pPacketIn->Data.Sender.NatNetVersion[1], pPacketIn->Data.Sender.NatNetVersion[2], pPacketIn->Data.Sender.NatNetVersion[3]);

		// build server info packet
		strcpy(pPacketOut->Data.Sender.szName, "SimpleServer");
		pPacketOut->Data.Sender.Version[0] = 2;
		pPacketOut->Data.Sender.Version[1] = 1;
		pPacketOut->iMessage = NAT_PINGRESPONSE;
		pPacketOut->nDataBytes = sizeof(sSender);
		iHandled = 1;
		break;

	case NAT_REQUEST_MODELDEF:
		printf("[SimpleServer] Received request for data descriptions.\n");
		theServer->PacketizeDataDescriptions(&descriptions, pPacketOut);
		break;

	case NAT_REQUEST_FRAMEOFDATA:
	{
									// note: Client does not typically poll for data, but we accomodate it here anyway
									// note: need to return response on same thread as caller
									printf("[SimpleServer] Received request for frame of data.\n");
									sFrameOfMocapData frame;
									//BuildFrame(g_lCurrentFrame, &descriptions, &frame, rd_p[3], rd_o[4]);
									theServer->PacketizeFrameOfMocapData(&frame, pPacketOut);
									FreeFrame(&frame);
	}
		break;

	case NAT_REQUEST:
		printf("[SampleServer] Received request from Client: %s\n", pPacketIn->Data.szData);
		pPacketOut->iMessage = NAT_UNRECOGNIZED_REQUEST;
		if (stricmp(pPacketIn->Data.szData, "TestRequest") == 0)
		{
			pPacketOut->iMessage = NAT_RESPONSE;
			strcpy(pPacketOut->Data.szData, "TestResponse");
			pPacketOut->nDataBytes = ((int)strlen(pPacketOut->Data.szData)) + 1;
		}
		break;

	default:
		pPacketOut->iMessage = NAT_UNRECOGNIZED_REQUEST;
		pPacketOut->nDataBytes = 0;
		iHandled = 0;
		break;
	}

	return iHandled; // 0 = not handled, 1 = handled;
}

// Build a DataSet description (MarkerSets, RigiBodies, Skeletons).
void BuildDescription(sDataDescriptions* pDescription)
{
	pDescription->nDataDescriptions = 0;
	int index = 0;

#if STREAM_MARKERS
	// Marker Set Description
	sMarkerSetDescription* pMarkerSetDescription = new sMarkerSetDescription();
	strcpy(pMarkerSetDescription->szName, "Katie");
	pMarkerSetDescription->nMarkers = 10;
	pMarkerSetDescription->szMarkerNames = new char*[pMarkerSetDescription->nMarkers];
	char szTemp[128];
	for (int i = 0; i < pMarkerSetDescription->nMarkers; i++)
	{
		sprintf(szTemp, "Marker %d", i);
		pMarkerSetDescription->szMarkerNames[i] = new char[MAX_NAMELENGTH];
		strcpy(pMarkerSetDescription->szMarkerNames[i], szTemp);
	}
	pDescription->arrDataDescriptions[index].type = Descriptor_MarkerSet;
	pDescription->arrDataDescriptions[index].Data.MarkerSetDescription = pMarkerSetDescription;
	pDescription->nDataDescriptions++;
	index++;
#endif

#if STREAM_RBS
	// Rigid Body Description
	for (int i = 0; i < 20; i++)
	{
		sRigidBodyDescription* pRigidBodyDescription = new sRigidBodyDescription();
		sprintf(pRigidBodyDescription->szName, "RigidBody %d", i);
		pRigidBodyDescription->ID = i;
		pRigidBodyDescription->offsetx = 1.0f;
		pRigidBodyDescription->offsety = 2.0f;
		pRigidBodyDescription->offsetz = 3.0f;
		pRigidBodyDescription->parentID = 2;
		pDescription->arrDataDescriptions[index].type = Descriptor_RigidBody;
		pDescription->arrDataDescriptions[index].Data.RigidBodyDescription = pRigidBodyDescription;
		pDescription->nDataDescriptions++;
		index++;
	}
#endif

#if STREAM_SKELETONS == 0
	// Skeleton description
	for (int i = 0; i < 2; i++)
	{
		sSkeletonDescription* pSkeletonDescription = new sSkeletonDescription();
		sprintf(pSkeletonDescription->szName, "Skeleton %d", i);
		pSkeletonDescription->skeletonID = index * 100;
		pSkeletonDescription->nRigidBodies = 5;
		for (int j = 0; j<pSkeletonDescription->nRigidBodies; j++)
		{
			int RBID = pSkeletonDescription->skeletonID + j;
			sprintf(pSkeletonDescription->RigidBodies[j].szName, "RB%d", RBID);
			pSkeletonDescription->RigidBodies[j].ID = RBID;
			pSkeletonDescription->RigidBodies[j].offsetx = 1.0f;
			pSkeletonDescription->RigidBodies[j].offsety = 2.0f;
			pSkeletonDescription->RigidBodies[j].offsetz = 3.0f;
			pSkeletonDescription->RigidBodies[j].parentID = 2;
		}
		pDescription->arrDataDescriptions[index].type = Descriptor_Skeleton;
		pDescription->arrDataDescriptions[index].Data.SkeletonDescription = pSkeletonDescription;
		pDescription->nDataDescriptions++;
		index++;
	}
#endif


}

// Send DataSet description to Client
void SendDescription(sDataDescriptions* pDescription)
{
	sPacket packet;
	theServer->PacketizeDataDescriptions(pDescription, &packet);
	theServer->SendPacket(&packet);
}

// Release any memory we allocated for our example
void FreeDescription(sDataDescriptions* pDescription)
{
	/*
	for(int i=0; i< pDescription->nDataDescriptions; i++)
	{
	if(pDescription->arrDataDescriptions[i].type == Descriptor_MarkerSet)
	{
	for(int i=0; i< pMarkerSetDescription->nMarkers; i++)
	{
	delete[] pMarkerSetDescription->szMarkerNames[i];
	}
	delete elete[] pMarkerSetDescription->szMarkerNames;
	sMarkerSetDescription* pMarkerSetDescription = pDescription->arrDataDescriptions[0].Data.MarkerSetDescription;
	}
	else if(pDescription->arrDataDescriptions[i].type == Descriptor_RigidBody)
	{
	*/

}

// Build frame of MocapData
void BuildFrame(long FrameNumber, sDataDescriptions* pModels, sFrameOfMocapData* pOutFrame)
{
	//----------------------------------
	//for (int i = 0; i < 1; i++)	{
		StepVR::Frame frame = manager->GetFrame();
		StepVR::Frame* frame2;
		frame2 = &frame;
		StepVR::SingleNode positionframe = frame.GetSingleNode();
		StepVR::Vector3f v3 = positionframe.GetPosition(StepVR::SingleNode::NodeID_LeftWrist);
		StepVR::Vector4f v4 = positionframe.GetQuaternion(StepVR::SingleNode::NodeID_LeftWrist);
		printf("pos: %f %f %f \n",v3.x,v3.y,v3.z);
		printf("ori: %f %f %f %f \n", v4.x,v4.y,v4.z,v4.w);

		rd_p[0] = v3.x;
		rd_p[1] = v3.y;
		rd_p[2] = v3.z;

		rd_o[0] = v4.x;
		rd_o[1] = v4.y;
		rd_o[2] = v4.z;
		rd_o[3] = v4.w;
		//Sleep(10);
	//}

	//----------------------------------
	if (!pModels)
	{
		printf("No models defined - nothing to send.\n");
		return;
	}

	ZeroMemory(pOutFrame, sizeof(sFrameOfMocapData));
	pOutFrame->iFrame = FrameNumber;
	pOutFrame->fLatency = (float)GetTickCount();
	pOutFrame->nOtherMarkers = 0;
	pOutFrame->nMarkerSets = 0;
	pOutFrame->nRigidBodies = 1;
	//pOutFrame->nLabeledMarkers = 0;

	int num01 = pModels->nDataDescriptions;
	//printf("num01 :%d\n", num01);    //20
	//for(int i=0; i < pModels->nDataDescriptions; i++)
	int i = 0;
	if (i == 0)
	{
#if STREAM_MARKERS == 0
		// MarkerSet data
		if (pModels->arrDataDescriptions[i].type == Descriptor_MarkerSet)
		{
			// add marker data
			int index = pOutFrame->nMarkerSets;

			sMarkerSetDescription* pMS = pModels->arrDataDescriptions[i].Data.MarkerSetDescription;
			strcpy(pOutFrame->MocapData[index].szName, pMS->szName);
			pOutFrame->MocapData[index].nMarkers = pMS->nMarkers;
			pOutFrame->MocapData[index].Markers = new MarkerData[pOutFrame->MocapData[0].nMarkers];

			for (int iMarker = 0; iMarker < pOutFrame->MocapData[index].nMarkers; iMarker++)
			{
				double rads = (double)counter * 3.14159265358979 / 180.0f;
				pOutFrame->MocapData[index].Markers[iMarker][0] = (float)sin(rads) + (10 * iMarker);		// x
				pOutFrame->MocapData[index].Markers[iMarker][1] = (float)cos(rads) + (20 * iMarker);		// y
				pOutFrame->MocapData[index].Markers[iMarker][2] = (float)tan(rads) + (30 * iMarker);		// z
				counter++;
				counter %= 360;
			}
			pOutFrame->nMarkerSets++;

		}
#endif

#if STREAM_RBS
		/*
		float rd_position[3];
		rd_position[0] = rd_p[0];
		rd_position[1] = rd_p[1];
		rd_position[2] = rd_p[2];

		float rd_orientation[4];
		rd_orientation[0] = rd_o[0];
		rd_orientation[1] = rd_o[1];
		rd_orientation[2] = rd_o[2];
		rd_orientation[3] = rd_o[3];
		*/
		// RigidBody data
		int anchor = pModels->arrDataDescriptions[i + 1].type;
		//printf("anchor :%d\n", anchor);
		if (pModels->arrDataDescriptions[i + 1].type == Descriptor_RigidBody)
		{
			sRigidBodyDescription* pMS = pModels->arrDataDescriptions[i].Data.RigidBodyDescription;
			//int index = pOutFrame->nRigidBodies;
			int index = 0;
			//printf("index: %d\n", index);
			sRigidBodyData* pRB = &pOutFrame->RigidBodies[index];

			//pRB->ID = pMS->ID;
			int test_id = 1;
			pRB->ID = test_id;
			//printf("ID:%d\n", pRB->ID);
			/*
			double rads = (double)counter * 3.14159265358979 / 180.0f;
			pRB->x = (float) sin(rads);
			pRB->y = (float) cos(rads);
			pRB->z = (float) tan(rads);
			*/
			pRB->x = rd_p[0];
			pRB->y = rd_p[1];
			pRB->z = rd_p[2];
			/*
			EulerAngles ea;
			ea.x = (float) sin(rads);
			ea.y = (float) cos(rads);
			ea.z = (float) tan(rads);
			ea.w = 0.0f;
			Quat q = Eul_ToQuat(ea);
			pRB->qx = q.x;
			pRB->qy = q.y;
			pRB->qz = q.z;
			pRB->qw = q.w;
			*/
			pRB->qx = rd_o[0];
			pRB->qy = rd_o[1];
			pRB->qz = rd_o[2];
			pRB->qw = rd_o[3];

			pRB->nMarkers = 1;
			pRB->Markers = new MarkerData[pRB->nMarkers];
			pRB->MarkerIDs = new int[pRB->nMarkers];
			pRB->MarkerSizes = new float[pRB->nMarkers];
			pRB->MeanError = 0.0f;
			int num02 = pRB->nMarkers;
			//printf("num02:%d\n", num02);
			//for(int iMarker=0; iMarker < pRB->nMarkers; iMarker++)
			int iMarker = 0;
			if (iMarker == 0)
			{
				pRB->Markers[iMarker][0] = iMarker + 0.1f;		// x
				pRB->Markers[iMarker][1] = iMarker + 0.2f;		// y
				pRB->Markers[iMarker][2] = iMarker + 0.3f;		// z
				pRB->MarkerIDs[iMarker] = iMarker + 200;
				pRB->MarkerSizes[iMarker] = 77.0f;
			}
			pOutFrame->nRigidBodies++;
			counter++;
			//test_id++;

		}
#endif

#if STREAM_SKELETONS == 0
		// Skeleton data
		if (pModels->arrDataDescriptions[i].type == Descriptor_Skeleton)
		{
			sSkeletonDescription* pSK = pModels->arrDataDescriptions[i].Data.SkeletonDescription;
			int index = pOutFrame->nSkeletons;

			pOutFrame->Skeletons[index].skeletonID = pSK->skeletonID;
			pOutFrame->Skeletons[index].nRigidBodies = pSK->nRigidBodies;
			// RigidBody data
			pOutFrame->Skeletons[index].RigidBodyData = new sRigidBodyData[pSK->nRigidBodies];
			for (int i = 0; i<pSK->nRigidBodies; i++)
			{
				sRigidBodyData* pRB = &pOutFrame->Skeletons[index].RigidBodyData[i];
				pRB->ID = pOutFrame->Skeletons[index].skeletonID + i;
				double rads = (double)counter * 3.14159265358979 / 180.0f;
				pRB->x = (float)sin(rads);
				pRB->y = (float)cos(rads);
				pRB->z = (float)tan(rads);
				fCounter += 0.1f;
				if (fCounter > 1.0f)
					fCounter = 0.1f;
				pRB->qx = fCounter;
				pRB->qy = fCounter + 0.1f;
				pRB->qz = fCounter + 0.2f;
				pRB->qw = 1.0f;
				pRB->nMarkers = 3;
				pRB->Markers = new MarkerData[pRB->nMarkers];
				pRB->MarkerIDs = new int[pRB->nMarkers];
				pRB->MarkerSizes = new float[pRB->nMarkers];
				pRB->MeanError = 0.0f;
				for (int iMarker = 0; iMarker < pRB->nMarkers; iMarker++)
				{
					pRB->Markers[iMarker][0] = iMarker + 0.1f;		// x
					pRB->Markers[iMarker][1] = iMarker + 0.2f;		// y
					pRB->Markers[iMarker][2] = iMarker + 0.3f;		// z
					pRB->MarkerIDs[iMarker] = iMarker + 200;
					pRB->MarkerSizes[iMarker] = 77.0f;
				}
				counter++;
			}

			pOutFrame->nSkeletons++;

		}
#endif
	}

#if STREAM_LABELED_MARKERS == 0
	// add marker data
	pOutFrame->nLabeledMarkers = 10;
	for (int iMarker = 0; iMarker < 10; iMarker++)
	{
		sMarker* pMarker = &pOutFrame->LabeledMarkers[iMarker];
		pMarker->ID = iMarker + 100;
		pMarker->x = (float)iMarker;
		pMarker->y = (float)(counter2 * iMarker);
		pMarker->z = (float)iMarker;
		pMarker->size = 5.0f;
	}
	counter2++;
	counter2 %= 100;
#endif

}

// Packetize and send a single frame of mocap data to the Client
void SendFrame(sFrameOfMocapData* pFrame)
{
	sPacket packet;
	theServer->PacketizeFrameOfMocapData(pFrame, &packet);
	theServer->SendPacket(&packet);
}

// Release any memory we allocated for this sample
void FreeFrame(sFrameOfMocapData* pFrame)
{
	for (int i = 0; i < pFrame->nMarkerSets; i++)
	{
		delete[] pFrame->MocapData[i].Markers;
	}

	for (int i = 0; i < pFrame->nOtherMarkers; i++)
	{
		delete[] pFrame->OtherMarkers;
	}

	for (int i = 0; i< pFrame->nRigidBodies; i++)
	{
		delete[] pFrame->RigidBodies[i].Markers;
		delete[] pFrame->RigidBodies[i].MarkerIDs;
		delete[] pFrame->RigidBodies[i].MarkerSizes;
	}

}

// PlayingThread_Func streams data at ~60hz 
DWORD WINAPI PlayingThread_Func(void *dummy)
{
	static int fre_div = 0;
	while (1)
	{
		/*
		char p;
		if (kbhit())
		{
			p = getch();
			if (p == 'x')
			{
				//printf("Stop\n");
				//manager->Stop;
				//delete manager;
				//_CrtDumpMemoryLeaks();
				break;
			}
		}*/
	//for (int i = 0; i < 15; i++)
		
			sFrameOfMocapData frame;
			BuildFrame(g_lCurrentFrame, &descriptions, &frame);
		if (fre_div % 3 == 0)
		{
			SendFrame(&frame);
			FreeFrame(&frame);
			//printf("Sent Frame %d", g_lCurrentFrame);
			g_lCurrentFrame++;
		    
			//printf("In frame\n");
		}
			HiResSleep(5);
		fre_div++;
		if (fre_div > 60000)fre_div = 0;
	}

	return ErrorCode_OK;
}

int StartPlayingThread()
{
	SECURITY_ATTRIBUTES security_attribs;
	security_attribs.nLength = sizeof(SECURITY_ATTRIBUTES);
	security_attribs.lpSecurityDescriptor = NULL;
	security_attribs.bInheritHandle = TRUE;

	PlayingThread_Handle = CreateThread(&security_attribs, 0, PlayingThread_Func, NULL, 0, &PlayingThread_ID);
	if (PlayingThread_Handle == NULL)
	{
		printf("Error creating play thread.");
		return -1;
	}
	else
		g_bPlaying = true;

	return ErrorCode_OK;
}

int StopPlayingThread()
{
	if (PlayingThread_Handle != NULL)
		TerminateThread(PlayingThread_Handle, 0);

	PlayingThread_Handle = NULL;
	g_bPlaying = false;

	return ErrorCode_OK;
}

void resetServer()
{
	int iSuccess = 0;
	char szIPAddress[128];
	theServer->IPAddress_LongToString(lAddresses[0], szIPAddress);	// use first IP detected

	printf("\nRe-initting Server\n\n");

	iSuccess = theServer->Uninitialize();
	if (iSuccess != 0)
		printf("Error uninitting server.\n");

	iSuccess = theServer->Initialize(szIPAddress);
	if (iSuccess != 0)
		printf("Error re-initting server.\n");

}

// MessageHandler receives NatNet error mesages
void __cdecl MessageHandler(int msgType, char* msg)
{
	printf("\n%s\n", msg);
}

// higer res sleep than standard.
int HiResSleep(int msecs)
{
	HANDLE hTempEvent = CreateEvent(0, true, FALSE, _T("TEMP_EVENT"));
	timeSetEvent(msecs, 1, (LPTIMECALLBACK)hTempEvent, 0, TIME_ONESHOT | TIME_CALLBACK_EVENT_SET);
	WaitForSingleObject(hTempEvent, msecs);
	CloseHandle(hTempEvent);
	return 1;
}
