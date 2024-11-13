#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"

#include "KBFLEditorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class KBFL_API UKBFLEditorSubsystem : public UEditorSubsystem {
GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override { Super::Deinitialize(); };

private:
	static void OnWorldPreActorTick(UWorld* World, ELevelTick TickType, float DeltaSeconds);
};