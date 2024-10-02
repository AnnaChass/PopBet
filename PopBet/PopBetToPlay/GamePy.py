import PopBetLib


class GameClass:
    def __init__(self):
        self.game: PopBetLib.GameClass() = PopBetLib.Game()
        self.easy_card: str = ""
        self.medium_card: str = ""
        self.hard_card: str = ""

    def add_new_player(self, pid):
        self.game.newPlayer(pid)

    def create_new_game(self, pid):
        self.add_new_player(pid)
        self.game.setCurrentPlayer(pid)

    def set_current_player(self, pid):
        self.game.setCurrentPlayer(pid)

    def set_points(self, is_win):
        self.game.setPoints(is_win)

    def get_players_points(self, pid):
        return self.game.getPoints(pid)

    def get_all_players(self):
        return self.game.getAllPlayers()

    def get_points_all_players_str(self):
        player_points_str = ""
        for player, points in self.game.getPointsAllPlayers().items():
            player_points_str += str(player) + ": " + str(points) + "\n"
        return self.game.getPointsAllPlayers()

    def get_new_card(self, level):
        card = self.game.getNewCard(level)
        if level == PopBetLib.Level.EASY:
            self.easy_card = card
        elif level == PopBetLib.Level.MEDIUM:
            self.medium_card = card
        else:
            self.hard_card = card
        return card

    def get_old_card(self, level):
        if level == PopBetLib.Level.EASY:
            return self.easy_card
        elif level == PopBetLib.Level.MEDIUM:
            return self.medium_card
        else:
            return self.hard_card

    def set_level(self, level):
        self.game.setLevel(level)

    def get_level_points(self):
        return PopBetLib.toNum(self.game.getLevel())

    def is_game_running(self):
        return self.game.isGameRunning()

