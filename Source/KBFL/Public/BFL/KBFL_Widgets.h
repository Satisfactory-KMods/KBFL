#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KBFL_Widgets.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam( FOnWidgetCreated, UUserWidget*, Widget );

UCLASS()
class KBFL_API UKBFL_Widgets : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Can Bind a Event what hit on construct a widget from a Class (Contain the widget Object) */
	UFUNCTION( BlueprintCallable, Category="KMods|WidgetUtils" )
	static void BindOnWidget( const TSubclassOf< UUserWidget > WidgetClass, FOnWidgetCreated Binding );

	/** Can Bind a Event what hit on construct a widget from a Class (Contain the widget Object) */
	UFUNCTION( BlueprintCallable, Category="KMods|WidgetUtils" )
	static void BindOnPreWidget( const TSubclassOf< UUserWidget > WidgetClass, FOnWidgetCreated Binding );

	static FOnWidgetCreated OnWidgetCreated;
};
