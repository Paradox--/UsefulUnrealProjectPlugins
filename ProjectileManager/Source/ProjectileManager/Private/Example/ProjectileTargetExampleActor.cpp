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

#include "ProjectileTargetExampleActor.h"
#include "ProjectileManager/Public/FunctionLibrary/ProjectileManagerFunctionLibrary.h"

//-----------------------------------------------------------------------------------
// Projectile Target Example Class Constructor										-
//-----------------------------------------------------------------------------------
AProjectileTargetExampleActor::AProjectileTargetExampleActor()
{
 	// -- Actor Class Defaults
	PrimaryActorTick.bCanEverTick = true;

	// scene root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = Root;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	if (BoxComp)
	{
		BoxComp->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
		BoxComp->bHiddenInGame = false;
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);				
		BoxComp->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	}	
}

//-----------------------------------------------------------------------------------
// Projectile Target Example Class Engine Events									-
//-----------------------------------------------------------------------------------
void AProjectileTargetExampleActor::BeginPlay()
{
	Super::BeginPlay();

	// -- bind for collision
	if (BoxComp)
	{
		BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileTargetExampleActor::OnOverlapBegin);
	}

	// -- get the projectile manager 
	ProjectileManager = UProjectileManagerFunctionLibrary::GetProjectileManager(this);	
}

void AProjectileTargetExampleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//-----------------------------------------------------------------------------------
// Projectile Target Example Class On Overlap Callbacks								-
//-----------------------------------------------------------------------------------
/*  Catches the Overlap of a component callback signature, if it is a projectile; handles it.  
	@param: OverlappedComponent: The component that was overlapped.
	@param: OtherActor: The actor that overlapped us.
	@param: OtherComp: The component that overlapped us
	@param: OtherBodyIndex: The index
	@param: bFromSweep: Was the overlap from a sweep?
	@param: SweepResult: The sweep hit result. 
	@returns: void. 
*/
void AProjectileTargetExampleActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;
	else
	{
		if (AManagedProjectileBase* Projectile = Cast<AManagedProjectileBase>(OtherActor))
		{
			if (ProjectileManager)
			{
				if (ProjectileManager->Request_ReturnProjectileToManager(Projectile))
				{
					if(bShowDebug) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Projectile Returned to Pool"));
				}
				else
				{
					if (bShowDebug) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Failed to Return Projectile to Pool"));
				}
			}
			else
			{
				if (bShowDebug) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("No Projectile Manager to return object back to the pool!"));
			}
		}
	}
}

