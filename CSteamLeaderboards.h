// Fill out your copyright notice in the Description page of Project Settings.
#include "ThirdParty/Steamworks/Steamv132/sdk/public/steam/steam_api.h"
#pragma once

/**
*
*/
class CF_4_13TEST_API CSteamLeaderboards
{
private:
	SteamLeaderboard_t m_CurrentLeaderboard; // Handle to leaderboard

public:
	//SteamLeaderboard_t m_CurrentLeaderboard;// Handle to leaderboard
	int m_nLeaderboardEntries; // How many entries do we have?
	LeaderboardEntry_t m_leaderboardEntries[9]; // The entries, user plus 8 top scores

	CSteamLeaderboards();
	~CSteamLeaderboards() {};

	void FindLeaderboard(const char *pchLeaderboardName);
	bool UploadScore(int score);
	bool DownloadScores(int numberOfScores);
	bool bLeaderboardFound = false;
	bool bLeaderboardScoresFound = false;
	bool DownloadScoreUser();
	bool bLeaderboardScoreUserFound = false;





	void OnFindLeaderboard(LeaderboardFindResult_t *pResult, bool bIOFailure);
	CCallResult<CSteamLeaderboards, LeaderboardFindResult_t> m_callResultFindLeaderboard;
	void OnUploadScore(LeaderboardScoreUploaded_t *pResult, bool bIOFailure);
	CCallResult<CSteamLeaderboards, LeaderboardScoreUploaded_t> m_callResultUploadScore;
	void OnDownloadScore(LeaderboardScoresDownloaded_t *pResult, bool bIOFailure);
	CCallResult<CSteamLeaderboards, LeaderboardScoresDownloaded_t> m_callResultDownloadScore;
	void OnDownloadScoreUser(LeaderboardScoresDownloaded_t *pResult, bool bIOFailure);
	CCallResult<CSteamLeaderboards, LeaderboardScoresDownloaded_t> m_callResultDownloadScoreUser;
};