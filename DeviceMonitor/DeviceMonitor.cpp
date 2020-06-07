#include "DeviceMonitor.h"

#include <QDebug>
#include <QTextStream>
#include <iostream>
#include <sstream>
#ifdef Q_OS_WIN

#include "windows.h"
#include "Wtsapi32.h"
#include <initguid.h>
#include "Usbiodef.h"
#include "Wiaintfc.h"
#include <Dbt.h>

static std::wstring lastError()
{
    LPWSTR buffer;
    DWORD errorCode = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR) &buffer,
        0, NULL);

    std::wstringstream ss;
    ss << std::wstring(buffer) << " (" << errorCode << ")";
    LocalFree(buffer);

    return ss.str();
}

class DeviceMonitorImpl
{
    DeviceMonitorImpl() = delete;

public:
    static LPCWSTR m_windowName;
    static HWND m_windowHandle;

public:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_DEVICECHANGE:
            switch(wParam)
            {
                case DBT_DEVICEARRIVAL:
                {
                    PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
                    if (pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                    {
                        std::cout << "Capture USB Arrival event.\n";
                    }
                    break;
                }
                case DBT_DEVICEREMOVECOMPLETE:
                {
                    PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
                    if (pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                    {
                        std::cout << "Capture USB Remove Complete event.\n";
                    }
                    break;
                }
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    static void createWindow()
    {
        LPCWSTR name = m_windowName;

        HINSTANCE instance = GetModuleHandle(NULL);

        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_NOCLOSE;
        wcex.lpfnWndProc = &DeviceMonitorImpl::WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = instance;
        wcex.hIcon = NULL;
        wcex.hCursor = NULL;
        wcex.hbrBackground = NULL;
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = name;
        wcex.hIconSm = NULL;

        if (FALSE == RegisterClassEx(&wcex))
        {
            DWORD errorCode = GetLastError();
            if (errorCode != ERROR_CLASS_ALREADY_EXISTS)
            {
                std::wcout << L"RegisterClassEx failed with" << lastError() << std::endl;
                return;
            }
        }

        m_windowHandle = CreateWindowEx(
                             WS_EX_NOACTIVATE,
                             name,
                             name,
                             NULL,
                             0, 0, 0, 0,
                             NULL,
                             NULL,
                             instance,
                             NULL);

        if (m_windowHandle == NULL)
        {
            std::wcout << L"CreateWindowEx failed with" << lastError() << std::endl;
            return;
        }

        DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
        ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));

        NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
        NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
        NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_IMAGE;//GUID_DEVINTERFACE_USB_DEVICE;

        if (!RegisterDeviceNotification(m_windowHandle, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE))
        {
            std::wcout << L"Register Device Notification failed with " << lastError() << std::endl;
            return;
        }
    }

    static void destroyWindow()
    {
        HINSTANCE instance = GetModuleHandle(NULL);

        if (FALSE == CloseWindow(m_windowHandle))
        {
            std::wcout << L"CloseWindow failed with " << lastError() << std::endl;
        }

        if (FALSE == DestroyWindow(m_windowHandle))
        {
            std::wcout << L"DestroyWindow failed with " << lastError() << std::endl;
        }

        if (FALSE == UnregisterClass(m_windowName, instance))
        {
            std::wcout << L"UnregisterClass failed with " << lastError() << std::endl;
        }

        m_windowHandle = NULL;
    }
};

/**************************************************************************************************/

LPCWSTR DeviceMonitorImpl::m_windowName = L"Device Monitor";
HWND DeviceMonitorImpl::m_windowHandle = NULL;

/**************************************************************************************************/

DeviceMonitor::DeviceMonitor()
{
    std::cout << "DeviceMonitor::DeviceMonitor" << std::endl;
    DeviceMonitorImpl::createWindow();
}

DeviceMonitor::~DeviceMonitor()
{
    std::cout << "DeviceMonitor::~DeviceMonitor" << std::endl;
    DeviceMonitorImpl::destroyWindow();
}

#endif
