/*демонстрация унарных минусов*/

program
{
    /*унарный минус раз*/
    int abc = 51, b = -6, c; /*abcd = abcd*/
    string x = "abce", y, z = "abcd";
    bool hj = true;
    int i = 0;
    c = (abc + b) * 2;
    /*унарный минус двас*/
    if ((c >= 100) or (-abc == b)){
        read (y);
        write (y);
        write (abc + b + c, c);
        /*break;*/
    }
    else{
        c = 21;
    }
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

    for (i = 1; i < c; i = i + 1){
        write(i);
        break;
    }


    hj = abc < b;
    write(hj);

    abc = b = 7;

    y = "apercot";

    goto hear;
    hear:write(c);
    goto hear;

    write (x);
}
