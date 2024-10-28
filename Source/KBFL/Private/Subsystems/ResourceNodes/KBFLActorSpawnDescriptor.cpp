#pragma once
#include "Subsystems/ResourceNodes/KBFLActorSpawnDescriptor.h"

#include "KBFLLogging.h"

void UKBFLActorSpawnDescriptor::ForeachLocations(TArray<AActor*>& ActorArray)
{
	UE_LOG(KBFLActorSpawnerLog, Warning, TEXT("ForeachLocations Actors"));

	for (FTransform Location : mLocations)
	{
		if (Location.GetLocation() == FVector(0))
		{
			continue;
		}

		UE_LOG(KBFLActorSpawnerLog, Warning, TEXT("Try Actor spawn at: %s"), *Location.ToString());
		AActor* OutActor;
		if (!CheckActorInRange(Location, OutActor) && IsRangeFree(Location))
		{
			UE_LOG(KBFLActorSpawnerLog, Warning,
				TEXT("!CheckActorInRange(Location, OutActor) && IsRangeFree(Location)"));
			SpawnActorAtLocation(Location, GetActorClass());
		}

		if (OutActor)
		{
			ActorArray.AddUnique(OutActor);
		}
	}
}