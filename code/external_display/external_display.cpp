
#include "events/events.h"
#include "hidapi/hidapi.h"
#include "osapi/outwnd.h"
#include <thread>
#include <vector>
#include "external_display/readerwriterqueue.h"
namespace external_display 
{
	std::atomic_flag HIDThreadEnabled= ATOMIC_FLAG_INIT;

std::vector<uint8_t>* QueuedValue;
std::vector<uint8_t>* DeQueuedValue;

	void HIDThread(uint16_t VendorID, uint16_t ProductID, moodycamel::BlockingReaderWriterQueue<std::vector<uint8_t>>* Queue)
	{
		hid_init();
		hid_device* CurrentDevice = hid_open(VendorID, ProductID, nullptr);
		if (CurrentDevice)
		{
			hid_set_nonblocking(CurrentDevice, 0);
		}
		std::vector<uint8_t> ReportData;
		while (HIDThreadEnabled.test_and_set())
		{
			DeQueuedValue = Queue->peek();
			ReportData.clear();
			if (Queue->try_dequeue(ReportData))
			{
				if (CurrentDevice == nullptr) {
					CurrentDevice = hid_open(VendorID, ProductID, nullptr);
				    if (CurrentDevice) {
					    hid_set_nonblocking(CurrentDevice, 0);
				    }
				}
				if (CurrentDevice != nullptr) {
				    if (hid_write(CurrentDevice, ReportData.data(), ReportData.size()) == -1) {
						CurrentDevice = hid_open(VendorID, ProductID, nullptr);
					    if (CurrentDevice) {
						    hid_set_nonblocking(CurrentDevice, 0);
					    }
				    }
			    }
			}
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}


	void OnRenderCountermeasureGauge(int NumberOfCountermeasures);
	void OnRenderPrimaryWeapon(int WeaponIndex, const char* WeaponName, int CurrentAmmo, int MaxAmmo, bool Linked);
	void OnRenderLockWarning(int Warning);
	void OnRenderMissionTime(int Minutes, int Seconds);
	enum class ReportID
	{
		REPORT_COUNTERMEASURES = 2,
		REPORT_PRIMARYWEAPON = 1
	};
	static bool ShowLockWarning = false;
	
	moodycamel::BlockingReaderWriterQueue <std::vector<uint8_t>> HIDQueue;
	
	void deinit() { HIDThreadEnabled.clear(); }

	
	void init() 
	{ 
		HIDThreadEnabled.test_and_set();
		events::EngineShutdown.add(deinit);
		std::vector<uint8_t> InitialValue;
		InitialValue.push_back(2);
		InitialValue.push_back(0b0111000);
		InitialValue.push_back(0b1011100);
		InitialValue.push_back(0b0111001);
		InitialValue.push_back(0b1110101);
		HIDQueue.enqueue(InitialValue);
		std::thread WriteThread(HIDThread, uint16_t(1155), uint16_t(22352), &HIDQueue);
		WriteThread.detach();
		events::RenderPrimaryWeapon.add(OnRenderPrimaryWeapon);
		//events::RenderCountermeasureGauge.add(OnRenderCountermeasureGauge); 
		events::RenderLockWarning.add(OnRenderLockWarning);
		events::RenderMissionTime.add(OnRenderMissionTime);
	}
	void OnRenderLockWarning(int Warning)
	{
		ShowLockWarning = (Warning != 0);
	}

	
	
void OnRenderPrimaryWeapon(int WeaponIndex, const char* WeaponName,int CurrentAmmo, int MaxAmmo, bool Linked )
{

	std::vector<uint8_t> ReportBuffer;
	ReportBuffer.push_back((uint8_t) ReportID::REPORT_PRIMARYWEAPON);
	ReportBuffer.push_back((uint8_t)WeaponIndex); 
	for (auto ReportIndex = 0; ReportIndex < 12; ReportIndex++)
	{
		ReportBuffer.push_back(WeaponName[ReportIndex]);
		if (WeaponName[ReportIndex] == 0)
		{
			break;
		}
	}
	ReportBuffer.push_back(CurrentAmmo % 10); //ones
	ReportBuffer.push_back((CurrentAmmo /10) % 10); //tens
	ReportBuffer.push_back((CurrentAmmo / 100) % 10); //hundreds
	ReportBuffer.push_back(MaxAmmo % 10);         // ones
	ReportBuffer.push_back((MaxAmmo/ 10) % 10);  // tens
	ReportBuffer.push_back((MaxAmmo/ 100) % 10); // hundreds

	ReportBuffer.push_back((uint8_t) Linked);
	HIDQueue.enqueue(ReportBuffer);
}

void OnRenderCountermeasureGauge(int NumberOfCountermeasures)
{
	
	static uint8_t Number[] = { 
	0b0111111,
	0b0000110,
	0b1011011,
	0b1001111,
	0b1100110,
	0b1101101,
	0b1111101,
	0b0000111,
	0b1111111,
	0b1101111,
	};

	std::vector<uint8_t> ReportBuffer;
	ReportBuffer.push_back((uint8_t)ReportID::REPORT_COUNTERMEASURES);
	if (ShowLockWarning)
	{
		ReportBuffer.push_back(0b0111000);
		ReportBuffer.push_back(0b1011100);
		ReportBuffer.push_back(0b0111001);
		ReportBuffer.push_back(0b1110101);
	}
	else 
	{
		ReportBuffer.push_back(0b0111001);
		ReportBuffer.push_back(0b1101101);
		ReportBuffer.push_back(Number[(NumberOfCountermeasures / 10) % 10]);
		ReportBuffer.push_back(Number[NumberOfCountermeasures % 10]);
	}
	QueuedValue = &ReportBuffer;
	HIDQueue.enqueue(std::move(ReportBuffer));

}
void OnRenderMissionTime(int Minutes, int Seconds)
{
	static uint8_t Number[] = {
	    0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110, 0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111,
	};
	std::vector<uint8_t> ReportBuffer;
	ReportBuffer.push_back(2);
	ReportBuffer.push_back(Number[(Minutes/ 10) % 10]);
	ReportBuffer.push_back(Number[Minutes% 10]);
	ReportBuffer.push_back(Number[(Seconds/ 10) % 10]);
	ReportBuffer.push_back(Number[Seconds% 10]);
	HIDQueue.enqueue(std::move(ReportBuffer));
}

} // namespace external_display