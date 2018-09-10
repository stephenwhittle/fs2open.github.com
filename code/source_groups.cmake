
set(source_files)

# top-level files
add_file_folder(""
	prefix_header.h
)


# Bmpman files
add_file_folder("Bmpman"
	bmpman/bm_internal.h
	bmpman/bmpman.cpp
	bmpman/bmpman.h
)


# CMeasure files
add_file_folder("CMeasure"
	cmeasure/cmeasure.cpp
	cmeasure/cmeasure.h
)

# ddsutils files
add_file_folder("ddsutils"
	ddsutils/ddsutils.cpp
	ddsutils/ddsutils.h
)


# ExternalDLL files
add_file_folder("ExternalDLL"
	external_dll/externalcode.h
)

# ExternalDisplay files
add_file_folder("ExternalDisplay"
	external_display/external_display.h
	external_display/external_display.cpp
	external_display/displaylibrary.h
	external_display/displaylibrary.cpp
	external_display/external_display_internal.h

)

add_file_folder("Generated Files"
	${GENERATED_SOURCE_DIR}/project.h
	${GENERATED_SOURCE_DIR}/scp_compiler_detection.h
	${PLATFORM_CHECK_HEADER}
)

# GlobalIncs files
add_file_folder("GlobalIncs"
	globalincs/alphacolors.cpp
	globalincs/alphacolors.h
	globalincs/fsmemory.h
	globalincs/globals.h
	globalincs/safe_strings.cpp
	globalincs/safe_strings.h
)

IF (WIN32)
	add_file_folder("GlobalIncs"
		${file_root_globalincs}
		globalincs/mspdb_callstack.cpp
		globalincs/mspdb_callstack.h
		globalincs/windebug.cpp
	)
ENDIF(WIN32)


# jpgutils files
add_file_folder("jpgutils"
	jpgutils/jpgutils.cpp
	jpgutils/jpgutils.h
)
add_file_folder("Libs"
)

add_file_folder("Libs\\\\Discord"
	libs/discord/discord.cpp
	libs/discord/discord.h
)
add_file_folder("Libs\\\\RenderDoc"
	libs/renderdoc/renderdoc.cpp
	libs/renderdoc/renderdoc.h
	libs/renderdoc/renderdoc_app.h
)

# Lighting files
add_file_folder("Lighting"
	lighting/lighting.cpp
	lighting/lighting.h
)

# Localization files
add_file_folder("Localization"
	localization/fhash.cpp
	localization/fhash.h
	localization/localize.cpp
	localization/localize.h
)

# MenuUI files
add_file_folder("MenuUI"
	menuui/barracks.cpp
	menuui/barracks.h
	menuui/credits.cpp
	menuui/credits.h
	menuui/fishtank.cpp
	menuui/fishtank.h
	menuui/mainhallmenu.cpp
	menuui/mainhallmenu.h
	menuui/mainhalltemp.cpp
	menuui/mainhalltemp.h
	menuui/optionsmenu.cpp
	menuui/optionsmenu.h
	menuui/optionsmenumulti.cpp
	menuui/optionsmenumulti.h
	menuui/playermenu.cpp
	menuui/playermenu.h
	menuui/readyroom.cpp
	menuui/readyroom.h
	menuui/snazzyui.cpp
	menuui/snazzyui.h
	menuui/techmenu.cpp
	menuui/techmenu.h
	menuui/trainingmenu.cpp
	menuui/trainingmenu.h
)

# Mission files
add_file_folder("Mission"
	mission/missionbriefcommon.cpp
	mission/missionbriefcommon.h
	mission/missioncampaign.cpp
	mission/missioncampaign.h
	mission/missiongoals.cpp
	mission/missiongoals.h
	mission/missiongrid.cpp
	mission/missiongrid.h
	mission/missionhotkey.cpp
	mission/missionhotkey.h
	mission/missionload.cpp
	mission/missionload.h
	mission/missionlog.cpp
	mission/missionlog.h
	mission/missionmessage.cpp
	mission/missionmessage.h
	mission/missionparse.cpp
	mission/missionparse.h
	mission/missiontraining.cpp
	mission/missiontraining.h
	mission/mission_flags.h
)

# MissionUI files
add_file_folder("MissionUI"
	missionui/chatbox.cpp
	missionui/chatbox.h
	missionui/fictionviewer.cpp
	missionui/fictionviewer.h
	missionui/missionbrief.cpp
	missionui/missionbrief.h
	missionui/missioncmdbrief.cpp
	missionui/missioncmdbrief.h
	missionui/missiondebrief.cpp
	missionui/missiondebrief.h
	missionui/missionloopbrief.cpp
	missionui/missionloopbrief.h
	missionui/missionpause.cpp
	missionui/missionpause.h
	missionui/missionscreencommon.cpp
	missionui/missionscreencommon.h
	missionui/missionshipchoice.cpp
	missionui/missionshipchoice.h
	missionui/missionweaponchoice.cpp
	missionui/missionweaponchoice.h
	missionui/redalert.cpp
	missionui/redalert.h
)

# mod_table files
add_file_folder("mod_table"
	mod_table/mod_table.cpp
	mod_table/mod_table.h
)

# Model files
add_file_folder("Model"
	model/model.h
	model/modelanim.cpp
	model/modelanim.h
	model/modelcollide.cpp
	model/modelinterp.cpp
	model/modeloctant.cpp
	model/modelread.cpp
	model/modelrender.h
	model/modelrender.cpp
	model/modelsinc.h
	model/model_flags.h
)

# Nebula files
add_file_folder("Nebula"
	nebula/neb.cpp
	nebula/neb.h
	nebula/neblightning.cpp
	nebula/neblightning.h
)

# Network files
add_file_folder("Network"
	network/chat_api.cpp
	network/chat_api.h
	network/multi.cpp
	network/multi.h
	network/multi_campaign.cpp
	network/multi_campaign.h
	network/multi_data.cpp
	network/multi_data.h
	network/multi_dogfight.cpp
	network/multi_dogfight.h
	network/multi_endgame.cpp
	network/multi_endgame.h
	network/multi_ingame.cpp
	network/multi_ingame.h
	network/multi_kick.cpp
	network/multi_kick.h
	network/multi_log.cpp
	network/multi_log.h
	network/multi_obj.cpp
	network/multi_obj.h
	network/multi_observer.cpp
	network/multi_observer.h
	network/multi_options.cpp
	network/multi_options.h
	network/multi_pause.cpp
	network/multi_pause.h
	network/multi_pinfo.cpp
	network/multi_pinfo.h
	network/multi_ping.cpp
	network/multi_ping.h
	network/multi_pmsg.cpp
	network/multi_pmsg.h
	network/multi_pxo.cpp
	network/multi_pxo.h
	network/multi_rate.cpp
	network/multi_rate.h
	network/multi_respawn.cpp
	network/multi_respawn.h
	network/multi_sexp.cpp
	network/multi_sexp.h
	network/multi_team.cpp
	network/multi_team.h
	network/multi_update.cpp
	network/multi_update.h
	network/multi_voice.cpp
	network/multi_voice.h
	network/multi_xfer.cpp
	network/multi_xfer.h
	network/multilag.cpp
	network/multilag.h
	network/multimsgs.cpp
	network/multimsgs.h
	network/multiteamselect.cpp
	network/multiteamselect.h
	network/multiui.cpp
	network/multiui.h
	network/multiutil.cpp
	network/multiutil.h
	network/psnet2.cpp
	network/psnet2.h
	network/stand_gui.h
)

IF(WIN32)
add_file_folder("Network"
	${file_root_network}
	network/stand_gui.cpp
)
ELSE(WIN32)
add_file_folder("Network"
	${file_root_network}
	network/stand_gui-unix.cpp
)
ENDIF(WIN32)

# Object files
add_file_folder("Object"
	object/collidedebrisship.cpp
	object/collidedebrisweapon.cpp
	object/collideshipship.cpp
	object/collideshipweapon.cpp
	object/collideweaponweapon.cpp
	object/deadobjectdock.cpp
	object/deadobjectdock.h
	object/objcollide.cpp
	object/objcollide.h
	object/object.cpp
	object/object.h
	object/objectdock.cpp
	object/objectdock.h
	object/objectshield.cpp
	object/objectshield.h
	object/objectsnd.cpp
	object/objectsnd.h
	object/objectsort.cpp
	object/parseobjectdock.cpp
	object/parseobjectdock.h
	object/waypoint.cpp
	object/waypoint.h
	object/object_flags.h
)

# Observer files
add_file_folder("Observer"
	observer/observer.cpp
	observer/observer.h
)

# Particle files
add_file_folder("Particle"
	particle/particle.cpp
	particle/particle.h
	particle/ParticleEffect.h
	particle/ParticleManager.cpp
	particle/ParticleManager.h
	particle/ParticleSource.cpp
	particle/ParticleSource.h
	particle/ParticleSourceWrapper.cpp
	particle/ParticleSourceWrapper.h
)

add_file_folder("Particle\\\\Effects"
	particle/effects/BeamPiercingEffect.cpp
	particle/effects/BeamPiercingEffect.h
	particle/effects/CompositeEffect.cpp
	particle/effects/CompositeEffect.h
	particle/effects/ConeShape.h
	particle/effects/GenericShapeEffect.h
	particle/effects/ParticleEmitterEffect.cpp
	particle/effects/ParticleEmitterEffect.h
	particle/effects/SingleParticleEffect.cpp
	particle/effects/SingleParticleEffect.h
	particle/effects/SphereShape.h
)

add_file_folder("Particle\\\\Util"
	particle/util/EffectTiming.cpp
	particle/util/EffectTiming.h
	particle/util/ParticleProperties.cpp
	particle/util/ParticleProperties.h
)

# PcxUtils files
add_file_folder("PcxUtils"
	pcxutils/pcxutils.cpp
	pcxutils/pcxutils.h
)

# Physics files
add_file_folder("Physics"
	physics/physics.cpp
	physics/physics.h
)

# PilotFile files
add_file_folder("PilotFile"
	pilotfile/BinaryFileHandler.cpp
	pilotfile/BinaryFileHandler.h
	pilotfile/csg.cpp
	pilotfile/FileHandler.h
	pilotfile/JSONFileHandler.cpp
	pilotfile/JSONFileHandler.h
	pilotfile/csg_convert.cpp
	pilotfile/pilotfile.cpp
	pilotfile/pilotfile.h
	pilotfile/pilotfile_convert.cpp
	pilotfile/pilotfile_convert.h
	pilotfile/plr.cpp
	pilotfile/plr_convert.cpp
)

# Playerman files
add_file_folder("Playerman"
	playerman/managepilot.cpp
	playerman/managepilot.h
	playerman/player.h
	playerman/playercontrol.cpp
)

# pngutils files
add_file_folder("pngutils"
	pngutils/pngutils.cpp
	pngutils/pngutils.h
)

# Popup files
add_file_folder("Popup"
	popup/popup.cpp
	popup/popup.h
	popup/popupdead.cpp
	popup/popupdead.h
)

# Radar files
add_file_folder("Radar"
	radar/radar.cpp
	radar/radar.h
	radar/radardradis.cpp
	radar/radardradis.h
	radar/radarorb.cpp
	radar/radarorb.h
	radar/radarsetup.cpp
	radar/radarsetup.h
)

# Render files
add_file_folder("Render"
	render/3d.h
	render/3dclipper.cpp
	render/3ddraw.cpp
	render/3dinternal.h
	render/3dlaser.cpp
	render/3dmath.cpp
	render/3dsetup.cpp
	render/batching.cpp
	render/batching.h
)

add_file_folder("Scripting"
	scripting/ade.cpp
	scripting/ade.h
	scripting/ade_api.h
	scripting/ade_args.cpp
	scripting/ade_args.h
	scripting/lua.cpp
	scripting/scripting.cpp
	scripting/scripting.h
)

add_file_folder("Scripting\\\\Api"
	scripting/api/LuaEventCallback.cpp
	scripting/api/LuaEventCallback.h
)

add_file_folder("Scripting\\\\Api\\\\Libs"
	scripting/api/libs/audio.cpp
	scripting/api/libs/audio.h
	scripting/api/libs/base.cpp
	scripting/api/libs/base.h
	scripting/api/libs/bitops.cpp
	scripting/api/libs/bitops.h
	scripting/api/libs/cfile.cpp
	scripting/api/libs/cfile.h
	scripting/api/libs/graphics.cpp
	scripting/api/libs/graphics.h
	scripting/api/libs/hookvars.cpp
	scripting/api/libs/hookvars.h
	scripting/api/libs/hud.cpp
	scripting/api/libs/hud.h
	scripting/api/libs/mission.cpp
	scripting/api/libs/mission.h
	scripting/api/libs/parse.cpp
	scripting/api/libs/parse.h
	scripting/api/libs/tables.cpp
	scripting/api/libs/tables.h
	scripting/api/libs/testing.cpp
	scripting/api/libs/testing.h
    scripting/api/libs/time_lib.cpp
    scripting/api/libs/time_lib.h
	scripting/api/libs/utf8.cpp
	scripting/api/libs/utf8.h
)

add_file_folder("Scripting\\\\Api\\\\Objs"
	scripting/api/objs/asteroid.cpp
	scripting/api/objs/asteroid.h
	scripting/api/objs/background_element.cpp
	scripting/api/objs/background_element.h
	scripting/api/objs/beam.cpp
	scripting/api/objs/beam.h
	scripting/api/objs/camera.cpp
	scripting/api/objs/camera.h
	scripting/api/objs/cockpit_display.cpp
	scripting/api/objs/cockpit_display.h
	scripting/api/objs/control_info.cpp
	scripting/api/objs/control_info.h
	scripting/api/objs/controls.cpp
	scripting/api/objs/controls.h
	scripting/api/objs/debris.cpp
	scripting/api/objs/debris.h
	scripting/api/objs/enums.cpp
	scripting/api/objs/enums.h
	scripting/api/objs/event.cpp
	scripting/api/objs/event.h
	scripting/api/objs/eye.cpp
	scripting/api/objs/eye.h
	scripting/api/objs/file.cpp
	scripting/api/objs/file.h
	scripting/api/objs/font.cpp
	scripting/api/objs/font.h
	scripting/api/objs/gameevent.cpp
	scripting/api/objs/gameevent.h
	scripting/api/objs/gamestate.cpp
	scripting/api/objs/gamestate.h
	scripting/api/objs/hudgauge.cpp
	scripting/api/objs/hudgauge.h
	scripting/api/objs/LuaSEXP.cpp
	scripting/api/objs/LuaSEXP.h
	scripting/api/objs/mc_info.cpp
	scripting/api/objs/mc_info.h
	scripting/api/objs/message.cpp
	scripting/api/objs/message.h
	scripting/api/objs/model.cpp
	scripting/api/objs/model.h
	scripting/api/objs/movie_player.cpp
	scripting/api/objs/movie_player.h
	scripting/api/objs/object.cpp
	scripting/api/objs/object.h
	scripting/api/objs/order.cpp
	scripting/api/objs/order.h
	scripting/api/objs/particle.cpp
	scripting/api/objs/particle.h
	scripting/api/objs/physics_info.cpp
	scripting/api/objs/physics_info.h
	scripting/api/objs/player.cpp
	scripting/api/objs/player.h
	scripting/api/objs/sexpvar.cpp
	scripting/api/objs/sexpvar.h
	scripting/api/objs/shields.cpp
	scripting/api/objs/shields.h
	scripting/api/objs/ship_bank.cpp
	scripting/api/objs/ship_bank.h
	scripting/api/objs/shipclass.cpp
	scripting/api/objs/shipclass.h
	scripting/api/objs/ship.cpp
	scripting/api/objs/ship.h
	scripting/api/objs/shiptype.cpp
	scripting/api/objs/shiptype.h
	scripting/api/objs/sound.cpp
	scripting/api/objs/sound.h
	scripting/api/objs/species.cpp
	scripting/api/objs/species.h
	scripting/api/objs/streaminganim.cpp
	scripting/api/objs/streaminganim.h
	scripting/api/objs/subsystem.cpp
	scripting/api/objs/subsystem.h
	scripting/api/objs/team.cpp
	scripting/api/objs/team.h
	scripting/api/objs/texture.cpp
	scripting/api/objs/texture.h
	scripting/api/objs/texturemap.cpp
	scripting/api/objs/texturemap.h
    scripting/api/objs/time_obj.cpp
    scripting/api/objs/time_obj.h
	scripting/api/objs/vecmath.cpp
	scripting/api/objs/vecmath.h
	scripting/api/objs/waypoint.cpp
	scripting/api/objs/waypoint.h
	scripting/api/objs/weaponclass.cpp
	scripting/api/objs/weaponclass.h
	scripting/api/objs/weapon.cpp
	scripting/api/objs/weapon.h
	scripting/api/objs/wing.cpp
	scripting/api/objs/wing.h
)

add_file_folder("Scripting\\\\Lua"
	scripting/lua/LuaArgs.cpp
	scripting/lua/LuaArgs.h
	scripting/lua/LuaConvert.cpp
	scripting/lua/LuaConvert.h
	scripting/lua/LuaException.h
	scripting/lua/LuaFunction.cpp
	scripting/lua/LuaFunction.h
	scripting/lua/LuaHeaders.h
	scripting/lua/LuaReference.cpp
	scripting/lua/LuaReference.h
	scripting/lua/LuaTable.cpp
	scripting/lua/LuaTable.h
	scripting/lua/LuaUtil.cpp
	scripting/lua/LuaUtil.h
	scripting/lua/LuaValue.cpp
	scripting/lua/LuaValue.h
)

# Ship files
add_file_folder("Ship"
	ship/afterburner.cpp
	ship/afterburner.h
	ship/awacs.cpp
	ship/awacs.h
	ship/shield.cpp
	ship/ship.cpp
	ship/ship.h
	ship/shipcontrails.cpp
	ship/shipcontrails.h
	ship/shipfx.cpp
	ship/shipfx.h
	ship/shiphit.cpp
	ship/shiphit.h
	ship/subsysdamage.h
	ship/ship_flags.h
)

# Species_Defs files
add_file_folder("Species_Defs"
	species_defs/species_defs.cpp
	species_defs/species_defs.h
)

# Starfield files
add_file_folder("Starfield"
	starfield/nebula.cpp
	starfield/nebula.h
	starfield/starfield.cpp
	starfield/starfield.h
	starfield/supernova.cpp
	starfield/supernova.h
)

# Stats files
add_file_folder("Stats"
	stats/medals.cpp
	stats/medals.h
	stats/scoring.cpp
	stats/scoring.h
	stats/stats.cpp
	stats/stats.h
)

# TgaUtils files
add_file_folder("TgaUtils"
	tgautils/tgautils.cpp
	tgautils/tgautils.h
)

# Tracing files
add_file_folder("Tracing"
	tracing/categories.cpp
	tracing/categories.h
	tracing/FrameProfiler.h
	tracing/FrameProfiler.cpp
	tracing/MainFrameTimer.h
	tracing/MainFrameTimer.cpp
	tracing/Monitor.h
	tracing/Monitor.cpp
	tracing/scopes.cpp
	tracing/scopes.h
	tracing/ThreadedEventProcessor.h
	tracing/TraceEventWriter.h
	tracing/TraceEventWriter.cpp
	tracing/tracing.h
	tracing/tracing.cpp
)

# Ui files
add_file_folder("Ui"
	ui/button.cpp
	ui/checkbox.cpp
	ui/gadget.cpp
	ui/icon.cpp
	ui/inputbox.cpp
	ui/keytrap.cpp
	ui/listbox.cpp
	ui/radio.cpp
	ui/scroll.cpp
	ui/slider.cpp
	ui/slider2.cpp
	ui/ui.h
	ui/uidefs.h
	ui/uidraw.cpp
	ui/uimouse.cpp
	ui/window.cpp
)


# Weapon files
add_file_folder("Weapon"
	weapon/beam.cpp
	weapon/beam.h
	weapon/corkscrew.cpp
	weapon/corkscrew.h
	weapon/emp.cpp
	weapon/emp.h
	weapon/flak.cpp
	weapon/flak.h
	weapon/muzzleflash.cpp
	weapon/muzzleflash.h
	weapon/shockwave.cpp
	weapon/shockwave.h
	weapon/swarm.cpp
	weapon/swarm.h
	weapon/trails.cpp
	weapon/trails.h
	weapon/weapon.h
	weapon/weapons.cpp
	weapon/weapon_flags.h
)

