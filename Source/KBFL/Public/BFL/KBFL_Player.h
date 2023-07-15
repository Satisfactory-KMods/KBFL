#pragma once

#include "CoreMinimal.h"

#include "FGPlayerController.h"
#include "Equipment/FGBuildGun.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "KBFL_Player.generated.h"

UCLASS()
class KBFL_API UKBFL_Player: public UBlueprintFunctionLibrary {
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintCallable, Category="KMods|Util")
		static FString GetPlayerId(AFGPlayerState* PlayerState);

		/** get the build gun from local player */
		UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext"), Category="KMods|Player")
		static AFGBuildGun* GetBuildingGun(UObject* WorldContext);

		/** get the player controller from local player */
		UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext"), Category="KMods|Player")
		static AFGPlayerController* GetFGController(UObject* WorldContext);

		/** get the player character from local player */
		UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext"), Category="KMods|Player")
		static AFGCharacterPlayer* GetFGCharacter(UObject* WorldContext);

		/** get the player State from local player */
		UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext"), Category="KMods|Player")
		static AFGPlayerState* GetFgPlayerState(UObject* WorldContext);

		/** Get buildgun HitResult from local player */
		UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext"), Category="KMods|Player")
		static void GetBuildingGunHitResult(UObject* WorldContext, bool& IsInBuildOrDismantleState, FHitResult& HitResult);

		/** Get buildgun buildstate from local player */
		UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext"), Category="KMods|Player")
		static EBuildGunState GetPlayerBuildState(UObject* WorldContext);
};
