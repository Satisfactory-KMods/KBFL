﻿#include "BFL/KBFL_Schematics.h"

#include "BFL/KBFL_Util.h"
#include "Kismet/RuntimeBlueprintFunctionLibrary.h"
#include "Registry/ModContentRegistry.h"

TArray< TSubclassOf< UFGSchematic > > UKBFL_Schematics::GetAllRegisteredSchematics( UObject* WorldContext ) {
	auto ModContent = UModContentRegistry::Get( WorldContext->GetWorld( ) );
	auto Schematics = ModContent->GetRegisteredSchematics( );
	TArray< TSubclassOf< UFGSchematic > > ReturnSchematics = { };

	for( auto Schematic : Schematics ) {
		ReturnSchematics.Add( TSubclassOf< UFGSchematic >( Cast< UClass >( Schematic.RegisteredObject ) ) );
	}

	return ReturnSchematics;
}

TArray< FKBFLSchematicInfo > UKBFL_Schematics::GetAllRegisteredSchematicsWithInfos( UObject* WorldContext ) {
	auto ModContent = UModContentRegistry::Get( WorldContext->GetWorld( ) );
	auto Schematics = ModContent->GetRegisteredSchematics( );
	TArray< FKBFLSchematicInfo > ReturnSchematics = { };

	for( auto Schematic : Schematics ) {
		FKBFLSchematicInfo Struc;
		Struc.mMod = Schematic.OwnedByModReference;
		Struc.mSchematic = TSubclassOf< UFGSchematic >( Cast< UClass >( Schematic.RegisteredObject ) );

		ReturnSchematics.Add( Struc );
	}

	return ReturnSchematics;
}

TArray< TSubclassOf< UFGSchematic > > UKBFL_Schematics::GetAllModSchematics( UObject* WorldContext, FName ModName ) {
	auto ModContent = UModContentRegistry::Get( WorldContext->GetWorld( ) );
	auto Schematics = ModContent->GetRegisteredSchematics( );
	TArray< TSubclassOf< UFGSchematic > > ReturnSchematics = { };

	for( auto Schematic : Schematics ) {
		if( ModName == Schematic.OwnedByModReference ) {
			ReturnSchematics.Add( TSubclassOf< UFGSchematic >( Cast< UClass >( Schematic.RegisteredObject ) ) );
		}
	}

	return ReturnSchematics;
}

TArray< TSubclassOf< UFGSchematic > > UKBFL_Schematics::GetAllModsSchematics( UObject* WorldContext, TArray< FName > ModNames ) {
	auto ModContent = UModContentRegistry::Get( WorldContext->GetWorld( ) );
	auto Schematics = ModContent->GetRegisteredSchematics( );
	TArray< TSubclassOf< UFGSchematic > > ReturnSchematics = { };

	for( auto Schematic : Schematics ) {
		if( ModNames.Contains( Schematic.OwnedByModReference ) ) {
			ReturnSchematics.Add( TSubclassOf< UFGSchematic >( Cast< UClass >( Schematic.RegisteredObject ) ) );
		}
	}

	return ReturnSchematics;
}

TArray< FKBFLSchematicInfo > UKBFL_Schematics::GetAllModsSchematicsWithInfo( UObject* WorldContext, TArray< FName > ModNames ) {
	auto ModContent = UModContentRegistry::Get( WorldContext->GetWorld( ) );
	auto Schematics = ModContent->GetRegisteredSchematics( );
	TArray< FKBFLSchematicInfo > ReturnSchematics = { };

	for( auto Schematic : Schematics ) {
		if( ModNames.Contains( Schematic.OwnedByModReference ) ) {
			// write Struc
			FKBFLSchematicInfo Struc;
			Struc.mMod = Schematic.OwnedByModReference;
			Struc.mSchematic = TSubclassOf< UFGSchematic >( Cast< UClass >( Schematic.RegisteredObject ) );

			ReturnSchematics.Add( Struc );
		}
	}

	return ReturnSchematics;
}
