#pragma once

#include "CoreMinimal.h"

#include "FGSchematic.h"
#include "KBFL_Struct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "KBFL_Schematics.generated.h"

UCLASS()
class KBFL_API UKBFL_Schematics: public UBlueprintFunctionLibrary {
	GENERATED_BODY()

	public:
		/** Get all Schematics in game */
		UFUNCTION(BlueprintPure, Category="KMods|Schematics", meta = (WorldContext = "WorldContext"))
		static TArray<TSubclassOf<UFGSchematic>> GetAllRegisteredSchematics(UObject* WorldContext);

		/** Get all Schematics in game */
		UFUNCTION(BlueprintPure, Category="KMods|Schematics", meta = (WorldContext = "WorldContext"))
		static TArray<FKBFLSchematicInfo> GetAllRegisteredSchematicsWithInfos(UObject* WorldContext);

		/** Get all Schematics in game from a Mod */
		UFUNCTION(BlueprintPure, Category="KMods|Schematics", meta = (WorldContext = "WorldContext"))
		static TArray<TSubclassOf<UFGSchematic>> GetAllModSchematics(UObject* WorldContext, FName ModName);

		/** Get all Schematics in game from Mods */
		UFUNCTION(BlueprintPure, Category="KMods|Schematics", meta = (WorldContext = "WorldContext"))
		static TArray<TSubclassOf<UFGSchematic>> GetAllModsSchematics(UObject* WorldContext, TArray<FName> ModNames);

		/** Get all Schematics in game from Mods with Infos from where it from [!Use not Subsystem = Slower!] */
		UFUNCTION(BlueprintPure, Category="KMods|Schematics", meta = (WorldContext = "WorldContext"))
		static TArray<FKBFLSchematicInfo> GetAllModsSchematicsWithInfo(UObject* WorldContext, TArray<FName> ModNames);
};
