//  Copyright Ryan Gadz 2017 All Rights Reserved

#pragma once

#include "Engine/UserDefinedStruct.h"
#include "LeaderboardData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FScorePackage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Leaderboard")
	FString PlayerName = "working";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Leaderboard")
		int32 Rank = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Leaderboard")
		int32 Score = 0;

};
