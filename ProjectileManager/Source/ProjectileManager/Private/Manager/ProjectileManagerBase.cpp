/*	Copyright / License  Disclaimer
 *	MIT Copyright 2020 Nicholas Mallonee
 *	Permission is hereby granted, free of charge, to any person obtaining a 
 *	copy of this software and associated documentation files(the "Software"), to deal 
 *	in the Software without restriction, including without limitation the rights to use, 
 *	copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the 
 *	Software, and to permit persons to whom the Software is furnished to do so, subject 
 *	to the following conditions :
 *	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. 
 *	
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 *	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
 *	PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
 *	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT 
 *	OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
 *	OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ProjectileManager/Public/Manager/ProjectileManagerBase.h"
#include "Kismet/GameplayStatics.h"

//-----------------------------------------------------------------------------------
// Projectile Manager Base Class Constructor										-
//-----------------------------------------------------------------------------------
AProjectileManagerBase::AProjectileManagerBase()
{
 	// -- Actor Class Defaults
	PrimaryActorTick.bCanEverTick = false;
}

//-----------------------------------------------------------------------------------
// Projectile Manager Base Class Engine Events										-
//-----------------------------------------------------------------------------------
/* Engine Begin play Event */
void AProjectileManagerBase::BeginPlay()
{
	// create the pool. 
	Create_ProjectilePool(GetInitProjectilePoolSize());

	Super::BeginPlay();	
}

/* Engine Tick Event */
void AProjectileManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/* Engine Endplay Event */
void AProjectileManagerBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// clean up the pool. 
	CleanUp_ProjectilePool();

	Super::EndPlay(EndPlayReason);
}

//-----------------------------------------------------------------------------------
// Projectile Manager Base Methods													-
//-----------------------------------------------------------------------------------
/*	Requests a pool resize 
	@param: InNewProjectilePoolSize: The new pool size to try and set. 
	@returns: if we were able to set the pool to the new size. 
*/
bool AProjectileManagerBase::Request_ResizeProjectilePool(int32& InNewProjectilePoolSize)
{
	return Resize_ProjectilePool(InNewProjectilePoolSize);
}

/*	Attempts to get a new projectile.
@param: OutProjectileToUse: The pointer to the projectile as returned by reference.
@returns: if we were able to get a projectile.
*/
bool AProjectileManagerBase::Request_GetProjectileFromManager(AManagedProjectileBase*& OutProjectileToUse, FProjectilePoolRequest& RetreieveSettings)
{
	if (GetCurrentPoolSize() <= 0)
	{
		UE_LOG(LogClass, Error, TEXT("Current pool size is 0; cant pull any projectiles out of it"));
		OutProjectileToUse = nullptr;
		return false;
	}
	else
	{
		// find the next entry. 
		int32 found = FindFirstAvalibleIndex(ShouldRetreieveFromTheFrontOfThePool());

		// if the entry is valid. 
		if (found >= 0)
		{
			// mark it and save it. 
			OutProjectileToUse = ManagedPool[found].MarkEntryInUse();

			// apply the pull settings and return. 
			return OutProjectileToUse ? OutProjectileToUse->Request_UpdateFromPool(RetreieveSettings) : false;
		}
		else
		{
			UE_LOG(LogClass, Error, TEXT("Could not find a projectile to return, try making your pool bigger."));
			OutProjectileToUse = nullptr;
			return false;
		}
	}
}

/*	Attempts to return a projectile to the pool. 
	@param: InProjectileToReturn: The pointer to the projectile to return. 
	@returns: if we were able to return the projectile. 
*/
bool AProjectileManagerBase::Request_ReturnProjectileToManager(AManagedProjectileBase*& InProjectileToReturn)
{
	if (!InProjectileToReturn)
	{
		UE_LOG(LogClass, Error, TEXT("Attempted to return nullptr to pool"));
		return false;
	}
	else
	{
		// verify its in the pool. 
		int32 found = FindIndexFromPointer(ShouldRetreieveFromTheFrontOfThePool(), InProjectileToReturn);

		if (found >= 0)
		{
			// if we need to remove on return. 
			if (bNeedToRemoveOnReturn)
			{
				ManagedPool[found].CleanUpEntry();
				ManagedPool.RemoveAt(found, 1, true);

				// switch the flag, if have met our goal, will be the not of if we hit our target.
				bNeedToRemoveOnReturn = !(GetCurrentPoolSize() == CurrentPoolSizeTarget);

				return true;
			}
			else
			{
				// mark as it nots in use
				ManagedPool[found].UnMarkEntryInUse();

				// apply the return settings.
				return InProjectileToReturn->Request_UpdateFromPool(RetrieveReturnSettings.ReturnProjectileRequest);
			}
		}
		else
		{
			UE_LOG(LogClass, Error, TEXT("Inputed object to return to the pool does not exist as an entry in the managed pool"));
			return false;
		}
	}
}

/* Returns the current managed pool size. */
int32 AProjectileManagerBase::GetCurrentPoolSize() const
{
	return ManagedPool.Num();
}

//-----------------------------------------------------------------------------------
// Projectile Manager Base Class Internal Methods									-
//-----------------------------------------------------------------------------------
/*  Creates a  new pool, or adds on to the current one 
	@param: DesiredSize: The Desired size the pool should be. 
	@returns: boolean if the operation is successful.
*/
bool AProjectileManagerBase::Create_ProjectilePool(int32 DesiredSize)
{
	if (DesiredSize <= 0 || DesiredSize - GetCurrentPoolSize() <= 0)
	{
		UE_LOG(LogClass, Error, TEXT("Projectile Manager Can not allocate a projectile pool at or below the value of 0. Requested Size: %d"), DesiredSize);
		return false;
	}
	else
	{
		UWorld* const world = GetWorld();

		if (!world) return false;
		else
		{
			// set up spawn params
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			spawnParams.Instigator = nullptr;

			// the amount to create.  
			int32 AmountToCreate = DesiredSize - GetCurrentPoolSize();

			// create the pool 
			for (int32 i = 0; i < AmountToCreate; i++)
			{
				// spawn a projectile 
				if (AManagedProjectileBase* projectile = world->SpawnActor<AManagedProjectileBase>(GetProjectileClassToUse(), GetPoolLocation(), FRotator::ZeroRotator, spawnParams))
				{
					// set if the projectiles outside collision needs to be on at start or not. 
					projectile->Request_UpdateFromPool(GetReturnRequestSettings());

					// add this object to the record as needed, save this object as the deleter. 
					ManagedPool.Add(FManagedProjectileEntry(projectile));
				}
			}

			// did we complete successfully? 
			return GetCurrentPoolSize() == DesiredSize;
		}
	}
}

/* Resizes the pool to a desired size if possible. 
	@param: InNewProjectilePoolSize: the requested size of the pool. 
	@return: if the pool was resized. 
*/
bool AProjectileManagerBase::Resize_ProjectilePool(int32& InNewProjectilePoolSize)
{
	if (InNewProjectilePoolSize <= 0)
	{
		UE_LOG(LogClass, Error, TEXT("Can not resize projectile manager pool to any value less than 1, you requested a value of %d for the new pool size"), InNewProjectilePoolSize);
		return false;
	}
	else if (InNewProjectilePoolSize == GetCurrentPoolSize())
	{
		UE_LOG(LogClass, Error, TEXT("No Need to resize the managed pool as the requested size is the current pool size."));
		return false;
	}
	else
	{
		// if we need to allocate more. 
		if (InNewProjectilePoolSize > GetCurrentPoolSize())
		{
			// update the pool with the new target amount
			return Create_ProjectilePool(InNewProjectilePoolSize);
		}
		else // else if we need to remove some from the pool. 
		{
			TArray<int32> PotentialIndexs; 
			int32 NumToRemove = GetCurrentPoolSize() - InNewProjectilePoolSize;

			// if we didnt find enough to remove, any incoming should be removed. 
			if (!FindPotentialEntriesToRemove(PotentialIndexs, NumToRemove))
			{
				bNeedToRemoveOnReturn = true;
				CurrentPoolSizeTarget = InNewProjectilePoolSize;
			}

			// return if we finished successfully. 
			return AttemptToRemoveEntries(PotentialIndexs, NumToRemove, InNewProjectilePoolSize);
		}
	}
}

/* Deallocated the whole pool. */
bool AProjectileManagerBase::CleanUp_ProjectilePool()
{
	if (ManagedPool.Num() <= 0) return false;
	else
	{
		// clean up the allocated objects
		for (FManagedProjectileEntry Record : ManagedPool)
		{
			Record.CleanUpEntry();
		}

		// remove all records. 
		ManagedPool.Empty();

		// return true;
		return true;
	}	
}

/*	Attempts to find a list a potential entries to remove starting from the back of the pool 
	@param: OutPotentialIndexs: Out entry indexes to remove
	@param: InNumWantingToRemove: The number of entries we want to remove. 
	@return: boolean if we found any to remove
*/
bool AProjectileManagerBase::FindPotentialEntriesToRemove(TArray<int32>& OutPotentialIndexs, int32& InNumWantingToRemove)
{
	// make sure the data is valid. 
	OutPotentialIndexs.Empty();

	// if we currently work from the front, start at the back. 
	if (ShouldRetreieveFromTheFrontOfThePool())
	{
		for (int32 i = ManagedPool.Num() - 1; i >= 0; i--)
		{
			if (!ManagedPool[i].IsInUse())
			{
				// add. 
				OutPotentialIndexs.Add(i);

				// if we found enough, break out. 
				if (OutPotentialIndexs.Num() >= InNumWantingToRemove)
					break;
			}
		}
	}
	else // else start from the front. 
	{
		// find candidates 
		for (int32 i = 0; i < ManagedPool.Num(); i++)
		{
			if (!ManagedPool[i].IsInUse())
			{
				// add. 
				OutPotentialIndexs.Add(i);

				// if we found enough, break out. 
				if (OutPotentialIndexs.Num() >= InNumWantingToRemove)
					break;
			}
		}
	}

	return OutPotentialIndexs.Num() > 0;
}

/* Attempts to remove entry from the pool. 
	@param: InPotentialIndexs: The indexs we want to remove. 
	@param: InNumToRemove: The number of entries to remove
	@param: CurrentPoolTargetSize: The Target Pool size we want.
	@returns: if the resize finished successfull. 
*/
bool AProjectileManagerBase::AttemptToRemoveEntries(TArray<int32>& InPotentialIndexs, int32& InNumToRemove, int32& CurrentPoolTargetSize)
{
	// for each index.
	for (int32 index : InPotentialIndexs)
	{
		// find and remove the entry with that index. 
		for (int32 i = ManagedPool.Num() - 1; i >= 0; i--)
		{
			if (i == index)
			{
				// clean up the entry
				ManagedPool[index].CleanUpEntry();

				// remove
				ManagedPool.RemoveAt(index, 1, true);

				// reduce.
				InNumToRemove--;
			}
		}
	}

	// if we removed what we needed too, go ahead return the result, else return true, as we will remove more on their return.
	// the timing of the return is up to the application. 
	return !bNeedToRemoveOnReturn ? GetCurrentPoolSize() == CurrentPoolTargetSize : true;
}

/*	Finds the first availble projectile index. 
	@param: bFromFront: do we start the search from the front or the back?
	@return: the index of the entry, -1 if not found.
*/
int32 AProjectileManagerBase::FindFirstAvalibleIndex(bool bFromFront) const
{
	if (bFromFront)
	{
		for (int32 i = 0; i < ManagedPool.Num(); i++)
		{
			if (!ManagedPool[i].IsInUse()) return i;
		}

		return -1;
	}
	else
	{
		for (int32 i = ManagedPool.Num(); i >= 0; --i)
		{
			if (!ManagedPool[i].IsInUse()) return i;
		}

		return -1;
	}
}

/*	Finds the entry index from a pointer to a projectile. 
	@param: bFromFront: do we start the search from the front or the back?
	@param: InProjectileToReturn: The projectile to return. 
	@return: the index of the entry, -1 if not found.
*/
int32 AProjectileManagerBase::FindIndexFromPointer(bool bFromFront, AManagedProjectileBase*& InProjectileToReturn) const
{
	if (!InProjectileToReturn) return -1;
	else
	{
		if (bFromFront)
		{
			for (int32 i = 0; i < ManagedPool.Num(); i++)
			{
				if (ManagedPool[i].IsEntry(InProjectileToReturn)) return i;
			}

			return -1;
		}
		else
		{
			for (int32 i = ManagedPool.Num(); i >= 0; --i)
			{
				if (ManagedPool[i].IsEntry(InProjectileToReturn)) return i;
			}

			return -1;
		}
	}
}
