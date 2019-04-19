#pragma once

#include "globalincs/pstypes.h"
#include "hud/hud.h"
#include "scripting/lua/LuaFunction.h"

class HudGaugeScripting: public HudGauge {
	luacpp::LuaFunction _renderFunction;
  public:
	HudGaugeScripting();

	void render(float frametime) override;

	void initName(std::string name);

	const luacpp::LuaFunction& getRenderFunction() const;
	void setRenderFunction(const luacpp::LuaFunction& renderFunction);
};
