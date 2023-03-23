#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <sstream>

using namespace std;


//сортировка пузырьком: по числу потоков
//для наглядности диаграммы
void
sort_by_p(int *arr_p, double *arr_t, int len){

    int tmp_1;
    double tmp_2;
    for (int i = 0; i < len; i++){
        for (int j = 0; j < len - i; j++){
            if (arr_p[j] > arr_p[j+1]){
                tmp_1 = arr_p[j];
                arr_p[j] = arr_p[j+1];
                arr_p[j+1] = tmp_1;

                tmp_2 = arr_t[j];
                arr_t[j] = arr_t[j+1];
                arr_t[j+1] = tmp_2;
            }
        }
    }
}


//чтение входного файла
void
read_in_file(char *f_name, int *arr_p, double *arr_t)
{
    //открываем. Если не получилось - сообщаем и выходим
    int fd = open(f_name, O_RDONLY);

    if (fd == -1){
        cout << "file error" << endl;
        exit(1);
    }

    //заполняем вектора для хранения колва процессов и времен
    int i = 0;
    while (i < 100){
        double cur_time;
        int cur_parallel;
        if (read(fd, &cur_time, sizeof(double)) == 0){
            break;
        }

        read(fd, &cur_parallel, sizeof(int));
        arr_p[i] = cur_parallel;
        arr_t[i] = cur_time;

        char be_space;
        read(fd, &be_space, sizeof(char));
        i++;
     }

     sort_by_p(arr_p, arr_t, i-1);
     //последним элементом векторов всегда будет -1
     arr_p[i] = -1;
     arr_t[i] = -1;
     close(fd);
     return;
}


//функция отрисовки с помощью библиотеки SFML
void
draw_time_diagram(int *arr_p, double *arr_t)
{
    //создаем окно
    sf::RenderWindow window(sf::VideoMode(1500, 750), "Time Distrubution");

    //определяем макс. время для масштабирования
    int i = 0;
    double max_t = 0;
    while (arr_p[i] != -1){
        if (arr_t[i] > max_t){
            max_t = arr_t[i];
        }
        i++;
    }

    //рисуем оси
    sf::RectangleShape shape_1;
    shape_1.setSize({1500,1});
    shape_1.setPosition({0,710});
    shape_1.setFillColor(sf::Color::Green);
    window.draw(shape_1);

    sf::RectangleShape shape_2;
    shape_2.setSize({1, 750});
    shape_2.setPosition({80, 0});
    shape_2.setFillColor(sf::Color::Green);
    window.draw(shape_2);

    //рисуем столбики для диаграммы

    //установили коэфф. масштабирования
    double width_k = (1300/i);
    double height_k = (680/round(max_t));

    //поставили шрифт надписей
    sf::Font f;
    f.loadFromFile("a_AlternaNr.ttf");

    sf::Text text_0("", f, 20);

    //подписываем оси
    std::ostringstream tt;
    tt << "time";
    text_0.setString(tt.str());
    text_0.setPosition(40,5);
    window.draw(text_0);

    std:: ostringstream pp;
    pp << "threads";
    text_0.setString(pp.str());
    text_0.setPosition(1430, 710);
    window.draw(text_0);

    i = 0;
    //для всех записей из файла формируем столбик
    //подробнее - (см. Readme)
    while (arr_p[i] != -1){
        //создаем прямоугольник(вертикальный) соотв. высоты (время из записи)
        int cur_h = round(arr_t[i]);
        sf::RectangleShape shape_cur;
        shape_cur.setSize({10, -float(cur_h * height_k)});
        shape_cur.setPosition({80 + float((i+1) * width_k), 710});
        shape_cur.setFillColor(sf::Color::Blue);
        window.draw(shape_cur);

        //формируем надпись для числа процессов (под столбиком)
        sf::Text text("", f, 20);

        std::ostringstream cur_p;
        cur_p << arr_p[i];
        text.setString(cur_p.str());
        text.setPosition(80 + float((i+1) * width_k), 720);
        window.draw(text);

        //формируем надпись для времени (над столбиком)
        std::ostringstream cur_t;
        cur_t << arr_t[i];
        text.setString(cur_t.str());
        text.setPosition(60 + float((i+1) * width_k), 680-float(cur_h) * height_k);
        window.draw(text);

        i++;
    }

    window.display();

    //выполянется, пока пользователь не закроет окно
    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            //событие: мы закрываем окно
            if (e.type == sf::Event::Closed)
                window.close();
        }
    }
}



int main(int argc, char **argv)
{
    //если не подали входного файла
    if (argc != 2){
        cout << "wrong argument string" << endl;
        return 1;
    }

    //будем считать, что в файле не более 100 записей,
    //потому что иначе диаграмма получится совсем загроможденной
    int arr_parallels[101];
    double arr_time[101];
    //считываем данные из файла
    read_in_file(argv[1], arr_parallels, arr_time);

    //рисуем
    draw_time_diagram(arr_parallels, arr_time);
    return 0;
}
