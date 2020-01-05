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

#include "ProjectileManager/Public/Example/ProjectileFireExampleActor.h"
#include "ProjectileManager/Public/FunctionLibrary/ProjectileManagerFunctionLibrary.h"

//-----------------------------------------------------------------------------------
// Projectile Fire Example Class Constructor										-
//-----------------------------------------------------------------------------------
AProjectileFireExampleActor::AProjectileFireExampleActor()
{
 	// -- Actor Class Defaults
	PrimaryActorTick.bCanEverTick = true;

	// scene root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = Root;

	// arrow component
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	if (Arrow)
	{
		Arrow->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Arrow->bHiddenInGame = false;
		Arrow->SetWorldScale3D(FVector(5.f, 5.f, 5.f));
	}	
}

//-----------------------------------------------------------------------------------
// Projectile Fire Example Class Engine Events										-
//-----------------------------------------------------------------------------------
void AProjectileFireExampleActor::BeginPlay()
{
	Super::BeginPlay();

	// -- get the projectile manager 
	ProjectileManager = UProjectileManagerFunctionLibrary::GetProjectileManager(this);
	
	// -- Set the timer to fire 
	UWorld* const world = GetWorld();

	if (!world) return;
	else
	{
		world->GetTimerManager().SetTimer(Timer_Fire, this, &AProjectileFireExampleActor::OnProjectileExampleFire, TimerIteractionRate, true);
	}
}

void AProjectileFireExampleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//-----------------------------------------------------------------------------------
// Projectile Fire Example Class Timer Methods										-
//-----------------------------------------------------------------------------------
/* The event that is called each time the timer goes off. */
void AProjectileFireExampleActor::OnProjectileExampleFire()
{
	if (ProjectileManager)
	{
		// incase we wanted to do something! Usually It would be best to make a list
		// or use a listen for a callback. 
		AManagedProjectileBase* LastSpawnedProjectile = nullptr;

		

		// build a pool request. 
		FProjectilePoolRequest PoolRequest = FProjectilePoolRequest(true, false, 
																	ECollisionEnabled::QueryAndPhysics, 
																	FMath::FRandRange(MinSpeed, MaxSpeed), 
																	Arrow ? Arrow->GetComponentLocation() : FVector::ZeroVector, 
																	Arrow ? Arrow->GetForwardVector() : FVector::ForwardVector);

		// request a projectile with the pool request we just built. 
		if (ProjectileManager->Request_GetProjectileFromManager(LastSpawnedProjectile, PoolRequest))
		{
			// we can do something here with the projectile if we needed to. 
			if(bShowDebug) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Fired Projectile!"));
		}
		else
		{
			if(bShowDebug) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Failed to Fire Projectile!"));
		}
	}
}

