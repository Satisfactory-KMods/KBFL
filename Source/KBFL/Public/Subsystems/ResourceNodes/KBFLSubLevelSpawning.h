// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "KBFLSubLevelSpawning.generated.h"

/**
* Class for adding Sublevel in the world
* Dont need any registration will automaticly added to the world if our subsystem is loaded
*/
UCLASS( Blueprintable, EditInlineNew, abstract, DefaultToInstanced )
class KBFL_API UKBFLSubLevelSpawning : public UObject
{
	GENERATED_BODY()

public:
#if WITH_ENGINE
	virtual UWorld* GetWorld() const override;
#endif

	// Init the Spawning sequenz
	virtual void InitSpawning();

	// Spawn all Level in the world
	virtual void SpawnSubLevel();

	// Called from subsystem to reset Informations
	virtual void Reset();

	// Will Called after InitSpawning() and if the Level was fully added to the world >> Register all saving stuff
	UFUNCTION()
	virtual void StreamingLevelReceived();

	// Check if mMapName match the Map (should only spawn in these map)
	bool CheckWorld() const;

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	bool ExecuteAllowed() const;
	virtual bool ExecuteAllowed_Implementation() const;

	// Map where we want to load our Sublevel
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category="Config" )
	FString mMapName = "Persistent_Level";

	// Do the Player need auth to spawn?
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category="Config" )
	bool mNeedAuth = false;

	// Should we print more debugs?
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category="Config" )
	bool mDetailedDebug = true;

	// All Worlds that need to add to these world
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category="SubLevelSpawing" )
	TArray< TSoftObjectPtr< UWorld > > mSubLevelArray;

	UPROPERTY( Transient, BlueprintReadWrite )
	class UKBFLResourceNodeSubsystem* mSubsystem = nullptr;

	UPROPERTY()
	TArray< ULevelStreaming* > mLevelStreaming;

	UPROPERTY()
	TArray< ULevelStreaming* > mAddedLevel;

	UPROPERTY()
	TArray< UWorld* > mCachedWorlds;

	UPROPERTY()
	TArray< ULevel* > mCachedLevels;
};
