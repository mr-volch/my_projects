program
{
    int b;
    read(b);
    for ( ; b < 10; b = b + 1){
        write(b);
    }
    
    read(b);
    if (b > 10){
        goto fin;
    }
    
    else{
        write(13+14, true);
    }
    fin:write(b);
}
