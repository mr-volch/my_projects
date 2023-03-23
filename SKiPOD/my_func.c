#include "my_func.h"

//функция, задающая функцию для которой вычисляем интеграл
double
my_func(double x)
{
    return exp(sin(exp(-x))+cos(exp(x)));
}
