#include "Subsystems/KBFLResourceNodeSubsystem.h"

#include "FGBuildableSubsystem.h"
#include "FGGameState.h"
#include "BFL/KBFL_Player.h"
#include "Equipment/FGResourceScanner.h"
#include "Interfaces/KBFLResourceNodeInterface.h"
#include "Module/WorldModuleManager.h"
#include "Resources/FGResourceNodeFrackingSatellite.h"
#include "Subsystem/SubsystemActorManager.h"
#include "Subsystems/KBFLAssetDataSubsystem.h"
#include "Subsystems/ResourceNodes/KBFLActorSpawnDescriptor.h"
#include "Subsystems/ResourceNodes/KBFLSubLevelSpawning.h"

DECLARE_LOG_CATEGORY_EXTERN( ResourceNodeSubsystem, Log, All )

DEFINE_LOG_CATEGORY( ResourceNodeSubsystem )

void UKBFLResourceNodeSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
	Collection.InitializeDependency( USubsystemActorManager::StaticClass() );
	//Collection.InitializeDependency(UKBFLAssetDataSubsystem::StaticClass());

#if WITH_EDITOR
	return;
#endif

	if( GetWorld()->GetMapName().Contains( "Untitled" ) )
	{
		Super::Initialize( Collection );
		return;
	}

	UWorld* OuterWorld = GetWorld();
	OuterWorld->OnWorldBeginPlay.AddUObject( this, &UKBFLResourceNodeSubsystem::OnWorldBeginPlay );
	UE_LOG( ResourceNodeSubsystem, Log, TEXT("Initialize Subsystem") );

	mCalledModules.Empty();
	SpawnSubLevel();

	Super::Initialize( Collection );
}

void UKBFLResourceNodeSubsystem::Deinitialize()
{
	mCalledModules.Empty();
	for( UKBFLSubLevelSpawning* SubLevelSpawning : mCalledSubLevelSpawning )
	{
		SubLevelSpawning->Reset();
	}
	mCalledSubLevelSpawning.Empty();
	Initialized = false;

	Super::Deinitialize();
}

void UKBFLResourceNodeSubsystem::OnWorldBeginPlay()
{
	if( !Initialized )
	{
		UE_LOG( ResourceNodeSubsystem, Log, TEXT("ResourceNodeSubsystem > GatherInterfaces") );
		GatherInterfaces();

		Initialized = true;
	}
}

void UKBFLResourceNodeSubsystem::SpawnSubLevel()
{
	TSet< TSubclassOf< UKBFLSubLevelSpawning > > SpawnerClasses;
	GetAllSubLevel( SpawnerClasses );

	for( TSubclassOf< UKBFLSubLevelSpawning > SpawnerClass : SpawnerClasses )
	{
		if( UKBFLSubLevelSpawning* Default = SpawnerClass.GetDefaultObject() )
		{
			if( !Default->mNeedAuth || GetWorld()->GetAuthGameMode() )
			{
				if( Default->ExecuteAllowed() )
				{
					Default->mSubsystem = this;
					Default->InitSpawning();
					mCalledSubLevelSpawning.Add( Default );
				}
			}
		}
	}
}

void UKBFLResourceNodeSubsystem::GetAllSubLevel( TSet< TSubclassOf< UKBFLSubLevelSpawning > >& Out ) const
{
	UKBFLAssetDataSubsystem* AssetSubsystem = UKBFLAssetDataSubsystem::Get( GetWorld() );
	if( AssetSubsystem )
	{
		AssetSubsystem->DoScan();
		Out = AssetSubsystem->mAllSubLevelSpawningClasses;
		UE_LOG( ResourceNodeSubsystem, Log, TEXT("Found %d UKBFLSubLevelSpawning"), Out.Num() );
	}
}

void UKBFLResourceNodeSubsystem::GatherInterfaces()
{
#if !WITH_EDITOR
	UWorldModuleManager* Subsystem = Cast< UWorldModuleManager >( GetWorld()->GetSubsystem< UWorldModuleManager >() );
	for ( const auto Module : Subsystem->RootModuleList )
	{
		if ( !WasCalled( Module ) )
		{
			BeginSpawningForModule( Module );
		}
	}
#endif
}

void UKBFLResourceNodeSubsystem::BeginSpawningForModule( UWorldModule* Module )
{
	if( UKismetSystemLibrary::DoesImplementInterface( Module, UKBFLResourceNodeInterface::StaticClass() ) )
	{
		TArray< TSubclassOf< UKBFLActorSpawnDescriptorBase > > ActorSpawner;
		ActorSpawner.Append( IKBFLResourceNodeInterface::Execute_GetActorSpawnDescriptors( Module ) );

		for( TSubclassOf< UKBFLActorSpawnDescriptorBase > Desc : ActorSpawner )
		{
			if( Desc )
			{
				UE_LOG( ResourceNodeSubsystem, Log, TEXT("Call UKBFLActorSpawnDescriptorBase: %s"), *Desc->GetName() );
				UKBFLActorSpawnDescriptorBase* Default = Desc.GetDefaultObject();
				if( !Default->mNeedAuth || GetWorld()->GetAuthGameMode() )
				{
					if( Default->ExecuteAllowed() )
					{
						Default->mSubsystem = this;
						Default->BeginSpawning();
					}
				}
				else
				{
					UE_LOG( ResourceNodeSubsystem, Log, TEXT("Skip UKBFLActorSpawnDescriptorBase: %s because no auth!"), *Desc->GetName() );
				}
			}
		}

		BeginRemoveActors( IKBFLResourceNodeInterface::Execute_GetRemoveClasses( Module ) );

		Server_FinishedSpawningNodes();
		mCalledModules.AddUnique( Module );
	}
}

void UKBFLResourceNodeSubsystem::Server_FinishedSpawningNodes_Implementation()
{
	for( FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator )
	{
		if( const APlayerController* PC = Iterator->Get() )
		{
			if( AFGCharacterPlayer* Player = Cast< AFGCharacterPlayer >( PC->GetPawn() ) )
			{
				if( AFGResourceScanner* scanner = Player->GetResourceScanner() )
				{
					UE_LOG( ResourceNodeSubsystem, Log, TEXT("GenerateNodeClusters") );

					scanner->mNodeClusters.Empty();
					scanner->GenerateNodeClusters();
				}
			}
		}
	}
}

bool UKBFLResourceNodeSubsystem::Server_FinishedSpawningNodes_Validate()
{
	return true;
}

// NEW System!

bool UKBFLResourceNodeSubsystem::WasCalled( UWorldModule* Module ) const
{
	return mCalledModules.Contains( Module );
}

void UKBFLResourceNodeSubsystem::BeginRemoveActors( TArray< TSubclassOf< AActor > > ActorClasses )
{
	UE_LOG( ResourceNodeSubsystem, Log, TEXT("BeginRemoveActors, %d"), ActorClasses.Num() );
	for( TSubclassOf< AActor > Class : ActorClasses )
	{
		TArray< AActor* > OutActors;
		UGameplayStatics::GetAllActorsOfClass( GetWorld(), Class, OutActors );
		for( AActor* Node : OutActors )
		{
			if( Node )
			{
				UE_LOG( ResourceNodeSubsystem, Log, TEXT("BeginRemoveActors, Remove: %s"), *Node->GetName() );
				Node->K2_DestroyActor();
			}
		}
	}
}
