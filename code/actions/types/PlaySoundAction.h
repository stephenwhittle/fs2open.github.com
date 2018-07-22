#pragma once

#include "actions/Action.h"
#include "gamesnd/gamesnd.h"

namespace actions {
namespace types {

class PlaySoundAction : public Action {
	gamesnd_id _soundId;

  public:
	~PlaySoundAction() override;

	ActionResult execute(ProgramLocals& locals) const override;

	void parseValues(const flagset<ProgramParseFlags>& parse_flags) override;

	PlaySoundAction* clone() const override;
};

} // namespace types
} // namespace actions
