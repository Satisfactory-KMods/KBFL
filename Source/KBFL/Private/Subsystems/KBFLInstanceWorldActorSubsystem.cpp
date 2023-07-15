// Copyright Coffee Stain Studios. All Rights Reserved.


#include "Subsystems/KBFLInstanceWorldActorSubsystem.h"

#include "AbstractInstanceManager.h"
#include "BFL/KBFL_Util.h"


// Sets default values
AKBFLInstanceWorldActorSubsystem::AKBFLInstanceWorldActorSubsystem() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

AKBFLInstanceWorldActorSubsystem* AKBFLInstanceWorldActorSubsystem::Get(UObject* WorldContext) {
	return Cast<AKBFLInstanceWorldActorSubsystem>(UKBFL_Util::GetSubsystemFromChild(WorldContext, StaticClass()));
}

void AKBFLInstanceWorldActorSubsystem::AddInstances(UStaticMesh* Mesh, TArray<FTransform> Locations) {
	for(FTransform Transform: Locations) {
		FInstanceData Data;

		Data.RelativeTransform = Transform;
		Data.StaticMesh = Mesh;

		FInstanceHandle* Handle = new FInstanceHandle();

		AAbstractInstanceManager::SetInstanceFromDataStatic(this, FTransform(), Data, Handle);
		mCachedInstances.Add(Handle);
	}
}

bool AKBFLInstanceWorldActorSubsystem::KBFL_OverwriteInstanceData(AFGBuildable* Buildable, UStaticMesh* Mesh, int32 Idx) {
	if(!IsValid(Buildable)) {
		return false;
	}

	if(!IsInGameThread()) {
		FFunctionGraphTask::CreateAndDispatchWhenReady([ &, Buildable, Mesh, Idx ]() {
			KBFL_OverwriteInstanceData(Buildable, Mesh, Idx);
		}, GET_STATID(STAT_TaskGraph_OtherTasks), nullptr, ENamedThreads::GameThread);
		return true;
	}

	if(Idx > INDEX_NONE && IsValid(Mesh)) {
		TArray<FInstanceData> Datas = Buildable->mInstanceDataCDO->GetInstanceData();
		if(Datas.IsValidIndex(Idx)) {
			return KBFL_OverwriteInstanceData_Transform(Buildable, Mesh, Datas[Idx].RelativeTransform, Idx);
		}
	}
	return false;
}

bool AKBFLInstanceWorldActorSubsystem::KBFL_OverwriteInstanceData_Transform(AFGBuildable* Buildable, UStaticMesh* Mesh, FTransform NewRelativTransform, int32 Idx) {
	if(!IsValid(Buildable)) {
		return false;
	}

	if(!IsInGameThread()) {
		FFunctionGraphTask::CreateAndDispatchWhenReady([ &, Buildable, Mesh, NewRelativTransform, Idx ]() {
			KBFL_OverwriteInstanceData_Transform(Buildable, Mesh, NewRelativTransform, Idx);
		}, GET_STATID(STAT_TaskGraph_OtherTasks), nullptr, ENamedThreads::GameThread);
		return true;
	}

	if(Idx > INDEX_NONE && IsValid(Mesh)) {
		AAbstractInstanceManager* Manager = AAbstractInstanceManager::GetInstanceManager(Buildable->GetWorld());
		TArray<FInstanceData>     Datas = Buildable->mInstanceDataCDO->GetInstanceData();
		if(Datas.IsValidIndex(Idx) && Buildable->mInstanceHandles.IsValidIndex(Idx) && IsValid(Manager)) {
			FInstanceData Data = Datas[Idx];
			Data.RelativeTransform = NewRelativTransform;
			Data.StaticMesh = Mesh;

			if(Buildable->mInstanceHandles[Idx]->IsInstanced()) {
				Manager->RemoveInstance(Buildable->mInstanceHandles[Idx]);
			}
			Manager->SetInstanced(Buildable, Buildable->GetActorTransform(), Data, Buildable->mInstanceHandles[Idx]);

			Buildable->ApplyCustomizationData_Native(Buildable->mCustomizationData);
			Buildable->SetCustomizationData_Native(Buildable->mCustomizationData);

			return true;
		}
	}

	return false;
}

bool AKBFLInstanceWorldActorSubsystem::KBFL_UpdateCustomFloat(AFGBuildable* Buildable, int32 FloatIndex, float Data, int32 InstanceIdx, bool MarkDirty) {
	if(!IsValid(Buildable)) {
		return false;
	}

	if(!IsInGameThread()) {
		FFunctionGraphTask::CreateAndDispatchWhenReady([ &, Buildable, FloatIndex, Data, InstanceIdx, MarkDirty ]() {
			KBFL_UpdateCustomFloat(Buildable, FloatIndex, Data, InstanceIdx, MarkDirty);
		}, GET_STATID(STAT_TaskGraph_OtherTasks), nullptr, ENamedThreads::GameThread);
		return true;
	}

	if(Buildable->mInstanceHandles.IsValidIndex(InstanceIdx)) {
		if(Buildable->mInstanceHandles[InstanceIdx]->IsInstanced()) {
			//UE_LOG( LogKPCL, Error, TEXT("mInstanceHandles[ %d ]->SetPrimitiveDataByID( %f, %d, %d ); IsValid(%d), Component(%d), Owner(%d)"), InstanceIdx, Data, FloatIndex, MarkDirty, mInstanceHandles[ InstanceIdx ]->IsValid(), IsValid( mInstanceHandles[ InstanceIdx ]->GetInstanceComponent() ), mInstanceHandles[ InstanceIdx ]->GetOwner() == this )
			Buildable->mInstanceHandles[InstanceIdx]->SetPrimitiveDataByID(Data/** float that we want to set */, FloatIndex /** Index where we want to set */, true);
			return true;
		}
	}

	return false;
}

bool AKBFLInstanceWorldActorSubsystem::KBFL_UpdateCustomFloatAsColor(AFGBuildable* Buildable, int32 StartFloatIndex, FLinearColor Data, int32 InstanceIdx, bool MarkDirty) {
	if(!IsValid(Buildable)) {
		return false;
	}

	if(!IsInGameThread()) {
		FFunctionGraphTask::CreateAndDispatchWhenReady([ &, Buildable, StartFloatIndex, Data, InstanceIdx, MarkDirty ]() {
			KBFL_UpdateCustomFloatAsColor(Buildable, StartFloatIndex, Data, InstanceIdx, MarkDirty);
		}, GET_STATID(STAT_TaskGraph_OtherTasks), nullptr, ENamedThreads::GameThread);
		return true;
	}

	KBFL_UpdateCustomFloat(Buildable, StartFloatIndex, Data.R, InstanceIdx, false);
	KBFL_UpdateCustomFloat(Buildable, StartFloatIndex + 1, Data.G, InstanceIdx, false);
	return KBFL_UpdateCustomFloat(Buildable, StartFloatIndex + 2, Data.B, InstanceIdx, MarkDirty);
}

bool AKBFLInstanceWorldActorSubsystem::KBFL_SetInstanceHidden(AFGBuildable* Buildable, int32 InstanceIdx, bool IsHidden) {
	if(!IsValid(Buildable)) {
		return false;
	}

	if(!IsInGameThread()) {
		FFunctionGraphTask::CreateAndDispatchWhenReady([ &, Buildable, InstanceIdx, IsHidden ]() {
			KBFL_SetInstanceHidden(Buildable, InstanceIdx, IsHidden);
		}, GET_STATID(STAT_TaskGraph_OtherTasks), nullptr, ENamedThreads::GameThread);
		return true;
	}

	if(Buildable->mInstanceHandles.IsValidIndex(InstanceIdx)) {
		if(Buildable->mInstanceHandles[InstanceIdx]->IsInstanced()) {
			if(UHierarchicalInstancedStaticMeshComponent* Hism = Buildable->mInstanceHandles[InstanceIdx]->GetInstanceComponent()) {
				const int32 HandleID = Buildable->mInstanceHandles[InstanceIdx]->GetHandleID();

				FTransform T = Buildable->mInstanceDataCDO->GetInstanceData()[InstanceIdx].RelativeTransform * Buildable->GetActorTransform();
				T.SetScale3D(!IsHidden ? T.GetScale3D() : FVector(1.e-4f));
				Buildable->mInstanceHandles[InstanceIdx]->UpdateTransform(T);
			}
			return true;
		}
	}

	return false;
}

bool AKBFLInstanceWorldActorSubsystem::KBFL_SetInstanceWorldTransform(AFGBuildable* Buildable, int32 InstanceIdx, FTransform Transform) {
	if(!IsValid(Buildable)) {
		return false;
	}

	if(!IsInGameThread()) {
		FFunctionGraphTask::CreateAndDispatchWhenReady([ &, Buildable, InstanceIdx, Transform ]() {
			KBFL_SetInstanceWorldTransform(Buildable, InstanceIdx, Transform);
		}, GET_STATID(STAT_TaskGraph_OtherTasks), nullptr, ENamedThreads::GameThread);
		return true;
	}

	if(Buildable->mInstanceHandles.IsValidIndex(InstanceIdx)) {
		if(Buildable->mInstanceHandles[InstanceIdx]->IsInstanced()) {
			Buildable->mInstanceHandles[InstanceIdx]->UpdateTransform(Transform);
			return true;
		}
	}

	return false;
}
