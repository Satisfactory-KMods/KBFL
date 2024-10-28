// Fill out your copyright notice in the Description page of Project Settings.


#include "KBFLMenuModule.h"

#include "BFL/KBFL_Widgets.h"
#include "Blueprint/UserWidget.h"
#include "Patching/BlueprintHookManager.h"
#include "Subsystems/KBFLAssetDataSubsystem.h"

UKBFLMenuModule::UKBFLMenuModule()
	: mHookMainMenuWidget(false)
{
	bRootModule = false;
}

void UKBFLMenuModule::DispatchLifecycleEvent(ELifecyclePhase Phase)
{
	if (Phase == ELifecyclePhase::CONSTRUCTION)
	{
		ConstructionPhase();

		Binding.BindUFunction(this, TEXT("MenuWidgetHooked"));

		if (!mMenuWidgetClass)
		{
			return;
		}
		UFunction* ConstructFunction = mMenuWidgetClass->FindFunctionByName(mFunctionName);
		if (!ConstructFunction || ConstructFunction->IsNative())
		{
			return;
		}
		UBlueprintHookManager* HookManager = GEngine->GetEngineSubsystem<UBlueprintHookManager>();
		HookManager->HookBlueprintFunction(ConstructFunction, [&](FBlueprintHookHelper& HookHelper) {
			Binding.ExecuteIfBound(Cast<UUserWidget>(HookHelper.GetContext()));
		}, Return);
	}

	if (Phase == ELifecyclePhase::INITIALIZATION)
	{
		InitPhase();
	}

	if (Phase == ELifecyclePhase::POST_INITIALIZATION)
	{
		PostInitPhase();
	}

	Super::DispatchLifecycleEvent(Phase);
}

void UKBFLMenuModule::MenuWidgetHooked(UUserWidget* WidgetClass)
{
	if (IsValid(WidgetClass))
	{
		OnMenuWidgetHooked(WidgetClass);
	}
}

void UKBFLMenuModule::OnMenuWidgetHooked_Implementation(UUserWidget* WidgetClass) {}

void UKBFLMenuModule::InitPhase_Implementation() {}

void UKBFLMenuModule::ConstructionPhase_Implementation() {}

void UKBFLMenuModule::PostInitPhase_Implementation() {}