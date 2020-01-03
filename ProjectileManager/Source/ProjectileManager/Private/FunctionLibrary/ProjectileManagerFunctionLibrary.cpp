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

#include "ProjectileManager/Public/FunctionLibrary/ProjectileManagerFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


//-----------------------------------------------------------------------------------
// projectile Manager Blueprint Library Function Methods							-
//-----------------------------------------------------------------------------------
/*	Returns the current Projectile manager in scene. 
	@param: ContextObject: The context object to get the world reference from
	@returns: the first instance of the projectile manager 
*/
AProjectileManagerBase* UProjectileManagerFunctionLibrary::GetProjectileManager(const UObject* ContextObject)
{
	if (!ContextObject)
	{
		UE_LOG(LogClass, Error, TEXT("Inputed Context Object is invalid, cant find the object without the world reference"));
		return nullptr;
	}
	else
	{
		TArray<AActor*> OutActors;

		UGameplayStatics::GetAllActorsOfClass(ContextObject, AProjectileManagerBase::StaticClass(), OutActors);

		if (OutActors.Num() <= 0)
		{
			UE_LOG(LogClass, Error, TEXT("Could not find the projectile manager in scene, make sure to put one in your scene."));
			return false;
		}
		else // if we have any, just get the first one. 
		{
			return Cast<AProjectileManagerBase>(OutActors[0]);
		}
	}	
}

/*	Returns if the manager was able to resize properly. 
	@param: ContextObject: The context object to get the world reference from
	@param: NewProjectilePoolSize: The requested pool size. 
	@returns: if the manager was able to resize properly. 
*/
bool UProjectileManagerFunctionLibrary::ResizeProjectilePool(const UObject* ContextObject, int32 NewProjectilePoolSize)
{
	if (NewProjectilePoolSize <= 0)
	{
		UE_LOG(LogClass, Error, TEXT("Can not resize projectile manager pool to any value less than 1, you requested a value of %d for the new pool size"), NewProjectilePoolSize);
		return false;
	}
	else
	{
		// get the manager in scene
		if (AProjectileManagerBase* CurrentManager = GetProjectileManager(ContextObject))
		{
			return CurrentManager->Request_ResizeProjectilePool(NewProjectilePoolSize);
		}
		else
		{
			return false;
		}
	}
}

/*	Returns if we were able to get a projectile from the manager
	@param: ContextObject: The context object to get the world reference from
	@param: OutProjectileToUse: The returned projectile pointer
	@param: RetreieveSettings: The retreieve settings to use on the projectile. 
	@returns: if we returned a valid projectile. 
*/
bool UProjectileManagerFunctionLibrary::GetProjectileFromManagerPool(const UObject* ContextObject, AManagedProjectileBase*& OutProjectileToUse, FProjectilePoolRequest& RetreieveSettings)
{
	// get the manager in scene, and request a projectile
	if (AProjectileManagerBase* CurrentManager = GetProjectileManager(ContextObject))
	{
		return CurrentManager->Request_GetProjectileFromManager(OutProjectileToUse, RetreieveSettings);
	}
	else
	{
		return false;
	}
}

/*	Returns if we were able to return a projectile to the pool
	@param: ContextObject: The context object to get the world reference from
	@param: InProjectileToReturn: The projectile to return to the pool.
	@returns: if we returned the projectile.
*/
bool UProjectileManagerFunctionLibrary::ReturnProjectileToManagerPool(const UObject* ContextObject, AManagedProjectileBase*& InProjectileToReturn)
{
	// get the manager in scene, and return the projectile. 
	if (AProjectileManagerBase* CurrentManager = GetProjectileManager(ContextObject))
	{
		return CurrentManager->Request_ReturnProjectileToManager(InProjectileToReturn);
	}
	else
	{
		return false;
	}
}

/*	Returns the current size of the projectile pool. 
	@param: ContextObject: The context object to get the world reference from
	@returns: the current projectile pool size, -1 if no current manager.
*/
int32 UProjectileManagerFunctionLibrary::GetCurrentProjectilePoolSize(const UObject* ContextObject)
{
	// get the manager in scene, and get the current pool size.
	if (AProjectileManagerBase* CurrentManager = GetProjectileManager(ContextObject))
	{
		return CurrentManager->GetCurrentPoolSize();
	}
	else
	{
		return -1;
	}
}