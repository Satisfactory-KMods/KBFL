#include "KBFLModule.h"

#include "FGGameMode.h"
#include "FGGameState.h"
#include "FGPlayerController.h"
#include "KBFLWorldModule.h"
#include "Buildables/FGBuildableFrackingExtractor.h"
#include "Buildables/FGBuildableWire.h"
#include "Misc/LazySingleton.h"
#include "Module/WorldModuleManager.h"
#include "Patching/NativeHookManager.h"
#include "Subsystems/KBFLCustomizerSubsystem.h"
#include "Subsystems/KBFLResourceNodeSubsystem.h"
#include "Subsystems/KBFLUtilSubsystem.h"
#include "Utils/KBFLUtilItemCreaterBuildable.h"

DECLARE_LOG_CATEGORY_EXTERN( LogKBFLModule, Log, All );

DEFINE_LOG_CATEGORY( LogKBFLModule );

void GetBuildingColorDataForSlot( CallScope< FFactoryCustomizationColorSlot( * )( AFGGameState*, uint8 ) >& scope, AFGGameState* GameState, uint8 slot ) {
	if( IsValid(GameState) && IsValid(GameState->GetOuter()) ) {
		if( GameState->GetWorld( ) ) {
			if( UKBFLCustomizerSubsystem* CustomizerSubsystem = Cast< UKBFLCustomizerSubsystem >( GameState->GetWorld( )->GetSubsystem< UKBFLCustomizerSubsystem >( ) ) ) {
				if( !CustomizerSubsystem->Initialized ) {
					CustomizerSubsystem->GatherInterfaces( );
				}
			}
		}
	}
}


void PlayerAdjustDamage( CallScope< float( * )( AFGCharacterPlayer*, AActor*, float, const UDamageType*, AController*, AActor* ) >& Scope, AFGCharacterPlayer* Player, AActor* damagedActor, float damageAmount, const UDamageType* damageType, AController* instigatedBy, AActor* damageCauser ) {
	AFGPlayerController* Controller = Cast< AFGPlayerController >( Player->GetController( ) );
	if( Controller ) {
		if( AKBFLUtilSubsystem* Subsystem = AKBFLUtilSubsystem::Get( Player ) ) {
			if( Subsystem->bNoDamage ) {
				Scope.Override( 0.0f );
			}
		}
	}
}

void FKBFLModule::StartupModule( ) {
	#if !WITH_EDITOR
	// Config
	const TArray< FString > ModModuleNames = { "KBFL", "KUI" };

	// Logic
	int32 Added = 0;

	TArray< FString > NewLocalizationPaths;
	GConfig->GetArray( TEXT( "Internationalization" ), TEXT( "LocalizationPaths" ), NewLocalizationPaths, GGameIni );

	for( FString ModModuleName : ModModuleNames ) {
		FString LocPath = "../../../FactoryGame/Mods/{ModuleName}/Localization/{ModuleName}";
		LocPath.ReplaceInline( *FString( "{ModuleName}" ), *ModModuleName );
		if( NewLocalizationPaths.AddUnique( LocPath ) > 0 ) {
			UE_LOG( LogTemp, Log, TEXT("Module: %s ; Added LocalizationPath: %s"), *FString("KBFL"), *LocPath );
			Added++;
		}
	}

	if( Added > 0 ) {
		GConfig->SetArray( TEXT( "Internationalization" ), TEXT( "LocalizationPaths" ), NewLocalizationPaths, GGameIni );
	}

	SUBSCRIBE_METHOD_VIRTUAL( AFGCharacterPlayer::AdjustDamage, GetMutableDefault<AFGCharacterPlayer>(), &PlayerAdjustDamage );

	AFGBuildableResourceExtractorBase* FrackingExtractor = GetMutableDefault< AFGBuildableResourceExtractorBase >( );


	//SUBSCRIBE_METHOD_VIRTUAL_AFTER( AFGBuildableWire::BeginPlay, GetMutableDefault<AFGBuildableWire>(), [&](AFGBuildableWire* Wire) { if(ensure(Wire)) { FTimerHandle TimerHandle; Wire->GetWorldTimerManager().SetTimer(TimerHandle, Wire, &AFGBuildableWire::UpdateWireMesh, 1.f, false); } } )

	//SUBSCRIBE_METHOD( AFGBuildableWire::UpdateWireMesh, [&](auto& Call, AFGBuildableWire* Wire) { if(ensure(Wire) && ensure(Wire->mWireMesh)) { Call(Wire); Wire->mWireMesh->SetCustomPrimitiveDataFloat(0, Wire->GetLength() / 100); Wire->mWireMesh->SetCustomPrimitiveDataFloat(1, 0.0f); Wire->mWireMesh->SetCustomPrimitiveDataFloat(2, 0.0f); Wire->mWireMesh->SetCustomPrimitiveDataFloat(3, -48.f); Wire->mWireMesh->SetCustomPrimitiveDataFloat(4, 0.0f); Wire->mWireMesh->SetCustomPrimitiveDataFloat(5, 0.703125); Wire->mWireMesh->SetCustomPrimitiveDataFloat(5, 0.0f); Wire->mWireMesh->SetCustomPrimitiveDataFloat(6, 0.0f); Wire->mWireMesh->SetCustomPrimitiveDataFloat(7, 0.0f); Wire->mWireMesh->SetCustomPrimitiveDataFloat(8, 0.0f); Wire->mWireMesh->SetCustomPrimitiveDataFloat(9, 0.0f); Wire->mWireMesh->SetCustomPrimitiveDataFloat(10, 0.0f); } } )

	#if !PLATFORM_LINUX
		SUBSCRIBE_METHOD( AFGGameState::GetBuildingColorDataForSlot, &GetBuildingColorDataForSlot );
	#endif

	SUBSCRIBE_METHOD( UWorldModuleManager::DispatchLifecycleEvent, [](auto& Call, UWorldModuleManager* Manager, ELifecyclePhase Phase) { if(Manager->RootModuleList.Num() > 0) { UKBFLContentCDOHelperSubsystem* CDOHelperSubsystem = Manager->GetWorld()->GetGameInstance()->GetSubsystem<UKBFLContentCDOHelperSubsystem>(); UKBFLCustomizerSubsystem* CustomizerSubsystem = Manager->GetWorld()->GetSubsystem<UKBFLCustomizerSubsystem>(); UKBFLResourceNodeSubsystem* NodeSubsystem = Manager->GetWorld()->GetSubsystem<UKBFLResourceNodeSubsystem>();

	                  // going save that it called not in the registry phase to avoid crashed while CDO!
	                  for (UWorldModule* RootModule : Manager->RootModuleList) { if(UKBFLWorldModule* WorldModule = Cast<UKBFLWorldModule>(RootModule)) { if(!WorldModule->bScanForCDOsDone) { WorldModule->FindAllCDOs(); } if(CDOHelperSubsystem) CDOHelperSubsystem->BeginCDOForModule(RootModule, Phase); if(CustomizerSubsystem && WorldModule->mCallCustomizerInPhase == Phase) { UE_LOG(LogKBFLModule, Log, TEXT("Starting CustomizerSubsystem for Mod: %s"), *WorldModule->GetOwnerModReference().ToString()); CustomizerSubsystem->BeginForModule(RootModule); } if(NodeSubsystem && WorldModule->mCallNodesInPhase == Phase) { UE_LOG(LogKBFLModule, Log, TEXT("Starting NodeSubsystem for Mod: %s"), *WorldModule->GetOwnerModReference().ToString()); NodeSubsystem->BeginSpawningForModule(RootModule); } } } for (UWorldModule* RootModule : Manager->RootModuleList) { if(ensureAlways(RootModule)) { UE_LOG(LogKBFLModule, Log, TEXT("Dispatching lifecycle event %s to Mod %s"), *UModModule::LifecyclePhaseToString(Phase), *RootModule->GetOwnerModReference().ToString()); RootModule->DispatchLifecycleEvent(Phase); } } Call.Cancel(); } } );
	#endif
}

IMPLEMENT_GAME_MODULE( FKBFLModule, KBFL );
