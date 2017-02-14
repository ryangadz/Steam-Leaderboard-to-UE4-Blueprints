//  Copyright Ryan Gadz 2017 All Rights Reserved
#pragma once

#include "GameFramework/GameMode.h"
#include "LeaderboardData.h"
#include "CFGameMode.generated.h"


class CSteamLeaderboards;




UCLASS()
class CF_4_13TEST_API ACFGameMode : public AGameMode
{
	GENERATED_BODY()
private:
	bool bLeaderboardFound = false;
	bool bLeaderboardScoresFound = false;
	int Score = 22;
	bool bSetScore = false;
	int ScoresAhead = 0;
	int NumberOfScores = 0;
	bool bGetScore = false;
	bool bGlobal = true;
	TArray<FScorePackage> Scores;


public:



	//The name of the leaderboard we're looking up from Steam
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Leaderboard")
		FString LBName;

	UFUNCTION(BlueprintCallable, Category = "SteamFunctions")
		void SetLeaderboardScore(int score);

	void SetLBScore();

	UFUNCTION(BlueprintCallable, Category = "SteamFunctions")
		void GetLeaderboardScore(int numberOfScores, bool global);

	void GetLBScores();

	UFUNCTION(BlueprintCallable, Category = "SteamFunctions")
		void SetLeaderboardName(FString LeaderboardName);

	UFUNCTION(BlueprintNativeEvent, Category = "SteamFunctions")
		void LeaderboardFound(int num);
        virtual void LeaderboardFound_Implementation(int num);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SteamFunctions")
		void LeaderboardScoresFound(const TArray<FScorePackage>& scores, bool global);
	    virtual void LeaderboardScoresFound_Implementation(TArray<FScorePackage>& scores, bool global);

	UFUNCTION(BlueprintCallable, Category = "SteamFunctions")
		void SteamAPI_Shutdown();

	CSteamLeaderboards* g_SteamLeaderboards = NULL;

	ACFGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;



	
};
