# UsefulUnrealProjectPlugins
A list of self made useful project level plugins for the unreal engine. These are all build in 
4.23.1 and 4.24.1. You will need to compile them for your project Follow the Install for each below.

## Plugins List
* Projectile Manager

## Plugin Overviews
#### Projectile Manager  
*A plugin meant to manage a single player games projectiles. Allocated a desired number
of projectiles at begin play. Users can access the managed pool via a blueprint library
or via static function calls.
Current Version: 1.0 
License: MIT*

## Install Instructions
#### Projectile Manager
* Clone or download the plugin from this repo. 
* Once downloaded onto your local machine, you need to add this to your project. 
* To do this, in your project directory, add a plugins directory/ folder. 
* Once you make a folder copy the plugin you have downloaded into the plugins folder.
* Now you need to compile the plugin with your project, but first you need to make sure the solution sees the plugin.
* To do this( Assuming your project is c++ based); delete your old .sln or .xproj file that is next to your .uproject file. 
* Now regenerate your solution files by right clicking on the uproject file. 
* Once regerated, open the solution and rebuild your project. 
* Once the project has been rebuild and the plugin binaries are built out. 
* Open your project and make sure the plugin is enabled in your editors plugin page.
