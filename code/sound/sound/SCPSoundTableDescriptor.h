#pragma once

#include "parse/SCPTableFormatDescriptor.h"

// clang-format off
namespace SCP
{

	SCPTableFormatDescriptor SoundsTableFile;
	auto SoundSetEntry = SoundsTableFile.Define("SoundSetEntry", 
		SoundsTableFile.Literal("$Name:")
		& SoundsTableFile.Optional(SoundsTableFile.DefinedLiteral("+nocreate"))
		& SoundsTableFile.Define(SoundsTableFile.GetVariableName("EntryIdentifier"), SoundsTableFile.WordDelimited(" \n\t))
		& SoundsTableFile.RequiredVariable("+Entry")
		& SoundsTableFile.OptionalVariable("+Cycle type")
		& SoundsTableFile.OptionalVariable("+Preload")
		& SoundsTableFile.OptionalVariable("+Volume")
			& SoundsTableFile.Optional(
				SoundsTableFile.ObjectHeader("+3D Sound")
				& SoundsTableFile.RequiredVariable("+Attenuation Start")
				& SoundsTableFile.RequiredVariable("+Attenuation End")
			)
		& SoundsTableFile.OptionalVariable("+Priority")
		& SoundsTableFile.OptionalVariable("+Limit")
		& SoundsTableFile.OptionalVariable("+Pitch")
	);

	auto SCPSyntaxEntry = SoundsTableFile.Define("SCPSyntaxEntry",
		SoundsTableFile.Literal("$Name:")
		& SoundsTableFile.Optional(SoundsTableFile.DefinedLiteral("+nocreate"))
		& SoundsTableFile.Define(SoundsTableFile.GetVariableName("EntryIdentifier"), SoundsTableFile.WordDelimited(" \n\t))
		& SoundsTableFile.RequiredVariable("+FileName")
		& SoundsTableFile.OptionalVariable("+Preload")
		& SoundsTableFile.OptionalVariable("+Volume")
			& SoundsTableFile.Optional(
				SoundsTableFile.ObjectHeader("+3D Sound")
				& SoundsTableFile.RequiredVariable("+Attenuation Start")
				& SoundsTableFile.RequiredVariable("+Attenuation End")
			)
		& SoundsTableFile.OptionalVariable("+Priority")
		& SoundsTableFile.OptionalVariable("+Limit")	
	);


	auto RetailGameInterfaceEntry = SoundsTableFile.Define("RetailGameInterfaceEntry",
		SoundsTableFile.Literal("$Name:")
		& SoundsTableFile.Define("SoundIndex", peg::oom(peg::cls("0-9")))
		& SoundsTableFile.Define("RetailFileName", SoundsTableFile.WordDelimited(","))
		& SoundsTableFile.Define("RetailPreloaded", SoundsTableFile.WordDelimited(","))
		& SoundsTableFile.Define("RetailVolume"), SoundsTableFile.WordDelimited(","))
		& SoundsTableFile.Optional(
			SoundsTableFile.OneOf(
				SoundsTableFile.Define("RetailGameSoundType", 
					SoundsTableFile.OneOf(
						SoundsTableFile.Define("StandardSound", 
							SoundsTableFile.Literal("0")
							& SoundsTableFile.Repetition(0, 2,
								SoundsTableFile.Literal(",")
								& peg::oom(peg::cls("0-9"))
							)
						),
						SoundsTableFile.Define("Retail3DSound", 
							SoundsTableFile.Literal("1")
							& SoundsTableFile.Literal(",")
							& SoundsTableFile.GetDecl("Number")
							& SoundsTableFile.Literal(",")
							& SoundsTableFile.GetDecl("Number")
						),
						SoundsTableFile.Define("Aureal3DSound", 
							SoundsTableFile.Literal("2")
							& SoundsTableFile.Literal(",")
							& SoundsTableFile.GetDecl("Number")
							& SoundsTableFile.Literal(",")
							& SoundsTableFile.GetDecl("Number")
						)
					)
				),
				SoundsTableFile.Define("InvalidRetailInterfaceSoundType", 
					peg::oom(peg::cls::("0-9"))
					& SoundsTableFile.Repetition(0, 2,
						SoundsTableFile.Literal(",")
						& peg::oom(peg::cls("0-9"))
					)
				)
			)
		);



	auto GameSoundsSection = SoundsTableFile.Section("Game Sounds", );
	auto InterfaceSoundsSection = SoundsTableFile.Section("Interface Sounds", );
	auto FlybySoundsSection = SoundsTableFile.Section("Flyby Sounds", );

}
// clang-format on