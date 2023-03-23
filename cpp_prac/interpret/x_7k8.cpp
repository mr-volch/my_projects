program
{
    int abc = 51, b = -6, c; /*abcd = abcd*/
    string x = "abce", y, z = "abcd";
    /*инициализируем булевскую переменную строковой контсантой*/
    bool hj = false;
    int i = 0;
    c = (abc + b) * 2;
    if ((c >= 100) or (-abc == b)){
        read (y);
        write (y);
        write (abc + b + c, c);
        /*break;*/
    }
    else{
        c = 21;
    }
    
    /*в while нелогическое выражение*/
    while ((c > 100) and (b < 7)){
        c = c - 5 / b;
        write (c);
        /*x = x + "";*/
        if (c != 80){
            write(true);
            break;
        }

        else{
            write(false);
        }
    }

    /*демонстрация for-ов*/
    for (i = 1; i < c; i = i + 1){
        write(i);
        break;
    }
    
    for (i < 1; i < c; "string"){}
    
    for ((1+3*i); hj and true; "string"){}


    hj = abc < b;
    /*демонстрация write*/
    write(hj, b, x);

    abc = b = 7;

    y = "apercot";

    /*метка нигде не описана*/
    goto hear;

    write (x);
}
