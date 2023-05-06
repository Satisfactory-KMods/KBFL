#pragma once

#include "CoreMinimal.h"
#include "Module/GameWorldModule.h"
#include "ResourceNodes/KBFLResourceNodeDescriptor_ResourceNode.h"
#include "ResourceNodes/KBFLResourceNodeDescriptor_ResourceWell.h"
#include "Subsystems/WorldSubsystem.h"

#include "KBFLResourceNodeSubsystem.generated.h"


// Native
USTRUCT()
struct FKBFLActorArray
{
	GENERATED_BODY()

	UPROPERTY( Transient )
	TArray< AActor* > mActors;
};

/**
 * 
 */
UCLASS()
class KBFL_API UKBFLResourceNodeSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	/** Implement this for initialization of instances of the system */
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;
	virtual void Deinitialize() override;

public:
	void OnWorldBeginPlay();

	void SpawnSubLevel();
	void GetAllSubLevel( TSet< TSubclassOf< class UKBFLSubLevelSpawning > >& Out ) const;

	void GatherInterfaces();

	UFUNCTION( BlueprintCallable )
	void BeginSpawningForModule( UWorldModule* Module );

	UFUNCTION( Server, WithValidation, Reliable, BlueprintCallable )
	void Server_FinishedSpawningNodes();

	UFUNCTION( BlueprintPure )
	bool WasCalled( UWorldModule* Module ) const;

	UFUNCTION( BlueprintCallable )
	void BeginRemoveActors( TArray< TSubclassOf< AActor > > ActorClasses );

private:
	bool Initialized = false;

	UPROPERTY( Transient )
	TArray< UWorldModule* > mCalledModules;

	UPROPERTY()
	TArray< class UKBFLSubLevelSpawning* > mCalledSubLevelSpawning;
};
