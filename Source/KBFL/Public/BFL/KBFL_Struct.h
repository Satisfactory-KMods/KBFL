#pragma once

#include "CoreMinimal.h"

#include "FGRecipe.h"
#include "FGSchematic.h"
#include "Module/ModModule.h"
#include "UObject/Object.h"
#include "KBFL_Struct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct KBFL_API FKBFLSchematicArray {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UFGSchematic>> mSchematics = {};
};

/**
* 
*/
USTRUCT(BlueprintType)
struct KBFL_API FKBFLRegistry {
	GENERATED_BODY()

	FKBFLRegistry() {
	};

	FKBFLRegistry(FString InPath) {
		Path = InPath;
	};

	FKBFLRegistry(FString InPath, bool InRecursive) {
		Path = InPath;
		Recursive = InRecursive;
	};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Path = "/<modref>/";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Recursive = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRegisterSchematics = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRegisterResearchTrees = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRegisterRecipes = false;

	FName GetPath() {
		Path.RemoveFromEnd("/", ESearchCase::IgnoreCase);
		return FName(Path);
	}
};

/**
* 
*/
USTRUCT(BlueprintType)
struct KBFL_API FKBFLCdoRegistry {
	GENERATED_BODY()

	FKBFLCdoRegistry() {
	};

	FKBFLCdoRegistry(FString InPath) {
		Path = InPath;
	};

	FKBFLCdoRegistry(FString InPath, bool InRecursive) {
		Path = InPath;
		Recursive = InRecursive;
	};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Path = "/<modref>/";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Recursive = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ELifecyclePhase bPutInPhase = ELifecyclePhase::CONSTRUCTION;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bFindRemover = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bFindCDOHelpers = true;

	FName GetPath() {
		Path.RemoveFromEnd("/", ESearchCase::IgnoreCase);
		return FName(Path);
	}
};

/**
* 
*/
USTRUCT(BlueprintType)
struct KBFL_API FKBFLDescArray {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UFGItemDescriptor>> mDesc = {};
};

/**
* 
*/
USTRUCT(BlueprintType)
struct KBFL_API FKBFLDescInfo {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UFGItemDescriptor> mDesc = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName mMod = {};
};

/**
* 
*/
USTRUCT(BlueprintType)
struct KBFL_API FKBFLSchematicInfo {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UFGSchematic> mSchematic = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName mMod = {};
};

/**
* 
*/
USTRUCT(BlueprintType)
struct KBFL_API FKBFLRecipeInfo {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UFGRecipe> mRecipe = {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName mMod = {};
};

/**
* 
*/
USTRUCT(BlueprintType)
struct KBFL_API FKBFLRecipeArray {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UFGRecipe>> mRecipes = {};
};
