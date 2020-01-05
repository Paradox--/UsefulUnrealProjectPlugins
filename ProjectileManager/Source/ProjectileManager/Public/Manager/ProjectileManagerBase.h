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
#pragma once

#include "CoreMinimal.h"
#include "Core.h"
#include "GameFramework/Actor.h"
#include "ProjectileManager/Public/Projectile/ManagedProjectileBase.h"
#include "ProjectileManagerBase.generated.h"


//-----------------------------------------------------------------------------------
// Projectile Manager Base Class Structs											-
//-----------------------------------------------------------------------------------
/* The Struct that is used to maintain a record of each managed projectile */
USTRUCT()
struct FManagedProjectileEntry
{
	GENERATED_BODY()

	// -- Public Information -- Properties -- //
public:
	UPROPERTY()
	bool bIsCurrentlyInUse = false;											/* Is this entry currently in use? */

	UPROPERTY()
	AManagedProjectileBase* ManagedProjectilePtr = nullptr;					/* Pointer to object */

public:
	/* Gets if the current entry is in use. */
	bool IsInUse() const { return bIsCurrentlyInUse; }

	/* Is this entry valid? */
	bool IsValid() const { return ManagedProjectilePtr != nullptr; }

	/* Is the incoming pointer the same as ours? */
	bool IsEntry(AManagedProjectileBase* InPtrToCheck) const { return GetManagedProjectilePtr() == InPtrToCheck; }

	/* Gets a ptr to the managed reference*/
	AManagedProjectileBase* GetManagedProjectilePtr() const { return ManagedProjectilePtr; }

	/* Mark an entry in use. */
	AManagedProjectileBase* MarkEntryInUse()
	{
		bIsCurrentlyInUse = true;
		return GetManagedProjectilePtr();	
	}

	AManagedProjectileBase* MarkEntryInUse(int32& ConfirmedIndex)
	{
		bIsCurrentlyInUse = true;
		if (ManagedProjectilePtr)ManagedProjectilePtr->UpdateLastKnownEntryInPool(ConfirmedIndex);
		return GetManagedProjectilePtr();
	}

	/* Unmark an entry as it is no longer in use. */
	void UnMarkEntryInUse()
	{
		bIsCurrentlyInUse = false;
	}

	/* Cleans Up the entry. */
	void CleanUpEntry()
	{
		if (!IsValid()) return;
		else
		{
			ManagedProjectilePtr->Deinit_ProjectileBase();
			ManagedProjectilePtr = nullptr;
		}
	}

public:
	FManagedProjectileEntry()
	{}

	explicit struct FManagedProjectileEntry(AManagedProjectileBase* NewPointerToManage)
	{
		ManagedProjectilePtr = NewPointerToManage;
	}
};

/* The Struct that defines the init properties of this manager  */
USTRUCT(BlueprintType)
struct FProjectileManagerInitSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Manager Init Settings")
	bool bStartWithNoCollisionOnProjectile = true; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Manager Init Settings")
	int32 StartingPoolSize = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Manager Init Settings")
	TSubclassOf<AManagedProjectileBase> ProjectileClassToUse;

public:
	/* Return if we start with collision */
	bool GetStartWithCollision() const { return bStartWithNoCollisionOnProjectile; }

	/* Return the starting pool size. */
	int32 GetStartingPoolSize() const { return StartingPoolSize; }

	/* Return the projectile class to spawn. */
	UClass* GetProjectileClassToSpawn() const { return ProjectileClassToUse; }

public:
	FProjectileManagerInitSettings()
	{}
};

/* The Struct that defines the retrieve and return settings of into and out of the pool  */
USTRUCT(BlueprintType)
struct FProjectileManagerRetrieveReturnSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Manager Retrieval Return Settings")
	bool bStartAtFrontOfPool = true;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Manager Retrieval Return Settings")
	FProjectilePoolRequest ReturnProjectileRequest;

public:
	/* Return if we start with collision */
	bool StartSearchAtBeginningOfPool() const { return bStartAtFrontOfPool; }

public:
	FProjectileManagerRetrieveReturnSettings()
	{}
};

/* The Struct that defines the global setting  */
USTRUCT(BlueprintType)
struct FProjectileManagerGlobalSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Manager Global Settings")
	bool bOptimizeWithNewRequestsUsingLastReturned = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Manager Global Settings")
	bool bAllowForProjectilesTickAsync = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Manager Global Settings")
	bool bAllowAsyncPullFromPool = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Manager Global Settings")
	bool bAllowAsyncReturnToPool = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Manager Global Settings")
	int32 LastReturnedCacheSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Manager Global Settings")
	float AsyncTaskWaitTime = 0.0083f;	// default of 120fps

	UPROPERTY()
	TArray<int32> LastReturned;

public:
	/* return if we can trying optimizing using the last returned index */
	bool AllowedToOptimizeWithRequestsUsingLastReturned() const { return bOptimizeWithNewRequestsUsingLastReturned; }

	bool AllowForProjectilesToTickAsync() { return bAllowForProjectilesTickAsync; }

	bool AllowAsyncPullFromPool() const { return bAllowAsyncPullFromPool; }

	bool AllowAsyncReturnToPool() const { return bAllowAsyncReturnToPool; }

	/* Returns the Last returned entry */
	int32 GetLastReturnedEntry() { return LastReturned.Num() > 0 ? LastReturned.Pop(true) : -1; }

	float GetAsyncWaitTime() const { return AsyncTaskWaitTime; }

	/* Updates the last returned entry */
	void AddLastReturned(int32& idx)
	{
		if (LastReturned.Num() >= LastReturnedCacheSize) return;
		else
			LastReturned.Push(idx);
	}

public:
	FProjectileManagerGlobalSettings()
	{}
};


//-----------------------------------------------------------------------------------
// Projectile Manager Base Class Declariations										-
//-----------------------------------------------------------------------------------
UCLASS()
class PROJECTILEMANAGER_API AProjectileManagerBase : public AActor
{
	GENERATED_BODY()
	
	// -- Public Information -- Projectile Manager Constructor and Engine Events -- //
public:	
	AProjectileManagerBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// -- Public Information -- Projectile Manager Methods -- //
public:
	UFUNCTION(BlueprintCallable, Category = "Projectile Manager Base")
	virtual bool Request_ResizeProjectilePool(UPARAM(ref)int32& InNewProjectilePoolSize);

	UFUNCTION(BlueprintCallable, Category = "Projectile Manager Base")
	virtual bool Request_GetProjectileFromManager(AManagedProjectileBase*& OutProjectileToUse, UPARAM(ref) FProjectilePoolRequest& RetreieveSettings);

	UFUNCTION(BlueprintCallable, Category = "Projectile Manager Base")
	virtual bool Request_ReturnProjectileToManager(UPARAM(ref) AManagedProjectileBase*& InProjectileToReturn);

	UFUNCTION(BlueprintCallable, Category = "Projectile Manager Base")
	int32 GetCurrentPoolSize() const;

	// -- Private Information -- Projectile Manager Internal Methods -- //
private:
	/* Creates a Projectile Pool, allocates space via the spawn */
	virtual bool Create_ProjectilePool(int32 DesiredSize);

	/* Resizes a projectile pool to a specific size, destroys or creates as needed */
	virtual bool Resize_ProjectilePool(int32& InNewProjectilePoolSize);

	/* Cleans Up the projectile pool, basically a destroy all */
	virtual bool CleanUp_ProjectilePool();

	/* Findes potential index to remove, false if none exist */
	bool FindPotentialEntriesToRemove(TArray<int32>& OutPotentialIndexs, int32& InNumWantingToRemove);

	/* Attempts to remove entries from the managed list */
	bool AttemptToRemoveEntries(TArray<int32>& InPotentialIndexs, int32& InNumToRemove, int32& CurrentPoolTargetSize);

	/* Doe have a valid last known entry ?*/
	bool DoWeHaveValidLastKnownEntry(int32& OutEntry);

	/* Finds the first object to return, -1 if none */
	int32 FindFirstAvalibleIndex(bool bFromFront = false);

	/* Find the object to return, -1 if not found */
	int32 FindIndexFromPointer(bool bFromFront, AManagedProjectileBase*& InProjectileToReturn);

	/* Updates Last Returned index */
	void UpdateLastReturnedEntry(int32 InEntryKey);

	// -- Private Information -- Settings Methods -- //
private:
	/* Does the actor start with collision enabled? */
	bool InitProjectilesWithCollisionEnabled() const { return InitSettings.GetStartWithCollision(); }

	/* Should we do any searchs from the front or the back of the managed pool? */
	bool ShouldRetreieveFromTheFrontOfThePool() const { return RetrieveReturnSettings.StartSearchAtBeginningOfPool(); }	

	/* Should we do any searchs from the front or the back of the managed pool? */
	bool OptimizeRequestsWithCachingLastReturned() const { return GlobalSettings.AllowedToOptimizeWithRequestsUsingLastReturned(); }

	bool OptimizeProjectilesMustTickAsync() { return GlobalSettings.AllowForProjectilesToTickAsync(); }

	/* What is the init pool size? */
	int32 GetInitProjectilePoolSize() const { return InitSettings.GetStartingPoolSize(); }

	/* WHere does the pool store the projectiles in the world? */
	FVector GetPoolLocation() const { return RetrieveReturnSettings.ReturnProjectileRequest.GetStartLocation(); }

	/* Return the settings */
	FProjectilePoolRequest GetReturnRequestSettings() const { return RetrieveReturnSettings.ReturnProjectileRequest; }

	/* What class do we work with? */
	UClass* GetProjectileClassToUse() const { return InitSettings.GetProjectileClassToSpawn(); }


	// -- Public Information -- Projectile Manager Exposed Properties -- //
public:
	UPROPERTY()
	bool bNeedToRemoveOnReturn = false;		// Used to check if we need to remove

	UPROPERTY()
	int32 CurrentPoolSizeTarget = 0;		// Used to determine the target size for passive. 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Manager | Settings | Init ")
	FProjectileManagerInitSettings InitSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Manager | Settings | Retrieve / Return  ")
	FProjectileManagerRetrieveReturnSettings RetrieveReturnSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Manager | Settings | Global ")
	FProjectileManagerGlobalSettings GlobalSettings;

	UPROPERTY()
	TArray<FManagedProjectileEntry> ManagedPool;
};
