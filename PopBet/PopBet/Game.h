#pragma once
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <random>

#include "Exception.h"

typedef unsigned int uint;
typedef int pid;

class Game
{
public:
	Game();

	struct playerWithPoints
	{
		playerWithPoints(pid player_id, const int points);
		pid player_id;
		int points;
	};
	enum levelPoints
	{
		EASY = 1,
		MEDIUM,
		HARD
	};

	int startGame();
	bool isGameRunning() const;
	
	int newPlayer(pid player_id, int points = 0);
	int deletePlayer(pid player_id);
	std::vector<pid> getAllPlayers() const;
	int setCurrentPlayer(pid player_id);

	std::string getNewCard(levelPoints level);

	int setPointsToCurrentPlayer(bool is_win = true);
	int getPoints(pid player_id) const;
	std::vector<playerWithPoints> getPointsAllPlayers() const;

	int setLevel(levelPoints level);
	levelPoints getLevel() const;

private:
	bool is_game_running_;
	bool isOutOfCards(levelPoints level) const;
	bool isOutOfAnyCards() const;

	class PlayersHandler
	{
	public:
		PlayersHandler();

		int newPlayer(pid player_id, int points = 0);
		int deletePlayer(pid player_id);
		std::vector<pid> getAllPlayers() const;
		int setCurrentPlayer(pid player_id);

		int setPointsToCurrentPlayer(bool is_win);
		int getPoints(pid player_id) const;
		std::vector<Game::playerWithPoints> getPointsAllPlayers() const;

		int setLevel(levelPoints level);
		levelPoints getLevel() const;

	private:
		levelPoints level_;
		std::map<pid, int> players_info_;
		pid current_player_; 

		bool isPlayerExist(pid player_id) const;
		int setPoints(pid player_id, bool is_win);

	} players;

	class CardsHandler
	{
	public:
		CardsHandler();

		const std::string& getNewCard(levelPoints level);
		bool isOutOfCards(levelPoints level) const;

	private:
		const std::string easy_cards_filename_ = "easy_cards.txt"; 
		const std::string medium_cards_filename_ = "medium_cards.txt"; 
		const std::string hard_cards_filename_ = "hard_cards.txt"; 
		std::vector<std::string> easy_cards_pull_;
		std::vector<std::string> medium_cards_pull_;
		std::vector<std::string> hard_cards_pull_;
		std::queue<uint> easy_card_order_;
		std::queue<uint> medium_card_order_;
		std::queue<uint> hard_card_order_;

		int createCardPull();
		std::vector<int> createRandomizedOrder(size_t size);

	} cards;
};

