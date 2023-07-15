#include "Subsystems/KBFLUtilSubsystem.h"

#include "FGCharacterMovementComponent.h"
#include "FGResearchManager.h"
#include "FGSchematicManager.h"
#include "FGTutorialIntroManager.h"
#include "BFL/KBFL_Player.h"
#include "BFL/KBFL_Schematics.h"
#include "BFL/KBFL_Util.h"
#include "Registry/ModContentRegistry.h"
#include "Resources/FGBuildDescriptor.h"

// Sets default values
AKBFLUtilSubsystem::AKBFLUtilSubsystem() {
	ReplicationPolicy = ESubsystemReplicationPolicy::SpawnOnServer_Replicate;

	SetActorTickEnabled(true);
	PrimaryActorTick.SetTickFunctionEnable(true);
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = .5f;
	bAlwaysRelevant = true;

	bReplicates = true;
}

AKBFLUtilSubsystem* AKBFLUtilSubsystem::Get(UObject* worldContext) {
	return Cast<AKBFLUtilSubsystem>(UKBFL_Util::GetSubsystem(worldContext, StaticClass()));
}

void AKBFLUtilSubsystem::BeginPlay() {
	Super::BeginPlay();

	mTimeSubsystem = AFGTimeOfDaySubsystem::Get(this);
	mPlayerController = UKBFL_Player::GetFGController(this);
	if(mPlayerController) {
		mPlayerController->EnableCheats();
	}

	if(HasAuthority()) {
		mCheatManager = Cast<UFGCheatManager>(mPlayerController->CheatManager);
	}
}

void AKBFLUtilSubsystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKBFLUtilSubsystem, mCheatManager);
	DOREPLIFETIME(AKBFLUtilSubsystem, mTimeSubsystem);

	DOREPLIFETIME(AKBFLUtilSubsystem, bIsHostEnableCheatMode);
	DOREPLIFETIME(AKBFLUtilSubsystem, bIsCheatEnabled);
	DOREPLIFETIME(AKBFLUtilSubsystem, bIsFlyEnabled);
	DOREPLIFETIME(AKBFLUtilSubsystem, bIsGhostEnabled);
	DOREPLIFETIME(AKBFLUtilSubsystem, bNoDamage);
	DOREPLIFETIME(AKBFLUtilSubsystem, bIsNoPower);
	DOREPLIFETIME(AKBFLUtilSubsystem, bIsNoMessages);
	DOREPLIFETIME(AKBFLUtilSubsystem, bIsNoCost);
	DOREPLIFETIME(AKBFLUtilSubsystem, bIsNoFuel);
}

void AKBFLUtilSubsystem::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if(HasAuthority()) {
		/*if(!mPlayerController)
		{
			mPlayerController = UKBFL_Player::GetFGController(this);
			if(mPlayerController->CheatE)
				mPlayerController->EnableCheats();
		}*/

		if(!mCheatManager && mPlayerController) {
			mCheatManager = Cast<UFGCheatManager>(mPlayerController->CheatManager);
		}

		if(mCheatManager) {
			if(bIsCheatEnabled && bIsHostEnableCheatMode) {
				if(mPlayerController) {
					ACharacter* Character = mPlayerController->GetCharacter();
					if(Character) {
						if(Character->IsA<AFGCharacterPlayer>()) {
							UFGCharacterMovementComponent* CharacterMovement = Cast<UFGCharacterMovementComponent>(Character->GetCharacterMovement());
							if(CharacterMovement) {
								if(CharacterMovement->bCheatFlying != bIsFlyEnabled) {
									CharacterMovement->SetMovementMode(bIsFlyEnabled ? MOVE_Flying : MOVE_Falling);
								}
								CharacterMovement->bCheatFlying = bIsFlyEnabled;
								if(CharacterMovement->IsA<UFGCharacterMovementComponent>()) {
									Character->SetActorEnableCollision(!bIsGhostEnabled);
								}
							}
						}
					}
				}

				//if(bIsTurboMode != mCheatManager->TurboMode_Get())
				//	mCheatManager->TurboProductionMode(bIsTurboMode);

				//if(bIsNoMessages != mCheatManager->NoMessages_Get())
				//mCheatManager->NoMessages(bIsNoMessages);

				if(bIsNoPower != mCheatManager->NoCost_Get() || bIsNoPower != mCheatManager->NoPower_Get() || bIsNoPower != mCheatManager->NoFuel_Get()) {
					mCheatManager->NoCost(bIsNoPower);
					mCheatManager->NoFuel(bIsNoPower);
					mCheatManager->NoPower(bIsNoPower);
					UE_LOG(LogTemp, Warning, TEXT("Update Cheats NoCost_Get( %d ), NoPower_Get( %d ), NoFuel_Get( %d )"), mCheatManager->NoCost_Get(), mCheatManager->NoPower_Get(), mCheatManager->NoFuel_Get())
				}
			} else if(!bIsCheatEnabled) {
				//if(mCheatManager->PlayerAllFly_Get())
				//{
				//	mCheatManager->PlayerAllFly(false);
				//	mCheatManager->PlayerFly(false);
				//}

				//if(mCheatManager->PlayerNoClipModeOnFly_Get())
				//	mCheatManager->PlayerNoClipModeOnFly(false);

				//if(mCheatManager->TurboMode_Get())
				//mCheatManager->TurboProductionMode(false);

				if(mCheatManager->NoPower_Get()) {
					mCheatManager->NoPower(false);
				}

				//if(mCheatManager->NoMessages_Get())
				//mCheatManager->NoMessages(false);

				if(mCheatManager->NoCost_Get()) {
					mCheatManager->NoCost(false);
				}

				if(mCheatManager->NoFuel_Get()) {
					mCheatManager->NoFuel(false);
				}
			}
		}
	}
}

void AKBFLUtilSubsystem::ToggleCheat() {
	if(HasAuthority()) {
		bIsCheatEnabled = !bIsCheatEnabled;
	} else if(AFGPlayerController* Player = UKBFL_Player::GetFGController(this)) {
		if(UKBFLCheatRCO*          RCO = Player->GetRemoteCallObjectOfClass<UKBFLCheatRCO>()) {
			RCO->Server_ToggleCheats(this);
		}
	}
}

void AKBFLUtilSubsystem::SetCheatModeByHost(bool NewValue) {
	if(HasAuthority()) {
		bIsHostEnableCheatMode = NewValue;
	}
}

void AKBFLUtilSubsystem::ToggleGhost() {
	if(HasAuthority()) {
		bIsGhostEnabled = !bIsGhostEnabled;
	} else if(AFGPlayerController* Player = UKBFL_Player::GetFGController(this)) {
		if(UKBFLCheatRCO*          RCO = Player->GetRemoteCallObjectOfClass<UKBFLCheatRCO>()) {
			RCO->Server_ToggleGhost(this);
		}
	}
}

void AKBFLUtilSubsystem::ToggleFly() {
	if(HasAuthority()) {
		bIsFlyEnabled = !bIsFlyEnabled;
	} else if(AFGPlayerController* Player = UKBFL_Player::GetFGController(this)) {
		if(UKBFLCheatRCO*          RCO = Player->GetRemoteCallObjectOfClass<UKBFLCheatRCO>()) {
			RCO->Server_ToggleFly(this);
		}
	}
}

void AKBFLUtilSubsystem::UnlockAll() {
	if(HasAuthority()) {
		if(mCheatManager) {
			mCheatManager->GiveAllSchematics();
			mCheatManager->GiveAllSchematicsAndPhases();
			mCheatManager->GiveActiveMilestoneSchematic();
			mCheatManager->GiveAllResearchTrees();

			AModContentRegistry*     ContentRegistry = AModContentRegistry::Get(GetWorld());
			AFGTutorialIntroManager* TutorialManager = AFGTutorialIntroManager::Get(GetWorld());
			AFGSchematicManager*     SchematicManager = AFGSchematicManager::Get(GetWorld());

			if(ContentRegistry && SchematicManager) {
				for(FSchematicRegistrationInfo RegisteredSchematic: ContentRegistry->GetRegisteredSchematics()) {
					TSubclassOf<UFGSchematic> Schematic = RegisteredSchematic.RegisteredObject;
					if(Schematic && !SchematicManager->IsSchematicPurchased(Schematic)) {
						SchematicManager->AddAvailableSchematic(Schematic);
						SchematicManager->GiveAccessToSchematic(Schematic, nullptr, true);
					}
				}
			}

			if(TutorialManager) {
				TutorialManager->CompleteTutorial();
			}
		}
	} else if(AFGPlayerController* Player = UKBFL_Player::GetFGController(this)) {
		if(UKBFLCheatRCO*          RCO = Player->GetRemoteCallObjectOfClass<UKBFLCheatRCO>()) {
			RCO->Server_UnlockAll(this);
		}
	}
}

void AKBFLUtilSubsystem::ResetAll() {
	if(HasAuthority()) {
		if(mCheatManager) {
			mCheatManager->ResetGamePhases();
			mCheatManager->ResetSchematics();
			mCheatManager->ResetHubTutorial();
			mCheatManager->ResetRecipes();
		}
	} else if(AFGPlayerController* Player = UKBFL_Player::GetFGController(this)) {
		if(UKBFLCheatRCO*          RCO = Player->GetRemoteCallObjectOfClass<UKBFLCheatRCO>()) {
			RCO->Server_ResetAll(this);
		}
	}
}

void AKBFLUtilSubsystem::ToggleNoPower() {
	if(HasAuthority()) {
		bIsNoPower = !bIsNoPower;
	} else if(AFGPlayerController* Player = UKBFL_Player::GetFGController(this)) {
		if(UKBFLCheatRCO*          RCO = Player->GetRemoteCallObjectOfClass<UKBFLCheatRCO>()) {
			RCO->Server_ToggleNoPower(this);
		}
	}
}

void AKBFLUtilSubsystem::ToggleNoCost() {
	if(HasAuthority()) {
		bIsNoCost = !bIsNoCost;
	} else if(AFGPlayerController* Player = UKBFL_Player::GetFGController(this)) {
		if(UKBFLCheatRCO*          RCO = Player->GetRemoteCallObjectOfClass<UKBFLCheatRCO>()) {
			RCO->Server_ToggleNoCost(this);
		}
	}
}

void AKBFLUtilSubsystem::ToggleNoFuel() {
	if(HasAuthority()) {
		bIsNoFuel = !bIsNoFuel;
	} else if(AFGPlayerController* Player = UKBFL_Player::GetFGController(this)) {
		if(UKBFLCheatRCO*          RCO = Player->GetRemoteCallObjectOfClass<UKBFLCheatRCO>()) {
			RCO->Server_ToggleNoFuel(this);
		}
	}
}

void AKBFLUtilSubsystem::ToggleNoMessages() {
	if(HasAuthority()) {
		bIsNoMessages = !bIsNoMessages;
	} else if(AFGPlayerController* Player = UKBFL_Player::GetFGController(this)) {
		if(UKBFLCheatRCO*          RCO = Player->GetRemoteCallObjectOfClass<UKBFLCheatRCO>()) {
			RCO->Server_ToggleNoMessages(this);
		}
	}
}

void AKBFLUtilSubsystem::ToggleNoDamage() {
	if(HasAuthority()) {
		bNoDamage = !bNoDamage;
	} else if(AFGPlayerController* Player = UKBFL_Player::GetFGController(this)) {
		if(UKBFLCheatRCO*          RCO = Player->GetRemoteCallObjectOfClass<UKBFLCheatRCO>()) {
			RCO->Server_ToggleNoDamage(this);
		}
	}
}

void AKBFLUtilSubsystem::FinishResearch() {
	if(HasAuthority()) {
		if(AFGResearchManager* Manager = AFGResearchManager::Get(GetWorld())) {
			if(Manager->IsAnyResearchBeingConducted()) {
				for(FResearchTime& Research: Manager->mOngoingResearch) {
					if(Research.TimerHandle.IsValid()) {
						GetWorldTimerManager().ClearTimer(Research.TimerHandle);
					}
					Manager->mCompletedResearch.Add(Research.ResearchData);
					Manager->Multicast_ResearchCompleted(Research.ResearchData.Schematic);
				}
				Manager->mOngoingResearch.Empty();
			}
		}
	} else if(AFGPlayerController* Player = UKBFL_Player::GetFGController(this)) {
		if(UKBFLCheatRCO*          RCO = Player->GetRemoteCallObjectOfClass<UKBFLCheatRCO>()) {
			RCO->Server_FinishResearch(this);
		}
	}
}

void AKBFLUtilSubsystem::SetTime(float NewTime) {
	if(HasAuthority()) {
		if(mTimeSubsystem) {
			mTimeSubsystem->SetDaySeconds(NewTime);
		}
	} else if(AFGPlayerController* Player = UKBFL_Player::GetFGController(this)) {
		if(UKBFLCheatRCO*          RCO = Player->GetRemoteCallObjectOfClass<UKBFLCheatRCO>()) {
			RCO->Server_SetTime(this, NewTime);
			if(mTimeSubsystem) {
				mTimeSubsystem->SetDaySeconds(NewTime);
			}
		}
	}
}
