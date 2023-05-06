// Fill out your copyright notice in the Description page of Project Settings.


#include "C++/KBFLCppActorHelper.h"

bool UKBFLCppActorHelper::ActorsInSphere( UObject* WorldContext, UClass* ActorClass, FVector Location, TArray< AActor* > Ignore, TArray< AActor* >& OutActors, float Range )
{
	const TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes = TArray< TEnumAsByte< EObjectTypeQuery > >{ EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2 };

	return UKismetSystemLibrary::SphereOverlapActors( WorldContext->GetWorld(), Location, Range, ObjectTypes, ActorClass, Ignore, OutActors );
}

bool UKBFLCppActorHelper::HasFreeLineToActor( UObject* WorldContext, AActor* ActorSource, AActor* ActorTarget, float ZOffsetSource, float ZOffsetTarget )
{
	if( ActorSource && ActorTarget )
	{
		FVector Source = ActorSource->GetActorLocation();
		FVector Target = ActorTarget->GetActorLocation();
		Source.Z += ZOffsetSource;
		Target.Z += ZOffsetTarget;

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor( ActorSource );
		CollisionParams.AddIgnoredActor( ActorTarget );
		FHitResult OutHit;
		WorldContext->GetWorld()->LineTraceSingleByChannel( OutHit, Source, Target, ECC_MAX, CollisionParams );

		return !OutHit.IsValidBlockingHit();
	}
	return false;
}

bool UKBFLCppActorHelper::HasFreeLineToComponent( UObject* WorldContext, USceneComponent* ComponentSource, USceneComponent* ComponentTarget, float ZOffsetSource, float ZOffsetTarget )
{
	if( ComponentSource && ComponentTarget )
	{
		FVector Source = ComponentSource->GetComponentLocation();
		FVector Target = ComponentSource->GetComponentLocation();
		Source.Z += ZOffsetSource;
		Target.Z += ZOffsetTarget;

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor( ComponentSource->GetOwner() );
		CollisionParams.AddIgnoredActor( ComponentTarget->GetOwner() );
		CollisionParams.MobilityType = EQueryMobilityType::Any;
		FHitResult OutHit;
		WorldContext->GetWorld()->LineTraceSingleByObjectType( OutHit, Source, Target, ECC_Visibility, CollisionParams );

		return !OutHit.IsValidBlockingHit();
	}
	return false;
}
