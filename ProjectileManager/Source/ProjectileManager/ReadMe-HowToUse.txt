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
//
//
//
// This plugin is meant to be a single player solution for 
// FPS games that need to use object pooling for their projecile management.
// An example map is included in the content folder along with example blueprints. 
// ProjectileManager/Content/Maps for the example maps
//
// Basic work flow. 
// To use this it comes down to three major steps. 
// 1)	Define a projectile class that the manager will use. 
//		The projectile should inheirt from the ManagedProjectileBase class. 
//		This class will be used by the manager, you can overload the lifecycle methods as needed. 
// 
// 2) Place the projectile manager in the scene. The GetProjectileManager() node will find it if its in scene.
//		In general you want to cache this reference so you dont need to search the scene each time to request a projectile
//		or put it back in the pool. 
//
// 3) Anything using this needs to conform the pipeline GetProjectileFromManagerPool() or ReturnProjectileToManagerPool(). 
//		These methods will act as a pipeline for you to get a projectile from the pool or to return one to the pool. 
//		your projectile user should stay within this pipeline. The manager assumes you will follow it. 
// 
// Best, Nicholas

