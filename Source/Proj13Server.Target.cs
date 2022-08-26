// Copyright Epic Games, Inc. All Rights Reserved.

//using UnrealBuildTool;
//using System.Collections.Generic;

//public class Proj13Target : TargetRules
//{
//	public Proj13Target(TargetInfo Target) : base(Target)
//	{
//		Type = TargetType.Game;
//		DefaultBuildSettings = BuildSettingsVersion.V2;
//		ExtraModuleNames.Add("Proj13");
//	}
//}


using UnrealBuildTool;
using System.Collections.Generic;

public class Proj13ServerTarget : TargetRules //Change this line according to the name of your project
{
    public Proj13ServerTarget(TargetInfo Target) : base(Target) //Change this line according to the name of your project
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("Proj13"); //Change this line according to the name of your project
    }
}