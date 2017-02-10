// Fill out your copyright notice in the Description page of Project Settings.

#include "CF_4_13test.h"
#include "CSteamLeaderboards.h"


CSteamLeaderboards::CSteamLeaderboards() :
	m_CurrentLeaderboard(NULL),
	m_nLeaderboardEntries(0)
{
}

void CSteamLeaderboards::FindLeaderboard(const char *pchLeaderboardName)
{
	const char* name = "FindLeaderboard Called";
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, name);

	m_CurrentLeaderboard = NULL;

	SteamAPICall_t hSteamAPICall = SteamUserStats()->FindLeaderboard(pchLeaderboardName);
	m_callResultFindLeaderboard.Set(hSteamAPICall, this,
		&CSteamLeaderboards::OnFindLeaderboard);
}

void CSteamLeaderboards::OnFindLeaderboard(LeaderboardFindResult_t *pCallback, bool bIOFailure)
{
	const char* name = "OnFindLeaderboard Called";
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, name);

	// see if we encountered an error during the call
	if (!pCallback->m_bLeaderboardFound)
	{
		const char* name = "Leaderboard could not be found";
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, name);
		return;
	}
	else if (bIOFailure)
	{
		const char* name = "bIOFailure";
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, name);
		return;
	}

	m_CurrentLeaderboard = pCallback->m_hSteamLeaderboard;

	if (m_CurrentLeaderboard != NULL)
	{
		bLeaderboardFound = true;
		const char* name = "m_CurrentLeaderboard is valid";
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, name);
	}
}

bool CSteamLeaderboards::UploadScore(int score)
{
	const char* name = "UploadScore Called";
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, name);

	if (!m_CurrentLeaderboard){
		const char* name = "no leaderboard object";
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, name);
		return false;
	}
		

	SteamAPICall_t hSteamAPICall =
		SteamUserStats()->UploadLeaderboardScore(m_CurrentLeaderboard, k_ELeaderboardUploadScoreMethodKeepBest, score, NULL, 0);

	m_callResultUploadScore.Set(hSteamAPICall, this, &CSteamLeaderboards::OnUploadScore);

	return true;
}

void CSteamLeaderboards::OnUploadScore(LeaderboardScoreUploaded_t *pCallback, bool bIOFailure)
{
	const char* name = "OnUploadScore Called";
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, name);

	if (!pCallback->m_bSuccess )
	{
		
		//uint8 m_bSuccess;			// 1 if the call was successful
		//SteamLeaderboard_t m_hSteamLeaderboard;	// the leaderboard handle that was
		//int32 m_nScore;				// the score that was attempted to set
		//uint8 m_bScoreChanged;		// true if the score in the leaderboard change, false if the existing score was better
		//int m_nGlobalRankNew;		// the new global rank of the user in this leaderboard
		//int m_nGlobalRankPrevious;

		FString DebugOutput = "";
		if (pCallback->m_bSuccess) DebugOutput += "Success:";
		else DebugOutput += "Failure: ";
		DebugOutput += FString::Printf(TEXT("Attempted Score: %i, bScoreChanged: %i"), pCallback->m_nScore, pCallback->m_bScoreChanged);
		GEngine->AddOnScreenDebugMessage(-1,10,FColor::Red, DebugOutput);

		const char* name = "Score could not be uploaded to Steam: WAS NOT SUCCESSFUL";
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, name);
	}
	else if(bIOFailure)
	{
		const char* name = "Score could not be uploaded to Steam: IO ERROR";
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, name);
	}
	else
	{
		const char* name = "Score was uploaded to Steam";
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, name);
	}
}


bool CSteamLeaderboards::DownloadScores(int NumberOfScores)
{
	const char* name = "DownloadScores Called";
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, name);

	if (!m_CurrentLeaderboard)
		return false;

	// load the specified leaderboard data from first to number of places
	SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(
		m_CurrentLeaderboard, k_ELeaderboardDataRequestGlobal, 1, NumberOfScores);

	m_callResultDownloadScore.Set(hSteamAPICall, this,
		&CSteamLeaderboards::OnDownloadScore);

	return true;
}

void CSteamLeaderboards::OnDownloadScore(LeaderboardScoresDownloaded_t *pCallback, bool bIOFailure)
{
	const char* name = "OnDownloadScore Called";
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, name);

	if (!bIOFailure)
	{

		m_nLeaderboardEntries = FMath::Min((pCallback->m_cEntryCount+1), 9);

		for (int index = 0; index <= m_nLeaderboardEntries; index++)
		{
			SteamUserStats()->GetDownloadedLeaderboardEntry(
				pCallback->m_hSteamLeaderboardEntries, index, &m_leaderboardEntries[index+1], NULL, 0);
		}
		bLeaderboardScoresFound = true;
	}
}



bool CSteamLeaderboards::DownloadScoreUser()
{
	const char* name = "DownloadScoresUser Called";
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, name);

	if (!m_CurrentLeaderboard)
		return false;

	// load the current user score must be array although there is only one in this case
	CSteamID* users = new CSteamID[1];
	users[0] = SteamUser()->GetSteamID();

	SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntriesForUsers(m_CurrentLeaderboard, users, 1);

	m_callResultDownloadScoreUser.Set(hSteamAPICall, this,
		&CSteamLeaderboards::OnDownloadScoreUser);

	return true;
}

void CSteamLeaderboards::OnDownloadScoreUser(LeaderboardScoresDownloaded_t *pCallback, bool bIOFailure)
{
	const char* name = "OnDownloadScoreUser Called";
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, name);

	if (!bIOFailure)
	{	
			SteamUserStats()->GetDownloadedLeaderboardEntry(
				pCallback->m_hSteamLeaderboardEntries, 0, &m_leaderboardEntries[0], NULL, 0);

		bLeaderboardScoreUserFound = true;
	}
}
