// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGBackgroundThread.h"
#include "KBFLWorldModule.h"
#include "BFL/KBFL_Struct.h"
#include "BFL/KBFL_Widgets.h"
#include "Interfaces/KBFLContentCDOHelperInterface.h"
#include "Interfaces/KBFLCustomizerInterface.h"
#include "Interfaces/KBFLResourceNodeInterface.h"
#include "Module/MenuWorldModule.h"
#include "KBFLMenuModule.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class KBFL_API UKBFLMenuModule: public UMenuWorldModule, public IKBFLContentCDOHelperInterface {
	GENERATED_BODY()

	public:
		UKBFLMenuModule();

		// BEGIN UGameWorldModule
		virtual void DispatchLifecycleEvent(ELifecyclePhase Phase) override;
		// END UGameWorldModule

		UFUNCTION(BlueprintNativeEvent, Category="LifecyclePhase")
		void ConstructionPhase();

		UFUNCTION(BlueprintNativeEvent, Category="LifecyclePhase")
		void InitPhase();

		UFUNCTION(BlueprintNativeEvent, Category="LifecyclePhase")
		void PostInitPhase();

		UFUNCTION(BlueprintNativeEvent, Category="LifecyclePhase")
		void OnMenuWidgetHooked(UUserWidget* WidgetClass);
		void MenuWidgetHooked(UUserWidget* WidgetClass);

	private:
		UPROPERTY(EditDefaultsOnly, Category="KMods|MainMenuHook")
		bool mHookMainMenuWidget;

		UPROPERTY(EditDefaultsOnly, Category="KMods|MainMenuHook", meta=(EditCondition=mHookMainMenuWidget))
		TSubclassOf<UUserWidget> mMenuWidgetClass;

		UPROPERTY(EditDefaultsOnly, Category="KMods|MainMenuHook", meta=(EditCondition=mHookMainMenuWidget))
		FName mFunctionName = "Construct";

		FOnWidgetCreated Binding;
};
