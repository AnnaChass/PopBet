#pragma once
#include <iostream>
#include <fstream>

#include "Game.h"

Game::Game() : is_game_running_(true)
{
}

int Game::startGame()
{

    return 0;
}

bool Game::isGameRunning() const
{
    return is_game_running_;
}

int Game::newPlayer(pid player_id, int points)
{
    if (!isGameRunning())
        throw(Exception(Exception::error::GAME_IS_OVER));

    return players.newPlayer(player_id, points);
}

int Game::deletePlayer(pid player_id)
{
    if (!isGameRunning())
        throw(Exception(Exception::error::GAME_IS_OVER));

    return players.deletePlayer(player_id);
}

std::vector<pid> Game::getAllPlayers() const
{
    return players.getAllPlayers();
}

int Game::setCurrentPlayer(pid player_id)
{
    return players.setCurrentPlayer(player_id);
}

std::string Game::getNewCard(levelPoints level)
{
    if (isOutOfCards(level))
        throw(Exception::error::OUT_OF_CARDS);

    std::string card = cards.getNewCard(level);
    if (isOutOfCards(level))
        is_game_running_ = false;
    return card;
}

bool Game::isOutOfCards(levelPoints level) const
{
    return cards.isOutOfCards(level);
}

bool Game::isOutOfAnyCards() const
{
    if (isOutOfCards(Game::levelPoints::EASY) ||
        isOutOfCards(Game::levelPoints::MEDIUM) ||
        isOutOfCards(Game::levelPoints::HARD)
        )
        return true;
    return false;
}

int Game::setPointsToCurrentPlayer(bool isWin)
{
    return players.setPointsToCurrentPlayer(isWin);
}

int Game::getPoints(pid player_id) const
{
    return players.getPoints(player_id);
}

std::vector<Game::playerWithPoints> Game::getPointsAllPlayers() const
{
    return players.getPointsAllPlayers();
}

int Game::setLevel(levelPoints level)
{
    return players.setLevel(level);
}

Game::levelPoints Game::getLevel() const
{
    return players.getLevel();
}

Game::PlayersHandler::PlayersHandler() : current_player_(0), level_(Game::levelPoints::EASY)
{
}

int Game::PlayersHandler::newPlayer(pid player_id, int points)
{
    if (player_id < 1)
        throw(Exception(Exception::error::INCORRECT_PLAYER_ID));
        
    if (isPlayerExist(player_id))
        throw(Exception(Exception::error::PLAYER_IS_ALREADY_EXIST));

    players_info_[player_id] = points;
    //players_order_.insert(players_order_.end(), player_id);
    return 0;
}

int Game::PlayersHandler::deletePlayer(pid player_id)
{
    if (!isPlayerExist(player_id))
        throw(Exception(Exception::error::PLAYER_IS_NOT_EXIST));
    if (!players_info_.erase(player_id))
        throw(Exception(Exception::error::PLAYER_IS_NOT_DELETED));
    /*if (players_order_.erase(orderIterByPid(player_id)) == players_order_.end());
        throw(Exception(Exception::error::PLAYER_IS_NOT_DELETED));*/

    return 0;
}

// не менять мапу, пока жив объект, возвращенный этой функцией
std::vector<pid> Game::PlayersHandler::getAllPlayers() const
{
    std::vector<pid> allPlayers;
    for (const auto& item : players_info_)
    {
        allPlayers.push_back(item.first);
    }
    return allPlayers;
}

int Game::PlayersHandler::setCurrentPlayer(pid player_id)
{
    if (!isPlayerExist(player_id))
        throw(Exception(Exception::error::PLAYER_IS_NOT_EXIST));

    current_player_ = player_id;
    //players_order_iter_ = orderIterByPid(player_id);
    return 0;
}

int Game::PlayersHandler::setPointsToCurrentPlayer(bool is_win)
{
    if (current_player_ == 0)
        throw(Exception(Exception::error::CURRENT_PLAYER_IS_NOT_SET));

    return setPoints(current_player_, is_win);
}

int Game::PlayersHandler::setPoints(pid player_id, bool is_win)
{
    if (!isPlayerExist(player_id))
        throw(Exception(Exception::error::PLAYER_IS_NOT_EXIST));

    players_info_[player_id] += (static_cast<int>(level_) * (is_win ? 1 : -1));
    return 0;
}

int Game::PlayersHandler::getPoints(pid player_id) const
{
    if (!isPlayerExist(player_id))
        throw(Exception(Exception::error::PLAYER_IS_NOT_EXIST));

    return players_info_.at(player_id);
}

std::vector<Game::playerWithPoints> Game::PlayersHandler::getPointsAllPlayers() const
{
    std::vector<playerWithPoints> allPlayers;
    for (const auto & item : players_info_)
    {
        allPlayers.push_back(playerWithPoints(item.first, item.second));
    }
    return allPlayers;
}

int Game::PlayersHandler::setLevel(levelPoints level) 
{
    this->level_ = level;
    return 0;
}

Game::levelPoints Game::PlayersHandler::getLevel() const
{
    return level_;
}

bool Game::PlayersHandler::isPlayerExist(pid player_id) const
{
    if (players_info_.find(player_id) != players_info_.end())
        return true;
    return false;
}

Game::playerWithPoints::playerWithPoints(pid player_id, const int points) : player_id(player_id), points(points)
{

}

Game::CardsHandler::CardsHandler()
{
    createCardPull();
}

const std::string& Game::CardsHandler::getNewCard(levelPoints level)
{
    if (level == levelPoints::EASY)
    {
        size_t num = easy_card_order_.front();
        easy_card_order_.pop();
        return easy_cards_pull_.at(num);
    }
    else if (level == levelPoints::MEDIUM)
    {
        size_t num = medium_card_order_.front();
        medium_card_order_.pop();
        return medium_cards_pull_.at(num);
    }
    else
    {
        size_t num = hard_card_order_.front();
        hard_card_order_.pop();
        return hard_cards_pull_.at(num);
    }
}

bool Game::CardsHandler::isOutOfCards(levelPoints level) const
{
    if ((level == Game::levelPoints::EASY && easy_card_order_.empty()) ||
        (level == Game::levelPoints::MEDIUM && medium_card_order_.empty()) ||
        (level == Game::levelPoints::HARD && hard_card_order_.empty())
        )
        return true;
    return false;
}

int Game::CardsHandler::createCardPull()
{
    // create pulls of cards
    std::ifstream easyCardsFile(easy_cards_filename_);
    for (std::string card; std::getline(easyCardsFile, card);)
        easy_cards_pull_.push_back(card);
    easyCardsFile.close();

    std::ifstream mediumCardsFile(medium_cards_filename_);
    for (std::string card; std::getline(mediumCardsFile, card);)
        medium_cards_pull_.push_back(card);
    mediumCardsFile.close();

    std::ifstream hardCardsFile(hard_cards_filename_);
    for (std::string card; std::getline(hardCardsFile, card);)
        hard_cards_pull_.push_back(card);
    hardCardsFile.close();

    // create temporary vector with order of cards' numbers
    std::vector<int> easyTmpOrder = createRandomizedOrder(easy_cards_pull_.size());
    std::vector<int> mediumTmpOrder = createRandomizedOrder(medium_cards_pull_.size());
    std::vector<int> hardTmpOrder = createRandomizedOrder(hard_cards_pull_.size());

    // create queue with randomized order of cards' numbers based on the temporary vector 
    for (int i = 0; i < easy_cards_pull_.size(); i++)
    {
        easy_card_order_.push(easyTmpOrder[i]);
    }
    for (int i = 0; i < medium_cards_pull_.size(); i++)
    {
        medium_card_order_.push(mediumTmpOrder[i]);
    }
    for (int i = 0; i < hard_cards_pull_.size(); i++)
    {
        hard_card_order_.push(hardTmpOrder[i]);
    }

    return 0;
}

std::vector<int> Game::CardsHandler::createRandomizedOrder(size_t size)
{
    // create temporary vector with order of cards' numbers
    std::vector<int> tmpOrder(size, -1);
    for (int i = 0; i < size; i++)
    {
        tmpOrder[i] = i;
    }

    // randomize order of cards' numbers in this temporary vector
    //srand(time(NULL));
    //for (int i = size - 1; i >= 1; i--)
    //{
    //    int j = rand() % (i + 1);

    //    int tmp = tmpOrder[j];
    //    tmpOrder[j] = tmpOrder[i];
    //    tmpOrder[i] = tmp;
    //}

    std::random_device rnd_device;
    std::mt19937 mersenne_engine{ rnd_device() };
    std::shuffle(tmpOrder.begin(), tmpOrder.end(), mersenne_engine);

    return tmpOrder;
}
