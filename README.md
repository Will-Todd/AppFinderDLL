# AppFinderDLL
## What it does?
Windows has a virtual folder containing all executables on the drive (you can see your's be hitting Win + R and entering shell:appsFolder), which the DLL enumerates using a COM object and a display name from each.


By importing and using this inside Unity, you can scan this folder for a keyword, for example the name of a game and create your own Psycho Mantis style spookiness.

## How to use it 
Import the DLL as an asset to Unity, and put it inside any folder called 'Plugins'. 


From a C# script, make sure to add 
```
using System.Runtime.InteropServices; 
```
and then within your monobehaviour place
```
[DllImport("PrintyBoi", EntryPoint = "SearchAppsForString")]
public static extern int SearchAppsForString(string name);
```
To make the code simpler, the function returns an a '1' if true and '0' if false. Regular booleans require marshaling that isn't necessary here. 
