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

#include "ProjectileManager/Public/Projectile/ManagedProjectileBase.h"

//-----------------------------------------------------------------------------------
// Managed Projectile Base Class Constructor										-
//-----------------------------------------------------------------------------------
AManagedProjectileBase::AManagedProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// -- sphere comp
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	SphereCollision->InitSphereRadius(32.f);
	RootComponent = SphereCollision;

	// -- Movement comp set up
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Comp"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
}

//-----------------------------------------------------------------------------------
// Managed Projectile Base Class Engine Events										-
//-----------------------------------------------------------------------------------
/* Engine Begin Play Event */
void AManagedProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

/* Engine Tick Event */
void AManagedProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//-----------------------------------------------------------------------------------
// Managed Projectile Base Class Lifecycle Methods									-
//-----------------------------------------------------------------------------------

/*	Lifecycle method for projectiles when the pool wants to update this projectile.
	@param: Settings: The settings coming in the request method
	@returns: if the projectile handled the update successfully.
*/
bool AManagedProjectileBase::Request_UpdateFromPool(FProjectilePoolRequest Settings)
{
	if (!ProjectileMovement || !SphereCollision) return false;
	else
	{
		// hide the actor on move. 
		SetActorHiddenInGame(Settings.GetHideOnMove());

		// make sure the collision is diabled to remove any weirdness
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// make sure this is not moving
		ProjectileMovement->StopMovementImmediately();

		// set the actor location and rotation.
		SetActorLocationAndRotation(Settings.GetStartLocation(), Settings.GetDirectionVector().ToOrientationQuat(), !Settings.GetTeleportOnMove(), nullptr, ETeleportType::TeleportPhysics);

		// set the velocity. 
		ProjectileMovement->Velocity = Settings.GetDirectionVector() * Settings.GetProjectileSpeed();

		// set the collision back to the new state. 
		SphereCollision->SetCollisionEnabled(Settings.GetCollisionEnabledSettings());	

		// Unhide the actor after the move
		SetActorHiddenInGame(Settings.GetHideAfterMove());

		return true;
	}
}

/*	Lifecycle method to destroy the projectile. 
	@returns: if the projectile was destroyed. 
*/
bool AManagedProjectileBase::Deinit_ProjectileBase()
{
	return Destroy();
}
