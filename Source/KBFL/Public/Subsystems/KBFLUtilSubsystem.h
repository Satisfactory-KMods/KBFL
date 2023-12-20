#pragma once

#include "CoreMinimal.h"
#include "FGCheatManager.h"
#include "FGRemoteCallObject.h"

#include "FGSaveInterface.h"
#include "FGTimeSubsystem.h"

#include "Net/UnrealNetwork.h"

#include "Subsystem/ModSubsystem.h"

#include "KBFLUtilSubsystem.generated.h"

UCLASS( )
class KBFL_API AKBFLUtilSubsystem : public AModSubsystem, public IFGSaveInterface {
	GENERATED_BODY( )

	public:
		AKBFLUtilSubsystem( );

		UFUNCTION( BlueprintPure, Category="Time", DisplayName="GetKBFLUtilSubsystem", meta=(DefaultToSelf="worldContext") )
		static AKBFLUtilSubsystem* Get( UObject* worldContext );

		virtual void BeginPlay( ) override;

		virtual void GetLifetimeReplicatedProps( TArray< FLifetimeProperty >& OutLifetimeProps ) const override;

		// Handle Cheats Per Tick!
		virtual void Tick( float DeltaSeconds ) override;

		/***** IFGSaveInterface *****/
		virtual bool ShouldSave_Implementation( ) const override {
			return true;
		};

		/***** Functions *****/
		UFUNCTION( BlueprintCallable, Category="Util Subsystem" )
		void ToggleCheat( );

		UFUNCTION( BlueprintCallable, Category="Util Subsystem" )
		void SetCheatModeByHost( bool NewValue );

		UFUNCTION( BlueprintCallable, Category="Util Subsystem" )
		void ToggleGhost( );

		UFUNCTION( BlueprintCallable, Category="Util Subsystem" )
		void ToggleFly( );

		UFUNCTION( BlueprintCallable, Category="Util Subsystem" )
		void UnlockAll( );

		UFUNCTION( BlueprintCallable, Category="Util Subsystem" )
		void ResetAll( );

		UFUNCTION( BlueprintCallable, Category="Util Subsystem" )
		void ToggleNoPower( );

		UFUNCTION( BlueprintCallable, Category="Util Subsystem" )
		void ToggleNoCost( );

		UFUNCTION( BlueprintCallable, Category="Util Subsystem" )
		void ToggleNoFuel( );

		UFUNCTION( BlueprintCallable, Category="Util Subsystem" )
		void ToggleNoMessages( );

		UFUNCTION( BlueprintCallable, Category="Util Subsystem" )
		void ToggleNoDamage( );

		UFUNCTION( BlueprintCallable, Category="Util Subsystem" )
		void FinishResearch( );

		UFUNCTION( BlueprintCallable, Category="Util Subsystem" )
		void SetTime( float NewTime );

		UPROPERTY( Transient, Replicated )
		UFGCheatManager* mCheatManager;

		UPROPERTY( Transient, Replicated )
		AFGTimeOfDaySubsystem* mTimeSubsystem;

		UPROPERTY( Transient )
		AFGPlayerController* mPlayerController;

		UPROPERTY( Replicated, BlueprintReadOnly )
		bool bIsHostEnableCheatMode;

		UPROPERTY( SaveGame, Replicated, BlueprintReadOnly )
		bool bIsCheatEnabled = true;

		UPROPERTY( SaveGame, Replicated, BlueprintReadOnly )
		bool bIsFlyEnabled;

		UPROPERTY( SaveGame, Replicated, BlueprintReadOnly )
		bool bIsGhostEnabled;

		UPROPERTY( SaveGame, Replicated, BlueprintReadOnly )
		bool bIsNoPower;

		UPROPERTY( SaveGame, Replicated, BlueprintReadOnly )
		bool bIsNoMessages;

		UPROPERTY( SaveGame, Replicated, BlueprintReadOnly )
		bool bIsNoCost;

		UPROPERTY( SaveGame, Replicated, BlueprintReadOnly )
		bool bIsNoFuel;

		UPROPERTY( SaveGame, Replicated, BlueprintReadOnly )
		bool bNoDamage;
};


UCLASS( )
class KBFL_API UKBFLCheatRCO : public UFGRemoteCallObject {
	GENERATED_BODY( )

	public:
		virtual void GetLifetimeReplicatedProps( TArray< FLifetimeProperty >& OutLifetimeProps ) const override {
			Super::GetLifetimeReplicatedProps( OutLifetimeProps );

			DOREPLIFETIME( UKBFLCheatRCO, bDummy );
		}

		/** Toggle Cheat */
		UFUNCTION( Server, WithValidation, Unreliable )
		void Server_ToggleCheats( AKBFLUtilSubsystem* Subsystem );

		bool Server_ToggleCheats_Validate( AKBFLUtilSubsystem* Subsystem ) {
			return true;
		}

		void Server_ToggleCheats_Implementation( AKBFLUtilSubsystem* Subsystem ) {
			if( Subsystem ) {
				Subsystem->ToggleCheat( );
			}
			Subsystem->ForceNetUpdate( );
		}

		/** Toggle Ghost */
		UFUNCTION( Server, WithValidation, Unreliable )
		void Server_ToggleGhost( AKBFLUtilSubsystem* Subsystem );

		bool Server_ToggleGhost_Validate( AKBFLUtilSubsystem* Subsystem ) {
			return true;
		}

		void Server_ToggleGhost_Implementation( AKBFLUtilSubsystem* Subsystem ) {
			if( Subsystem ) {
				Subsystem->ToggleGhost( );
			}
			Subsystem->ForceNetUpdate( );
		}

		/** Toggle Fly */
		UFUNCTION( Server, WithValidation, Unreliable )
		void Server_ToggleFly( AKBFLUtilSubsystem* Subsystem );

		bool Server_ToggleFly_Validate( AKBFLUtilSubsystem* Subsystem ) {
			return true;
		}

		void Server_ToggleFly_Implementation( AKBFLUtilSubsystem* Subsystem ) {
			if( Subsystem ) {
				Subsystem->ToggleFly( );
			}
			Subsystem->ForceNetUpdate( );
		}

		/** Unlock All Schematics */
		UFUNCTION( Server, WithValidation, Unreliable )
		void Server_UnlockAll( AKBFLUtilSubsystem* Subsystem );

		bool Server_UnlockAll_Validate( AKBFLUtilSubsystem* Subsystem ) {
			return true;
		}

		void Server_UnlockAll_Implementation( AKBFLUtilSubsystem* Subsystem ) {
			if( Subsystem ) {
				Subsystem->UnlockAll( );
			}
			Subsystem->ForceNetUpdate( );
		}

		/** Unlock All Schematics */
		UFUNCTION( Server, WithValidation, Unreliable )
		void Server_ResetAll( AKBFLUtilSubsystem* Subsystem );

		bool Server_ResetAll_Validate( AKBFLUtilSubsystem* Subsystem ) {
			return true;
		}

		void Server_ResetAll_Implementation( AKBFLUtilSubsystem* Subsystem ) {
			if( Subsystem ) {
				Subsystem->ResetAll( );
			}
			Subsystem->ForceNetUpdate( );
		}

		/** Toggle NoCost */
		UFUNCTION( Server, WithValidation, Unreliable )
		void Server_ToggleNoCost( AKBFLUtilSubsystem* Subsystem );

		bool Server_ToggleNoCost_Validate( AKBFLUtilSubsystem* Subsystem ) {
			return true;
		}

		void Server_ToggleNoCost_Implementation( AKBFLUtilSubsystem* Subsystem ) {
			if( Subsystem ) {
				Subsystem->ToggleNoCost( );
			}
			Subsystem->ForceNetUpdate( );
		}

		/** Toggle NoFuel */
		UFUNCTION( Server, WithValidation, Unreliable )
		void Server_ToggleNoFuel( AKBFLUtilSubsystem* Subsystem );

		bool Server_ToggleNoFuel_Validate( AKBFLUtilSubsystem* Subsystem ) {
			return true;
		}

		void Server_ToggleNoFuel_Implementation( AKBFLUtilSubsystem* Subsystem ) {
			if( Subsystem ) {
				Subsystem->ToggleNoFuel( );
			}
			Subsystem->ForceNetUpdate( );
		}

		/** Toggle NoMessages */
		UFUNCTION( Server, WithValidation, Unreliable )
		void Server_ToggleNoMessages( AKBFLUtilSubsystem* Subsystem );

		bool Server_ToggleNoMessages_Validate( AKBFLUtilSubsystem* Subsystem ) {
			return true;
		}

		void Server_ToggleNoMessages_Implementation( AKBFLUtilSubsystem* Subsystem ) {
			if( Subsystem ) {
				Subsystem->ToggleNoMessages( );
			}
			Subsystem->ForceNetUpdate( );
		}

		/** Toggle NoPower */
		UFUNCTION( Server, WithValidation, Unreliable )
		void Server_ToggleNoPower( AKBFLUtilSubsystem* Subsystem );

		bool Server_ToggleNoPower_Validate( AKBFLUtilSubsystem* Subsystem ) {
			return true;
		}

		void Server_ToggleNoPower_Implementation( AKBFLUtilSubsystem* Subsystem ) {
			if( Subsystem ) {
				Subsystem->ToggleNoPower( );
			}
			Subsystem->ForceNetUpdate( );
		}

		/** Set Time */
		UFUNCTION( Server, WithValidation, Unreliable )
		void Server_SetTime( AKBFLUtilSubsystem* Subsystem, float NewTime );

		bool Server_SetTime_Validate( AKBFLUtilSubsystem* Subsystem, float NewTime ) {
			return true;
		}

		void Server_SetTime_Implementation( AKBFLUtilSubsystem* Subsystem, float NewTime ) {
			if( Subsystem ) {
				Subsystem->SetTime( NewTime );
			}
			Subsystem->ForceNetUpdate( );
		}

		/** Set NoDamage */
		UFUNCTION( Server, WithValidation, Unreliable )
		void Server_ToggleNoDamage( AKBFLUtilSubsystem* Subsystem );

		bool Server_ToggleNoDamage_Validate( AKBFLUtilSubsystem* Subsystem ) {
			return true;
		}

		void Server_ToggleNoDamage_Implementation( AKBFLUtilSubsystem* Subsystem ) {
			if( Subsystem ) {
				Subsystem->ToggleNoDamage( );
			}
			Subsystem->ForceNetUpdate( );
		}

		/** FinishResearch */
		UFUNCTION( Server, WithValidation, Unreliable )
		void Server_FinishResearch( AKBFLUtilSubsystem* Subsystem );

		bool Server_FinishResearch_Validate( AKBFLUtilSubsystem* Subsystem ) {
			return true;
		}

		void Server_FinishResearch_Implementation( AKBFLUtilSubsystem* Subsystem ) {
			if( Subsystem ) {
				Subsystem->FinishResearch( );
			}
			Subsystem->ForceNetUpdate( );
		}


		UPROPERTY( Replicated )
		bool bDummy = true;
};
