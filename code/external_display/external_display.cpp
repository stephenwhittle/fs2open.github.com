
#include "events/events.h"
#include "hidapi/hidapi.h"
#include "osapi/outwnd.h"


namespace external_display 
{
	void OnRenderCountermeasureGauge(int NumberOfCountermeasures);
	void OnRenderPrimaryWeapon(int WeaponIndex, const char* WeaponName, int CurrentAmmo, int MaxAmmo, bool Linked);
	void OnRenderLockWarning(int Warning);
	enum class ReportID
	{
		REPORT_COUNTERMEASURES = 2,
		REPORT_PRIMARYWEAPON = 1
	};
	static bool ShowLockWarning = false;
	hid_device* CurrentDevice = nullptr;
	
	void init() 
	{ 
		hid_init();
		CurrentDevice = hid_open(1155, 22352, nullptr);
		events::RenderPrimaryWeapon.add(OnRenderPrimaryWeapon);
		events::RenderCountermeasureGauge.add(OnRenderCountermeasureGauge); 
		events::RenderLockWarning.add(OnRenderLockWarning);
	}

	void OnRenderLockWarning(int Warning)
	{
		ShowLockWarning = (Warning != 0);
	}

	uint8_t ReportBuffer[21] = {0};

	
void OnRenderPrimaryWeapon(int WeaponIndex, const char* WeaponName,int CurrentAmmo, int MaxAmmo, bool Linked )
{
	for (uint8_t ReportByte : ReportBuffer)
	{
		ReportByte = 0;
	}

	ReportBuffer[0] = (uint8_t) ReportID::REPORT_PRIMARYWEAPON;
	ReportBuffer[1] =  (uint8_t)WeaponIndex; 
	for (auto ReportIndex = 0; ReportIndex < 12; ReportIndex++)
	{
		ReportBuffer[2+ ReportIndex] = WeaponName[ReportIndex];
		if (ReportBuffer[2 + ReportIndex] == 0)
		{
			break;
		}
	}
	ReportBuffer[14] = CurrentAmmo % 10; //ones
	ReportBuffer[15] = (CurrentAmmo /10) % 10; //tens
	ReportBuffer[16] = (CurrentAmmo / 100) % 10; //hundreds
	ReportBuffer[17] = MaxAmmo % 10;         // ones
	ReportBuffer[18] = (MaxAmmo/ 10) % 10;  // tens
	ReportBuffer[19] = (MaxAmmo/ 100) % 10; // hundreds

	ReportBuffer[20] = (uint8_t) Linked;
	hid_write(CurrentDevice, ReportBuffer, 21);
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

	uint8_t ReportBuffer[5] = { 0 };
	ReportBuffer[0] = (uint8_t)ReportID::REPORT_COUNTERMEASURES;
	if (ShowLockWarning)
	{
		ReportBuffer[1] = 0b0111000;
		ReportBuffer[2] = 0b1011100;
		ReportBuffer[3] = 0b0111001;
		ReportBuffer[4] = 0b1110101;
	}
	else 
	{
		ReportBuffer[1] = 0b0111001;
		ReportBuffer[2] = 0b1101101;
		ReportBuffer[3] = Number[(NumberOfCountermeasures / 10) % 10];
		ReportBuffer[4] = Number[NumberOfCountermeasures % 10];
	}

	hid_write(CurrentDevice, ReportBuffer, 5);

}

} // namespace external_display