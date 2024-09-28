#include "BFL/KBFL_Asset.h"

#include "FGRecipe.h"
#include "FGSchematic.h"
#include "AssetRegistry/AssetRegistryModule.h"

DECLARE_LOG_CATEGORY_EXTERN(KBFLAssetLog, Log, All)

DEFINE_LOG_CATEGORY(KBFLAssetLog)

template <class T>
bool UKBFL_Asset::GetAllClassesOfSubclass(TArray<FAssetData> AllAssets, TArray<TSubclassOf<T>>& OutClasses)
{
	for (const FAssetData& AssetData : AllAssets)
	{
		//Make sure found asset is a blueprint
		if (AssetData.AssetClassPath != FTopLevelAssetPath(UBlueprint::StaticClass()))
		{
			continue;
		}

		//Retrieve GeneratedClass tag containing a text path to generated class
		FString GeneratedClassExportedPath;
		if (!AssetData.GetTagValue(FBlueprintTags::GeneratedClassPath, GeneratedClassExportedPath))
		{
			continue;
		}

		//Make sure export path represents a valid path and convert it to pure objectt path
		FString GeneratedClassPath;
		if (!FPackageName::ParseExportTextPath(GeneratedClassExportedPath, nullptr, &GeneratedClassPath))
		{
			continue;
		}

		//Load UBlueprintGeneratedClass for provided object and make sure it has been loaded
		UClass* ClassObject = LoadObject<UClass>(nullptr, *GeneratedClassPath);
		if (ClassObject == nullptr)
		{
			continue;
		}

		//Verify that generated class is actually a child of the base class, and then add it to the list
		if (ClassObject->IsChildOf(T::StaticClass()))
		{
			OutClasses.Add(ClassObject);
		}
	}

	return !OutClasses.IsEmpty();
}

template <class T>
bool UKBFL_Asset::GetAllClassesOfSubclass(TArray<FAssetData> AllAssets, TSet<TSubclassOf<T>>& OutClasses)
{
	OutClasses.Empty();
	TArray<T> TempClasses;
	if (GetAllClassesOfSubclass(AllAssets, TempClasses))
	{
		for (T Class : TempClasses)
		{
			OutClasses.Add(Class);
		}
	}
	return !OutClasses.IsEmpty();
}

bool UKBFL_Asset::GetAllClassesOfSubclass(TArray<FAssetData> AllAssets, TArray<TSubclassOf<UObject>>& OutClasses)
{
	return GetAllClassesOfSubclass<UObject>(AllAssets, OutClasses);
}

bool UKBFL_Asset::FindChildsByClass(TArray<TSubclassOf<UObject>>& OutClasses, TSubclassOf<UObject> Search,
                                    bool bSearchSubClasses)
{
	OutClasses.Empty();
	if (!IsValid(Search))
	{
		return false;
	}

	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(
		AssetRegistryConstants::ModuleName);
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	TArray<FAssetData> AssetData;

	GetAllClassesOfSubclass(AssetData, OutClasses);

	if (AssetRegistry.GetAssetsByClass(Search->GetFName(), AssetData, bSearchSubClasses))
	{
		return GetAllClassesOfSubclass(AssetData, OutClasses);
	}
	return false;
}

void UKBFL_Asset::TryToEnableAllLocOnTheseClass(UClass* Class)
{
	if (IsValid(Class))
	{
		UObject* Obj = GetMutableDefault<UObject>(Class);
		if (UFGItemDescriptor* ItemDesc = Cast<UFGItemDescriptor>(Obj))
		{
			if (!ItemDesc->mDisplayName.IsEmpty())
			{
				ItemDesc->mDisplayName = ItemDesc->mDisplayName;
			}

			if (!ItemDesc->mDescription.IsEmpty())
			{
				ItemDesc->mDescription = ItemDesc->mDescription;
			}

			ItemDesc->MarkPackageDirty();
		}

		if (UFGSchematic* Schematic = Cast<UFGSchematic>(Obj))
		{
			if (!Schematic->mDisplayName.IsEmpty())
			{
				Schematic->mDisplayName = Schematic->mDisplayName;
			}

			if (!Schematic->mDescription.IsEmpty())
			{
				Schematic->mDescription = Schematic->mDescription;
			}

			Schematic->MarkPackageDirty();
		}

		if (UFGRecipe* Recipe = Cast<UFGRecipe>(Obj))
		{
			if (!Recipe->mDisplayName.IsEmpty())
			{
				Recipe->mDisplayName = Recipe->mDisplayName;
			}

			Recipe->MarkPackageDirty();
		}
	}
}

void UKBFL_Asset::MarkDefaultDirty(UClass* Class)
{
	if (IsValid(Class))
	{
		Class->MarkPackageDirty();
		Class->GetDefaultObject()->MarkPackageDirty();
	}
}
