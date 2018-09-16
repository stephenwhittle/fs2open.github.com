
#include "events/events.h"
#include "hidapi/hidapi.h"
#include "osapi/outwnd.h"


namespace external_display 
{
	void OnRenderCountermeasureGauge(int NumberOfCountermeasures);
	void OnRenderPrimaryWeapon(int WeaponIndex, const char* WeaponName, int CurrentAmmo, int MaxAmmo, bool Linked);

	enum class ReportID
	{
		REPORT_PRIMARYWEAPON = 1
	};

	hid_device* CurrentDevice = nullptr;
	
	void init() 
	{ 
		hid_init();
		CurrentDevice = hid_open(1155, 22352, nullptr);
		events::RenderPrimaryWeapon.add(OnRenderPrimaryWeapon);
		events::RenderCountermeasureGauge.add(OnRenderCountermeasureGauge); 
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
	outwnd_printf2("The number of countermeasures is %d\n", NumberOfCountermeasures);
}

} // namespace external_display