// Copyright Coffee Stain Studios. All Rights Reserved.


#include "Subsystems/ResourceNodes/KBFLInstanceMeshSpawnDescriptor.h"

#include "KBFLLogging.h"
#include "BFL/KBFL_Util.h"
#include "Subsystems/KBFLInstanceWorldActorSubsystem.h"

void UKBFLInstanceMeshSpawnDescriptor::ForeachLocations(TArray<AActor*>& ActorArray)
{
	if (AKBFLInstanceWorldActorSubsystem* Subsystem = Cast<AKBFLInstanceWorldActorSubsystem>(
		UKBFL_Util::GetSubsystemFromChild(GetWorld(), AKBFLInstanceWorldActorSubsystem::StaticClass())))
	{
		for (FMeshInformationSpawn InstanceInfo : mInstanceInfos)
		{
			if (InstanceInfo.mStaticMesh)
			{
				UE_LOG(KBFLInstanceSpawnerLog, Warning,
					TEXT("UKBFLInstanceMeshSpawnDescriptor: Instances for mesh %s will be added (total: %d)"),
					*InstanceInfo.mStaticMesh->GetName(), InstanceInfo.mLocations.Num());
				Subsystem->AddInstances(InstanceInfo.mStaticMesh, InstanceInfo.mLocations);
				UE_LOG(KBFLInstanceSpawnerLog, Warning,
					TEXT("UKBFLInstanceMeshSpawnDescriptor: Instances for mesh %s added (total: %d)"),
					*InstanceInfo.mStaticMesh->GetName(), InstanceInfo.mLocations.Num());
			}
			else
			{
				UE_LOG(KBFLInstanceSpawnerLog, Error, TEXT("UKBFLInstanceMeshSpawnDescriptor: InstanceInfo.mStaticMesh is not Valid!"))
			}
		}
	}
	else
	{
		UE_LOG(KBFLInstanceSpawnerLog, Error, TEXT("UKBFLInstanceMeshSpawnDescriptor: Cant find AKBFLInstanceWorldActorSubsystem!!"))
	}
}