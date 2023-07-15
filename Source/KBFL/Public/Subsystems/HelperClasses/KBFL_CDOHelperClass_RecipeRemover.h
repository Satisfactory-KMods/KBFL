#pragma once

#include "CoreMinimal.h"
#include "FGRecipe.h"
#include "FGRecipeManager.h"
#include "KBFL_CDOHelperClass_RemoverBase.h"
#include "KBFL_CDOHelperClass_RecipeRemover.generated.h"


/**
* 
*/
UCLASS()
class KBFL_API UKBFL_CDOHelperClass_RecipeRemover: public UKBFL_CDOHelperClass_RemoverBase {
	GENERATED_BODY()

	public:
		virtual void            DoCDO() override;
		virtual TArray<UClass*> GetClasses() override;

		/** must be set for CDO */
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="CDO Helper")
		TArray<TSoftClassPtr<UFGRecipe>> mRecipes;
};
