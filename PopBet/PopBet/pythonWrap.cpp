#define BOOST_PYTHON_STATIC_LIB
#include <string>
#include <iostream>
#include <boost/python.hpp>

#include "Game.h"
#include "Exception.h"

using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(newPlayer_overloads, Game::newPlayer, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(setPoints_overloads, Game::setPointsToCurrentPlayer, 0, 1)

boost::python::list getAllPlayersWrapper(Game* game)
{
	std::vector<pid> vec = game->getAllPlayers();
	boost::python::list list;
	for (auto iter = vec.begin(); iter != vec.end(); ++iter)
	{
		list.append(*iter);
	}
	return list;
}

boost::python::dict getPointsAllPlayersWrapper(Game* game)
{
	std::vector<Game::playerWithPoints> vec = game->getPointsAllPlayers();
	boost::python::dict dict;
	for (auto iter = vec.begin(); iter != vec.end(); ++iter)
	{
		dict[iter->player_id] = iter->points;
	}
	return dict;
}

int toNum(Game::levelPoints level)
{
    return level;
}

void GameModule()
{
	class_<Game>("Game")
		.def(init<>())
		.def("newPlayer",					&Game::newPlayer,					newPlayer_overloads(args("playerId", "points")))
		.def("deletePlayer",				&Game::deletePlayer,				args("playerId"))
		.def("getAllPlayers",				&getAllPlayersWrapper)
		.def("setCurrentPlayer",			&Game::setCurrentPlayer,			args("playerId"))
		.def("getNewCard",					&Game::getNewCard,					args("level"))
		.def("setPoints",					&Game::setPointsToCurrentPlayer,	setPoints_overloads(args("isWin")))	
		.def("getPoints",					&Game::getPoints,					args("playerId"))
		.def("getPointsAllPlayers",  		&getPointsAllPlayersWrapper)
		.def("setLevel",					&Game::setLevel,					args("level"))
		.def("getLevel",					&Game::getLevel)
		.def("isGameRunning",				&Game::isGameRunning)
		;

	enum_<Game::levelPoints>("Level")
		.value("EASY", Game::levelPoints::EASY)
		.value("MEDIUM", Game::levelPoints::MEDIUM)
		.value("HARD", Game::levelPoints::HARD)
		;
	def("toNum", toNum);
}

void exceptionTranslator(const Exception& x) 
{
	PyErr_SetString(PyExc_BaseException, x.what());
}

void ExceptionModule()
{
	register_exception_translator<Exception>(&exceptionTranslator);
}

BOOST_PYTHON_MODULE(PopBetLib)
{
	GameModule();
	ExceptionModule();
}