// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/KBFLContentCDOHelperInterface.h"
#include "Interfaces/KBFLCustomizerInterface.h"
#include "Interfaces/KBFLResourceNodeInterface.h"
#include "Module/GameWorldModule.h"
#include "Module/WorldModule.h"
#include "UObject/Object.h"
#include "KBFLWorldModule.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class KBFL_API UKBFLWorldModule
	: public UGameWorldModule, public IKBFLCustomizerInterface,
	  public IKBFLResourceNodeInterface, public IKBFLContentCDOHelperInterface
{
	GENERATED_BODY()

public:
	UKBFLWorldModule();

	// BEGIN IKBFLContentCDOHelperInterface
	virtual FKBFLCDOInformation
	GetCDOInformationFromPhase_Implementation(ELifecyclePhase Phase, bool& HasPhase) override;

	// END IKBFLContentCDOHelperInterface

	// BEGIN IKBFLResourceNodeInterface
	virtual TArray<TSubclassOf<AActor>> GetRemoveClasses_Implementation() override;

	virtual TArray<TSubclassOf<UKBFLActorSpawnDescriptorBase>> GetActorSpawnDescriptors_Implementation() override;

	// END IKBFLResourceNodeInterface

	// BEGIN IKBFLCustomizerInterface
	virtual TMap<TSubclassOf<UFGSwatchGroup>, TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch>>
	GetSwatchGroups_Implementation() override;

	virtual TArray<FKBFLSwatchInformation> GetSwatchDescriptionInformation_Implementation() override;

	virtual TArray<FKBFLMaterialDescriptorInformation> GetMaterialInformation_Implementation() override;

	// END IKBFLCustomizerInterface

	// BEGIN UGameWorldModule
	virtual void DispatchLifecycleEvent(ELifecyclePhase Phase) override;

	// END UGameWorldModule

	UFUNCTION(BlueprintNativeEvent, Category="LifecyclePhase")
	void ConstructionPhase();

	UFUNCTION(BlueprintNativeEvent, Category="LifecyclePhase")
	void InitPhase();

	UFUNCTION(BlueprintNativeEvent, Category="LifecyclePhase")
	void PostInitPhase();

	virtual void RegisterKBFLLogicContent();

	virtual void FindAllCDOs();
	
	virtual bool IsAllowedToRegister(TSubclassOf<UObject> Object) const;

	bool bScanForCDOsDone = false;
	/** Information for CDO's */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|CDOHelper")
	TMap<ELifecyclePhase, FKBFLCDOInformation> mCDOInformationMap;

	/** Material Information for add to SF Material Desc */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|Customizer|Materials")
	TArray<FKBFLMaterialDescriptorInformation> mMaterialInformation = {};

	/** Swatches that should add to the Subsystem */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|Customizer|Swatches")
	TArray<FKBFLSwatchInformation> mSwatchDescriptionInformation = {};

	/** Default Swatches for the Swatch Group */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|Customizer|Swatches")
	TMap<TSubclassOf<UFGSwatchGroup>, TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch>> mSwatchGroups;

	/** Default Swatches for the Swatch Group */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|Customizer")
	ELifecyclePhase mCallCustomizerInPhase;

	/** Informations for every Actor to Spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|ActorSpawnSystem")
	TArray<TSubclassOf<UKBFLActorSpawnDescriptorBase>> mActorSpawnDescriptors;

	/** Resource Nodes that should remove from world */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|ActorSpawnSystem")
	TArray<TSubclassOf<AActor>> mActorsToRemove;

	/** Default Swatches for the Swatch Group */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|ActorSpawnSystem")
	ELifecyclePhase mCallNodesInPhase = ELifecyclePhase::INITIALIZATION;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="KMods|AssetRegistry")
	bool mUseAssetRegistry = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="KMods|AssetRegistry",
		meta = ( EditCondition = mUseAssetRegistry ))
	bool mRegisterCDOs = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="KMods|AssetRegistry",
		meta = ( EditCondition = mUseAssetRegistry ))
	bool mRegisterRecipes = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="KMods|AssetRegistry",
		meta = ( EditCondition = mUseAssetRegistry ))
	bool mRegisterSchematics = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="KMods|AssetRegistry",
		meta = ( EditCondition = mUseAssetRegistry ))
	bool mRegisterResearchTrees = true;

	/**
	* Path for automatic find classes to register
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="KMods|AssetRegistry")
	TArray<TSubclassOf<UObject>> mBlacklistedClasses;

	/**
	* Path for automatic find classes to register
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="KMods|AssetRegistry",
		meta = ( EditCondition = mUseAssetCDOSearch ))
	TArray<TSubclassOf<UObject>> mBlacklistedCDOClasses;
};