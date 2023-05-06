// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Resources/FGResourceDescriptor.h"
#include "Resources/FGResourceNode.h"
#include "Subsystems/ResourceNodes/KBFLResourceNodeDescriptor.h"
#include "UObject/Interface.h"
#include "KBFLResourceNodeInterface.generated.h"


UINTERFACE()
class UKBFLResourceNodeInterface : public UInterface
{
	GENERATED_BODY()
};


/**
 * 
 */
class KBFL_API IKBFLResourceNodeInterface
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "KMods|ResourceNode Interface" )
	TArray< TSubclassOf< class UKBFLActorSpawnDescriptorBase > > GetActorSpawnDescriptors();

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "KMods|ResourceNode Interface" )
	TArray< TSubclassOf< AActor > > GetRemoveClasses();
};
