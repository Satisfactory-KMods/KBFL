#pragma once
#include "Subsystems/ResourceNodes/KBFLResourceNodeDescriptor.h"
#include "Subsystems/ResourceNodes/ResourceNodesLogging.h"

bool UKBFLResourceNodeDescriptor::IsAllowedToRemoveActor( AActor* InActor )
{
	const AFGResourceNodeBase* ResourceNode = Cast< AFGResourceNodeBase >( InActor );
	if( ResourceNode && !mRemoveOccupied )
	{
		if( ResourceNode->IsOccupied() )
		{
			return false;
		}
	}
	return true;
}
