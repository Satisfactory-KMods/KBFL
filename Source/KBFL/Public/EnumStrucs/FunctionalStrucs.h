// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "FunctionalStrucs.generated.h"

template< typename T >
USTRUCT()
struct FSortedComponentDistanceArray
{
	FSortedComponentDistanceArray()
	{
	};

	FSortedComponentDistanceArray( T* RefObject )
	{
		SetSortRef( RefObject );
	};

	/** Set our ref for sorting */
	void SetSortRef( T* Object )
	{
		if( Object )
		{
			mSortRef = Object;
		}
	}

	/** Sorting Functions */
	void Sort()
	{
		if( mSortRef.IsValid() )
		{
			mObjects = SortByTempRef( mSortRef );
		}
	}

	/** Getter Functions */
	uint64 Num() const
	{
		return mObjects.Num();
	}

	TArray< TWeakObjectPtr< T > > GetAllConst() const
	{
		return mObjects;
	}

	void SortArrayByLocation( FVector LocationRef )
	{
		mObjects = SortByLocationRef( LocationRef );
	}

	/** Note: Get the last sort state! */
	TArray< TWeakObjectPtr< T > > GetAllObjects( const bool Sorted = true )
	{
		if( Sorted )
		{
			Sort();
		}
		return mObjects;
	}

	TArray< TWeakObjectPtr< T > > GetSortedByRef( T* Object ) const
	{
		if( Object )
		{
			return SortByTempRef( Object );
		}
		return mObjects;
	}

	TWeakObjectPtr< T > GetClosedObject()
	{
		return GetClosedObjectWithRef( mSortRef.Get() );
	}

	TWeakObjectPtr< T > GetClosedObjectWithRef( T* Object )
	{
		if( !Object )
		{
			if( mObjects.Num() > 0 )
			{
				return mObjects[ 0 ];
			}
		}

		if( Object )
		{
			TArray< TWeakObjectPtr< T > > Sorted = SortByTempRef( Object );
			if( Sorted.Num() > 0 )
			{
				return Sorted[ 0 ];
			}
		}

		return {};
	}

	TWeakObjectPtr< T > GetClosedObjectWithRefAndDistance( T* Object, float Distance = 500.f )
	{
		TWeakObjectPtr< T > ClosedObject = GetClosedObjectWithRef( Object );
		if( ClosedObject.IsValid() && mSortRef.IsValid() )
		{
			T* TargetObject = ClosedObject.Get();
			if( FVector::Distance( TargetObject->GetComponentLocation(), Object->GetComponentLocation() ) <= Distance )
			{
				return ClosedObject;
			}
		}

		if( !Object )
		{
			if( mObjects.Num() > 0 )
			{
				if( FVector::Distance( mObjects[ 0 ]->GetComponentLocation(), Object->GetComponentLocation() ) <= Distance )
				{
					return mObjects[ 0 ];
				}
			}
		}

		return {};
	}

	TWeakObjectPtr< T > GetClosedObjectWithLocRefAndDistance( FVector Location, float Distance = 500.f )
	{
		TWeakObjectPtr< T > ClosedObject = GetClosedObject();
		if( ClosedObject.IsValid() )
		{
			T* TargetObject = ClosedObject.Get();
			if( FVector::Distance( TargetObject->GetComponentLocation(), Location ) <= Distance )
			{
				return ClosedObject;
			}
		}

		if( !ClosedObject.IsValid() )
		{
			if( mObjects.Num() > 0 )
			{
				if( FVector::Distance( mObjects[ 0 ]->GetComponentLocation(), Location ) <= Distance )
				{
					return mObjects[ 0 ];
				}
			}
		}

		return {};
	}

	TWeakObjectPtr< T > GetClosedObjectWithDistance( float Distance = 500.f )
	{
		TWeakObjectPtr< T > ClosedObject = GetClosedObject();
		if( ClosedObject.IsValid() && mSortRef.IsValid() )
		{
			T* RefObject = mSortRef.Get();
			T* Object = ClosedObject.Get();
			if( FVector::Distance( RefObject->GetComponentLocation(), Object->GetComponentLocation() ) <= Distance )
			{
				return ClosedObject;
			}
		}

		return {};
	}

	uint64 AddObject( T* Object )
	{
		if( Object )
		{
			uint64 Return = mObjects.AddUnique( Object );
			Sort();
			return Return;
		}
		return -1;
	}

	void RemoveObject( T* Object )
	{
		if( Object )
		{
			if( mObjects.Contains( Object ) )
			{
				mObjects.Remove( Object );
			}
		}
	}

	void Empty()
	{
		mObjects.Empty();
	}

private:
	/** Sort the Array */
	TArray< TWeakObjectPtr< T > > SortByTempRef( TWeakObjectPtr< T > Ref ) const
	{
		TArray< TWeakObjectPtr< T > > SortArray = mObjects;
		if( Ref.IsValid() && SortArray.Num() > 1 )
		{
			SortArray.Sort( [Ref]( const TWeakObjectPtr< T >& A, const TWeakObjectPtr< T >& B )
			{
				float ADistance = 0.0f;
				float BDistance = 0.0f;

				if( A.IsValid() && B.IsValid() && Ref.IsValid() )
				{
					ADistance = FVector::Distance( A->GetComponentLocation(), Ref->GetComponentLocation() );
					BDistance = FVector::Distance( B->GetComponentLocation(), Ref->GetComponentLocation() );
				}
				return ADistance < BDistance;
			} );
		}
		return SortArray;
	}

	/** Sort the Array by Location*/
	TArray< TWeakObjectPtr< T > > SortByLocationRef( FVector LocationRef ) const
	{
		TArray< TWeakObjectPtr< T > > SortArray = mObjects;
		if( SortArray.Num() > 1 )
		{
			SortArray.Sort( [LocationRef]( const TWeakObjectPtr< T >& A, const TWeakObjectPtr< T >& B )
			{
				float ADistance = 0.0f;
				float BDistance = 0.0f;

				if( A.IsValid() && B.IsValid() )
				{
					ADistance = FVector::Distance( A->GetComponentLocation(), LocationRef );
					BDistance = FVector::Distance( B->GetComponentLocation(), LocationRef );
				}
				return ADistance < BDistance;
			} );
		}
		return SortArray;
	}

	TArray< TWeakObjectPtr< T > > mObjects;
	TWeakObjectPtr< T > mSortRef;
};


template< typename T >
USTRUCT()
struct FSortedActorDistanceArray
{
	FSortedActorDistanceArray()
	{
	};

	FSortedActorDistanceArray( T* RefObject )
	{
		SetSortRef( RefObject );
	};

	/** Set our ref for sorting */
	void SetSortRef( T* Object )
	{
		if( Object )
		{
			mSortRef = Object;
		}
	}

	/** Sorting Functions */
	void Sort()
	{
		if( mSortRef.IsValid() )
		{
			mObjects = SortByTempRef( mSortRef );
		}
	}

	/** Getter Functions */
	uint64 Num() const
	{
		return mObjects.Num();
	}

	TArray< TWeakObjectPtr< T > > GetAllConst() const
	{
		return mObjects;
	}

	/** Note: Get the last sort state! */
	TArray< TWeakObjectPtr< T > > GetAllObjects( const bool Sorted = true )
	{
		if( Sorted )
		{
			Sort();
		}
		return mObjects;
	}

	TArray< TWeakObjectPtr< T > > GetSortedByRef( T* Object ) const
	{
		if( Object )
		{
			return SortByTempRef( Object );
		}
	}

	TWeakObjectPtr< T > GetClosedObject()
	{
		return GetClosedObjectWithRef( mSortRef );
	}

	TWeakObjectPtr< T > GetClosedObjectWithRef( T* Object )
	{
		if( Object )
		{
			TArray< TWeakObjectPtr< T > > Sorted = SortByTempRef( Object );
			if( Sorted.Num() > 0 )
			{
				return Sorted[ 0 ];
			}
		}

		return {};
	}

	TWeakObjectPtr< T > GetClosedObjectWithRefAndDistance( T* Object, float Distance = 500.f )
	{
		TWeakObjectPtr< T > ClosedObject = GetClosedObjectWithRef( Object );
		if( ClosedObject.IsValid() && mSortRef.IsValid() )
		{
			T* TargetObject = ClosedObject.Get();
			if( Object->GetDistanceTo( TargetObject ) <= Distance )
			{
				return ClosedObject;
			}
		}

		return {};
	}

	TWeakObjectPtr< T > GetClosedObjectWithDistance( float Distance = 500.f )
	{
		TWeakObjectPtr< T > ClosedObject = GetClosedObject();
		if( ClosedObject.IsValid() && mSortRef.IsValid() )
		{
			T* RefObject = mSortRef.Get();
			T* Object = ClosedObject.Get();
			if( RefObject->GetDistanceTo( Object ) <= Distance )
			{
				return ClosedObject;
			}
		}

		return {};
	}

	void AddObject( T* Object )
	{
		if( Object )
		{
			if( Object )
			{
				mObjects.AddUnique( Object );
			}
		}
	}

	void RemoveObject( T* Object )
	{
		if( Object )
		{
			if( mObjects.Contains( Object ) )
			{
				mObjects.Remove( Object );
			}
		}
	}

	void Empty()
	{
		mObjects.Empty();
	}

private:
	/** Sort the Array */
	TArray< TWeakObjectPtr< T > > SortByTempRef( TWeakObjectPtr< T > Ref ) const
	{
		TArray< TWeakObjectPtr< T > > SortArray = mObjects;
		if( Ref.IsValid() )
		{
			SortArray.Sort( [Ref]( const TWeakObjectPtr< T >& A, const TWeakObjectPtr< T >& B )
			{
				float ADistance = A.Get()->GetDistanceTo( Ref.Get() );
				float BDistance = B.Get()->GetDistanceTo( Ref.Get() );
				return ADistance > BDistance;
			} );
		}
		return SortArray;
	}

	TArray< TWeakObjectPtr< T > > mObjects;
	TWeakObjectPtr< T > mSortRef;
};


template< typename T >
USTRUCT()
struct FSortedComponentDistanceArrayV2
{
	FSortedComponentDistanceArrayV2()
	{
	};

	FSortedComponentDistanceArrayV2( FVector RefLocation )
	{
		SetSortRef( RefLocation );
	};

	/** Set our ref for sorting */
	void SetSortRef( FVector RefLocation )
	{
		mSortRef = RefLocation;
	}

	/** Sorting Functions */
	void Sort()
	{
		mObjects = SortByTempRef( mSortRef );
	}

	/** Getter Functions */
	uint64 Num() const
	{
		return mObjects.Num();
	}

	TArray< TWeakObjectPtr< T > > GetAllConst() const
	{
		return mObjects;
	}

	void SortArrayByLocation( FVector LocationRef )
	{
		mObjects = SortByLocationRef( LocationRef );
	}

	/** Note: Get the last sort state! */
	TArray< TWeakObjectPtr< T > > GetAllObjects( const bool Sorted = true )
	{
		if( Sorted )
		{
			Sort();
		}
		return mObjects;
	}

	TArray< TWeakObjectPtr< T > > GetSortedByRef( FVector RefLocation ) const
	{
		return SortByTempRef( RefLocation );
	}

	TWeakObjectPtr< T > GetClosedObject()
	{
		return GetClosedObjectWithRef( mSortRef );
	}

	TWeakObjectPtr< T > GetClosedObjectWithRef( FVector RefLocation )
	{
		TArray< TWeakObjectPtr< T > > Sorted = SortByTempRef( RefLocation );
		if( Sorted.Num() > 0 )
		{
			return Sorted[ 0 ];
		}
		return nullptr;
	}

	TWeakObjectPtr< T > GetClosedObjectWithRefAndDistance( FVector RefLocation, float Distance = 500.f )
	{
		TWeakObjectPtr< T > ClosedObject = GetClosedObjectWithRef( RefLocation );
		if( ClosedObject.IsValid() )
		{
			T* TargetObject = ClosedObject.Get();
			if( FVector::Distance( TargetObject->GetComponentLocation(), RefLocation ) <= Distance )
			{
				return ClosedObject;
			}
		}

		return nullptr;
	}

	TWeakObjectPtr< T > GetClosedObjectWithDistance( float Distance = 500.f )
	{
		TWeakObjectPtr< T > ClosedObject = GetClosedObject();
		if( ClosedObject.IsValid() )
		{
			T* Object = ClosedObject.Get();
			if( FVector::Distance( mSortRef, Object->GetComponentLocation() ) <= Distance )
			{
				return ClosedObject;
			}
		}

		return {};
	}

	uint64 AddObject( T* Object )
	{
		if( Object )
		{
			uint64 Return = mObjects.AddUnique( Object );
			Sort();
			return Return;
		}
		return -1;
	}

	void RemoveObject( T* Object )
	{
		if( Object )
		{
			if( mObjects.Contains( Object ) )
			{
				mObjects.Remove( Object );
			}
		}
	}

	void Empty()
	{
		mObjects.Empty();
	}

private:
	/** Sort the Array */
	TArray< TWeakObjectPtr< T > > SortByTempRef( FVector RefLocation ) const
	{
		TArray< TWeakObjectPtr< T > > SortArray = mObjects;
		if( SortArray.Num() > 1 )
		{
			SortArray.Sort( [RefLocation]( const TWeakObjectPtr< T >& A, const TWeakObjectPtr< T >& B )
			{
				float ADistance = 0.0f;
				float BDistance = 0.0f;

				if( A.IsValid() && B.IsValid() && Ref.IsValid() )
				{
					ADistance = FVector::Distance( A->GetComponentLocation(), RefLocation );
					BDistance = FVector::Distance( B->GetComponentLocation(), RefLocation );
				}
				return ADistance < BDistance;
			} );
		}
		return SortArray;
	}

	TArray< TWeakObjectPtr< T > > mObjects;
	FVector mSortRef;
};


template< typename T >
USTRUCT()
struct FSortedActorDistanceArrayV2
{
	FSortedActorDistanceArrayV2()
	{
	};

	FSortedActorDistanceArrayV2( FVector RefLocation )
	{
		SetSortRef( RefLocation );
	};

	/** Set our ref for sorting */
	void SetSortRef( FVector RefLocation )
	{
		mSortRef = RefLocation;
	}

	/** Sorting Functions */
	void Sort()
	{
		mObjects = SortByTempRef( mSortRef );
	}

	/** Getter Functions */
	uint64 Num() const
	{
		return mObjects.Num();
	}

	TArray< TWeakObjectPtr< T > > GetAllConst() const
	{
		return mObjects;
	}

	void SortArrayByLocation( FVector LocationRef )
	{
		mObjects = SortByLocationRef( LocationRef );
	}

	/** Note: Get the last sort state! */
	TArray< TWeakObjectPtr< T > > GetAllObjects( const bool Sorted = true )
	{
		if( Sorted )
		{
			Sort();
		}
		return mObjects;
	}

	TArray< TWeakObjectPtr< T > > GetSortedByRef( FVector RefLocation ) const
	{
		return SortByTempRef( RefLocation );
	}

	TWeakObjectPtr< T > GetClosedObject()
	{
		return GetClosedObjectWithRef( mSortRef );
	}

	TWeakObjectPtr< T > GetClosedObjectWithRef( FVector RefLocation )
	{
		TArray< TWeakObjectPtr< T > > Sorted = SortByTempRef( RefLocation );
		if( Sorted.Num() > 0 )
		{
			return Sorted[ 0 ];
		}
		return nullptr;
	}

	TWeakObjectPtr< T > GetClosedObjectWithRefAndDistance( FVector RefLocation, float Distance = 500.f )
	{
		TWeakObjectPtr< T > ClosedObject = GetClosedObjectWithRef( RefLocation );
		if( ClosedObject.IsValid() )
		{
			T* TargetObject = ClosedObject.Get();
			if( FVector::Distance( TargetObject->GetActorLocation(), RefLocation ) <= Distance )
			{
				return ClosedObject;
			}
		}

		return nullptr;
	}

	TWeakObjectPtr< T > GetClosedObjectWithDistance( float Distance = 500.f )
	{
		TWeakObjectPtr< T > ClosedObject = GetClosedObject();
		if( ClosedObject.IsValid() )
		{
			T* Object = ClosedObject.Get();
			if( FVector::Distance( mSortRef, Object->GetActorLocation() ) <= Distance )
			{
				return ClosedObject;
			}
		}

		return {};
	}

	uint64 AddObject( T* Object )
	{
		if( Object )
		{
			uint64 Return = mObjects.AddUnique( Object );
			Sort();
			return Return;
		}
		return -1;
	}

	void RemoveObject( T* Object )
	{
		if( Object )
		{
			if( mObjects.Contains( Object ) )
			{
				mObjects.Remove( Object );
			}
		}
	}

	void Empty()
	{
		mObjects.Empty();
	}

private:
	/** Sort the Array */
	TArray< TWeakObjectPtr< T > > SortByTempRef( FVector RefLocation ) const
	{
		TArray< TWeakObjectPtr< T > > SortArray = mObjects;
		if( SortArray.Num() > 1 )
		{
			SortArray.Sort( [RefLocation]( const TWeakObjectPtr< T >& A, const TWeakObjectPtr< T >& B )
			{
				float ADistance = 0.0f;
				float BDistance = 0.0f;

				if( A.IsValid() && B.IsValid() && Ref.IsValid() )
				{
					ADistance = FVector::Distance( A->GetActorLocation(), RefLocation );
					BDistance = FVector::Distance( B->GetActorLocation(), RefLocation );
				}
				return ADistance < BDistance;
			} );
		}
		return SortArray;
	}

	TArray< TWeakObjectPtr< T > > mObjects;
	FVector mSortRef;
};
