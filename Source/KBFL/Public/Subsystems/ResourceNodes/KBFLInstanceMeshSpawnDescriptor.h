// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "KBFLActorSpawnDescriptor.h"
#include "KBFLInstanceMeshSpawnDescriptor.generated.h"

USTRUCT( BlueprintType )
struct FMeshInformationSpawn
{
	GENERATED_BODY()

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite )
	UStaticMesh* mStaticMesh;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite )
	TArray< FTransform > mLocations;
};

/**
 * 
 */
UCLASS()
class KBFL_API UKBFLInstanceMeshSpawnDescriptor : public UKBFLActorSpawnDescriptorBase
{
	GENERATED_BODY()

	virtual void ForeachLocations( TArray< AActor* >& ActorArray ) override;

public:
	UPROPERTY( BlueprintReadWrite, EditDefaultsOnly )
	TArray< FMeshInformationSpawn > mInstanceInfos;
};
