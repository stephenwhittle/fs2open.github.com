#ifndef HID_H
#define HID_H

#include <vector>
#include <string>

using namespace std;

class HidDeviceImpl;

class HidDevice
{
public:
	// copy constructor: creates new value object and increases reference counter in m_impl
	HidDevice(const HidDevice& d);

	// decreases reference counter in m_impl and deletes it, if 0
	~HidDevice();

	// return a vector of all devices (the devices are opened)
	static vector<HidDevice> searchDevice(int VendorID, int ProductID /*, string manufacturer, string product */ );

	// get report lengths
	int getInputReportByteLength();
	int getOutputReportByteLength();

	// send a message
	// if smaller than output record byte length, filled with 0
	int sendMessage(unsigned char* pMessage, int size);

	// read a message
	// max length specified in size and size of read data returned in size
	int readMessage(unsigned char* pMessage, int size);

private:
	HidDevice() {}
	HidDeviceImpl* m_impl;
};

#endif
