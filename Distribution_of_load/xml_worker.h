#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <iostream>
#include <string>




using namespace xercesc;

//функция, которая заполняет все свои аргументы начиная со второго
//значениями из файла d соответсвующим образом (см. Readme)
void make_parsing(char *to_parse, int &res, int &n_proc, int *&proc_load, int &n_prog, int *&prog_load, int **&intensity);
