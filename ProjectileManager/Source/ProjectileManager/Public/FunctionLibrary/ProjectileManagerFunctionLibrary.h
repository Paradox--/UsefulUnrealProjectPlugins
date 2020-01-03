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
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProjectileManager/Public/Manager/ProjectileManagerBase.h"
#include "ProjectileManagerFunctionLibrary.generated.h"

/*
 * A blueprint function library to help with highlevel actions using this plugin. 
 * Allows for other blueprints to get the projectile manager from anywhere, and use it as needed.
 * any user needs to pass in a UObject context object. 
 */
UCLASS()
class PROJECTILEMANAGER_API UProjectileManagerFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	// -- Public Information -- Function Library Methods -- //
public:
	/* Returns the Projectile Manager in scene */
	/* This will be expensive for those who don't cache this pointer out*/
	UFUNCTION(BlueprintPure, Category = "Projectile Manager", meta = (WorldContext = "ContextObject"))
	static class AProjectileManagerBase* GetProjectileManager(const UObject* ContextObject);

	/* Requests to resize the manager to a new size of projectiles */
	UFUNCTION(BlueprintCallable, Category = "Projectile Manager", meta = (WorldContext = "ContextObject"))
	static bool ResizeProjectilePool(const UObject* ContextObject, int32 NewProjectilePoolSize);

	/* Geat projectile from the managers current pool. */
	UFUNCTION(BlueprintCallable, Category = "Projectile Manager", meta = (WorldContext = "ContextObject"))
	static bool GetProjectileFromManagerPool(const UObject* ContextObject, class AManagedProjectileBase*& OutProjectileToUse, UPARAM(ref) FProjectilePoolRequest& RetreieveSettings);

	/* Returns a projectile to the pool, passes it in by reference */
	UFUNCTION(BlueprintCallable, Category = "Projectile Manager", meta = (WorldContext = "ContextObject"))
	static bool ReturnProjectileToManagerPool(const UObject* ContextObject, UPARAM(ref) class AManagedProjectileBase*& InProjectileToReturn);

	/* Get the current projectile managers pool size. */
	UFUNCTION(BlueprintCallable, Category = "Projectile Manager", meta = (WorldContext = "ContextObject"))
	static int32 GetCurrentProjectilePoolSize(const UObject* ContextObject);
};
