// Fill out your copyright notice in the Description page of Project Settings.
#include "Editor/KBFLEditorSubsystem.h"

#include "AbstractInstanceManager.h"
#include "Buildables/FGBuildable.h"

class AFGBuildable;

void UKBFLEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	FWorldDelegates::OnWorldPreActorTick.AddStatic(&UKBFLEditorSubsystem::OnWorldPreActorTick);
}

void UKBFLEditorSubsystem::OnWorldPreActorTick(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	if (World->WorldType == EWorldType::EditorPreview)
	{
		for (FConstLevelIterator LevelIt = World->GetLevelIterator(); LevelIt; ++LevelIt)
		{
			for (AActor* actor : (*LevelIt)->Actors)
			{
				if (AFGBuildable* Buildable = Cast<AFGBuildable>(actor))
				{
					if (Buildable->mCanContainLightweightInstances && IsValid(Buildable->mInstanceDataCDO) && Buildable->mInstanceHandles.IsEmpty())
					{
						for (FInstanceData InstanceData : Buildable->mInstanceDataCDO->GetInstanceData())
						{
							if (IsValid(InstanceData.StaticMesh) && !InstanceData.OverridenMaterials.Contains(nullptr))
							{
								FInstanceHandle* Handle;
								AAbstractInstanceManager::SetInstanceFromDataStatic(Buildable, FTransform(), InstanceData, Handle, false);
								Buildable->mInstanceHandles.Add(Handle);
							}
						}
					}
				}
			}
		}
	}
}
