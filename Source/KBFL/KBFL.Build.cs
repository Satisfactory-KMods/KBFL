using UnrealBuildTool;

public class KBFL : ModuleRules
{
	public KBFL(ReadOnlyTargetRules Target) : base(Target)
	{
		CppStandard = CppStandardVersion.Cpp20;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bLegacyPublicIncludePaths = false;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core", "CoreUObject",
				"Engine",
				"EditorSubsystem",
				"DeveloperSettings",
				"PhysicsCore",
				"InputCore",
				//"OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemNull",
				//"SignificanceManager",
				"GeometryCollectionEngine",
				//"ChaosVehiclesCore", "ChaosVehicles", "ChaosSolverEngine",
				"AnimGraphRuntime",
				//"AkAudio",
				"AssetRegistry",
				"NavigationSystem",
				//"ReplicationGraph",
				"AIModule",
				"GameplayTasks",
				"SlateCore", "Slate", "UMG",
				//"InstancedSplines",
				"RenderCore",
				"CinematicCamera",
				"Foliage",
				//"Niagara",
				"EnhancedInput",
				//"GameplayCameras",
				//"TemplateSequence",
				"Projects",
				"NetCore",
				"GameplayTags",
				"Json", "JsonUtilities"
			});

		// FactoryGame plugins
		PublicDependencyModuleNames.AddRange(
		new string[] {
			"AbstractInstance",
			//"InstancedSplinesComponent",
			//"SignificanceISPC"
		});

		// Header stubs
		PublicDependencyModuleNames.AddRange(new[]
		{
			"DummyHeaders"
		});
		
		if (Target.Type == TargetRules.TargetType.Editor) {
			PublicDependencyModuleNames.AddRange(new string[] {/*"OnlineBlueprintSupport",*/ "AnimGraph"});
		}
		PublicDependencyModuleNames.AddRange(new string[] {"FactoryGame", "SML"});
	}
}