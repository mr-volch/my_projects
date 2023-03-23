import random
import sys
import cowsay
from io import StringIO

#добавляем свиновью корову
my_cow = cowsay.read_dot_cow(StringIO("""
$the_cow = <<EOC;
         $thoughts
          $thoughts
        ┈┈┏━╮╭━┓┈┈┈┈┈┈┈
        ┈┈┃┏┗┛┓┃┈┈┈┈┈┈┈
        ┈┈╰┓▋▋┏╯┈┈┈┈┈┈┈
        ┈╭━┻╮╲┗━━━━╮╭╮┈
        ┈┃▎▎┃╲╲╲╲╲╲┣━╯┈
        ┈╰━┳┻▅╯╲╲╲╲┃┈┈┈
        ┈┈┈╰━┳┓┏┳┓┏╯┈┈┈
        ┈┈┈┈┈┗┻┛┗┻┛┈┈┈┈
EOC
"""))

def ask(prompt: str, valid: list[str] = None) -> str:
    #Выводим приглашение ко вводу с помощью свмновей коровы
    print(cowsay.cowsay(prompt, cowfile = my_cow))
    prompt = input()

    #если есть ограничения на ввод, просим вводить, пока не удовлетворим их
    if valid:
        while  prompt not in valid:
            #Выводим сообщение о недопустимости ввода свиновей коровой
            print(cowsay.cowsay(f"Введено недопустимое слово {prompt}, повоторите ввод", cowfile = my_cow))
            prompt = input()

    return prompt


def inform(format_string: str, bulls: int, cows: int) -> None:
    #Информируем о кол-ве быков и коров при помощи случайной коровы
    print(cowsay.cowsay(format_string.format(bulls, cows), cow = random.choice(cowsay.list_cows())))


def bullscows(guess: str, secret: str) -> (int, int):   
    #считаем кол-во быков: пробегаемся по массивам до конца меньшего из них
    #если в очередной позиции нашли совпадение, увеличиваем счетчик
    bulls = 0
    for i in range(min(len(guess), len(secret))):
        if guess[i] == secret[i]:
            bulls += 1

    #считаем кол-во коров: находим мощность пересечения
    #множеств всех символов из guess и secret
    guess_set = set(guess)
    secret_set = set(secret)
    cows = len(guess_set & secret_set)

    return (bulls, cows)


def gameplay(ask: callable, inform: callable, words: list[str]) -> int:
    #загадываем слово и инициализируем счетчик попыток
    secret_word = random.choice(words)
    tries = 1

    #пока догадка игрока не сопадет с загадкой, выводим
    #количество быков и коров, а ткаже увеличиваем счетчик
    while secret_word != (guess_word := ask("Введите слово: ", words)):
        bc = bullscows(guess_word, secret_word)
        inform("Быки: {}, Коровы: {}", bc[0], bc[1])
        tries += 1

    #На последок информируем пользователя о том, что кол-во быков и коров
    #совпадает с длиной слова и возвращаем число попыток
    bc = bullscows(guess_word, secret_word)
    inform("Быки: {}, Коровы: {}", bc[0], bc[1])
    return tries


def main(game_dict: list[str], length: int = 5) -> int:
    #отбираем все слова нужной длины и запускаем игру
    filtered_dict = [word for word in game_dict if len(word) == length]
    return gameplay(ask, inform, filtered_dict)


with open(sys.argv[1], 'r') as f:
    #читаем файл и в зависимости от ввода вызываем main
    #с указанием длины слова или без
    game_dict = (f.read()).split()
    
    match len(sys.argv):
        case 3:
            print(f"Поздравляем! Слово отгадано за {main(game_dict, int(sys.argv[2]))} попыток")
        case 2:
            print(f"Поздравляем! Слово отгадано за {main(game_dict)} попыток")
