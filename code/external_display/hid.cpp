#define UNICODE

#include <wtypes.h>
#include <initguid.h>
#include <stdio.h>
#include <wchar.h>
#include "hid.h"

extern "C" {
#include "hidsdi.h"
#include <dbt.h>

#include <setupapi.h>
}

class HidDeviceImpl
{
public:
        HidDeviceImpl(wchar_t* path, HANDLE deviceHandle) : m_path(_wcsdup(path)), m_deviceHandle(deviceHandle) {
		m_referenceCounter = 1;

		// get report sizes
		PHIDP_PREPARSED_DATA preparsedData;
		HidD_GetPreparsedData(m_deviceHandle, &preparsedData);
		HIDP_CAPS capabilities;
		HidP_GetCaps(preparsedData, &capabilities);
		m_inputReportByteLength = capabilities.InputReportByteLength;
		m_outputReportByteLength = capabilities.OutputReportByteLength;
		HidD_FreePreparsedData(preparsedData);

		// reserve memory for io-buffers
		m_outputBuffer = new unsigned char[m_outputReportByteLength];
		m_inputBuffer = new unsigned char[m_inputReportByteLength];

		// open file handles
                m_readFileHandle = CreateFileW(m_path, GENERIC_READ , FILE_SHARE_READ | FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
                m_writeFileHandle = CreateFileW(m_path, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, 0, NULL);
                m_overlappedSignal = CreateEvent(NULL, TRUE, FALSE, NULL);
        }
        ~HidDeviceImpl();
	int m_referenceCounter;

	int m_inputReportByteLength;
	int m_outputReportByteLength;
        wchar_t* m_path;
	HANDLE m_deviceHandle;
	HANDLE m_readFileHandle;
        HANDLE m_writeFileHandle;
        HANDLE m_overlappedSignal;
	unsigned char* m_outputBuffer;
	unsigned char* m_inputBuffer;
};

HidDeviceImpl::~HidDeviceImpl()
{
        CloseHandle(m_writeFileHandle);
	CloseHandle(m_readFileHandle);
	CloseHandle(m_deviceHandle);
	delete m_outputBuffer;
	delete m_inputBuffer;
        delete [] m_path;
}

HidDevice::HidDevice(const HidDevice& d)
{
	m_impl = d.m_impl;
	m_impl->m_referenceCounter++;
}

HidDevice::~HidDevice()
{
	if (--m_impl->m_referenceCounter == 0) delete m_impl;
}

int HidDevice::getInputReportByteLength()
{
	return m_impl->m_inputReportByteLength;
}

int HidDevice::getOutputReportByteLength()
{
	return m_impl->m_outputReportByteLength;
}

int HidDevice::sendMessage(unsigned char* pMessage, int size)
{
	ZeroMemory(m_impl->m_outputBuffer, m_impl->m_outputReportByteLength);
	int max = min(size, m_impl->m_outputReportByteLength);
	memcpy(m_impl->m_outputBuffer + 1, pMessage, max);
        //printf("m_impl->m_outputReportByteLength: %i\n", m_impl->m_outputReportByteLength);
	DWORD bytesWritten;
        int result = WriteFile(m_impl->m_writeFileHandle, m_impl->m_outputBuffer, m_impl->m_outputReportByteLength, &bytesWritten, NULL);
        //printf("write finished\n");
	return result;
}

int HidDevice::readMessage(unsigned char* pMessage, int size)
{
    while (true) {
        OVERLAPPED overlapped;
        memset(&overlapped, 0, sizeof(overlapped));
        overlapped.hEvent = m_impl->m_overlappedSignal;
        DWORD numberOfBytesRead = 0;
        int result = ReadFile(m_impl->m_readFileHandle, m_impl->m_inputBuffer, m_impl->m_inputReportByteLength, &numberOfBytesRead, &overlapped);
        if (result) {
                // read was successful, copy to client
                int max = min(size, int(numberOfBytesRead));
                memcpy(pMessage, m_impl->m_inputBuffer + 1, max);
                return result;
        } else {
                // deal with the error code
                int error = GetLastError();
                switch (error) {
                        case ERROR_IO_PENDING:
                                // asynchronous i/o is still in progress,
                                // wait until end or read thread stop request
                                WaitForSingleObject(m_impl->m_overlappedSignal, INFINITE);

                                // check on the results of the asynchronous read
                                result = GetOverlappedResult(m_impl->m_readFileHandle, &overlapped, &numberOfBytesRead, FALSE);

                                if (result) {
                                        // read was successful, copy to client
                                        int max = min(size, int(numberOfBytesRead));
                                        memcpy(pMessage, m_impl->m_inputBuffer + 1, max);
                                        return result;
                                } else {
                                        // if there was a problem, notify client and stop thread
                                        //onDeviceClosed();
                                        //m_readThreadRunning = false;
                                        //exit(1);
                                    continue;
                                }
                                break;
                        case ERROR_HANDLE_EOF:
                        default:
                                // we have reached the end of the file during the call to ReadFile, or it was another error,
                                // notify client and stop thread
                                exit(1);
                }
        }
    }
}

// search device
// based on code from Jan Axelson, http://www.lvr.com/hidpage.htm
// the hid.lib and the header files hidpi.h, hidsdi.h and hidusage.h
// are available from the WDK (http://www.microsoft.com/whdc/ddk/winddk.mspx) 
// returns a vector of all detected devices
vector<HidDevice> HidDevice::searchDevice(int VendorID, int ProductID /*, string manufacturer, string product */)
{
	vector<HidDevice> devices;
	LONG result;	

	// get the GUID for all system HIDs
	GUID hidGuid;
	HidD_GetHidGuid(&hidGuid);	
	
	// get a handle to a device information set for all installed devices
	HANDLE devInfo = SetupDiGetClassDevs(&hidGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);

	// step through the available devices looking for the one we want
	int memberIndex = 0;

	while (1) {
		// get next device
		SP_DEVICE_INTERFACE_DATA devInfoData;
		devInfoData.cbSize = sizeof(devInfoData);
		result = SetupDiEnumDeviceInterfaces(devInfo, 0, &hidGuid, memberIndex, &devInfoData);
		if (result) {
			// a device has been detected, so get more information about it
			ULONG length = 0;
			result = SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, NULL, 0, &length, NULL);

			// allocate memory for the hDevInfo structure, using the returned Length
			PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc(length);
			
			// set cbSize in the detailData structure
			detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			// call the function again, this time passing it the returned buffer size
			ULONG required;
			result = SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, detailData, length, &required, NULL);

			// open a handle to the device
			HANDLE deviceHandle = CreateFile(detailData->DevicePath, 0, FILE_SHARE_READ|FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, 0, NULL);

			// HidD_GetAttributes gets a HIDD_ATTRIBUTES structure, containing
			// the Vendor ID, Product ID, and Product Version Number
			HIDD_ATTRIBUTES attributes;
			attributes.Size = sizeof(attributes);
			result = HidD_GetAttributes(deviceHandle, &attributes);
			if (attributes.VendorID == VendorID && attributes.ProductID == ProductID) {
				// vendor id and product id match
				HidDevice d;
				d.m_impl = new HidDeviceImpl(detailData->DevicePath, deviceHandle);
				devices.push_back(d);

#if 0
				// test for manufacturer and product string
				WCHAR wbuf[512];
				HidD_GetManufacturerString(deviceHandle, wbuf, 512);
				char currentManufacturer[512];
				if (wcstombs(currentManufacturer, wbuf, 512) == -1) currentManufacturer[0] = 0;
				HidD_GetProductString(deviceHandle, wbuf, 512);
				char currentProduct[512];
				if (wcstombs(currentProduct, wbuf, 512) == -1) currentProduct[0] = 0;
				if (manufacturer == currentManufacturer && product == currentProduct) {
					// strings matches, add device
					HidDevice d;
					d.m_impl = new HidDeviceImpl(detailData->DevicePath, deviceHandle);
					HidDevice d2 = d;
					devices.push_back(d);
				} else {
					CloseHandle(deviceHandle);
				}
				free(detailData);
#endif
			}
		} else {
			// SetupDiEnumDeviceInterfaces returned 0, so there are no more devices to check
			break;
		}

		// try the next device
		memberIndex = memberIndex + 1;
	}

	// free the memory reserved for hDevInfo by SetupDiClassDevs
	SetupDiDestroyDeviceInfoList(devInfo);

	return devices;
}
