#include "BFL/KBFL_Util.h"

#include "FGLocalPlayer.h"
#include "FGPlayerController.h"
#include "Unlocks/FGUnlockScannableResource.h"
#include "FGUnlockSubsystem.h"
#include "BFL/KBFL_Player.h"
#include "Blueprint/UserWidget.h"
#include "Buildables/FGBuildableResourceExtractorBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Patching/BlueprintHookHelper.h"
#include "Subsystem/SubsystemActorManager.h"
#include "Util/RuntimeBlueprintFunctionLibrary.h"

void UKBFL_Util::RemoveAllNotAllowedScanners( UObject* WorldContext, const TArray< TSubclassOf< UFGResourceDescriptor > > Resources )
{
	auto UnlockSubsystem = AFGUnlockSubsystem::Get( WorldContext );

	for( int i = 0; i < UnlockSubsystem->mScannableResourcesPairs.Num(); i++ )
	{
		if( !Resources.Contains( UnlockSubsystem->mScannableResourcesPairs[ i ].ResourceDescriptor ) )
		{
			UnlockSubsystem->mScannableResourcesPairs.RemoveAt( i );
		}
	}

	UnlockSubsystem->MarkPackageDirty();
}

void UKBFL_Util::SortItemArray( TArray< TSubclassOf< UFGItemDescriptor > >& Out_Items, const TArray< TSubclassOf< UFGItemDescriptor > >& In_Items, const TArray< TSubclassOf< UFGItemDescriptor > >& ForceFirstItems, bool Reverse )
{
	Out_Items = In_Items;
	if( Out_Items.Num() > 1 )
	{
		Out_Items.Sort( [Reverse]( const TSubclassOf< UFGItemDescriptor > A, const TSubclassOf< UFGItemDescriptor > B )
		{
			if( !Reverse )
			{
				return UFGItemDescriptor::GetItemName( A ).ToString() < UFGItemDescriptor::GetItemName( B ).ToString();
			}
			return UFGItemDescriptor::GetItemName( A ).ToString() > UFGItemDescriptor::GetItemName( B ).ToString();
		} );

		if( ForceFirstItems.Num() > 0 )
		{
			TArray< TSubclassOf< UFGItemDescriptor > > ForceReturn;
			for( TSubclassOf< UFGItemDescriptor > Out_Item : ForceFirstItems )
			{
				if( Out_Items.Contains( Out_Item ) )
				{
					Out_Items.Remove( Out_Item );
					ForceReturn.Add( Out_Item );
				}
			}
			ForceReturn.Append( Out_Items );
			Out_Items = ForceReturn;
		}
	}
}

void UKBFL_Util::KBFL_SetResourceNodeToExtractor( AFGBuildableResourceExtractorBase* Extractor, AFGResourceNodeBase* Node )
{
	if( ensure( IsValid(Node) && IsValid(Extractor) ) )
	{
		if( Node->IsOccupied() )
		{
			UE_LOG( LogTemp, Warning, TEXT("Skipp Because is IsOccupied") )
			return;
		}

		if( AFGResourceNode* ResNode = Cast< AFGResourceNode >( Node ) )
		{
			if( !Node->IsOccupied() )
			{
				Extractor->SetResourceNode( ResNode );
				UE_LOG( LogTemp, Warning, TEXT("AFGResourceNode") )
			}
		}

		if( UKismetSystemLibrary::DoesImplementInterface( Node, UFGExtractableResourceInterface::StaticClass() ) )
		{
			if( !Node->IsOccupied() )
			{
				Extractor->SetExtractableResource( Node );
				UE_LOG( LogTemp, Warning, TEXT("SetResourceNodeToExtractor: SetExtractableResource") )
			}
		}
	}
}

AModSubsystem* UKBFL_Util::GetSubsystem( UObject* WorldContext, TSubclassOf< AModSubsystem > Subsystem )
{
	if( WorldContext )
	{
		USubsystemActorManager* SubsystemActorManager = WorldContext->GetWorld()->GetSubsystem< USubsystemActorManager >();
		check( SubsystemActorManager );
		return SubsystemActorManager->K2_GetSubsystemActor( Subsystem );
	}
	return nullptr;
}

void UKBFL_Util::RemoveArrayFromScanners( UObject* WorldContext, const TArray< TSubclassOf< UFGResourceDescriptor > > Resources )
{
	auto UnlockSubsystem = AFGUnlockSubsystem::Get( WorldContext );

	for( int i = 0; i < UnlockSubsystem->mScannableResourcesPairs.Num(); i++ )
	{
		if( Resources.Contains( UnlockSubsystem->mScannableResourcesPairs[ i ].ResourceDescriptor ) )
		{
			UnlockSubsystem->mScannableResourcesPairs.RemoveAt( i );
		}
	}
	UnlockSubsystem->MarkPackageDirty();
}

void UKBFL_Util::RemoveFromScanner( UObject* WorldContext, const TSubclassOf< UFGResourceDescriptor > Resource )
{
	auto UnlockSubsystem = AFGUnlockSubsystem::Get( WorldContext );

	for( int i = 0; i < UnlockSubsystem->mScannableResourcesPairs.Num(); i++ )
	{
		if( UnlockSubsystem->mScannableResourcesPairs[ i ].ResourceDescriptor == Resource )
		{
			UnlockSubsystem->mScannableResourcesPairs.RemoveAt( i );
			break;
		}
	}
	UnlockSubsystem->MarkPackageDirty();
}

void UKBFL_Util::RemoveNodeClassFromWorld( UObject* WorldContext, const TSubclassOf< AFGResourceNode > NodeClass )
{
	TArray< AActor* > Actors;
	UGameplayStatics::GetAllActorsOfClass( WorldContext, NodeClass, Actors );

	if( Actors.Num() > 0 )
	{
		for( auto Actor : Actors )
		{
			Actor->Destroy();
		}
	}
}

void UKBFL_Util::RemoveNodeResourceClassFromWorld( UObject* WorldContext, const TSubclassOf< UFGResourceDescriptor > Resource )
{
	TArray< AActor* > Actors;
	UGameplayStatics::GetAllActorsOfClass( WorldContext, AFGResourceNode::StaticClass(), Actors );

	if( Actors.Num() > 0 )
	{
		for( auto Actor : Actors )
		{
			auto Node = Cast< AFGResourceNode >( Actor );
			if( Node->GetResourceClass() == Resource )
			{
				Actor->Destroy();
			}
		}
	}
}

void UKBFL_Util::RemoveNodeResourcesClassFromWorld( UObject* WorldContext, const TArray< TSubclassOf< UFGResourceDescriptor > > Resources )
{
	TArray< AActor* > Actors;
	UGameplayStatics::GetAllActorsOfClass( WorldContext, AFGResourceNode::StaticClass(), Actors );

	if( Actors.Num() > 0 )
	{
		for( auto Actor : Actors )
		{
			auto Node = Cast< AFGResourceNode >( Actor );
			if( Resources.Contains( Node->GetResourceClass() ) )
			{
				Actor->Destroy();
			}
		}
	}
}

void UKBFL_Util::RemoveNodeNOTResourcesClassFromWorld( UObject* WorldContext, const TArray< TSubclassOf< UFGResourceDescriptor > > Resources )
{
	TArray< AActor* > Actors;
	UGameplayStatics::GetAllActorsOfClass( WorldContext, AFGResourceNode::StaticClass(), Actors );

	if( Actors.Num() > 0 )
	{
		for( auto Actor : Actors )
		{
			auto Node = Cast< AFGResourceNode >( Actor );
			if( !Resources.Contains( Node->GetResourceClass() ) )
			{
				Actor->Destroy();
			}
		}
	}
}

bool UKBFL_Util::DoPlayerViewLineTrace( UObject* WorldContext, FHitResult& Hit, float Distance, TArray< AActor* > ActorsToIgnore, ETraceTypeQuery TraceChannel, bool TraceComplex )
{
	if( AFGCharacterPlayer* Char = UKBFL_Player::GetFGCharacter( WorldContext ) )
	{
		FVector Start = Char->GetCameraComponentWorldLocation();
		FVector End = Start + Distance * Char->GetCameraComponentForwardVector();

		// Add Player self that he dont trace!
		ActorsToIgnore.AddUnique( Char );

		UKismetSystemLibrary::LineTraceSingle( WorldContext, Start, End, TraceChannel, TraceComplex, ActorsToIgnore, EDrawDebugTrace::None, Hit, true );
		return Hit.IsValidBlockingHit();
	}
	return false;
}

bool UKBFL_Util::DoPlayerViewLineTraceSphere( UObject* WorldContext, TArray< AActor* >& OutActors, float Distance, TArray< AActor* > ActorsToIgnore, ETraceTypeQuery TraceChannel, TArray< TEnumAsByte< EObjectTypeQuery > > ObjTypes, TSubclassOf< AActor > ActorClass, float SphereSize, bool TraceComplex )
{
	if( AFGCharacterPlayer* Char = UKBFL_Player::GetFGCharacter( WorldContext ) )
	{
		FHitResult LineHit;
		if( DoPlayerViewLineTrace( WorldContext, LineHit, Distance, ActorsToIgnore, TraceChannel, TraceComplex ) )
		{
			ActorsToIgnore.AddUnique( Char );
			return UKismetSystemLibrary::SphereOverlapActors( WorldContext, LineHit.ImpactPoint, SphereSize, ObjTypes, ActorClass, ActorsToIgnore, OutActors );
		}
	}
	return false;
}

AModSubsystem* UKBFL_Util::GetSubsystemFromChild( UObject* WorldContext, TSubclassOf< AModSubsystem > SubsystemClass )
{
#if WITH_EDITOR
	return SubsystemClass.GetDefaultObject();
#endif

	if( WorldContext )
	{
		const UWorld* WorldObject = GEngine->GetWorldFromContextObjectChecked( WorldContext );
		USubsystemActorManager* SubsystemActorManager = WorldObject->GetSubsystem< USubsystemActorManager >();
		check( SubsystemActorManager );

		for( auto Subsystem : SubsystemActorManager->SubsystemActors )
		{
			if( Subsystem.Key->IsChildOf( SubsystemClass ) )
			{
				return Subsystem.Value;
			}
		}
	}
	return nullptr;
}

void UKBFL_Util::GetAllSubsystemsFromChild( UObject* WorldContext, TSubclassOf< AModSubsystem > SubsystemClass, TArray< AModSubsystem* >& Subsystems )
{
#if WITH_EDITOR
	return;
#endif

	if( WorldContext )
	{
		const UWorld* WorldObject = GEngine->GetWorldFromContextObjectChecked( WorldContext );
		USubsystemActorManager* SubsystemActorManager = WorldObject->GetSubsystem< USubsystemActorManager >();
		check( SubsystemActorManager );

		for( auto Subsystem : SubsystemActorManager->SubsystemActors )
		{
			if( Subsystem.Key->IsChildOf( SubsystemClass ) )
			{
				Subsystems.AddUnique( Subsystem.Value );
			}
		}
	}
}
