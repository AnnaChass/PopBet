# PopBet
## PopBet - настольная игра про поп-культуру в жанре party. 

### Описание проекта
Данный проект представляет из себя dll-библиотеку PopBetLib, написанную на C++ и обернутую для python, и python-скрипт для Telegram-бота. Библиотека выполняет функцию имитации настольной игры - создание рума, формирование карточек, выбор активного игрока, назначение ему победных очков. Скрипт реализует Telegram-бота для комфортной имитации игры. 
Папка PopBetToPlay содержит все необходимое для начала игры. Для начала игры достаточно запустить скрипт PopBetPy.py, открыть Telegram-бота https://t.me/pop_bet_bot и написать ему "/start".

### Правила игры
Правила игры PopBet следующие. Активный игрок тянет карточку (кнопка "Новая карта") и из трех заданий разного уровня сложности выбирает одно (кнопки "Низкий", "Средний", "Высокий" соответственно выбранному уровню сложности; уровни сложности влияют на получаемые победные очки). Затем игроки за столом, начиная с активного игрока, делают ставки - количество ответов, которые они готовы дать по заданию, - либо пасуют и выбывают из игры ставок. Игрок, оставшийся последним, кто не пасанул, становится новым активным игроком (кнопка "Я буду играть"). Ему дается минута, чтобы выполнить задание в соответствии со сделанной ставкой. По истечении минуты он фиксирует результат (кнопки "Я выиграл", "Я проиграл"), после чего начинается новый раунд, и выбирается новая карточка. Когда карточки закончатся, игра автоматически завершится и всем игрокам будет выведена таблица победных очков. 

Разберем игру на примере. Аня, Боря и Валя играют в PopBet. Аня - активный игрок. Она тянет карточку (кнопка "Новая карта") и получает следующие задания на выбор: Персонажи Властелина Колец на низком уровне сложности, Фильмы про гонки - на среднем, Читкоды в видеоиграх - на высоком. Аня выбирает Фильмы про гонки (кнопка "Средний") и делает свою ставку - она готова назвать 3 фильма. Боря повышает ставку: 4 фильма. Валя любит фильмы про гонки, поэтому повышает ставку сразу до 10. Аня не готова назвать столько фильмов, поэтому она пасует. Боря повышает ставку: 12. Валя тоже повышает: 15. Боря пасует. Валя - последний не пасанувший игрок, поэтому она становится новым активным игроком (кнопка "Я буду играть"). Игроки засекают минуту, в течение нее Валя называет только 14 фильмов. Она проиграла ставку (кнопка "Я проиграл") и получает -2 победных очка. Теперь Валя - активный игрок, и она выбирает новую карточку.