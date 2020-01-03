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
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectileManager/Public/Manager/ProjectileManagerBase.h"
#include "ProjectileTargetExampleActor.generated.h"

UCLASS()
class PROJECTILEMANAGER_API AProjectileTargetExampleActor : public AActor
{
	GENERATED_BODY()
	
	// -- Public Information -- Projectile Target Example Constructor and Engine Events -- //
public:	
	AProjectileTargetExampleActor();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// -- Puiblic Information -- On Overlap Callbacks -- // 
public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// -- Public Information -- Target Example Properties -- //
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool bShowDebug = false;

	UPROPERTY()
	AProjectileManagerBase* ProjectileManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scene Root")
	USceneComponent* Root = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	UBoxComponent* BoxComp = nullptr;
};
