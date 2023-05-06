#pragma once

#include "CoreMinimal.h"

#include "Configuration/ModConfiguration.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Util/RuntimeBlueprintFunctionLibrary.h"
#include "KBFL_ConfigTools.generated.h"

/**
* 
*/
UCLASS()
class KBFL_API UKBFL_ConfigTools : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// native helper
	static UConfigPropertySection* GetPropertySection( TSubclassOf< UModConfiguration > Config );

	template< class T >
	static T* GetPropertyByKey( TSubclassOf< UModConfiguration > Config, FString Key );

	static UConfigProperty* GetConfigPropertyByKey( TSubclassOf< UModConfiguration > Config, FString Key );

	static void SaveProperty( TSubclassOf< UModConfiguration > Config, FString Key );

	/** get an Bool from given Config
	 * return false if the Config is invalid
	 */
	UFUNCTION( BlueprintCallable, BlueprintPure, Category="KMods|ConfigHelper" )
	static bool GetBoolFromConfig( TSubclassOf< UModConfiguration > Config, FString Key );

	/** Write an Bool from given Config */
	UFUNCTION( BlueprintCallable, Category="KMods|ConfigHelper" )
	static void SetBoolInConfig( TSubclassOf< UModConfiguration > Config, FString Key, bool Value );

	/** get an Float from given Config
	 * return 0.0f if the Config is invalid
	 */
	UFUNCTION( BlueprintCallable, BlueprintPure, Category="KMods|ConfigHelper" )
	static float GetFloatFromConfig( TSubclassOf< UModConfiguration > Config, FString Key );

	/** Write an Bool from given Config */
	UFUNCTION( BlueprintCallable, Category="KMods|ConfigHelper" )
	static void SetFloatInConfig( TSubclassOf< UModConfiguration > Config, FString Key, float Value );

	/** get an UClass* from given Config
	 * return nullptr if the Config is invalid
	 */
	UFUNCTION( BlueprintCallable, BlueprintPure, Category="KMods|ConfigHelper" )
	static UClass* GetClassFromConfig( TSubclassOf< UModConfiguration > Config, FString Key );

	/** Write an UClass* from given Config */
	UFUNCTION( BlueprintCallable, Category="KMods|ConfigHelper" )
	static void SetClassInConfig( TSubclassOf< UModConfiguration > Config, FString Key, UClass* Value );

	/** get an Int from given Config
	 * return 0 if the Config is invalid
	 */
	UFUNCTION( BlueprintCallable, BlueprintPure, Category="KMods|ConfigHelper" )
	static int GetIntFromConfig( TSubclassOf< UModConfiguration > Config, FString Key );

	/** Write an Int from given Config */
	UFUNCTION( BlueprintCallable, Category="KMods|ConfigHelper" )
	static void SetIntInConfig( TSubclassOf< UModConfiguration > Config, FString Key, int Value );

	/** get an Name from given Config
	 * return nothing if the Config is invalid
	 */
	UFUNCTION( BlueprintCallable, BlueprintPure, Category="KMods|ConfigHelper" )
	static FName GetNameFromConfig( TSubclassOf< UModConfiguration > Config, FString Key );

	/** Write an FName from given Config */
	UFUNCTION( BlueprintCallable, Category="KMods|ConfigHelper" )
	static void SetNameInConfig( TSubclassOf< UModConfiguration > Config, FString Key, FName Value );

	/** get an String from given Config
	 * return nothing if the Config is invalid
	 */
	UFUNCTION( BlueprintCallable, BlueprintPure, Category="KMods|ConfigHelper" )
	static FString GetStringFromConfig( TSubclassOf< UModConfiguration > Config, FString Key );

	/** Write an String from given Config */
	UFUNCTION( BlueprintCallable, Category="KMods|ConfigHelper" )
	static void SetStringInConfig( TSubclassOf< UModConfiguration > Config, FString Key, FString Value );

	/** get an Text from given Config
	 * return nothing if the Config is invalid
	 */
	UFUNCTION( BlueprintCallable, BlueprintPure, Category="KMods|ConfigHelper" )
	static FText GetTextFromConfig( TSubclassOf< UModConfiguration > Config, FString Key );

	/** Write an Text from given Config */
	UFUNCTION( BlueprintCallable, Category="KMods|ConfigHelper" )
	static void SetTextInConfig( TSubclassOf< UModConfiguration > Config, FString Key, FText Value );
};

template< class T >
T* UKBFL_ConfigTools::GetPropertyByKey( TSubclassOf< UModConfiguration > Config, FString Key )
{
	if( UConfigPropertySection* Configuration = GetPropertySection( Config ) )
	{
		return Cast< T >( URuntimeBlueprintFunctionLibrary::Conv_ConfigPropertySectionToConfigProperty( Configuration, Key ) );
	}
	return nullptr;
}
