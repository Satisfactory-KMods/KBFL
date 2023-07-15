﻿#include "BFL/KBFL_Player.h"

#include "FGLocalPlayer.h"
#include "FGPlayerController.h"

FString UKBFL_Player::GetPlayerId(AFGPlayerState* PlayerState) {
	if(IsValid(PlayerState)) {
		if(PlayerState->IsInPlayerArray()) {
			if(IsValid(PlayerState->GetOwningController())) {
				UFGLocalPlayer* LocalPlayer = Cast<UFGLocalPlayer>(PlayerState->GetOwningController()->GetLocalPlayer());
				if(IsValid(LocalPlayer)) {
					const TSharedPtr<const FUniqueNetId> EpicID = LocalPlayer->GetPlayerId();
					const TSharedPtr<const FUniqueNetId> SteamID = LocalPlayer->GetPlayerIdSteam();

					if(EpicID || SteamID) {
						const TSharedPtr<const FUniqueNetId> ID = (EpicID) ? EpicID : SteamID;

						if(ID->ToString() != "UnknownId") {
							return ID->ToString();
						}
					}
				}
			}
			if(PlayerState->GetUserID() != "UnknownId") {
				return PlayerState->GetUserID();
			}
			return PlayerState->GetPlayerName();
		}
	}

	return FString("Invalid");
}

AFGBuildGun* UKBFL_Player::GetBuildingGun(UObject* WorldContext) {
	const auto Character = GetFGCharacter(WorldContext);

	if(IsValid(Character)) {
		return Character->GetBuildGun();
	}

	return nullptr;
}

AFGPlayerController* UKBFL_Player::GetFGController(UObject* WorldContext) {
	return Cast<AFGPlayerController>(WorldContext->GetWorld()->GetFirstPlayerController());
}

AFGCharacterPlayer* UKBFL_Player::GetFGCharacter(UObject* WorldContext) {
	const auto PlayerController = Cast<AFGPlayerController>(WorldContext->GetWorld()->GetFirstPlayerController());

	if(IsValid(PlayerController)) {
		return Cast<AFGCharacterPlayer>(PlayerController->GetCharacter());
	}

	return nullptr;
}

AFGPlayerState* UKBFL_Player::GetFgPlayerState(UObject* WorldContext) {
	const auto PlayerController = Cast<AFGPlayerController>(WorldContext->GetWorld()->GetFirstPlayerController());

	if(IsValid(PlayerController)) {
		return PlayerController->GetPlayerState<AFGPlayerState>();
	}

	return nullptr;
}

void UKBFL_Player::GetBuildingGunHitResult(UObject* WorldContext, bool& IsInBuildOrDismantleState, FHitResult& HitResult) {
	const auto BuildGun = GetBuildingGun(WorldContext);
	if(IsValid(BuildGun)) {
		IsInBuildOrDismantleState = BuildGun->IsInState(EBuildGunState::BGS_BUILD) || BuildGun->IsInState(EBuildGunState::BGS_DISMANTLE);
		HitResult = BuildGun->GetHitResult();
		return;
	}
	IsInBuildOrDismantleState = false;
}

EBuildGunState UKBFL_Player::GetPlayerBuildState(UObject* WorldContext) {
	const auto BuildGun = GetBuildingGun(WorldContext);
	if(IsValid(BuildGun)) {
		return BuildGun->mCurrentStateEnum;
	}
	return EBuildGunState::BGS_NONE;
}
