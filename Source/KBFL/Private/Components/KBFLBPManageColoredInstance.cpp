#include "Components/KBFLBPManageColoredInstance.h"

#include "FGBuildableSubsystem.h"

UKBFLBPManageColoredInstance::UKBFLBPManageColoredInstance()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UKBFLBPManageColoredInstance::UpdateDataMap( TMap< uint8, float > DataMap )
{
	bool Apply = false;
	for( const auto Map : DataMap )
	{
		if( CheckIndex( Map.Key ) )
		{
			mInstanceHandle.SetCustomDataById( Map.Key, Map.Value );
			Apply = true;
		}
	}

	if( Apply )
	{
		ReApplyData();
	}
}

void UKBFLBPManageColoredInstance::UpdateData( uint8 Index, float Value )
{
	if( CheckIndex( Index ) )
	{
		mInstanceHandle.SetCustomDataById( Index, Value );
		ReApplyData();
	}
}

bool UKBFLBPManageColoredInstance::ReApplyData()
{
	AFGBuildableSubsystem* Subsystem = AFGBuildableSubsystem::Get( this );
	if( Subsystem )
	{
		if( Subsystem->GetColoredInstanceManager( this ) )
		{
			Subsystem->GetColoredInstanceManager( this )->UpdateColorForInstanceFromDataArray( mInstanceHandle );
			return true;
		}
	}
	return false;
}
