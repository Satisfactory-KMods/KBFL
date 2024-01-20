#pragma once

#include "CoreMinimal.h"
#include "FGFactoryColoringTypes.h"
#include "FGSwatchGroup.h"
#include "Interfaces/KBFLContentCDOHelperInterface.h"
#include "Interfaces/KBFLCustomizerInterface.h"
#include "Module/ModModule.h"

#include "Module/WorldModule.h"
#include "Resources/FGResourceNode.h"
#include "Subsystems/WorldSubsystem.h"
#include "KBFLCustomizerSubsystem.generated.h"


/**
 * 
 */
UCLASS()
class KBFL_API UKBFLCustomizerSubsystem: public UWorldSubsystem, public FTickableGameObject {
	GENERATED_BODY()

	virtual void    Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	public:
		virtual void OnWorldBeginPlay(UWorld& InWorld) override;

		bool GatherDefaultCollections();
		void GatherInterfaces();
		bool RegisterSwatchesInSubsystem(TArray<FKBFLSwatchInformation> SwatchInformations);
		bool RegisterSwatchGroups(TMap<TSubclassOf<UFGSwatchGroup> , TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch>> Map);
		void CDOMaterials(TArray<FKBFLMaterialDescriptorInformation> CDOInformation);

		UFUNCTION(BlueprintCallable)
		void BeginForModule(UWorldModule* Module);

		UFUNCTION(BlueprintCallable)
		bool WasForModuleCalled(UWorldModule* Module) const {
			return mCalled.Contains(Module);
		};

		/**
		* Set Default Swatch to Swatch group (should call early before the player can place something)
		* otherwise it will crash
		*/
		UFUNCTION(BlueprintCallable, Category="KMods|Customizer Subsystem")
		bool SetDefaultToSwatchGroup(TSubclassOf<UFGSwatchGroup> SwatchGroup, TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch> Swatch);

		UFUNCTION(BlueprintPure, Category="KMods|Customizer Subsystem")
		FORCEINLINE TMap<uint8 , TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch>> GetSwatchMap() const { return mSwatchIDMap; }

		bool Initialized = false;
		bool Gathered = false;

	private:
		UPROPERTY()
		TArray<UWorldModule*> mCalled;

		TMap<uint8 , TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch>> mSwatchIDMap;
		TSubclassOf<UFGFactoryCustomizationCollection>                      mDefaultSwatchCollection;
		TSubclassOf<UFGFactoryCustomizationCollection>                      mDefaultMaterialCollection;
		TSubclassOf<UFGFactoryCustomizationCollection>                      mDefaultPatternCollection;
		TSubclassOf<UFGFactoryCustomizationCollection>                      mDefaultSkinCollection;
};
