#pragma once

#include "CoreMinimal.h"
#include "Resources/FGResourceNode.h"

#include "KBFLActorSpawnDescriptorBase.generated.h"

/**
* 
*/
UCLASS(Blueprintable, EditInlineNew, abstract, DefaultToInstanced)
class KBFL_API UKBFLActorSpawnDescriptorBase: public UObject {
	GENERATED_BODY()

	public:
#if WITH_ENGINE
		virtual UWorld* GetWorld() const override;
#endif

		void         BeginSpawning();
		virtual void ForeachLocations(TArray<AActor*>& ActorArray);

		virtual bool    CheckActorInRange(FTransform Transform, AActor*& OutActor);
		virtual void    ModifyCheckActor(AActor*& InActor, FTransform FoundTransform);
		virtual bool    IsRangeFree(FTransform Transform);
		virtual void    RemoveWrongActors(TArray<AActor*>& ActorArray);
		virtual AActor* SpawnActorAtLocation(FTransform Transform, TSubclassOf<AActor> ClassToSpawn);
		virtual void    ModifySpawnedActorPreSpawn(AActor*& InActor);
		virtual void    ModifySpawnedActorPostSpawn(AActor*& InActor);
		virtual void    AfterSpawning();

		virtual TArray<TEnumAsByte<EObjectTypeQuery>> GetSphereCheckChannels();
		void                                          SetSphereCheckChannels(TArray<TEnumAsByte<EObjectTypeQuery>> Channels);

		void ApplyMaterialData(AActor* Actor, TMap<uint8 , UMaterialInterface*> MaterialInfo);

		virtual TArray<TSubclassOf<AActor>> GetSearchingActorClasses();

		virtual bool IsAllowedToRemoveActor(AActor* InActor);
		bool         CheckWorld() const;

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		bool         ExecuteAllowed() const;
		virtual bool ExecuteAllowed_Implementation() const;

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ModifyValues();

		virtual void ModifyValues_Implementation() {
		};

		virtual TSubclassOf<AActor> GetActorClass();
		virtual TSubclassOf<AActor> GetActorFreeClass();

		// Bool
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Config")
		bool mAllowToMove = true;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Config")
		bool mRemoveOld = true;

		UPROPERTY(meta=(NoAutoJson = true))
		bool mPreventSpawningByOverlapFreeActorClass;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Config", meta=(editcondition = "mPreventSpawningByOverlapFreeActorClass"))
		TSubclassOf<AActor> mActorFreeClass = AActor::StaticClass();

		// Floats
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Config")
		float mCheckRange = 500.0f;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Config")
		FString mMapName = "Persistent_Level";

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
		bool mNeedAuth = false;

		UPROPERTY(Transient, BlueprintReadWrite)
		class UKBFLResourceNodeSubsystem* mSubsystem = nullptr;

		UPROPERTY(EditDefaultsOnly, Category="Config|SphereCheckChannels")
		TArray<TEnumAsByte<EObjectTypeQuery>> mObjectTypeQuery = TArray<TEnumAsByte<EObjectTypeQuery>>{ObjectTypeQuery1, ObjectTypeQuery2, ObjectTypeQuery5};

		UPROPERTY()
		TArray<AActor*> mAllActors;
};
