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
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "ManagedProjectileBase.generated.h"

//-----------------------------------------------------------------------------------
// Managed Projectile Base Class Structs											-
//-----------------------------------------------------------------------------------
/* Struct that Defines the pull or putting back into the pool. */
USTRUCT(BlueprintType)
struct FProjectilePoolRequest
{
	GENERATED_BODY()

	// -- Public Information -- Struct Properties -- 
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Pool Request Settings")
	bool bTeleportOnMove = true;												// teleport or sweep when pulling or putting it back? 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Pool Request Settings")
	bool bHideActorOnMove = true;												// should the projectile actor be invisible till after we are done moving? 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Pool Request Settings")
	bool bHideActorAfterMove = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Pool Request Settings")
	TEnumAsByte<ECollisionEnabled::Type> CollisionSettings = ECollisionEnabled::NoCollision;	// what do we want the collision to be set to on pull from pool or back into pool

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Pool Request Settings")
	float ProjectileSpeed = 0.f;												// the speed of the projectile movement? 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Pool Request Settings")
	FVector LocationToMoveTo = FVector::ZeroVector;								// the location to move the projectile to?

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Pool Request Settings")
	FVector DirectionUnitVector = FVector::ForwardVector;						// the direction we want the projectile to be facing. 

	// -- Public Information -- Struct Methods -- //
public:
	/* Teleport On move */
	bool GetTeleportOnMove() const { return bTeleportOnMove; }

	/* Do we want to hide the actor on move? */
	bool GetHideOnMove() const { return bHideActorOnMove; }

	/* Do we want to hide the actor after the move? */
	bool GetHideAfterMove() const { return bHideActorAfterMove; }
	
	/* Get the collision enabled flag? */
	ECollisionEnabled::Type GetCollisionEnabledSettings() const { return CollisionSettings; }

	/* Get the projectile speed */
	float GetProjectileSpeed() const { return ProjectileSpeed; }

	/* Get the projectile start locaiton */
	FVector GetStartLocation() const { return LocationToMoveTo; }

	/* Get the direction vector */
	FVector GetDirectionVector() const { return DirectionUnitVector; }

public:
	FProjectilePoolRequest()
	{}

	explicit struct FProjectilePoolRequest(bool bTeleport, bool bHideOn, bool bHideAfter, ECollisionEnabled::Type CollSettings, float Speed, FVector Loc, FVector Unit)
	{
		bTeleportOnMove = bTeleport;
		bHideActorOnMove = bHideOn;
		bHideActorAfterMove = bHideAfter;
		CollisionSettings = CollSettings;
		ProjectileSpeed = Speed;
		LocationToMoveTo = Loc;
		DirectionUnitVector = Unit;
	}
};

//-----------------------------------------------------------------------------------
// Managed Projectile Base Class Declariation										-
//-----------------------------------------------------------------------------------
UCLASS()
class PROJECTILEMANAGER_API AManagedProjectileBase : public AActor
{
	GENERATED_BODY()
	
	// -- Public Information -- Managed Projectile Constructor and Engine Events -- //
public:	
	AManagedProjectileBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// -- Public Information -- Projectile Life Cycle Methods -- //
public:
	UFUNCTION(BlueprintCallable, Category = "Managed Projectile | Lifecycle ")
	bool Request_UpdateFromPool(FProjectilePoolRequest Settings);

	UFUNCTION(BlueprintCallable, Category = "Managed Projectile | Lifecycle ")
	bool Deinit_ProjectileBase();

	// -- Public Information -- Class Properties -- //
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties | Projectile Components")
	USphereComponent* SphereCollision = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties | Projectile Components")
	UProjectileMovementComponent* ProjectileMovement = nullptr;
};
