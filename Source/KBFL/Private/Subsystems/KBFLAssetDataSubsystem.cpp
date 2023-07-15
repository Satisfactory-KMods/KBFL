#include "Subsystems/KBFLAssetDataSubsystem.h"

#include "FGGameState.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "BFL/KBFL_Asset.h"
#include "BFL/KBFL_Player.h"
#include "Buildables/FGBuildableWire.h"
#include "Hologram/FGHologram.h"
#include "Resources/FGAnyUndefinedDescriptor.h"
#include "Resources/FGNoneDescriptor.h"
#include "Resources/FGOverflowDescriptor.h"
#include "Resources/FGWildCardDescriptor.h"

DECLARE_LOG_CATEGORY_EXTERN(AssetDataSubsystemLog, Log, All)

DEFINE_LOG_CATEGORY(AssetDataSubsystemLog)
/*
void UKBFLAssetDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if(!bWasInit)
	{
		if(!GetWorld()->GetMapName().Contains("Menu") && !GetWorld()->GetMapName().Contains("Untitled"))
		{
			UE_LOG(AssetDataSubsystemLog, Log, TEXT("Initialize Subsystem in WorldName: %s"), *GetWorld()->GetMapName());
			InitAssetFinder();
			bWasInit = true;
			PrintFound();
		}
		else 
			UE_LOG(AssetDataSubsystemLog, Log, TEXT("Skip Initialize on WorldName: %s"), *GetWorld()->GetMapName());
	}
	else
	{
		PrintFound();
	}
}
	*/

void UKBFLAssetDataSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);

#if WITH_EDITOR
#endif
}

void UKBFLAssetDataSubsystem::Deinitialize() {
	mAllFoundedSchematics.Empty();
	mAllFoundedRecipes.Empty();
	mAllFoundedItems.Empty();
	mAllFoundedBuildables.Empty();
	mAllFoundedDriveablePawns.Empty();
	mAllFoundedHolograms.Empty();
	mAllFoundedModModules.Empty();
	mAllFoundedCDOHelpers.Empty();
	mAllFoundedResourceDescriptors.Empty();
	mAllFoundedObjects.Empty();

	bWasInit = false;

	Super::Deinitialize();
}

void UKBFLAssetDataSubsystem::DoScan(bool Force) {
	if(!bWasInit || Force) {
		UE_LOG(AssetDataSubsystemLog, Log, TEXT("FORCE! Initialize Subsystem in WorldName: %s"), *GetWorld()->GetMapName());
		InitAssetFinder();
		bWasInit = true;
		PrintFound();
	}
}

UKBFLAssetDataSubsystem* UKBFLAssetDataSubsystem::Get(const UObject* WorldContext) {
	if(IsValid(WorldContext)) {
		return WorldContext->GetWorld()->GetGameInstance()->GetSubsystem<UKBFLAssetDataSubsystem>();
	}
	return nullptr;
}

void UKBFLAssetDataSubsystem::PrintFound() {
	UE_LOG(AssetDataSubsystemLog, Log, TEXT("--------------------------------------------"));
	UE_LOG(AssetDataSubsystemLog, Log, TEXT("mAllSubLevelSpawningClasses: %d"), mAllSubLevelSpawningClasses.Num());
	UE_LOG(AssetDataSubsystemLog, Log, TEXT("Successful Initialize UKBFLAssetDataSubsystem"));
	UE_LOG(AssetDataSubsystemLog, Log, TEXT("mAllFoundedSchematics: %d"), mAllFoundedSchematics.Num());
	UE_LOG(AssetDataSubsystemLog, Log, TEXT("mAllFoundedBuildables: %d"), mAllFoundedBuildables.Num());
	UE_LOG(AssetDataSubsystemLog, Log, TEXT("mAllFoundedItems: %d"), mAllFoundedItems.Num());
	UE_LOG(AssetDataSubsystemLog, Log, TEXT("mAllFoundedRecipes: %d"), mAllFoundedRecipes.Num());
	UE_LOG(AssetDataSubsystemLog, Log, TEXT("mAllFoundedDriveablePawns: %d"), mAllFoundedDriveablePawns.Num());
	UE_LOG(AssetDataSubsystemLog, Log, TEXT("mAllFoundedHolograms: %d"), mAllFoundedHolograms.Num());
	UE_LOG(AssetDataSubsystemLog, Log, TEXT("mAllFoundedModModules: %d"), mAllFoundedModModules.Num());
	UE_LOG(AssetDataSubsystemLog, Log, TEXT("mAllFoundedCDOHelpers: %d"), mAllFoundedCDOHelpers.Num());
	UE_LOG(AssetDataSubsystemLog, Log, TEXT("mAllFoundedResourceDescriptors: %d"), mAllFoundedResourceDescriptors.Num());
	UE_LOG(AssetDataSubsystemLog, Log, TEXT("mAllFoundedObjects: %d"), mAllFoundedObjects.Num());
	UE_LOG(AssetDataSubsystemLog, Log, TEXT("--------------------------------------------"));
}

void UKBFLAssetDataSubsystem::InitAssetFinder() {
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
	IAssetRegistry&             AssetRegistry = AssetRegistryModule.Get();
	TArray<FAssetData>          AssetData;

	if(AssetRegistry.GetAssetsByPath(TEXT("/"), AssetData, true)) {
		for(FAssetData Asset: AssetData) {
			// ONLY READ BP!
			if(Asset.AssetClass != TEXT("Blueprint")) {
				continue;
			}

			TSubclassOf<UFGSchematic>                  Schematic;
			TSubclassOf<UFGRecipe>                     Recipe;
			TSubclassOf<UFGItemDescriptor>             Item;
			TSubclassOf<AFGBuildable>                  Buildable;
			TSubclassOf<UObject>                       Object;
			TSubclassOf<AFGDriveablePawn>              DriveablePawn;
			TSubclassOf<AFGHologram>                   Holograms;
			TSubclassOf<UModModule>                    ModModules;
			TSubclassOf<UKBFL_CDOHelperClass_Base>     CDOHelpers;
			TSubclassOf<UKBFLActorSpawnDescriptorBase> ResourceDescriptors;
			TSubclassOf<UKBFLSubLevelSpawning>         SubLevelSpawning;

			if(Local_FilterAsset(Asset)) {
				// SubLevelSpawning
				if(UKBFL_Asset::GetSubclassFromAsset(Asset, SubLevelSpawning)) {
					if(SubLevelSpawning) {
						mAllSubLevelSpawningClasses.Add(SubLevelSpawning);
						mAssetClassMap.Add(SubLevelSpawning, Asset);
					}
				}

				// Schematic
				if(UKBFL_Asset::GetSubclassFromAsset(Asset, Holograms)) {
					if(Holograms) {
						mAllFoundedHolograms.Add(Holograms);
						//UE_LOG(AssetDataSubsystemLog, Log, TEXT("Add Hologram To Subsystem > %s"), *Holograms->GetName());
						mAssetClassMap.Add(Holograms, Asset);
					}
				}

				// ModModules
				if(UKBFL_Asset::GetSubclassFromAsset(Asset, ModModules)) {
					if(ModModules) {
						mAllFoundedModModules.Add(ModModules);
						//UE_LOG(AssetDataSubsystemLog, Log, TEXT("Add ModModule To Subsystem > %s"), *ModModules->GetName());
						mAssetClassMap.Add(ModModules, Asset);
					}
				}

				// CDOHelpers
				if(UKBFL_Asset::GetSubclassFromAsset(Asset, CDOHelpers)) {
					if(CDOHelpers) {
						mAllFoundedCDOHelpers.Add(CDOHelpers);
						//UE_LOG(AssetDataSubsystemLog, Log, TEXT("Add CDOHelper To Subsystem > %s"), *CDOHelpers->GetName());
						mAssetClassMap.Add(CDOHelpers, Asset);
					}
				}

				// ResourceDescriptors
				if(UKBFL_Asset::GetSubclassFromAsset(Asset, ResourceDescriptors)) {
					if(ResourceDescriptors) {
						mAllFoundedResourceDescriptors.Add(ResourceDescriptors);
						//UE_LOG(AssetDataSubsystemLog, Log, TEXT("Add ResourceDescriptor To Subsystem > %s"), *ResourceDescriptors->GetName());
						mAssetClassMap.Add(ResourceDescriptors, Asset);
					}
				}

				// Schematic
				if(UKBFL_Asset::GetSubclassFromAsset(Asset, Schematic)) {
					if(Schematic) {
						mAllFoundedSchematics.Add(Schematic);
						//UE_LOG(AssetDataSubsystemLog, Log, TEXT("Add Schematic To Subsystem > %s"), *Schematic->GetName());
						mAssetClassMap.Add(Schematic, Asset);
					}
				}

				// Recipe
				else if(UKBFL_Asset::GetSubclassFromAsset(Asset, Recipe)) {
					if(Recipe) {
						mAllFoundedRecipes.Add(Recipe);
						//UE_LOG(AssetDataSubsystemLog, Log, TEXT("Add Recipe To Subsystem > %s"), *Recipe->GetName());
						mAssetClassMap.Add(Recipe, Asset);
					}
				}

				// Item
				else if(UKBFL_Asset::GetSubclassFromAsset(Asset, Item)) {
					if(Item) {
						mAllFoundedItems.Add(Item);
						//UE_LOG(AssetDataSubsystemLog, Log, TEXT("Add Item To Subsystem > %s"), *Item->GetName());
						mAssetClassMap.Add(Item, Asset);
					}
				}

				// Buildable
				else if(UKBFL_Asset::GetSubclassFromAsset(Asset, Buildable)) {
					if(Buildable) {
						mAllFoundedBuildables.Add(Buildable);
						//UE_LOG(AssetDataSubsystemLog, Log, TEXT("Add Buildable To Subsystem > %s"), *Buildable->GetName());
						mAssetClassMap.Add(Buildable, Asset);
					}
				}

				// DriveablePawn
				else if(UKBFL_Asset::GetSubclassFromAsset(Asset, DriveablePawn)) {
					if(DriveablePawn) {
						mAllFoundedDriveablePawns.Add(DriveablePawn);
						//UE_LOG(AssetDataSubsystemLog, Log, TEXT("Add DriveablePawn To Subsystem > %s"), *DriveablePawn->GetName());
						mAssetClassMap.Add(DriveablePawn, Asset);
					}
				}

				// All
				if(UKBFL_Asset::GetSubclassFromAsset(Asset, Object)) {
					if(Object) {
						mAllFoundedObjects.Add(Object);
						//UE_LOG(AssetDataSubsystemLog, Log, TEXT("Add Object To Subsystem > %s"), *Object->GetName());
						mAssetClassMap.Add(Object, Asset);
					}
				}
			}
		}
	} else UE_LOG(AssetDataSubsystemLog, Error, TEXT("FAIL TO FIND ASSET PATH"));
}

bool UKBFLAssetDataSubsystem::FilterAsset(const FAssetData& AssetData) {
	FString ExportedPath;
	if(!AssetData.GetTagValue(FBlueprintTags::GeneratedClassPath, ExportedPath)) {
		return false;
	}
	if(ExportedPath.Contains("PassiveMode")) {
		return false;
	}

	FString AssetClassName = AssetData.AssetClass.ToString();
	if(AssetClassName.IsEmpty()) {
		return false;
	}
	if(AssetClassName == "Blueprint") {
		return true;
	}
	return false;
}

bool UKBFLAssetDataSubsystem::Local_FilterAsset(const FAssetData& AssetData) const {
	FString ExportedPath;
	if(!AssetData.GetTagValue(FBlueprintTags::GeneratedClassPath, ExportedPath)) {
		return false;
	}
	for(auto String: mPreventStrings) {
		if(ExportedPath.Contains(String)) {
			return false;
		}
	}

	FString AssetClassName = AssetData.AssetClass.ToString();
	if(AssetClassName.IsEmpty()) {
		return false;
	}
	if(AssetClassName == "Blueprint") {
		return true;
	}
	return false;
}

void UKBFLAssetDataSubsystem::GetItemsOfForms(TArray<EResourceForm> Forms, TArray<TSubclassOf<UFGItemDescriptor>>& Out_Items) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}

	for(UClass* Class: mAllFoundedItems) {
		if(TSubclassOf<UFGItemDescriptor> Item = Class) {
			if(Forms.Contains(UFGItemDescriptor::GetForm(Item))) {
				Out_Items.Add(Item);
			}
		}
	}
}

void UKBFLAssetDataSubsystem::GetItemsOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<UFGItemDescriptor>>& Out_Items, bool UseNativeCheck) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}

	for(UClass* Class: mAllFoundedItems) {
		if(TSubclassOf<UFGItemDescriptor> Item = Class) {
			if(CheckChild(Item, Childs, UseNativeCheck)) {
				Out_Items.Add(Item);
			}
		}
	}
}

void UKBFLAssetDataSubsystem::GetItemsFiltered(TArray<TSubclassOf<UFGItemDescriptor>>& Out_Items) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}

	for(UClass* Class: mAllFoundedItems) {
		if(TSubclassOf<UFGItemDescriptor> Item = Class) {
			if(UFGItemDescriptor::GetForm(Item) != EResourceForm::RF_INVALID && !Item->IsChildOf(UFGBuildDescriptor::StaticClass()) && !Item->IsChildOf(UFGFactoryCustomizationDescriptor::StaticClass()) && !UFGItemDescriptor::GetItemName(Item).IsEmpty() && !Item->IsChildOf(UFGNoneDescriptor::StaticClass()) && !Item->IsChildOf(UFGAnyUndefinedDescriptor::StaticClass()) && !Item->IsChildOf(UFGOverflowDescriptor::StaticClass()) && !Item->IsChildOf(UFGWildCardDescriptor::StaticClass())) {
				Out_Items.Add(Item);
			}
		}
	}
}

void UKBFLAssetDataSubsystem::GetItemsFilteredWithForm(TArray<EResourceForm> Forms, TArray<TSubclassOf<UFGItemDescriptor>>& Out_Items) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}


	for(UClass* Class: mAllFoundedItems) {
		if(TSubclassOf<UFGItemDescriptor> Item = Class) {
			if(Forms.Contains(UFGItemDescriptor::GetForm(Item)) && !Item->IsChildOf(UFGBuildDescriptor::StaticClass()) && !Item->IsChildOf(UFGFactoryCustomizationDescriptor::StaticClass()) && !UFGItemDescriptor::GetItemName(Item).IsEmpty() && !Item->IsChildOf(UFGNoneDescriptor::StaticClass()) && !Item->IsChildOf(UFGAnyUndefinedDescriptor::StaticClass()) && !Item->IsChildOf(UFGOverflowDescriptor::StaticClass()) && !Item->IsChildOf(UFGWildCardDescriptor::StaticClass())) {
				Out_Items.Add(Item);
			}
		}
	}
}

TArray<TSubclassOf<UFGItemDescriptor>> UKBFLAssetDataSubsystem::GetAllItems() {
	return TArray<TSubclassOf<UFGItemDescriptor>>(mAllFoundedItems.Array());
}

void UKBFLAssetDataSubsystem::GetSchematicsOfTypes(TArray<ESchematicType> Types, TArray<TSubclassOf<UFGSchematic>>& Out_Schematics) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}

	for(UClass* Class: mAllFoundedSchematics) {
		if(TSubclassOf<UFGSchematic> Item = Class) {
			if(Types.Contains(UFGSchematic::GetType(Item))) {
				Out_Schematics.Add(Item);
			}
		}
	}
}

void UKBFLAssetDataSubsystem::GetSchematicsOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<UFGSchematic>>& Out_Items, bool UseNativeCheck) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}

	for(UClass* FoundedClass: mAllFoundedSchematics) {
		if(TSubclassOf<UFGSchematic> Class = FoundedClass) {
			if(CheckChild(Class, Childs, UseNativeCheck)) {
				Out_Items.Add(Class);
			}
		}
	}
}

TArray<TSubclassOf<UFGSchematic>> UKBFLAssetDataSubsystem::GetAllSchematics() {
	return TArray<TSubclassOf<UFGSchematic>>(mAllFoundedSchematics.Array());
}

void UKBFLAssetDataSubsystem::GetRecipesOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<UFGRecipe>>& Out_Items, bool UseNativeCheck) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}

	for(UClass* FoundedClass: mAllFoundedRecipes) {
		if(TSubclassOf<UFGRecipe> Class = FoundedClass) {
			if(CheckChild(Class, Childs, UseNativeCheck)) {
				Out_Items.Add(Class);
			}
		}
	}
}

void UKBFLAssetDataSubsystem::GetRecipesOfProducer(TArray<TSubclassOf<UObject>> Producers, TArray<TSubclassOf<UFGRecipe>>& Out_Items) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}

	for(UClass* FoundedClass: mAllFoundedRecipes) {
		if(TSubclassOf<UFGRecipe> Class = FoundedClass) {
			if(CheckHasRecipeProducer(Class, Producers)) {
				Out_Items.Add(Class);
			}
		}
	}
}

TArray<TSubclassOf<UFGRecipe>> UKBFLAssetDataSubsystem::GetAllRecipes() {
	return TArray<TSubclassOf<UFGRecipe>>(mAllFoundedRecipes.Array());
}

void UKBFLAssetDataSubsystem::GetBuildableOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<AFGBuildable>>& Out_Items, bool UseNativeCheck) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}

	for(UClass* FoundedClass: mAllFoundedBuildables) {
		if(TSubclassOf<AFGBuildable> Class = FoundedClass) {
			if(CheckChild(Class, Childs, UseNativeCheck)) {
				Out_Items.Add(Class);
			}
		}
	}
}

TArray<TSubclassOf<AFGBuildable>> UKBFLAssetDataSubsystem::GetAllBuildable() {
	return TArray<TSubclassOf<AFGBuildable>>(mAllFoundedBuildables.Array());
}

bool UKBFLAssetDataSubsystem::CheckChild(UClass* TestClass, TArray<UClass*> ClassesToTest, bool UseNativeCheck) {
	if(TestClass->IsNative() && UseNativeCheck) {
		for(UClass* ClassToTest: ClassesToTest) {
			if(GetParentNativeClass(ClassToTest) == TestClass) {
				return true;
			}
		}
		return false;
	}

	for(UClass* ClassToTest: ClassesToTest) {
		if(TestClass->IsChildOf(ClassToTest)) {
			return true;
		}
	}
	return false;
}

bool UKBFLAssetDataSubsystem::CheckHasRecipeProducer(TSubclassOf<UFGRecipe> TestClass, TArray<TSubclassOf<UObject>> Producers) {
	if(TestClass) {
		const TArray<TSubclassOf<UObject>> RecipeProducers = UFGRecipe::GetProducedIn(TestClass);
		for(TSubclassOf<UObject> Producer: Producers) {
			if(Producer) {
				if(RecipeProducers.Contains(Producer)) {
					return true;
				}
			}
		}
	}
	return false;
}

void UKBFLAssetDataSubsystem::GetObjectsOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<UObject>>& Out_Items, bool UseNativeCheck) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}

	for(UClass* FoundedClass: mAllFoundedObjects) {
		if(TSubclassOf<UObject> Class = FoundedClass) {
			if(CheckChild(Class, Childs, UseNativeCheck)) {
				Out_Items.Add(Class);
			}
		}
	}
}

void UKBFLAssetDataSubsystem::GetDriveablePawnsOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<AFGDriveablePawn>>& Out_Items, bool UseNativeCheck) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}

	for(UClass* FoundedClass: mAllFoundedDriveablePawns) {
		if(TSubclassOf<AFGDriveablePawn> Class = FoundedClass) {
			if(CheckChild(Class, Childs, UseNativeCheck)) {
				Out_Items.Add(Class);
			}
		}
	}
}

void UKBFLAssetDataSubsystem::GetHologramsOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<AFGHologram>>& Out_Items, bool UseNativeCheck) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}

	for(UClass* FoundedClass: mAllFoundedHolograms) {
		if(TSubclassOf<AFGHologram> Class = FoundedClass) {
			if(CheckChild(Class, Childs, UseNativeCheck)) {
				Out_Items.Add(Class);
			}
		}
	}
}

void UKBFLAssetDataSubsystem::GetModModulesOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<UModModule>>& Out_Items, bool UseNativeCheck) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}

	for(UClass* FoundedClass: mAllFoundedModModules) {
		if(TSubclassOf<UModModule> Class = FoundedClass) {
			if(CheckChild(Class, Childs, UseNativeCheck)) {
				Out_Items.Add(Class);
			}
		}
	}
}

void UKBFLAssetDataSubsystem::GetCDOHelpersOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<UKBFL_CDOHelperClass_Base>>& Out_Items, bool UseNativeCheck) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}

	for(UClass* FoundedClass: mAllFoundedCDOHelpers) {
		if(TSubclassOf<UKBFL_CDOHelperClass_Base> Class = FoundedClass) {
			if(CheckChild(Class, Childs, UseNativeCheck)) {
				Out_Items.Add(Class);
			}
		}
	}
}

void UKBFLAssetDataSubsystem::GetResourceDescriptorsOfChilds(TArray<UClass*> Childs, TArray<TSubclassOf<UKBFLActorSpawnDescriptorBase>>& Out_Items, bool UseNativeCheck) {
	if(!bWasInit) {
		UE_LOG(AssetDataSubsystemLog, Error, TEXT("Try to get classes without Init before the subsytem! Do ForceScan!"));
		DoScan();
	}

	for(UClass* FoundedClass: mAllFoundedResourceDescriptors) {
		if(TSubclassOf<UKBFLActorSpawnDescriptorBase> Class = FoundedClass) {
			if(CheckChild(Class, Childs, UseNativeCheck)) {
				Out_Items.Add(Class);
			}
		}
	}
}
