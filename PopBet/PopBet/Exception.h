#pragma once
//#include <assert.h>
#include <string>
#include <vector>

class Exception : public std::exception
{
public:
	enum error
	{
		SUCCESS = 0,
		INCORRECT_PLAYER_ID,
		PLAYER_IS_ALREADY_EXIST,
		PLAYER_IS_NOT_EXIST,
		PLAYER_IS_NOT_DELETED,
		plab,
		GAME_IS_OVER,
		OUT_OF_CARDS,
		CURRENT_PLAYER_IS_NOT_SET,
		FILE_COULD_NOT_BE_OPENED,

	};

    Exception(int exception_code);
	~Exception();// throw();
	
	const char* what() const throw();
    int getCode();

private:
	int code_;
	const std::vector<std::string> error_message_ = 
	{ 
		"SUCCESS",
		"INCORRECT_PLAYER_ID",
		"PLAYER_IS_ALREADY_EXIST",
		"PLAYER_IS_NOT_EXIST",
		"PLAYER_IS_NOT_DELETED",
		"plab",
		"GAME_IS_OVER",
		"OUT_OF_CARDS",
		"CURRENT_PLAYER_IS_NOT_SET",
		"FILE_COULD_NOT_BE_OPENED"
	};
};

