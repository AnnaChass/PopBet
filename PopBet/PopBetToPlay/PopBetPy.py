import os
import telebot
from telebot import types
from dataclasses import dataclass
import GamePy
from PopBetLib import Level


@dataclass
class CallbackNames:
    EMPTY: str = "EMPTY"
    START: str = "START"
    NEW_GAME: str = "NEW_GAME"
    JOIN_GAME: str = "JOIN_GAME"
    READY: str = "READY"
    NEW_CARD: str = "NEW_CARD"
    EASY_LEVEL: str = "EASY_LEVEL"
    MED_LEVEL: str = "MED_LEVEL"
    HARD_LEVEL: str = "HARD_LEVEL"
    WANT_TO_PLAY: str = "WANT_TO_PLAY"
    WIN: str = "WIN"
    LOSE: str = "LOSE"
    END_OF_GAME = "END_OF_GAME"


@dataclass
class ButtonHandler:
    message_text_: str
    button_text_: str
    new_callback_: []


@dataclass
class MessageHandler:
    chat: int
    message: int


bot = telebot.TeleBot("6637599974:AAEnBgGHlAGV4FdTVh2LaBwO2c_6TW6U7WY")
lang = 'ru'
game: GamePy.GameClass
button_handler_ = {CallbackNames.EMPTY: ButtonHandler(message_text_="empty",
                                                      button_text_="empty",
                                                      new_callback_=[CallbackNames.EMPTY]),
                   CallbackNames.START: ButtonHandler(message_text_="Добро пожаловать в игру PopBet!",
                                                      button_text_="Старт",
                                                      new_callback_=[CallbackNames.NEW_GAME,
                                                                     CallbackNames.JOIN_GAME]),
                   CallbackNames.NEW_GAME: ButtonHandler(
                       message_text_="Дождитесь, пока присоединятся все игроки, а затем нажмите кнопку.",
                       button_text_="Создать новую игру",
                       new_callback_=[CallbackNames.READY]),
                   CallbackNames.JOIN_GAME: ButtonHandler(
                       message_text_="Вы присоединились к игре. \n"
                                     "Пожалуйста, дождитесь начала игры.",
                       button_text_="Присоединиться к игре",
                       new_callback_=[CallbackNames.EMPTY]),
                   CallbackNames.READY: ButtonHandler(message_text_="Отлично! Игра началась!",
                                                      button_text_="Мы готовы!",
                                                      new_callback_=[CallbackNames.NEW_CARD]),
                   CallbackNames.NEW_CARD: ButtonHandler(
                       message_text_="Выберите карту. Нажмите на кнопку с уровнем сложности, соответствующем выбранной карте. \n\n"
                                     "Низкий уровень: %s\n\n"
                                     "Средний уровень: %s\n\n"
                                     "Высокий уровень: %s\n\n",
                       button_text_="Новая карта",
                       new_callback_=[CallbackNames.EASY_LEVEL,
                                      CallbackNames.MED_LEVEL,
                                      CallbackNames.HARD_LEVEL]),
                   CallbackNames.EASY_LEVEL: ButtonHandler(message_text_="Был выбран низкий уровень сложности.\n"
                                                                         "Текст карточки: %s\n\n"
                                                                         "Делайте выши ставки. \n"
                                                                         "Пожалуйста, нажмите на кнопку только если вы победили в ставках!",
                                                           button_text_="Низкий",
                                                           new_callback_=[CallbackNames.WANT_TO_PLAY]),
                   CallbackNames.MED_LEVEL: ButtonHandler(message_text_="Был выбран средний уровень сложности.\n"
                                                                        "Текст карточки: %s\n\n"
                                                                        "Делайте выши ставки. \n"
                                                                        "Пожалуйста, нажмите на кнопку только если вы победили в ставках!",
                                                          button_text_="Средний",
                                                          new_callback_=[CallbackNames.WANT_TO_PLAY]),
                   CallbackNames.HARD_LEVEL: ButtonHandler(message_text_="Был выбран высокий уровень сложности.\n"
                                                                         "Текст карточки: %s\n\n"
                                                                         "Делайте выши ставки. \n"
                                                                         "Пожалуйста, нажмите на кнопку только если вы победили в ставках!",
                                                           button_text_="Высокий",
                                                           new_callback_=[CallbackNames.WANT_TO_PLAY]),
                   CallbackNames.WANT_TO_PLAY: ButtonHandler(message_text_="Нажмите на подходящий результат.",
                                                             button_text_="Я буду играть",
                                                             new_callback_=[CallbackNames.WIN, CallbackNames.LOSE]),
                   CallbackNames.WIN: ButtonHandler(message_text_="Поздравляем! \n"
                                                                  "Вы выиграли %i очк. \n"
                                                                  "Ваш текущий счет: %i очк.",
                                                    button_text_="Я выиграл",
                                                    new_callback_=[CallbackNames.NEW_CARD]),
                   CallbackNames.LOSE: ButtonHandler(message_text_="Очень жаль. \n"
                                                                   "Вы проиграли %i очк. \n"
                                                                   "Ваш текущий счет: %i очк.",
                                                     button_text_="Я проиграл",
                                                     new_callback_=[CallbackNames.NEW_CARD]),
                   CallbackNames.END_OF_GAME: ButtonHandler(message_text_="Игра окончена! Спасибо за участие!\n\n"
                                                                          "Результаты игры: \n%s",
                                                            button_text_="empty",
                                                            new_callback_=[CallbackNames.EMPTY]),
                   }
last_message_ = {}  #player_id: MessageHandler


def create_markup(callback_name):
    markup = types.InlineKeyboardMarkup()
    for callback in button_handler_[callback_name].new_callback_:
        if not callback == CallbackNames.EMPTY:
            markup.add(types.InlineKeyboardButton(text=button_handler_[callback].button_text_,
                                                  callback_data=callback))
    return markup


def reply(call, message):
    global game
    global last_message_
    last_message_[call.from_user.id].message = bot.send_message(call.message.chat.id,
                                                                message,
                                                                reply_markup=create_markup(call.data)
                                                                ).id


def multi_reply(call, message):
    global game
    global last_message_
    for player in game.get_all_players():
        last_message_[player].message = bot.send_message(player,
                                                         message,
                                                         reply_markup=create_markup(call.data)
                                                         ).id


def end_of_game_check(call):
    if game.is_game_running():
        return

    bot.edit_message_reply_markup(last_message_[call.from_user.id].chat,
                                  last_message_[call.from_user.id].message)

    player_points_str = ""
    for player, points in game.get_points_all_players_str().items():
        player_name = bot.get_chat_member(player, player).user.first_name + " " + bot.get_chat_member(player, player).user.last_name
        player_points_str += player_name + ": " + str(points) + "\n"
    message = button_handler_[CallbackNames.END_OF_GAME].message_text_ % player_points_str

    call.data = CallbackNames.END_OF_GAME
    multi_reply(call, message)
    bot.stop_polling()
    exit(0)


@bot.message_handler(commands=['start'])
def handle_start(message):
    bot.send_message(message.chat.id,
                     button_handler_[CallbackNames.START].message_text_,
                     reply_markup=create_markup(CallbackNames.START))


@bot.callback_query_handler(func=lambda call: True)
def handle_button(call):
    global game
    global last_message_
    bot.edit_message_reply_markup(call.message.chat.id, call.message.message_id)

    match call.data:
        case CallbackNames.NEW_GAME:
            game = GamePy.GameClass()
            game.create_new_game(call.from_user.id)
            last_message_[call.from_user.id] = MessageHandler(call.message.chat.id, 0)
            reply(call, button_handler_[call.data].message_text_)
        case CallbackNames.JOIN_GAME:
            game.add_new_player(call.from_user.id)
            last_message_[call.from_user.id] = MessageHandler(call.message.chat.id, 0)
            reply(call, button_handler_[call.data].message_text_)
        case CallbackNames.READY:
            for player in game.get_all_players():
                if player != call.from_user.id:
                    last_message_[player].message = bot.send_message(player,
                                                                     button_handler_[call.data].message_text_,
                                                                     reply_markup=None
                                                                     ).id
            reply(call, button_handler_[call.data].message_text_)
        case CallbackNames.NEW_CARD:
            message = button_handler_[call.data].message_text_ % (game.get_new_card(Level.EASY),
                                                                  game.get_new_card(Level.MEDIUM),
                                                                  game.get_new_card(Level.HARD))
            reply(call, message)
        case CallbackNames.EASY_LEVEL:
            game.set_level(Level.EASY)
            message = button_handler_[call.data].message_text_ % game.get_old_card(Level.EASY)
            multi_reply(call, message)
        case CallbackNames.MED_LEVEL:
            game.set_level(Level.MEDIUM)
            message = button_handler_[call.data].message_text_ % game.get_old_card(Level.MEDIUM)
            multi_reply(call, message)
        case CallbackNames.HARD_LEVEL:
            game.set_level(Level.MEDIUM)
            message = button_handler_[call.data].message_text_ % game.get_old_card(Level.HARD)
            multi_reply(call, message)
        case CallbackNames.WANT_TO_PLAY:
            game.set_current_player(call.from_user.id)
            for player in game.get_all_players():
                if player != call.from_user.id:
                    bot.edit_message_reply_markup(last_message_[player].chat, last_message_[player].message)
            reply(call, button_handler_[call.data].message_text_)
        case CallbackNames.WIN:
            player = call.from_user.id
            game.set_points(True)
            message = button_handler_[call.data].message_text_ %(game.get_level_points(),game.get_players_points(player))
            reply(call, message)
            end_of_game_check(call)
        case CallbackNames.LOSE:
            player = call.from_user.id
            game.set_points(False)
            message = button_handler_[call.data].message_text_ % (game.get_level_points(), game.get_players_points(player))
            reply(call, message)
            end_of_game_check(call)


bot.infinity_polling()
