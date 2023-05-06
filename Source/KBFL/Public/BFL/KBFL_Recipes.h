#pragma once

#include "CoreMinimal.h"

#include "FGRecipe.h"
#include "KBFL_Enum.h"
#include "KBFL_Struct.h"
#include "Buildables/FGBuildableManufacturer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KBFL_Recipes.generated.h"

UCLASS()
class KBFL_API UKBFL_Recipes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Get all recipes in game */
	UFUNCTION( BlueprintPure, Category="KMods|Recipes", meta = (WorldContext = "WorldContext") )
	static TArray< TSubclassOf< UFGRecipe > > GetAllRegisteredRecipes( UObject* WorldContext );

	/** Get all recipes in game with Infos */
	UFUNCTION( BlueprintPure, Category="KMods|Recipes", meta = (WorldContext = "WorldContext") )
	static TArray< FKBFLRecipeInfo > GetAllRegisteredRecipesWithInfo( UObject* WorldContext );

	/** Get all recipes in game from a Building */
	UFUNCTION( BlueprintPure, Category="KMods|Recipes", meta = (WorldContext = "WorldContext") )
	static TArray< TSubclassOf< UFGRecipe > > GetAllRecipesFromBuilding( UObject* WorldContext, TSubclassOf< AFGBuildableManufacturer > Building );

	/** Get all recipes in game from a Mod */
	UFUNCTION( BlueprintPure, Category="KMods|Recipes", meta = (WorldContext = "WorldContext") )
	static TArray< TSubclassOf< UFGRecipe > > GetAllModRecipes( UObject* WorldContext, FName ModName );

	/** Get all recipes in game from Mods */
	UFUNCTION( BlueprintPure, Category="KMods|Recipes", meta = (WorldContext = "WorldContext") )
	static TArray< TSubclassOf< UFGRecipe > > GetAllModsRecipes( UObject* WorldContext, TArray< FName > ModNames );

	/** Get all recipes in game from Mods with Infos from where it from [!Use not Subsystem = Slower!] */
	UFUNCTION( BlueprintPure, Category="KMods|Recipes", meta = (WorldContext = "WorldContext") )
	static TArray< FKBFLRecipeInfo > GetAllModsRecipesWithInfo( UObject* WorldContext, TArray< FName > ModNames );

	/** Return has this Recipe a Form in it */
	UFUNCTION( BlueprintPure, Category="KMods|Recipes" )
	static bool HasRecipeForm( TSubclassOf< UFGRecipe > Recipe, KBFLInOut Side, EResourceForm Form );

	/** Moves all Recipes from a building to a other */
	UFUNCTION( BlueprintCallable, Category="KMods|Recipes", meta = (WorldContext = "WorldContext") )
	static void CDO_MoveRecipesFromBuilding( UObject* WorldContext, TSoftClassPtr< UObject > From, TSoftClassPtr< UObject > To, TArray< TSubclassOf< UFGItemCategory > > IgnoreCategory, TArray< TSubclassOf< UFGRecipe > > IgnoreRecipe );
};
