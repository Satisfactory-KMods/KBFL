#include "BFL/KBFL_ConfigTools.h"

#include "Configuration/Properties/ConfigPropertyClass.h"

DECLARE_LOG_CATEGORY_EXTERN(KBFLConfigToolLog, Log, All)

DEFINE_LOG_CATEGORY(KBFLConfigToolLog)

UConfigPropertySection* UKBFL_ConfigTools::GetPropertySection(TSubclassOf<UModConfiguration> Config) {
	return Cast<UConfigPropertySection>(URuntimeBlueprintFunctionLibrary::GetModConfigurationPropertyByClass(Config));
}

UConfigProperty*               UKBFL_ConfigTools::GetConfigPropertyByKey(TSubclassOf<UModConfiguration> Config, FString Key) {
	if(UConfigPropertySection* Configuration = GetPropertySection(Config)) {
		return URuntimeBlueprintFunctionLibrary::Conv_ConfigPropertySectionToConfigProperty(Configuration, Key);
	}
	return nullptr;
}

void                           UKBFL_ConfigTools::SaveProperty(TSubclassOf<UModConfiguration> Config, FString Key) {
	if(UConfigPropertySection* Section = GetPropertySection(Config)) {
		Section->MarkDirty();
		if(UConfigProperty* Property = GetPropertyByKey<UConfigProperty>(Config, Key)) {
			Property->MarkDirty();
		}
	}
}

bool UKBFL_ConfigTools::GetBoolFromConfig(TSubclassOf<UModConfiguration> Config, FString Key) {
	return URuntimeBlueprintFunctionLibrary::Conv_ConfigPropertyToBool(GetConfigPropertyByKey(Config, Key));
}

void                        UKBFL_ConfigTools::SetBoolInConfig(TSubclassOf<UModConfiguration> Config, FString Key, bool Value) {
	if(UConfigPropertyBool* Property = GetPropertyByKey<UConfigPropertyBool>(Config, Key)) {
		Property->Value = Value;
		SaveProperty(Config, Key);
	}
}

float UKBFL_ConfigTools::GetFloatFromConfig(TSubclassOf<UModConfiguration> Config, FString Key) {
	return URuntimeBlueprintFunctionLibrary::Conv_ConfigPropertyToFloat(GetConfigPropertyByKey(Config, Key));
}

void                         UKBFL_ConfigTools::SetFloatInConfig(TSubclassOf<UModConfiguration> Config, FString Key, float Value) {
	if(UConfigPropertyFloat* Property = GetPropertyByKey<UConfigPropertyFloat>(Config, Key)) {
		Property->Value = Value;
		SaveProperty(Config, Key);
	}
}

UClass*                      UKBFL_ConfigTools::GetClassFromConfig(TSubclassOf<UModConfiguration> Config, FString Key) {
	if(UConfigPropertyClass* Property = GetPropertyByKey<UConfigPropertyClass>(Config, Key)) {
		return Property->BaseClass;
	}
	return nullptr;
}

void                         UKBFL_ConfigTools::SetClassInConfig(TSubclassOf<UModConfiguration> Config, FString Key, UClass* Value) {
	if(UConfigPropertyClass* Property = GetPropertyByKey<UConfigPropertyClass>(Config, Key)) {
		Property->Value = Value;
		SaveProperty(Config, Key);
	}
}

int UKBFL_ConfigTools::GetIntFromConfig(TSubclassOf<UModConfiguration> Config, FString Key) {
	return URuntimeBlueprintFunctionLibrary::Conv_ConfigPropertyToInteger(GetConfigPropertyByKey(Config, Key));
}

void                           UKBFL_ConfigTools::SetIntInConfig(TSubclassOf<UModConfiguration> Config, FString Key, int Value) {
	if(UConfigPropertyInteger* Property = GetPropertyByKey<UConfigPropertyInteger>(Config, Key)) {
		Property->Value = Value;
		SaveProperty(Config, Key);
	}
}

FName UKBFL_ConfigTools::GetNameFromConfig(TSubclassOf<UModConfiguration> Config, FString Key) {
	return URuntimeBlueprintFunctionLibrary::Conv_ConfigPropertyToName(GetConfigPropertyByKey(Config, Key));
}

void                          UKBFL_ConfigTools::SetNameInConfig(TSubclassOf<UModConfiguration> Config, FString Key, FName Value) {
	if(UConfigPropertyString* Property = GetPropertyByKey<UConfigPropertyString>(Config, Key)) {
		Property->Value = Value.ToString();
		SaveProperty(Config, Key);
	}
}

FString UKBFL_ConfigTools::GetStringFromConfig(TSubclassOf<UModConfiguration> Config, FString Key) {
	return URuntimeBlueprintFunctionLibrary::Conv_ConfigPropertyToString(GetConfigPropertyByKey(Config, Key));
}

void                          UKBFL_ConfigTools::SetStringInConfig(TSubclassOf<UModConfiguration> Config, FString Key, FString Value) {
	if(UConfigPropertyString* Property = GetPropertyByKey<UConfigPropertyString>(Config, Key)) {
		Property->Value = Value;
		SaveProperty(Config, Key);
	}
}

FText UKBFL_ConfigTools::GetTextFromConfig(TSubclassOf<UModConfiguration> Config, FString Key) {
	return URuntimeBlueprintFunctionLibrary::Conv_ConfigPropertyToText(GetConfigPropertyByKey(Config, Key));
}

void                          UKBFL_ConfigTools::SetTextInConfig(TSubclassOf<UModConfiguration> Config, FString Key, FText Value) {
	if(UConfigPropertyString* Property = GetPropertyByKey<UConfigPropertyString>(Config, Key)) {
		Property->Value = Value.ToString();
		SaveProperty(Config, Key);
	}
}
