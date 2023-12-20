// Copyright Coffee Stain Studios. All Rights Reserved.


#include "Subsystems/ResourceNodes/KBFLSubLevelSpawning.h"

#include "FGSaveSession.h"
#include "FGWorldSettings.h"
#include "Engine/LevelStreamingAlwaysLoaded.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/KBFLResourceNodeSubsystem.h"
#include "Subsystems/ResourceNodes/ResourceNodesLogging.h"

UWorld* UKBFLSubLevelSpawning::GetWorld() const {
	if(mSubsystem) {
		if(mSubsystem->GetWorld()) {
			return mSubsystem->GetWorld();
		}
	}
	return Super::GetWorld();
}

void UKBFLSubLevelSpawning::InitSpawning() {
	if(CheckWorld()) {
		UE_LOG(SubLevelSpawnerLog, Log, TEXT("Init SubLevelSpawning %s"), *GetName());
		SpawnSubLevel();
		StreamingLevelReceived();
	} else {
		UE_LOG(SubLevelSpawnerLog, Log, TEXT("Skip SubLevelSpawning because wrong world %s"), *GetName());
	}
}

void UKBFLSubLevelSpawning::SpawnSubLevel() {
	UE_LOG(SubLevelSpawnerLog, Log, TEXT("Try to load total of %d levels"), mSubLevelArray.Num());
	for(TSoftObjectPtr<UWorld> Level: mSubLevelArray) {
		// Load the World and store the ref to avoid Garb.
		mCachedWorlds.AddUnique(Level.LoadSynchronous());

		FString LevelName = Level.GetLongPackageName();
		UE_LOG(SubLevelSpawnerLog, Log, TEXT("Try to load Level %s"), *LevelName);

		// Create ULevelStreamingAlwaysLoaded because we want a always loaded stream
		if(ULevelStreamingAlwaysLoaded* StreamingLevel = static_cast<ULevelStreamingAlwaysLoaded*>(StaticConstructObject_Internal(ULevelStreamingAlwaysLoaded::StaticClass(), GetWorld(), FName(*Level.GetAssetName())))) {
			// Associate a package name.
			StreamingLevel->SetWorldAssetByPackageName(FName(LevelName));
			if(GetWorld()->IsPlayInEditor()) {
				const FWorldContext WorldContext = GEngine->GetWorldContextFromWorldChecked(GetWorld());
				StreamingLevel->RenameForPIE(WorldContext.PIEInstance);
			}
			StreamingLevel->LevelColor = FColor::MakeRandomColor();
			StreamingLevel->bShouldBlockOnLoad = false;
			StreamingLevel->SetShouldBeLoaded(true);
			StreamingLevel->SetShouldBeVisible(true);
			StreamingLevel->SetPriority(-100);
			StreamingLevel->LevelTransform = FTransform();
			StreamingLevel->PackageNameToLoad = FName(LevelName);

			FString PackageFileName;
			if(!FPackageName::DoesPackageExist(StreamingLevel->PackageNameToLoad.ToString(), nullptr, &PackageFileName)) {
				UE_LOG(SubLevelSpawnerLog, Error, TEXT("Invalid Level: %s"), *StreamingLevel->PackageNameToLoad.ToString());
				return;
			}
			StreamingLevel->PackageNameToLoad = FName(*FPackageName::FilenameToLongPackageName(PackageFileName));


			// Add Stream to World (Unique so dont need a check if its already exsists
			GetWorld()->AddUniqueStreamingLevel(StreamingLevel);

			// Add Callback if the Level Was fully loaded to applay the Actors in UFGSaveSession
			FLatentActionInfo LatentInfo;
			LatentInfo.UUID = FMath::RandRange(0, 100000);
			LatentInfo.Linkage = LatentInfo.UUID;
			LatentInfo.CallbackTarget = this;
			LatentInfo.ExecutionFunction = FName("StreamingLevelReceived");

			// Load the Level
			UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(), Level, true, true, LatentInfo);
			StreamingLevel->SetShouldBeLoaded(true);

			// Get the Streaming Level again to test if its was success

			if(ULevelStreaming* NewLoadedLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), FName(LevelName))) {
				UE_LOG(SubLevelSpawnerLog, Log, TEXT("Level Added to Current world: %s"), *StreamingLevel->PackageNameToLoad.ToString());
				// Store ref to avoid Garb.
				mLevelStreaming.Add(NewLoadedLevel);
			}
		}
	}
}

void UKBFLSubLevelSpawning::Reset() {
	// Empty all Ref if we load to a other savegame
	mSubsystem = nullptr;
	mLevelStreaming.Empty();
	mAddedLevel.Empty();
	mCachedWorlds.Empty();
	mCachedLevels.Empty();
}

void UKBFLSubLevelSpawning::StreamingLevelReceived() {
	for(ULevelStreaming* LevelStreaming: mLevelStreaming) {
		if(LevelStreaming) {
			// Check if we allready add that stuff to the Session
			if(mAddedLevel.Contains(LevelStreaming)) {
				continue;
			}

			// Check if we have a valid Level
			if(LevelStreaming->HasLoadedLevel()) {
				// make sure that the Level is loaded should not needed because we use ULevelStreamingAlwaysLoaded
				LevelStreaming->SetShouldBeLoaded(true);
				ULevel* LoadLevel = LevelStreaming->GetLoadedLevel();

				// Adding the level again to the world if it NOT exsists to make sue that the nodes will shown in the world! "Levels.AddUnique( InLevel );"
				GetWorld()->AddLevel(LoadLevel);

				// Todo: get this to work again


				// Get our AFGWorldSettings if can cast to it.
				/*	if(AFGWorldSettings* Settings = Cast<AFGWorldSettings>(LoadLevel->GetWorldSettings())) {
						// We makr the Save Actors as Dirty and prepare the Level to make sure that the array has all actors!
						Settings->mSaveActorsDirty = true;
						Settings->PrepareSaveActors();
						UE_LOG(SubLevelSpawnerLog, Log, TEXT("Save Actors: %d"), Settings->GetSaveActors().Num());
	
						// Get our Session... Should always Valid! > no check needed.
						UE_LOG(SubLevelSpawnerLog, Log, TEXT("LevelStreaming is loaded"));
						UFGSaveSession* Session = UFGSaveSession::Get(GetWorld());
	
						// Is a check for MP on Client looks like the Session is dont needed bc we also never save on Client side!
						if(!ensureAlwaysMsgf(Session, TEXT("Session is invalid Skipp StreamingLevel (Maybe Client?)"))) {
							mAddedLevel.Add(LevelStreaming);
							return;
						}
	
						// We add the world to the Session that all information will create
						UE_LOG(SubLevelSpawnerLog, Log, TEXT("Session->OnLevelAddedToWorld > %s"), *LoadLevel->GetName());
						Session->OnLevelAddedToWorld(LoadLevel, GetWorld());
						if(Session && LoadLevel) {
							// Manuell add our PersistentActors because our level is not "Persistent" but it is tbh.
							UE_LOG(SubLevelSpawnerLog, Log, TEXT("Add Level actors to mPersistentAndRuntimeLoadedObjects"));
							for(AActor* SaveActor: Settings->GetSaveActors()) {
								if(Session->mPersistentAndRuntimeLoadedObjects.AddUnique(SaveActor) > 0) {
									if(mDetailedDebug) {
										UE_LOG(SubLevelSpawnerLog, Log, TEXT("Add %s to Session"), *SaveActor->GetName());
									}
								} else if(mDetailedDebug) {
									UE_LOG(SubLevelSpawnerLog, Warning, TEXT("Cannot add %s to Session because in not Unique and allready exsists!"), *SaveActor->GetName());
								}
							}
	
							// Only for debug if our level was added.
							if(mDetailedDebug) {
								for(TTuple<FString , FPerLevelSaveData*> Data: Session->mPerLevelDataMap) {
									UE_LOG(SubLevelSpawnerLog, Warning, TEXT("%s > %d"), *Data.Key, Data.Value->IsPersistentLevelData);
								}
							}
	
							// add this level that its not added again.
							mAddedLevel.Add(LevelStreaming);
						}
					}*/
			} else {
				UE_LOG(SubLevelSpawnerLog, Log, TEXT("!HasLoadedLevel"));
			}
		}
	}
}

bool UKBFLSubLevelSpawning::CheckWorld() const {
	if(GetWorld()) {
#if WITH_EDITOR
		return FString("UEDPIE_0_").Append(mMapName) != GetWorld()->GetMapName();
#else
		return mMapName == GetWorld()->GetMapName();
#endif
	}
	return false;
}

bool UKBFLSubLevelSpawning::ExecuteAllowed_Implementation() const {
	// for BP uses for example config etc.
	return true;
}
