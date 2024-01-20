#pragma once

#include "CoreMinimal.h"
#include "FGDriveablePawn.h"
#include "Interfaces/KBFLContentCDOHelperInterface.h"
#include "Module/WorldModule.h"
#include "ResourceNodes/KBFLActorSpawnDescriptorBase.h"
#include "ResourceNodes/KBFLSubLevelSpawning.h"
#include "KBFLAssetDataSubsystem.generated.h"


USTRUCT(Blueprintable)
struct FKBFLAssetData
{
	GENERATED_BODY()

public:
	void cleanup()
	{
		mAllFoundedBuildables.Empty();
		mAllFoundedCDOHelpers.Empty();
		mAllFoundedDriveablePawns.Empty();
		mAllFoundedHolograms.Empty();
		mAllFoundedItems.Empty();
		mAllFoundedModModules.Empty();
		mAllFoundedRecipes.Empty();
		mAllFoundedResourceDescriptors.Empty();
		mAllFoundedSchematics.Empty();
		mAllFoundResearchTrees.Empty();
	}

	void WriteClass(UClass* Class, int32 Type)
	{
		switch (Type)
		{
		case 0:
			mAllFoundedSchematics.Add(Class);
			break;
		case 1:
			mAllFoundedRecipes.Add(Class);
			break;
		case 2:
			mAllFoundedItems.Add(Class);
			break;
		case 3:
			mAllFoundedBuildables.Add(Class);
			break;
		case 4:
			mAllFoundedDriveablePawns.Add(Class);
			break;
		case 5:
			mAllFoundedHolograms.Add(Class);
			break;
		case 6:
			mAllFoundedModModules.Add(Class);
			break;
		case 7:
			mAllFoundedCDOHelpers.Add(Class);
			break;
		case 8:
			mAllFoundedResourceDescriptors.Add(Class);
			break;
		case 10:
			mAllFoundResearchTrees.Add(Class);
			break;
		}
	}

	UPROPERTY(BlueprintReadOnly)
	TSet<UClass*> mAllFoundResearchTrees = {};

	UPROPERTY(BlueprintReadOnly)
	TSet<UClass*> mAllFoundedSchematics = {};

	UPROPERTY(BlueprintReadOnly)
	TSet<UClass*> mAllFoundedRecipes = {};

	UPROPERTY(BlueprintReadOnly)
	TSet<UClass*> mAllFoundedItems = {};

	UPROPERTY(BlueprintReadOnly)
	TSet<UClass*> mAllFoundedBuildables = {};

	UPROPERTY(BlueprintReadOnly)
	TSet<UClass*> mAllFoundedDriveablePawns = {};

	UPROPERTY(BlueprintReadOnly)
	TSet<UClass*> mAllFoundedHolograms = {};

	UPROPERTY(BlueprintReadOnly)
	TSet<UClass*> mAllFoundedModModules = {};

	UPROPERTY(BlueprintReadOnly)
	TSet<UClass*> mAllFoundedCDOHelpers = {};

	UPROPERTY(BlueprintReadOnly)
	TSet<UClass*> mAllFoundedResourceDescriptors = {};
};


/**
 * 
 */
UCLASS(Blueprintable)
class KBFL_API UKBFLAssetDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

public:
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void DoScan(bool Force = false);

	void setMapClass(UClass* Class, int32 Type)
	{
		TArray<FString> DirectoryArray;
		Class->GetFullName().ParseIntoArray(DirectoryArray, TEXT("/"));
		FString ModName = DirectoryArray[1];
		// UE_LOG( LogTemp, Warning, TEXT("setMapClass: %s > %s > %d"), *ModName, *Class->GetFullName( ), Type );

		if (FKBFLAssetData* AssetData = mDirectoryMappings.Find(FName(ModName.ToLower())))
		{
			AssetData->WriteClass(Class, Type);
		}
		else
		{
			FKBFLAssetData KBFLAssetData = FKBFLAssetData();
			KBFLAssetData.WriteClass(Class, Type);
			mDirectoryMappings.Add(FName(ModName.ToLower()), KBFLAssetData);
		}
	}

	// NATIVE GETTER
	static UKBFLAssetDataSubsystem* Get(const UObject* WorldContext);

	void PrintFound();

	void InitAssetFinder();

	static bool FilterAsset(const FAssetData& AssetData);

	bool Local_FilterAsset(const FAssetData& AssetData) const;

	// Begin Items
	/** Get All Items that hit the Forms  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetItemsOfForms(TArray<EResourceForm> Forms, TArray<TSubclassOf<UFGItemDescriptor>>& Out_Items);

	/** Get All Items that hit the Child Classes  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetItemsOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<UFGItemDescriptor>>& Out_Items,
	                      bool UseNativeCheck = false);

	/** Get All Items with a filter for Items & Equip  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetItemsFiltered(TArray<TSubclassOf<UFGItemDescriptor>>& Out_Items);

	/** Get All Items with a filter for Items & Equip & Forms  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetItemsFilteredWithForm(TArray<EResourceForm> Forms, TArray<TSubclassOf<UFGItemDescriptor>>& Out_Items);

	/** Get All Items that found while reading the game */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Asset Data Subsystem")
	TArray<TSubclassOf<UFGItemDescriptor>> GetAllItems();

	// END Items


	// Begin Schematics
	/** Get All Schematics that hit the Types  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetSchematicsOfTypes(TArray<ESchematicType> Types, TArray<TSubclassOf<UFGSchematic>>& Out_Schematics);

	/** Get All Items that hit the Child Classes  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetSchematicsOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<UFGSchematic>>& Out_Items,
	                           bool UseNativeCheck = false);

	/** Get All Schematics that found while reading the game */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Asset Data Subsystem")
	TArray<TSubclassOf<UFGSchematic>> GetAllSchematics();

	// END Schematics


	// Begin Recipes
	/** Get All Items that hit the Child Classes  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetRecipesOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<UFGRecipe>>& Out_Items,
	                        bool UseNativeCheck = false);

	/** Get All Items that hit the Child Classes  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetRecipesOfProducer(TArray<TSubclassOf<UObject>> Producers, TArray<TSubclassOf<UFGRecipe>>& Out_Items);

	/** Get All Recipes that found while reading the game */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Asset Data Subsystem")
	TArray<TSubclassOf<UFGRecipe>> GetAllRecipes();

	// END Recipes

	// Begin Buildables
	/** Get All Items that hit the Child Classes  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetBuildableOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<AFGBuildable>>& Out_Items,
	                          bool UseNativeCheck = false);

	/** Get All Buildables that found while reading the game */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Asset Data Subsystem")
	TArray<TSubclassOf<AFGBuildable>> GetAllBuildable();

	// END Buildables


	// Start General Functions
	/** Check is a child of Array Class */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Asset Data Subsystem")
	static bool CheckChild(UClass* TestClass, TArray<UClass*> ClassesToTest, bool UseNativeCheck = false);

	/** Check has recipe producer of Array Class */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Asset Data Subsystem")
	static bool CheckHasRecipeProducer(TSubclassOf<UFGRecipe> TestClass, TArray<TSubclassOf<UObject>> Producers);

	// END General Functions


	/** Get All Objects that hit the Child Classes  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetObjectsOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<UObject>>& Out_Items,
	                        bool UseNativeCheck = false);

	template <class T>
	void GetObjectsOfChilds_Internal(TArray<UClass*> Childs, TArray<TSubclassOf<T>>& Out_Items,
	                                 bool UseNativeCheck = false);


	/** Get All DriveablePawns that hit the Child Classes  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetDriveablePawnsOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<AFGDriveablePawn>>& Out_Items,
	                               bool UseNativeCheck = false);


	/** Get All Holograms that hit the Child Classes  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetHologramsOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<AFGHologram>>& Out_Items,
	                          bool UseNativeCheck = false);


	/** Get All tModModules that hit the Child Classes  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetModModulesOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<UModModule>>& Out_Items,
	                           bool UseNativeCheck = false);


	/** Get All CDOHelpers that hit the Child Classes  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetCDOHelpersOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<UKBFL_CDOHelperClass_Base>>& Out_Items,
	                           bool UseNativeCheck = false);

	/** get data from a mod */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	FKBFLAssetData GetModRelatedData(UModModule* ModModule);

	/** Get All ResourceDescriptors that hit the Child Classes  */
	UFUNCTION(BlueprintCallable, Category="Asset Data Subsystem")
	void GetResourceDescriptorsOfChilds(TArray<UClass*> Childs,
	                                    TArray<TSubclassOf<UKBFLActorSpawnDescriptorBase>>& Out_Items,
	                                    bool UseNativeCheck = false);

	inline static bool bWasInit = false;

	UPROPERTY()
	TSet<TSubclassOf<UKBFLSubLevelSpawning>> mAllSubLevelSpawningClasses;

private:
	UPROPERTY()
	TSet<UClass*> mAllFoundedSchematics;

	UPROPERTY()
	TSet<UClass*> mAllFoundedRecipes;

	UPROPERTY()
	TSet<UClass*> mAllFoundedItems;

	UPROPERTY()
	TSet<UClass*> mAllFoundedBuildables;

	UPROPERTY()
	TSet<UClass*> mAllFoundedDriveablePawns;

	UPROPERTY()
	TSet<UClass*> mAllFoundedHolograms;

	UPROPERTY()
	TSet<UClass*> mAllFoundedModModules;

	UPROPERTY()
	TSet<UClass*> mAllFoundedCDOHelpers;

	UPROPERTY()
	TSet<UClass*> mAllFoundedResourceDescriptors;

	UPROPERTY()
	TSet<UClass*> mAllFoundedObjects;

	UPROPERTY()
	TSet<UClass*> mAllFoundResearchTrees;

	UPROPERTY()
	TMap<FName, FKBFLAssetData> mDirectoryMappings;

public:
	TMap<UClass*, FAssetData> mAssetClassMap;

	// Small fix for PassiveMode
	TArray<FString> mPreventStrings = {"/PassiveMode/"};
};

template <class T>
void UKBFLAssetDataSubsystem::GetObjectsOfChilds_Internal(const TArray<UClass*> Childs,
                                                          TArray<TSubclassOf<T>>& Out_Items, bool UseNativeCheck)
{
	if (!bWasInit)
	{
		UE_LOG(LogTemp, Error, TEXT("Try to get classes without Init before the subsytem! CANCEL!"));
		return;
	}

	for (UClass* FoundedClass : mAllFoundedObjects)
	{
		if (TSubclassOf<UObject> Class = FoundedClass)
		{
			if (CheckChild(Class, Childs, UseNativeCheck))
			{
				if (TSubclassOf<T> CastObject = TSubclassOf<T>(Class))
				{
					Out_Items.Add(CastObject);
				}
			}
		}
	}
}
