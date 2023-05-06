// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGBackgroundThread.h"
#include "BFL/KBFL_Struct.h"
#include "Interfaces/KBFLContentCDOHelperInterface.h"
#include "Interfaces/KBFLCustomizerInterface.h"
#include "Interfaces/KBFLResourceNodeInterface.h"
#include "Module/GameWorldModule.h"
#include "Module/WorldModule.h"
#include "UObject/Object.h"
#include "KBFLWorldModule.generated.h"

USTRUCT( BlueprintType )
struct KBFL_API FKBFLPool
{
	GENERATED_BODY()

	/* Type */
	UPROPERTY( EditDefaultsOnly )
	EPoolType Type;

	/* Component placed in the blueprint to handle this instance. */
	UPROPERTY( EditDefaultsOnly )
	TSubclassOf< UFGPoolableProxyComponentBase > mProxyComponent;

	/* Max Default count of instances in the world. */
	UPROPERTY( EditDefaultsOnly, meta = ( EditCondition = "!bInstanced" ) )
	int32 Count;

	/* Draw distance the world instances get relevant,
	* it could be that lights further away from the player still isn't relevant due to the max pool count.*/
	UPROPERTY( EditDefaultsOnly )
	float RelevanceDistance;

	/* Should the instance try to snap to the nearest foundation / non factory building.
    * Needed for ceiling lights and other spotlight based lights */
	UPROPERTY( EditDefaultsOnly, Category = "Range" )
	bool bAdjustHeight;

	// TODO uncomment with ue 4.25
	UPROPERTY( EditDefaultsOnly, Category = "Mesh" /*, meta = ( EditCondition = "Type == EPT_StaticMesh || Type == EPT_InstanceMesh") */ )
	UStaticMesh* mVisual_Mesh;

	/* DEPRECATED */
	UPROPERTY( EditDefaultsOnly )
	bool bVisual_Instanced;

	/*~~~~~~ Count scalability ~~~~~~*/
	/* The string used for count scalability checks. */
	UPROPERTY( EditDefaultsOnly )
	FString mCVarCountScalabilityString;
	IConsoleVariable* mCachedCountScalabilityConsoleVariable;

	UPROPERTY( EditDefaultsOnly, meta = (DisplayName = "Count Default Value") )
	int32 mCachedCountScalabilityValue;
	bool mIsCountScalabilityDirty;

	/*~~~~~~ Relevancy scalability ~~~~~~*/
	/* The string used for relevancy scalability checks. */
	UPROPERTY( EditDefaultsOnly )
	FString mCVarRelevancyScalabilityString;
	IConsoleVariable* mCachedRelevancyScalabilityConsoleVariable;

	UPROPERTY( EditDefaultsOnly, meta = (DisplayName = "Relevancy Scale Default Value") )
	float mCachedRelevancyScaleScalabilityValue;
	bool mIsRelevancyScalabilityDirty;

	/*~~~~~~ Quality scalability ~~~~~~*/
	/* The string used for quality scalability checks. */
	UPROPERTY( EditDefaultsOnly )
	FString mCvarQualityScalabilityString;
	IConsoleVariable* mCachedQualityScaleConsoleVariable;

	UPROPERTY( EditDefaultsOnly, meta = (DisplayName = "Quality Default Value") )
	int32 mCachedQualityScalabilityValue;
	bool mIsQualityScalabilityDirty;

	FFGPoolType ToFg() const
	{
		FFGPoolType PoolType = FFGPoolType();

		PoolType.Type = Type;
		PoolType.mProxyComponent = mProxyComponent;
		PoolType.Count = Count;
		PoolType.RelevanceDistance = RelevanceDistance;
		PoolType.bAdjustHeight = bAdjustHeight;
		PoolType.mVisual_Mesh = mVisual_Mesh;
		PoolType.bVisual_Instanced = bVisual_Instanced;
		PoolType.mCachedQualityScalabilityValue = mCachedQualityScalabilityValue;
		PoolType.mCvarQualityScalabilityString = mCvarQualityScalabilityString;
		PoolType.mCachedRelevancyScaleScalabilityValue = mCachedRelevancyScaleScalabilityValue;
		PoolType.mCVarRelevancyScalabilityString = mCVarRelevancyScalabilityString;
		PoolType.mCachedCountScalabilityValue = mCachedCountScalabilityValue;
		PoolType.mCVarCountScalabilityString = mCVarCountScalabilityString;

		return PoolType;
	}
};

/**
 * 
 */
UCLASS( Blueprintable )
class KBFL_API UKBFLWorldModule : public UGameWorldModule, public IKBFLCustomizerInterface, public IKBFLResourceNodeInterface, public IKBFLContentCDOHelperInterface
{
	GENERATED_BODY()

public:
	UKBFLWorldModule();

	// BEGIN IKBFLContentCDOHelperInterface
	virtual FKBFLCDOInformation GetCDOInformationFromPhase_Implementation( ELifecyclePhase Phase, bool& HasPhase ) override;
	// END IKBFLContentCDOHelperInterface

	// BEGIN IKBFLResourceNodeInterface
	virtual TArray< TSubclassOf< AActor > > GetRemoveClasses_Implementation() override;
	virtual TArray< TSubclassOf< UKBFLActorSpawnDescriptorBase > > GetActorSpawnDescriptors_Implementation() override;
	// END IKBFLResourceNodeInterface

	// BEGIN IKBFLCustomizerInterface
	virtual TMap< TSubclassOf< UFGSwatchGroup >, TSubclassOf< UFGFactoryCustomizationDescriptor_Swatch > > GetSwatchGroups_Implementation() override;
	virtual TArray< FKBFLSwatchInformation > GetSwatchDescriptionInformation_Implementation() override;
	virtual TArray< FKBFLMaterialDescriptorInformation > GetMaterialInformation_Implementation() override;
	// END IKBFLCustomizerInterface

	// BEGIN UGameWorldModule
	virtual void DispatchLifecycleEvent( ELifecyclePhase Phase ) override;
	// END UGameWorldModule

	UFUNCTION( BlueprintNativeEvent, Category="LifecyclePhase" )
	void ConstructionPhase();

	UFUNCTION( BlueprintNativeEvent, Category="LifecyclePhase" )
	void InitPhase();

	UFUNCTION( BlueprintNativeEvent, Category="LifecyclePhase" )
	void PostInitPhase();

	virtual void RegisterKBFLLogicContent();
	virtual void FindAllCDOs();

	virtual void RegisterPoolSettings();
	virtual bool IsAllowedToRegister( TSubclassOf< UObject > Object ) const;

	static bool IsPoolEntryThere( TArray< FFGPoolType > Source, FKBFLPool CheckStruc );

	/** Information for CDO's */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|CDOHelper" )
	TMap< ELifecyclePhase, FKBFLCDOInformation > mCDOInformationMap;

	/** Material Information for add to SF Material Desc */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|Customizer|Materials" )
	TArray< FKBFLMaterialDescriptorInformation > mMaterialInformation = {};

	/** Swatches that should add to the Subsystem */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|Customizer|Swatches" )
	TArray< FKBFLSwatchInformation > mSwatchDescriptionInformation = {};

	/** Default Swatches for the Swatch Group */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|Customizer|Swatches" )
	TMap< TSubclassOf< UFGSwatchGroup >, TSubclassOf< UFGFactoryCustomizationDescriptor_Swatch > > mSwatchGroups;

	/** Default Swatches for the Swatch Group */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|Customizer" )
	ELifecyclePhase mCallCustomizerInPhase;

	/** Informations for every Actor to Spawn */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|ActorSpawnSystem" )
	TArray< TSubclassOf< UKBFLActorSpawnDescriptorBase > > mActorSpawnDescriptors;

	/** Resource Nodes that should remove from world */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|ActorSpawnSystem" )
	TArray< TSubclassOf< AActor > > mActorsToRemove;

	/** Default Swatches for the Swatch Group */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "KMods|ActorSpawnSystem" )
	ELifecyclePhase mCallNodesInPhase = ELifecyclePhase::INITIALIZATION;

	UPROPERTY( meta=(NoAutoJson = true) )
	bool mUseAssetRegistry = false;
	/**
	* Path for automatic find classes to register
	*/
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category="KMods|AssetRegistry", meta = ( EditCondition = mUseAssetRegistry ) )
	TArray< FKBFLRegistry > mAssetRegistryOptions;

	UPROPERTY( meta=(NoAutoJson = true) )
	bool mUseAssetCDOSearch = false;
	bool bScanForCDOsDone = false;
	/**
	* Path for automatic find classes for CDO's
	*/
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category="KMods|AssetRegistry", meta = ( EditCondition = mUseAssetCDOSearch ) )
	TArray< FKBFLCdoRegistry > mAssetCdoFinder;

	UPROPERTY( meta=(NoAutoJson = true) )
	bool mAddPoolEntry = false;

	/**
	* Pool Entrys to add new Light pools
	*/
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category="KMods|AssetRegistry", meta = ( EditCondition = mAddPoolEntry ) )
	TArray< FKBFLPool > mPoolEntryToAdd;

	/**
	* Path for automatic find classes to register
	*/
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category="KMods|AssetRegistry" )
	TArray< TSubclassOf< UObject > > mBlacklistedClasses;
};
