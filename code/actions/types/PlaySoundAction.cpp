//
//

#include "PlaySoundAction.h"
#include "ship/ship.h"

namespace actions {
namespace types {
PlaySoundAction::~PlaySoundAction() = default;
ActionResult PlaySoundAction::execute(ProgramLocals& locals) const
{
	vec3d local_pos;
	if (locals.hostSubsystem != nullptr) {
		get_local_subsystem_pos(&local_pos, locals.host.objp, locals.hostSubsystem);
	} else {
		local_pos = vmd_zero_vector;
	}

	vm_vec_add2(&local_pos, &locals.position);

	vec3d global_pos;
	vm_vec_unrotate(&global_pos, &local_pos, &locals.host.objp->orient);
	vm_vec_add2(&global_pos, &locals.host.objp->pos);

	// Sound is not attached to the host but the object sound system currently only supports persistent sounds
	snd_play_3d(gamesnd_get_game_sound(_soundId), &global_pos, &Eye_position);

	return ActionResult::Finished;
}
void PlaySoundAction::parseValues(const flagset<ProgramParseFlags>& /*parse_flags*/)
{
	_soundId = parse_game_sound_inline();
}
PlaySoundAction* PlaySoundAction::clone() const { return new PlaySoundAction(*this); }
} // namespace types
} // namespace actions
