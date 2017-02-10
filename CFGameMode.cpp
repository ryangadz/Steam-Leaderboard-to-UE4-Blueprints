//  Copyright Ryan Gadz 2017 All Rights Reserved

#include "CF_4_13test.h"
#include "CFGameMode.h"
#include "CSteamLeaderboards.h"




ACFGameMode::ACFGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	//set default values, if any
}


void ACFGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (SteamAPI_Init()) {
		if (!g_SteamLeaderboards) {
			g_SteamLeaderboards = new CSteamLeaderboards();
		}
		const char* name = "steam API working";
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, name);
	}
}

//manage the callbacks on the tick, not sure if there is another way
void ACFGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SteamAPI_Init()) {
		SteamAPI_RunCallbacks();
		if (g_SteamLeaderboards) {
			if (!bLeaderboardFound)
			{
				bLeaderboardFound = g_SteamLeaderboards->bLeaderboardFound;
			}
			else if (bSetScore)
				SetLBScore();
			else if (bGetScore && !bSetScore)
					GetLBScores();
			if(g_SteamLeaderboards->bLeaderboardScoresFound && g_SteamLeaderboards->bLeaderboardScoreUserFound)
					LeaderboardScoresFound_Implementation(Scores);
		}
	}
}

//Blueprint set leaderboard name
void ACFGameMode::SetLeaderboardName(FString LeaderboardName)
{
	if (SteamAPI_Init()) {
		if (g_SteamLeaderboards)
		{
			LBName = LeaderboardName;
			const char *chaLBName = TCHAR_TO_ANSI(*LBName);
			g_SteamLeaderboards->FindLeaderboard(chaLBName);
		}
	}
}

//does not do anything
void ACFGameMode::LeaderboardFound_Implementation(int num)
{
	const char* name = "Leaderboard Found From Tick";
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, name);
	
}



// you will want to adjust this depending on what scores you want to show in your game
void ACFGameMode::LeaderboardScoresFound_Implementation(TArray<FScorePackage>& scores)
{
	if (SteamAPI_Init()) {
		if (g_SteamLeaderboards) {
			FScorePackage ThisScore;
			TArray<FScorePackage> Scores;
			Scores.Init(ThisScore, NumberOfScores+1);
			for (int i = 0; i <= NumberOfScores; i++)
			{		
				LeaderboardEntry_t LBRow = g_SteamLeaderboards->m_leaderboardEntries[i];
				ThisScore.PlayerName = SteamFriends()->GetFriendPersonaName(LBRow.m_steamIDUser);
				if (ThisScore.PlayerName == ""){ //check if returning a null entry
					ThisScore.PlayerName = "---";
					ThisScore.Rank = -1; //just looks cleaner 
					ThisScore.Score = 0;
				}
				else {
					ThisScore.Rank = LBRow.m_nGlobalRank;
					ThisScore.Score = LBRow.m_nScore;
				}

				Scores[i] = ThisScore;
			}
			g_SteamLeaderboards->bLeaderboardScoresFound = false; 
			g_SteamLeaderboards->bLeaderboardScoreUserFound = false;
			if (Scores[0].Rank > 8)
				Scores[NumberOfScores] = Scores[0];
			LeaderboardScoresFound(Scores);
		}
	}
}




//Blueprint node sets the current player score
void ACFGameMode::SetLeaderboardScore(int score)
{
	const char* name = "SetLeaderboardScore Called";
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, name);
	Score = score;
	bSetScore = true;
}

//Setting the player score
void ACFGameMode::SetLBScore()
{
	if (SteamAPI_Init()) {
		if (g_SteamLeaderboards) {
			g_SteamLeaderboards->UploadScore(Score);
			bSetScore = false;
		}
	}
}

//Blueprint node gets range of scores needed
void ACFGameMode::GetLeaderboardScore(int numberOfScores)
{
	const char* name = "GetLeaderboardScore Called";
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, name);
	NumberOfScores = numberOfScores;
	bGetScore = true;
}

//Getting the range of scores
void ACFGameMode::GetLBScores()
{
	const char* name = "GetLBScores Called";
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, name);
	if (SteamAPI_Init()) {
		if (g_SteamLeaderboards) {
			g_SteamLeaderboards->DownloadScoreUser();

			//skip the rest of the list if only current player is called
			if (NumberOfScores > 1)
				g_SteamLeaderboards->DownloadScores(NumberOfScores);
			else
				g_SteamLeaderboards->bLeaderboardScoresFound = true;

			bGetScore = false;

		}
	}
}
