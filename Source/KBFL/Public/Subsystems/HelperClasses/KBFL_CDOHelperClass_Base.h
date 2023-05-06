#pragma once

#include "CoreMinimal.h"

#include "FGRecipe.h"
#include "Equipment/FGBuildGun.h"

#include "KBFL_CDOHelperClass_Base.generated.h"


/**
* Call Order:
* ExecuteAllowed (if false we dont call all after that)
* ModifyValues
* DoCDO
* ExecuteBlueprintCDO
*/
UCLASS( Blueprintable, EditInlineNew, abstract, DefaultToInstanced )
class KBFL_API UKBFL_CDOHelperClass_Base : public UObject
{
	GENERATED_BODY()

public:
#if WITH_ENGINE
	virtual UWorld* GetWorld() const override;
#endif

	UFUNCTION( BlueprintCallable )
	virtual void DoCDO();

	UFUNCTION( BlueprintCallable )
	virtual TArray< UClass* > GetClasses();

	UFUNCTION( BlueprintCallable )
	virtual void GetDefaultObjects( TArray< UObject* >& CDOs );

	UFUNCTION( BlueprintPure )
	static bool IsValidSoftClass( TSoftClassPtr< UObject > Class );

	UFUNCTION( BlueprintPure )
	bool HasAuth();

	UFUNCTION( BlueprintPure )
	static bool ContainBuildGun( TSubclassOf< UFGRecipe > Subclass );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	bool ExecuteAllowed() const;
	virtual bool ExecuteAllowed_Implementation() const;

	UFUNCTION( BlueprintNativeEvent )
	void ExecuteBlueprintCDO();

	virtual void ExecuteBlueprintCDO_Implementation()
	{
	}

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable )
	void ModifyValues();

	virtual void ModifyValues_Implementation()
	{
	}

	UPROPERTY( BlueprintReadWrite )
	class UKBFLContentCDOHelperSubsystem* mSubsystem = nullptr;
};
