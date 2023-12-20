#pragma once

#include "CoreMinimal.h"
#include "FGPlayerState.h"
#include "Buildables/FGBuildableResourceExtractorBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Resources/FGResourceDescriptor.h"
#include "Resources/FGResourceNode.h"
#include "Subsystem/ModSubsystem.h"

#include "KBFL_Util.generated.h"

UCLASS()
class KBFL_API UKBFL_Util: public UBlueprintFunctionLibrary {
	GENERATED_BODY()

	public:
		/**
		* Try to rebind the Node to the Extractor.
		* Is for LevelStreaming problemes for now as workaround.
		*/
		UFUNCTION(BlueprintCallable, Category="KMods|BPFL")
		static void KBFL_SetResourceNodeToExtractor(AFGBuildableResourceExtractorBase* Extractor, AFGResourceNodeBase* Node);

		/** Sorting a Item Array */
		UFUNCTION(BlueprintCallable, Category="KMods|Util")
		static void SortItemArray(TArray<TSubclassOf<UFGItemDescriptor>>& Out_Items, const TArray<TSubclassOf<UFGItemDescriptor>>& In_Items, const TArray<TSubclassOf<UFGItemDescriptor>>& ForceFirstItems, bool Reverse = false);

		/** Get Subsystem by Class */
		UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext"), Category="KMods|Util")
		static AModSubsystem* GetSubsystem(UObject* WorldContext, TSubclassOf<AModSubsystem> Subsystem);

		UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", AutoCreateRefTerm), Category="KMods|Util")
		static AModSubsystem* GetSubsystemFromChild(UObject* WorldContext, TSubclassOf<AModSubsystem> SubsystemClass);

		UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext"), Category="KMods|Util")
		static void GetAllSubsystemsFromChild(UObject* WorldContext, TSubclassOf<AModSubsystem> SubsystemClass, TArray<AModSubsystem*>& Subsystems);

		/** Remove a Array from Resources from the Scanner */
		UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"), Category="KMods|Util")
		static void RemoveArrayFromScanners(UObject* WorldContext, const TArray<TSubclassOf<UFGResourceDescriptor>> Resources);

		/** Remove all from the Scanner what not in the Array */
		UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"), Category="KMods|Util")
		static void RemoveAllNotAllowedScanners(UObject* WorldContext, const TArray<TSubclassOf<UFGResourceDescriptor>> Resources);

		/** Remove one Resource from the Scanner */
		UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"), Category="KMods|Util")
		static void RemoveFromScanner(UObject* WorldContext, const TSubclassOf<UFGResourceDescriptor> Resource);

		/** Remove all Resource Nodes from a Class */
		UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"), Category="KMods|Util")
		static void RemoveNodeClassFromWorld(UObject* WorldContext, const TSubclassOf<AFGResourceNode> NodeClass);

		/** Remove all Resource Nodes from a resource item Class */
		UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"), Category="KMods|Util")
		static void RemoveNodeResourceClassFromWorld(UObject* WorldContext, const TSubclassOf<UFGResourceDescriptor> Resource);

		/** Remove all Resource Nodes from a Array of resource item Class */
		UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"), Category="KMods|Util")
		static void RemoveNodeResourcesClassFromWorld(UObject* WorldContext, const TArray<TSubclassOf<UFGResourceDescriptor>> Resources);

		/** Remove all Resource not what NOT in the Array */
		UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"), Category="KMods|Util")
		static void RemoveNodeNOTResourcesClassFromWorld(UObject* WorldContext, const TArray<TSubclassOf<UFGResourceDescriptor>> Resources);

		/** Remove all Resource not what NOT in the Array */
		UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"), Category="KMods|Util")
		static bool DoPlayerViewLineTrace(UObject* WorldContext, FHitResult& Hit, float Distance, TArray<AActor*> ActorsToIgnore, ETraceTypeQuery TraceChannel, bool TraceComplex = true);

		/** Remove all Resource not what NOT in the Array */
		UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"), Category="KMods|Util")
		static bool DoPlayerViewLineTraceSphere(UObject* WorldContext, TArray<AActor*>& OutActors, float Distance, TArray<AActor*> ActorsToIgnore, ETraceTypeQuery TraceChannel, TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypes, TSubclassOf<AActor> ActorClass, float SphereSize = 20.f, bool TraceComplex = true);
};
