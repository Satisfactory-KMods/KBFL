// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "AbstractInstanceManager.h"
#include "Buildables/FGBuildable.h"
#include "Subsystem/ModSubsystem.h"
#include "KBFLInstanceWorldActorSubsystem.generated.h"

UCLASS()
class KBFL_API AKBFLInstanceWorldActorSubsystem: public AModSubsystem {
	GENERATED_BODY()

	public:
		// Sets default values for this actor's properties
		AKBFLInstanceWorldActorSubsystem();

		UFUNCTION(BlueprintPure, Category = "Subsystem", DisplayName = "GetKBFLInstanceWorldActorSubsystem", meta = ( DefaultToSelf = "WorldContext" ))
		static AKBFLInstanceWorldActorSubsystem* Get(UObject* WorldContext);

		UFUNCTION(BlueprintCallable)
		void AddInstances(UStaticMesh* Mesh, TArray<FTransform> Locations);

		UFUNCTION(BlueprintCallable, Category="KMods")
		static bool KBFL_OverwriteInstanceData(AFGBuildable* Buildable, UStaticMesh* Mesh, int32 Idx);

		UFUNCTION(BlueprintCallable, Category="KMods")
		static bool KBFL_OverwriteInstanceData_Transform(AFGBuildable* Buildable, UStaticMesh* Mesh, FTransform NewRelativTransform, int32 Idx);

		UFUNCTION(BlueprintCallable, Category="KMods")
		static bool KBFL_UpdateCustomFloat(AFGBuildable* Buildable, int32 FloatIndex, float Data, int32 InstanceIdx, bool MarkDirty = true);

		UFUNCTION(BlueprintCallable, Category="KMods")
		static bool KBFL_UpdateCustomFloatAsColor(AFGBuildable* Buildable, int32 StartFloatIndex, FLinearColor Data, int32 InstanceIdx, bool MarkDirty = true);

		UFUNCTION(BlueprintCallable, Category="KMods")
		static bool KBFL_SetInstanceHidden(AFGBuildable* Buildable, int32 InstanceIdx, bool IsHidden);

		UFUNCTION(BlueprintCallable, Category="KMods")
		static bool KBFL_SetInstanceWorldTransform(AFGBuildable* Buildable, int32 InstanceIdx, FTransform Transform);

	private:
		TArray<FInstanceHandle*> mCachedInstances;
};
