#pragma once
#include "Subsystems/ResourceNodes/KBFLActorSpawnDescriptorBase.h"

#include "Subsystems/KBFLResourceNodeSubsystem.h"
#include "Subsystems/ResourceNodes/ResourceNodesLogging.h"

UWorld* UKBFLActorSpawnDescriptorBase::GetWorld() const {
	if(mSubsystem) {
		if(mSubsystem->GetWorld()) {
			return mSubsystem->GetWorld();
		}
	}
	return Super::GetWorld();
}

void UKBFLActorSpawnDescriptorBase::BeginSpawning() {
	UE_LOG(ActorSpawnerLog, Log, TEXT("BeginSpawning"));
	if(CheckWorld() && mSubsystem && ExecuteAllowed()) {
		ModifyValues();

		for(TSubclassOf<AActor> ActorClass: GetSearchingActorClasses()) {
			TArray<AActor*> Founded;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActorClass, Founded);
			if(Founded.Num() > 0) {
				mAllActors.Append(Founded);
			}
		}

		if(GetActorClass() && GetActorFreeClass()) {
			UE_LOG(ActorSpawnerLog, Log, TEXT("BeginSpawning > Classes Valid"));

			TArray<AActor*> ActorArray;
			ForeachLocations(ActorArray);
			AfterSpawning();

			if(mRemoveOld) {
				RemoveWrongActors(ActorArray);
			}
		}
	}

	// free for garbage collector 
	this->RemoveFromRoot();
	this->MarkPendingKill();
}

void UKBFLActorSpawnDescriptorBase::ForeachLocations(TArray<AActor*>& ActorArray) {
}

bool UKBFLActorSpawnDescriptorBase::CheckActorInRange(FTransform Transform, AActor*& OutActor) {
	const TArray<AActor*> ActorsToIgnore = {};
	TArray<AActor*>       OutActors = {};

	const bool Free = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Transform.GetLocation(), mCheckRange, GetSphereCheckChannels(), GetActorClass(), ActorsToIgnore, OutActors);

	if(Free && OutActors.Num() > 0) {
		OutActor = OutActors[0];
		ModifyCheckActor(OutActor, Transform);
	} else {
		if(mAllActors.Num() > 0) {
			for(AActor* Actor: mAllActors) {
				if(ensure(Actor)) {
					if(Actor->GetClass() != GetActorClass()) {
						continue;
					}

					if(FVector::Distance(Actor->GetActorLocation(), Transform.GetLocation()) < mCheckRange) {
						UE_LOG(ActorSpawnerLog, Error, TEXT("Found Actor only by Distance Checking!"));
						OutActor = Actor;
						ModifyCheckActor(OutActor, Transform);
						return true;
					}
				}
			}
		}
	}

	UE_LOG(ActorSpawnerLog, Warning, TEXT("CheckActorInRange > found actor count: %d ; GetActorClass() = %s, mCheckRange = %f, GetLocation = %s"), OutActors.Num(), *GetActorClass()->GetName(), mCheckRange, *Transform.GetLocation().ToString());
	return Free;
}

void UKBFLActorSpawnDescriptorBase::ModifyCheckActor(AActor*& InActor, FTransform FoundTransform) {
	UE_LOG(ActorSpawnerLog, Warning, TEXT("ModifyCheckActor, %s"), *InActor->GetName());
	if(mAllowToMove) {
		InActor->SetActorTransform(FoundTransform);
	}
}

bool UKBFLActorSpawnDescriptorBase::IsRangeFree(FTransform Transform) {
	if(!mPreventSpawningByOverlapFreeActorClass) {
		return true;
	}

	const TArray<AActor*> ActorsToIgnore = {};
	TArray<AActor*>       OutActors = {};

	const bool Free = !UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Transform.GetLocation(), mCheckRange, GetSphereCheckChannels(), GetActorFreeClass(), ActorsToIgnore, OutActors);

	UE_LOG(ActorSpawnerLog, Warning, TEXT("IsRangeFree SPHERE, %d"), Free && OutActors.Num() == 0);
	return Free && OutActors.Num() == 0;
}

void UKBFLActorSpawnDescriptorBase::RemoveWrongActors(TArray<AActor*>& ActorArray) {
	if(GetActorClass()) {
		for(TActorIterator Actor(GetWorld(), GetActorClass()); Actor; ++Actor) {
			AActor* ActorPointer = *Actor;
			bool    IsActorCorrect = ActorArray.Contains(ActorPointer);

			if(!IsActorCorrect && IsAllowedToRemoveActor(ActorPointer)) {
				UE_LOG(ActorSpawnerLog, Log, TEXT("Remove Actors > Destroy wrong at, %s"), *ActorPointer->GetTransform().ToString());
				Actor->Destroy();
			}
		}
	}
}

void UKBFLActorSpawnDescriptorBase::ApplyMaterialData(AActor* Actor, TMap<uint8 , UMaterialInterface*> MaterialInfo) {
	UE_LOG(ActorSpawnerLog, Log, TEXT("ApplyMaterialData <%s> | %d"), *Actor->GetName(), MaterialInfo.Num());
	if(Actor && MaterialInfo.Num() > 0) {
		TArray<UActorComponent*> PrimitiveComponent = Actor->GetComponentsByClass(UStaticMeshComponent::StaticClass());
		if(PrimitiveComponent.Num() > 0) {
			UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(PrimitiveComponent[0]);
			if(StaticMesh) {
				for(auto Info: MaterialInfo) {
					if(StaticMesh->GetNumMaterials() > Info.Key && Info.Value) {
						StaticMesh->SetMaterial(Info.Key, Info.Value);
					}
				}
			}
		}
	}
}

TArray<TSubclassOf<AActor>> UKBFLActorSpawnDescriptorBase::GetSearchingActorClasses() {
	return {};
}

bool UKBFLActorSpawnDescriptorBase::IsAllowedToRemoveActor(AActor* InActor) {
	if(InActor) {
		return true;
	}
	return false;
}

bool UKBFLActorSpawnDescriptorBase::CheckWorld() const {
	if(GetWorld()) {
#if WITH_EDITOR
		return FString("UEDPIE_0_").Append(mMapName) != GetWorld()->GetMapName();
#else
		return mMapName == GetWorld()->GetMapName();
#endif
	}
	return false;
}

bool UKBFLActorSpawnDescriptorBase::ExecuteAllowed_Implementation() const {
	return true;
}

TSubclassOf<AActor> UKBFLActorSpawnDescriptorBase::GetActorClass() {
	return AActor::StaticClass();
}

TSubclassOf<AActor> UKBFLActorSpawnDescriptorBase::GetActorFreeClass() {
	return mActorFreeClass ? mActorFreeClass : GetActorClass();
}

AActor*        UKBFLActorSpawnDescriptorBase::SpawnActorAtLocation(FTransform Transform, TSubclassOf<AActor> ClassToSpawn) {
	if(AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(ClassToSpawn, Transform)) {
		ModifySpawnedActorPreSpawn(NewActor);
		NewActor->FinishSpawning(Transform, true);
		ModifySpawnedActorPostSpawn(NewActor);

		UE_LOG(ActorSpawnerLog, Log, TEXT("Actor Spawned <%s> | %s"), *Transform.ToString(), *NewActor->GetName());

		return NewActor;
	}
	return nullptr;
}

void UKBFLActorSpawnDescriptorBase::ModifySpawnedActorPreSpawn(AActor*& InActor) {
	//
}

void UKBFLActorSpawnDescriptorBase::ModifySpawnedActorPostSpawn(AActor*& InActor) {
	//
}

void UKBFLActorSpawnDescriptorBase::AfterSpawning() {
	//
}

TArray<TEnumAsByte<EObjectTypeQuery>> UKBFLActorSpawnDescriptorBase::GetSphereCheckChannels() {
	return mObjectTypeQuery;
}

void UKBFLActorSpawnDescriptorBase::SetSphereCheckChannels(TArray<TEnumAsByte<EObjectTypeQuery>> Channels) {
	mObjectTypeQuery = Channels;
}
