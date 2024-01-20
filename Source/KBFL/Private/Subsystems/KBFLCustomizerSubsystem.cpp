#include "Subsystems/KBFLCustomizerSubsystem.h"

#include "FGBuildableSubsystem.h"
#include "FGCheatManager.h"
#include "FGFactoryColoringTypes.h"
#include "FGGameMode.h"
#include "FGGameState.h"
#include "BFL/KBFL_Player.h"
#include "Interfaces/KBFLCustomizerInterface.h"
#include "Module/WorldModuleManager.h"
#include "Subsystem/SubsystemActorManager.h"
#include "Subsystems/KBFLContentCDOHelperSubsystem.h"

DECLARE_LOG_CATEGORY_EXTERN(CustomizerSubsystem, Log, All)

DEFINE_LOG_CATEGORY(CustomizerSubsystem)

void UKBFLCustomizerSubsystem::Tick(float DeltaTime) {
	if(Initialized && !Gathered) {
		Initialized = false;
		AFGGameMode* GameMode = Cast<AFGGameMode>(GetWorld()->GetAuthGameMode());
		if(!Initialized && !(GameMode != nullptr && GameMode->IsMainMenuGameMode())) {
			if(!Gathered) {
				UE_LOG(CustomizerSubsystem, Log, TEXT("CustomizerSubsystem > RegisterCollectionsInSubsystem"));
				GatherDefaultCollections();
			}

			UE_LOG(CustomizerSubsystem, Log, TEXT("CustomizerSubsystem > GatherInterfaces"));
			GatherInterfaces();

			Initialized = true;
		}
	}
}

TStatId UKBFLCustomizerSubsystem::GetStatId() const {
	RETURN_QUICK_DECLARE_CYCLE_STAT(UKBFLCustomizerSubsystem, STATGROUP_Tickables);
}

void UKBFLCustomizerSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Collection.InitializeDependency(USubsystemActorManager::StaticClass());

	UWorld* OuterWorld = GetWorld();
	//OuterWorld->OnActorsInitialized.AddUObject(this, &UKBFLCustomizerSubsystem::OnActorsInitialized);
	//OuterWorld->OnWorldBeginPlay.AddUObject(this, &UKBFLCustomizerSubsystem::OnWorldBeginPlay);
	UE_LOG(CustomizerSubsystem, Log, TEXT("Initialize Subsystem"));

	Super::Initialize(Collection);
}

void UKBFLCustomizerSubsystem::Deinitialize() {
	if(mDefaultSwatchCollection) {
		for(auto SwatchMap: mSwatchIDMap) {
			UFGFactoryCustomizationCollection* Default = mDefaultSwatchCollection.GetDefaultObject();
			if(Default) {
				if(Default->mCustomizations.Contains(SwatchMap.Value)) {
					Default->mCustomizations.Remove(SwatchMap.Value);
				}
			}
		}
	}

	mCalled.Empty();
	mSwatchIDMap.Empty();
	Initialized = false;
	Gathered = false;

	Super::Deinitialize();
}

void UKBFLCustomizerSubsystem::OnWorldBeginPlay(UWorld& InWorld) {
	AFGGameMode* GameMode = Cast<AFGGameMode>(GetWorld()->GetAuthGameMode());
	if(!Initialized && !(GameMode != nullptr && GameMode->IsMainMenuGameMode())) {
		if(!Gathered) {
			UE_LOG(CustomizerSubsystem, Log, TEXT("CustomizerSubsystem > RegisterCollectionsInSubsystem"));
			GatherDefaultCollections();
		}

		UE_LOG(CustomizerSubsystem, Log, TEXT("CustomizerSubsystem > GatherInterfaces"));
		GatherInterfaces();

		Initialized = true;
	}
}

bool UKBFLCustomizerSubsystem::RegisterSwatchesInSubsystem(TArray<FKBFLSwatchInformation> SwatchInformations) {
	AFGBuildableSubsystem*          Subsystem = AFGBuildableSubsystem::Get(this);
	UKBFLContentCDOHelperSubsystem* CDOHelperSubsystem = UKBFLContentCDOHelperSubsystem::Get(this);
	AFGGameState*                   FGGameState = Cast<AFGGameState>(UGameplayStatics::GetGameState(this));

	if(Subsystem && FGGameState && CDOHelperSubsystem) {
		for(FKBFLSwatchInformation Swatch: SwatchInformations) {
			if(Swatch.mSwatch) {
				UFGFactoryCustomizationDescriptor_Swatch* SwatchDefauls = Swatch.mSwatch.GetDefaultObject();
				if(SwatchDefauls) {
					uint8 ColourIndex = SwatchDefauls->ID;
					SwatchDefauls->mMenuPriority = static_cast<float>(ColourIndex);
					if(!mSwatchIDMap.Find(ColourIndex)) {
						if(mDefaultSwatchCollection) {
							UFGFactoryCustomizationCollection* Default = CDOHelperSubsystem->GetAndStoreDefaultObject_Native<UFGFactoryCustomizationCollection>(mDefaultSwatchCollection);
							if(IsValid(Default)) {
								Default->mCustomizations.AddUnique(Swatch.mSwatch);
							}
						}

						if(!Subsystem->mColorSlots_Data.IsValidIndex(ColourIndex)) {
							UE_LOG(CustomizerSubsystem, Log, TEXT("Try to add new color Slot at index, %d - %d"), ColourIndex, Subsystem->mColorSlots_Data.Num());
							for(uint8 i = Subsystem->mColorSlots_Data.Num(); i <= ColourIndex; ++i) {
								// Defaults
								FFactoryCustomizationColorSlot NewColourSlot = FFactoryCustomizationColorSlot(Swatch.mPrimaryColour, Swatch.mSecondaryColour);

								// Add to Array
								Subsystem->mColorSlots_Data.Add(NewColourSlot);
								FGGameState->SetupColorSlots_Data(Subsystem->mColorSlots_Data);
								FGGameState->Server_SetBuildingColorDataForSlot(i, NewColourSlot);

								FTimerDelegate TimerDel;
								FTimerHandle   TimerHandle;
								TimerDel.BindUFunction(Subsystem, FName("SetColorSlot_Data"), i, NewColourSlot);
								GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 2.0f, false);
								TimerDel.BindUFunction(FGGameState, FName("Server_SetBuildingColorDataForSlot"), i, NewColourSlot);
								GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 2.0f, false);

								Subsystem->SetColorSlot_Data(i, NewColourSlot);

								// Mark Slots as Dirty
								Subsystem->mColorSlotsAreDirty = true;

								UE_LOG(CustomizerSubsystem, Log, TEXT("New Colour slot added: %d"), i);
							}
						}

						// Add to Array
						if(!FGGameState->mBuildingColorSlots_Data.IsValidIndex(ColourIndex)) {
							FGGameState->mBuildingColorSlots_Data.SetNum(ColourIndex + 1, false);
							FGGameState->mBuildingColorSlots_Data[ColourIndex] = Subsystem->mColorSlots_Data[ColourIndex];
							FGGameState->SetupColorSlots_Data(Subsystem->mColorSlots_Data);
							UE_LOG(CustomizerSubsystem, Log, TEXT("write color again to gamestate: %d / %d"), FGGameState->mBuildingColorSlots_Data.Num(), Subsystem->mColorSlots_Data.Num());
						}

						mSwatchIDMap.Add(ColourIndex, Swatch.mSwatch);
						FGGameState->SetupColorSlots_Data(Subsystem->mColorSlots_Data);
						Subsystem->mColorSlotsAreDirty = true;
						UE_LOG(CustomizerSubsystem, Log, TEXT("Swatch found and success: %d > %s (%d/%d)"), ColourIndex, *Swatch.mSwatch->GetName(), FGGameState->mBuildingColorSlots_Data.Num(), Subsystem->mColorSlots_Data.Num());
					} else {
						UE_LOG(CustomizerSubsystem, Fatal, TEXT("Duplicate Swatch ID: %s | %d >< %s | %d"), *Swatch.mSwatch->GetName(), Swatch.mSwatch.GetDefaultObject()->ID, *mSwatchIDMap[ColourIndex]->GetName(), ColourIndex)
					}

					// Ignore Slots used by CSS (Slot 16 for example is used twice)
					if(!(ColourIndex > 18 && ColourIndex < 255)) {
						UE_LOG(CustomizerSubsystem, Fatal, TEXT("Please use a Index between 19 and 254 (Dont use Slots from SF!)"));
					}
				}
			}
		}

		TArray<FFactoryCustomizationColorSlot> ColorSlots = Subsystem->mColorSlots_Data;
		UE_LOG(CustomizerSubsystem, Error, TEXT("Slots: %d"), ColorSlots.Num());
		FGGameState->SetupColorSlots_Data(ColorSlots);
		//FGGameState->Init();
		Subsystem->mColorSlotsAreDirty = true;

		return true;
	}

	if(FGGameState) {
		for(FKBFLSwatchInformation Swatch: SwatchInformations) {
			if(Swatch.mSwatch) {
				UFGFactoryCustomizationDescriptor_Swatch* SwatchDefauls = Swatch.mSwatch.GetDefaultObject();
				if(SwatchDefauls) {
					uint8 ColourIndex = SwatchDefauls->ID;
					SwatchDefauls->mMenuPriority = static_cast<float>(ColourIndex);
					if(!FGGameState->mBuildingColorSlots_Data.IsValidIndex(ColourIndex)) {
						FGGameState->mBuildingColorSlots_Data.SetNum(ColourIndex + 1, false);
						FGGameState->mBuildingColorSlots_Data[ColourIndex] = Subsystem->mColorSlots_Data[ColourIndex];
						FGGameState->SetupColorSlots_Data(Subsystem->mColorSlots_Data);
						UE_LOG(CustomizerSubsystem, Log, TEXT("write color again to gamestate: %d / %d"), FGGameState->mBuildingColorSlots_Data.Num(), Subsystem->mColorSlots_Data.Num());
					}
				}
			}
		}
	}

	UE_LOG(CustomizerSubsystem, Error, TEXT("Cannot load AFGBuildableSubsystem for Swatches"));
	return false;
}

bool UKBFLCustomizerSubsystem::RegisterSwatchGroups(TMap<TSubclassOf<UFGSwatchGroup> , TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch>> Map) {
	for(TTuple<TSubclassOf<UFGSwatchGroup> , TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch>> Group: Map) {
		if(Group.Key && Group.Value) {
			if(!SetDefaultToSwatchGroup(Group.Key, Group.Value)) {
				return false;
			}
		}
	}
	return true;
}

void UKBFLCustomizerSubsystem::CDOMaterials(TArray<FKBFLMaterialDescriptorInformation> CDOInformation) {
	UKBFLContentCDOHelperSubsystem* CDOHelperSubsystem = UKBFLContentCDOHelperSubsystem::Get(this);
	if(CDOInformation.Num() > 0 && CDOHelperSubsystem) {
		for(FKBFLMaterialDescriptorInformation Information: CDOInformation) {
			if(Information.mApplyThisInformationTo.Num() > 0) {
				for(auto Descriptor: Information.mApplyThisInformationTo) {
					if(Descriptor) {
						if(UFGFactoryCustomizationDescriptor_Material* Default = CDOHelperSubsystem->GetAndStoreDefaultObject_Native<UFGFactoryCustomizationDescriptor_Material>(Descriptor)) {
							if(Information.mBuildableMap.Num() > 0) {
								Default->mBuildableMap.Append(Information.mBuildableMap);
							}
							if(Information.mValidBuildables.Num() > 0) {
								Default->mValidBuildables.Append(Information.mValidBuildables);
							}
						}
					}
				}
			}
		}
	}
}

void UKBFLCustomizerSubsystem::BeginForModule(UWorldModule* Module) {
	if(Module && !WasForModuleCalled(Module)) {
		if(!Gathered) {
			UE_LOG(CustomizerSubsystem, Log, TEXT("RegisterCollectionsInSubsystem"));
			Gathered = GatherDefaultCollections();
		}

		RegisterSwatchesInSubsystem(IKBFLCustomizerInterface::Execute_GetSwatchDescriptionInformation(Module));
		if(Gathered) {
			RegisterSwatchGroups(IKBFLCustomizerInterface::Execute_GetSwatchGroups(Module));
			CDOMaterials(IKBFLCustomizerInterface::Execute_GetMaterialInformation(Module));

			mCalled.AddUnique(Module);
		} else {
			const FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &UKBFLCustomizerSubsystem::BeginForModule, Module);
			GetWorld()->GetTimerManager().SetTimerForNextTick(RespawnDelegate);
			UE_LOG(CustomizerSubsystem, Log, TEXT("Gather was invalid Retry next tick!"));
		}
	}
}

bool UKBFLCustomizerSubsystem::GatherDefaultCollections() {
	AFGBuildableSubsystem* Subsystem = AFGBuildableSubsystem::Get(this);

	if(Subsystem) {
		for(auto CustomizationClass: Subsystem->mCustomizationCollectionClasses) {
			if(CustomizationClass.GetDefaultObject()->mCustomizationClass->IsChildOf(UFGFactoryCustomizationDescriptor_Material::StaticClass())) {
				mDefaultMaterialCollection = CustomizationClass;
			}
			if(CustomizationClass.GetDefaultObject()->mCustomizationClass->IsChildOf(UFGFactoryCustomizationDescriptor_Pattern::StaticClass())) {
				mDefaultPatternCollection = CustomizationClass;
			}
			if(CustomizationClass.GetDefaultObject()->mCustomizationClass->IsChildOf(UFGFactoryCustomizationDescriptor_Swatch::StaticClass())) {
				mDefaultSwatchCollection = CustomizationClass;
			}
			if(CustomizationClass.GetDefaultObject()->mCustomizationClass->IsChildOf(UFGFactoryCustomizationDescriptor_Skin::StaticClass())) {
				mDefaultSkinCollection = CustomizationClass;
			}
		}

		return true;
	}

	UE_LOG(CustomizerSubsystem, Error, TEXT("Cannot load AFGBuildableSubsystem for Collections"));
	return false;
}

void UKBFLCustomizerSubsystem::GatherInterfaces() {
#if !WITH_EDITOR
	UWorldModuleManager* Subsystem = Cast< UWorldModuleManager >( GetWorld()->GetSubsystem< UWorldModuleManager >() );
	for ( auto i : Subsystem->RootModuleList )
	{
		const bool bImplementsInterface = i->GetClass()->ImplementsInterface( UKBFLCustomizerInterface::StaticClass() );
		if ( bImplementsInterface )
		{
			BeginForModule( i );
		}
	}
#endif
}

bool UKBFLCustomizerSubsystem::SetDefaultToSwatchGroup(TSubclassOf<UFGSwatchGroup> SwatchGroup, TSubclassOf<UFGFactoryCustomizationDescriptor_Swatch> Swatch) {
	UE_LOG(CustomizerSubsystem, Log, TEXT("SetDefaultToSwatchGroup"));
	AFGBuildableSubsystem* Subsystem = AFGBuildableSubsystem::Get(this);
	AFGGameState*          GameState = Cast<AFGGameState>(UGameplayStatics::GetGameState(this));

	if(Subsystem && GameState && Swatch && SwatchGroup) {
		FSwatchGroupData NewGroup;
		NewGroup.Swatch = Swatch;
		NewGroup.SwatchGroup = SwatchGroup;

		bool Contain = false;
		for(auto Group: GameState->mSwatchGroupDatum) {
			if(Group.SwatchGroup == SwatchGroup) {
				Contain = true;
			}
		}

		if(!Contain) {
			GameState->mSwatchGroupDatum.Add(NewGroup);
			GameState->SetDefaultSwatchForBuildableGroup(SwatchGroup, Swatch);

			if(AFGPlayerController* PlayerController = UKBFL_Player::GetFGController(this)) {
				PlayerController->SetDefaultSwatchForBuildableGroup(SwatchGroup, Swatch);
			}

			Subsystem->mColorSlotsAreDirty = true;
			UE_LOG(CustomizerSubsystem, Log, TEXT("Swatch Group found and success added: %s > %s"), *SwatchGroup->GetName(), *Swatch->GetName());
		}

		UE_LOG(CustomizerSubsystem, Log, TEXT("Swatch Group found: %s > %s"), *SwatchGroup->GetName(), *Swatch->GetName());
		return true;
	}

	UE_LOG(CustomizerSubsystem, Log, TEXT("Subsystem && GameState && Swatch && SwatchGroup something is invalid!"));
	return false;
}
