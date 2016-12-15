// svcapp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conslsvc.h"
#include <Rpc.h>
#pragma comment(lib, "rpcrt4.lib")

class MyService : public TConsoleService<FileLogger> {	// uses FileLogger
	typedef TConsoleService<FileLogger> baseClass;
public:
	MyService()
		: baseClass(_T("myservice"))
		, lw_(L"myservice", getLogger())
	{}

	static const int LOG_LEVEL_INFO=Logger::LOG_LEVEL_INFORMATION;
	static const int LOG_LEVEL_WARN=Logger::LOG_LEVEL_WARNING;
	static const int LOG_LEVEL_ERROR=Logger::LOG_LEVEL_ERROR;

	// override Run() so that we can register for device notifications
	DWORD run()
	{
		DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;

		GUID guidStorageClass = { 0x53f56307, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b };

		ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
		NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
		NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
		NotificationFilter.dbcc_classguid = guidStorageClass;

		HDEVNOTIFY hDeviceNotify = ::RegisterDeviceNotificationW(
			hServiceStatus_, 
			&NotificationFilter, 
			DEVICE_NOTIFY_SERVICE_HANDLE);
		if (hDeviceNotify == NULL) {
			lw_.getStreamW(LOG_LEVEL_INFO) << L"Error registering device notification, rc: " << ::GetLastError() << L"\r\n";
		} else {
			lw_.getStreamW(LOG_LEVEL_WARN) << L"Notification for device registered\r\n";
		}

		// specify flags for additional control commands we accept
		status_.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN;

		// call base class method to block on the quit event
		DWORD dwRet = baseClass::run();

		// service quitting...

		if (hDeviceNotify != NULL) {
			// gracefully release the device notification handle
			lw_.getStreamW(LOG_LEVEL_INFO) << L"Unregistering device notification...\r\n";
			::UnregisterDeviceNotification(hDeviceNotify);
		}

		return dwRet;
	}

	virtual void onShutdown()
	{
		// do any shutdown wind-down tasks here
	}

	// handle device events for our registered device interface type
    virtual DWORD onDeviceEvent(DWORD dwDBT, PDEV_BROADCAST_HDR pHdr)
	{
		PDEV_BROADCAST_DEVICEINTERFACE pDev = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
		lw_.getStreamW(LOG_LEVEL_INFO) 
			<< L"Device event:-  dwDBT: 0x" << std::hex << dwDBT 
			<< L", device type: " << pHdr->dbch_devicetype 
			<< std::endl;

		if (dwDBT == DBT_DEVICEARRIVAL) {
			if (pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
				PDEV_BROADCAST_DEVICEINTERFACE pInt = reinterpret_cast<PDEV_BROADCAST_DEVICEINTERFACE>(pHdr);
				RPC_WSTR strGUID;
				::UuidToStringW(&pInt->dbcc_classguid, &strGUID);
				lw_.getStreamW(LOG_LEVEL_INFO) 
					<< L"New Device Interface arrival, guid: " << (const wchar_t*)strGUID 
					<< L", name: " << pInt->dbcc_name << L"\r\n";
				::RpcStringFreeW(&strGUID);
			}
		}
		return NO_ERROR;
	}

private:
	LogWriter lw_;
};

MyService _service;

int _tmain(int argc, _TCHAR* argv[])
{
	std::ostringstream os;
	os << "Hello";
	_service.start();
	return 0;
}

