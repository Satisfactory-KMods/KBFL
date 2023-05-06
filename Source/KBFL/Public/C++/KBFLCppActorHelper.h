#pragma once

#include "CoreMinimal.h"

#include "FGFactoryConnectionComponent.h"
#include "FGPipeConnectionFactory.h"
#include "FGInventoryComponent.h"

#include "KBFLCppActorHelper.generated.h"

UCLASS()
class KBFL_API UKBFLCppActorHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintCallable, Category="KMods|Actor Helper", meta = (WorldContext = "WorldContext") )
	static bool ActorsInSphere( UObject* WorldContext, UClass* ActorClass, FVector Location, TArray< AActor* > Ignore, TArray< AActor* >& OutActors, float Range = 250.0f );

	UFUNCTION( BlueprintCallable, Category="KMods|Actor Helper", meta = (WorldContext = "WorldContext") )
	static bool HasFreeLineToActor( UObject* WorldContext, AActor* ActorSource, AActor* ActorTarget, float ZOffsetSource = 0.0f, float ZOffsetTarget = 0.0f );

	UFUNCTION( BlueprintCallable, Category="KMods|Actor Helper", meta = (WorldContext = "WorldContext") )
	static bool HasFreeLineToComponent( UObject* WorldContext, USceneComponent* ComponentSource, USceneComponent* ComponentTarget, float ZOffsetSource = 0.0f, float ZOffsetTarget = 0.0f );
};
