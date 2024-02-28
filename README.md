## Folders / Files to delete when regerating
## Visual Studio project files
1. Delete Binaries folder, Intermediate folder, Saved folder and <file>.sln file
2. Right mouse button click and select "Generate Visual studio project files"

## Debugging
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,
	//		15.f,
	//		FColor::Blue,
	//		SelectedPlayerName
	//	);
	//}

## Adding Enhanced Input
1. Go to the *.Build.cs file of the project
2. Add "EnhancedInput" module in this line:
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

## Unreal Engine 5 Texture Streaming Pool Fix
https://www.youtube.com/watch?v=ahAX3w5ovts
1. Enter on the Console Command: Streaming.PoolSize to seee current poolsize
2. Type: Streaming.PoolSize 8000

## To ensure animation root motion works during retargetting process
1. Select - open the retargetter file
2. Choose Target source tab
3. Select the "Root" bone in the Chain Mapping panel ( under the Details panel )
4. In the details panel, under FK group, find the Translation Mode. 
5. In Translation Mode, select Globally Scaled in the dropdown.