#include "BFL/KBFL_Recipes.h"

#include "FGItemCategory.h"
#include "BFL/KBFL_Util.h"
#include "Registry/ModContentRegistry.h"
#include "Subsystems/KBFLAssetDataSubsystem.h"
#include "Subsystems/KBFLContentCDOHelperSubsystem.h"

TArray<TSubclassOf<UFGRecipe>> UKBFL_Recipes::GetAllRegisteredRecipes(UObject* WorldContext) {
	const auto                     ModContent = AModContentRegistry::Get(WorldContext->GetWorld());
	auto                           Recipes = ModContent->GetRegisteredRecipes();
	TArray<TSubclassOf<UFGRecipe>> ReturnRecipes = {};

	for(auto Recipe: Recipes) {
		ReturnRecipes.Add(TSubclassOf<UFGRecipe>(Recipe.RegisteredObject));
	}

	return ReturnRecipes;
}

TArray<FKBFLRecipeInfo> UKBFL_Recipes::GetAllRegisteredRecipesWithInfo(UObject* WorldContext) {
	const auto              ModContent = AModContentRegistry::Get(WorldContext->GetWorld());
	auto                    Recipes = ModContent->GetRegisteredRecipes();
	TArray<FKBFLRecipeInfo> ReturnRecipes = {};

	for(auto Recipe: Recipes) {
		FKBFLRecipeInfo Struc;
		Struc.mMod = Recipe.OwnedByModReference;
		Struc.mRecipe = TSubclassOf<UFGRecipe>(Recipe.RegisteredObject);

		ReturnRecipes.Add(Struc);
		// write Struc
	}

	return ReturnRecipes;
}

TArray<TSubclassOf<UFGRecipe>> UKBFL_Recipes::GetAllRecipesFromBuilding(UObject* WorldContext, TSubclassOf<AFGBuildableManufacturer> Building) {
	TArray<TSubclassOf<UFGRecipe>> ReturnRecipes = {};
	if(Building) {
		const auto ModContent = AModContentRegistry::Get(WorldContext->GetWorld());
		auto       Recipes = ModContent->GetRegisteredRecipes();

		for(auto Recipe: Recipes) {
			auto RecipeClass = TSubclassOf<UFGRecipe>(Recipe.RegisteredObject);
			auto RecipeClassDefault = static_cast<UFGRecipe*>(RecipeClass->GetDefaultObject());
			check(RecipeClassDefault);

			TArray<AFGBuildableManufacturer*> lClassList = {};
			for(auto Class: RecipeClassDefault->mProducedIn) {
				lClassList.Add((AFGBuildableManufacturer*) Class.Get());
			}

			if(lClassList.Contains((AFGBuildableManufacturer*) Building.Get())) {
				ReturnRecipes.Add(RecipeClass);
			}
		}
	}

	return ReturnRecipes;
}

TArray<TSubclassOf<UFGRecipe>> UKBFL_Recipes::GetAllModRecipes(UObject* WorldContext, FName ModName) {
	const auto                     ModContent = AModContentRegistry::Get(WorldContext->GetWorld());
	auto                           Recipes = ModContent->GetRegisteredRecipes();
	TArray<TSubclassOf<UFGRecipe>> ReturnRecipes = {};

	for(auto Recipe: Recipes) {
		if(ModName == Recipe.OwnedByModReference) {
			ReturnRecipes.Add(TSubclassOf<UFGRecipe>(Recipe.RegisteredObject));
		}
	}

	return ReturnRecipes;
}

TArray<TSubclassOf<UFGRecipe>> UKBFL_Recipes::GetAllModsRecipes(UObject* WorldContext, TArray<FName> ModNames) {
	const auto                     ModContent = AModContentRegistry::Get(WorldContext->GetWorld());
	auto                           Recipes = ModContent->GetRegisteredRecipes();
	TArray<TSubclassOf<UFGRecipe>> ReturnRecipes = {};

	for(auto Recipe: Recipes) {
		if(ModNames.Contains(Recipe.OwnedByModReference)) {
			ReturnRecipes.Add(TSubclassOf<UFGRecipe>(Recipe.RegisteredObject));
		}
	}

	return ReturnRecipes;
}

TArray<FKBFLRecipeInfo> UKBFL_Recipes::GetAllModsRecipesWithInfo(UObject* WorldContext, TArray<FName> ModNames) {
	const auto              ModContent = AModContentRegistry::Get(WorldContext->GetWorld());
	auto                    Recipes = ModContent->GetRegisteredRecipes();
	TArray<FKBFLRecipeInfo> ReturnRecipes = {};

	for(auto Recipe: Recipes) {
		if(ModNames.Contains(Recipe.OwnedByModReference)) {
			FKBFLRecipeInfo Struc;
			Struc.mMod = Recipe.OwnedByModReference;
			Struc.mRecipe = TSubclassOf<UFGRecipe>(Recipe.RegisteredObject);

			ReturnRecipes.Add(Struc);
			// write Struc
		}
	}

	return ReturnRecipes;
}

void UKBFL_Recipes::CDO_MoveRecipesFromBuilding(UObject* WorldContext, TSoftClassPtr<UObject> From, TSoftClassPtr<UObject> To, TArray<TSubclassOf<UFGItemCategory>> IgnoreCategory, TArray<TSubclassOf<UFGRecipe>> IgnoreRecipe) {
	UKBFLContentCDOHelperSubsystem* Sub = UKBFLContentCDOHelperSubsystem::Get(WorldContext);
	if(IsValid(Sub)) {
		Sub->MoveRecipesFromBuilding(From, To, IgnoreCategory, IgnoreRecipe);
	}
}

bool UKBFL_Recipes::HasRecipeForm(TSubclassOf<UFGRecipe> Recipe, KBFLInOut Side, EResourceForm Form) {
	if(Recipe) {
		const auto RecipeClass = Recipe.GetDefaultObject();
		check(RecipeClass);

		TArray<FItemAmount> lChecks;
		if(Side == KBFLInOut::Both || Side == KBFLInOut::Input) {
			RecipeClass->mIngredients;
		}

		if(Side == KBFLInOut::Both || Side == KBFLInOut::Output) {
			RecipeClass->mProduct;
		}

		bool lFound = false;
		for(const auto ItemSlot: lChecks) {
			const auto SlotDefault = ItemSlot.ItemClass.GetDefaultObject();
			check(SlotDefault);
			if(SlotDefault->mForm == Form) {
				lFound = true;
				break;
			}
		}
		return lFound;
	}
	return false;
}
