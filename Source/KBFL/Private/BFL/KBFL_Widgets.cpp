#include "BFL/KBFL_Widgets.h"

#include <string>

#include "Blueprint/UserWidget.h"
#include "Patching/BlueprintHookHelper.h"
#include "Patching/BlueprintHookManager.h"

void UKBFL_Widgets::BindOnWidget( const TSubclassOf< UUserWidget > WidgetClass, FOnWidgetCreated Binding )
{
	if( !WidgetClass )
	{
		return;
	}
	UFunction* ConstructFunction = WidgetClass->FindFunctionByName( TEXT( "Construct" ) );
	if( !ConstructFunction || ConstructFunction->IsNative() )
	{
		return;
	}
	UBlueprintHookManager* HookManager = GEngine->GetEngineSubsystem< UBlueprintHookManager >();
	HookManager->HookBlueprintFunction( ConstructFunction, [Binding]( FBlueprintHookHelper& HookHelper )
	{
		Binding.ExecuteIfBound( Cast< UUserWidget >( HookHelper.GetContext() ) );
	}, EPredefinedHookOffset::Return );
}

void UKBFL_Widgets::BindOnPreWidget( const TSubclassOf< UUserWidget > WidgetClass, FOnWidgetCreated Binding )
{
	if( !WidgetClass )
	{
		return;
	}
	UFunction* ConstructFunction = WidgetClass->FindFunctionByName( TEXT( "PreConstruct" ) );
	if( !ConstructFunction || ConstructFunction->IsNative() )
	{
		return;
	}
	UBlueprintHookManager* HookManager = GEngine->GetEngineSubsystem< UBlueprintHookManager >();
	HookManager->HookBlueprintFunction( ConstructFunction, [Binding]( FBlueprintHookHelper& HookHelper )
	{
		Binding.ExecuteIfBound( Cast< UUserWidget >( HookHelper.GetContext() ) );
	}, EPredefinedHookOffset::Return );
}
