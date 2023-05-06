﻿#pragma once

#include "CoreMinimal.h"

#include "FGSaveInterface.h"
#include "BFL/KBFL_Struct.h"
#include "Buildables/FGBuildableFactory.h"
#include "Subsystem/ModSubsystem.h"
#include "BFL/KBFL_Util.h"
#include "ResourceNodes/KBFLResourceNodeDescriptor_ResourceNode.h"

#include "KBFLLocationSubsystem.generated.h"

USTRUCT( BlueprintType )
struct FKBFLTransformArrayMap
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TMap< UClass*, FKBFLTransformArray > mMap;
};

UCLASS()
class KBFL_API AKBFLLocationSubsystem : public AModSubsystem
{
	GENERATED_BODY()

public:
	/** DEBUG */
	AKBFLLocationSubsystem();

	UFUNCTION( BlueprintPure, meta = (WorldContext = "WorldContext"), Category="RSS Subsystems" )
	static AKBFLLocationSubsystem* GetAKBFLLocationSubsystem( UObject* WorldContext );

	virtual void BeginPlay() override;

	FString GetFilePath();

	void SaveLocationsToFile();

	UFUNCTION( BlueprintCallable )
	void SaveLocation( UClass* ActorClass, FTransform Transform );

	UFUNCTION( BlueprintCallable )
	void ClearLocation();

	UFUNCTION( BlueprintCallable )
	void RemoveLocation( UClass* ActorClass, FTransform Transform );

	UPROPERTY( EditDefaultsOnly, Category="RSS Subsystems" )
	FString mAddFilePath = "../KMods/";

	UPROPERTY( EditDefaultsOnly, Category="RSS Subsystems" )
	TArray< FString > mCheckParameter = { "Struc", "mTransforms" };

	UPROPERTY()
	TMap< UClass*, FKBFLTransformArray > Mapping;

	UPROPERTY()
	FRotator AddRotation = FRotator( 0 );

	UPROPERTY()
	FVector AddVector = FVector( 0 );

	UPROPERTY()
	FVector Scale = FVector( 1 );

	UPROPERTY()
	UClass* LastClass;
};
