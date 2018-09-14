
#include "events/events.h"
#include "hidapi/hidapi.h"
#include "osapi/outwnd.h"

void OnRenderCountermeasureGauge(int NumberOfCountermeasures);

namespace external_display 
{

	void init() 
	{ 
		events::RenderCountermeasureGauge.add(OnRenderCountermeasureGauge); 
	}
} // namespace external_display

void OnRenderCountermeasureGauge(int NumberOfCountermeasures)
{
	outwnd_printf2("The number of countermeasures is %d\n", NumberOfCountermeasures);
}