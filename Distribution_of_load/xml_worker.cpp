#include "xml_worker.h"


using namespace std;

//процедура, которая парсит xml-файл и заполняет 
//переменные основной программы значениями из него
void
make_parsing(char *to_parse, int &res, int &n_proc, int *&proc_load, int &n_prog, int *&prog_load, int **&intensity)
{
    //запускаем парсинг XML файла через libxerces
    try {
        XMLPlatformUtils::Initialize();
    }

    //ловим стандартную ошибку
    catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            cout << "Error during initialization! :\n"
                 << message << "\n";
            XMLString::release(&message);
            res = 1;
            return;
    }

    //создаем сам по себе "парсер" и средство контроля ошибок
    XercesDOMParser* parser = new XercesDOMParser();
    parser->setValidationScheme(XercesDOMParser::Val_Always);
    parser->setDoNamespaces(true);

    ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
    parser->setErrorHandler(errHandler);

    try {
        //запускаем парсинг поданного xml файла
        parser->parse(to_parse);
        DOMDocument* d = parser->adoptDocument();
        //заполняем наши переменные значениями из файла
        
        //получаем массив "процессоров"
        XMLCh *data = XMLString::transcode("processor");
        DOMNodeList* proc = d->getElementsByTagName(data);
        XMLString::release(&data);

        //если массив пуст - ошибка
        n_proc = proc->getLength();
        if (n_proc == 0){
            d->release();
            throw 1;
        }

        //выделяем место под массив значений загрзок процессоров
        proc_load = new int[n_proc];

        //переводим массив из "процессоров" в массив загрузок процессоров
        for (int i = 0; i < n_proc; i++){
            char *cur = XMLString::transcode(proc->item(i)->getTextContent());
            
            //если значение поля не совпадает с допустимыми значениями загрузки процессорв, то ошибка
            if ((strcmp(cur, "50") != 0) && (strcmp(cur, "70") != 0) && (strcmp(cur, "90") != 0) && (strcmp(cur, "100") != 0)){
                //формируем строку для сообщения об ошибке и чистим память
                char *s = new char [strlen(cur) + 2];
                s[0] = 'p';
                memmove(s+1, cur, strlen(cur)+1);
                delete []proc_load;
                d->release();
                XMLString::release(&cur);
                throw s;
            }
            //переводим в число и кладем в массив
            proc_load[i] = atoi(cur);
            XMLString::release(&cur);
        }

        //то же самое для программ
        data = XMLString::transcode("program");
        DOMNodeList* prog = d->getElementsByTagName(data);
        XMLString::release(&data);

        n_prog = prog->getLength();
        if (n_prog == 0){
            delete []proc_load;
            d->release();
            throw 2;
        }

        //std::cout << "n_prog" << n_prog << std::endl;

        prog_load = new int[n_prog];

        for (int i = 0; i < n_prog; i++){
            char *cur = XMLString::transcode(prog->item(i)->getTextContent());
            
            if ((strcmp(cur, "5") != 0) && (strcmp(cur, "10") != 0) && (strcmp(cur, "15") != 0) && (strcmp(cur, "20") != 0)){
                char *s = new char [strlen(cur) + 2];
                s[0] = 'r';
                memmove(s+1, cur, strlen(cur)+1);
                delete []proc_load;
                delete []prog_load;
                d->release();
                XMLString::release(&cur);
                throw s;
            }
            
            prog_load[i] = atoi(cur);
            XMLString::release(&cur);
        }

        //считываем данные ою интенсивности обменов
        data = XMLString::transcode("link");
        DOMNodeList* link = d->getElementsByTagName(data);
        XMLString::release(&data);

        int amount = link->getLength();

        //согласно устройству входного xml-файла, колво записей об обменах
        //должно быть таким (см. Readme)
        if (amount != n_prog*(n_prog-1)/2){
            //читстим память, кидаем ошибку
            delete []proc_load;
            delete []prog_load;
            d->release();
            throw 3;
        }

        //выделели память под массив массивов (см. Readme)
        intensity = new int*[n_prog-1];

        int amount_read = 0;
        for (int i = 0; i < n_prog-1; i++){
            //выделили память под i-ый массив(см. Readme)
            intensity[i] = new int[n_prog-1-i];
            
            for (int j = 0; j < n_prog-1-i; j++){
                char *cur = XMLString::transcode(link->item(amount_read)->getTextContent());
                
                if ((strcmp(cur, "0") != 0) && (strcmp(cur, "10") != 0) && (strcmp(cur, "50") != 0) && (strcmp(cur, "70") != 0) && (strcmp(cur, "100") != 0)){
                    char *s = new char [strlen(cur) + 2];
                    s[0] = 'l';
                    memmove(s+1, cur, strlen(cur)+1);
                    delete []proc_load;
                    delete []prog_load;
                    for (int k = 0; k <= i; k++){
                        delete []intensity[k];
                    }
                
                    delete []intensity;
                    d->release();
                    XMLString::release(&cur);
                    throw s;
                }
                intensity[i][j] = atoi(cur);
                XMLString::release(&cur);
                amount_read++;
            }
        }
        
        d->release();

        //если надо посмотреть что прочитали из файла:

        /*
        cout << "n_proc " << n_proc << endl;
        for (int i = 0; i < n_proc; i++){
            cout << proc_load[i] << endl;
        }

        cout << "n_prog " << n_prog << endl;
        for (int j = 0; j < n_prog; j++){
            cout << prog_load[j] << endl;
        }

        cout << "links" << endl;

        for (int i = 0; i < n_prog-1; i++){
            for (int j = 0; j < n_prog-i-1; j++){
                cout << intensity[i][j] << endl;
            }
        }
        */
    }

    //ловим стандартные ошибки из библиотеки
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "Exception message is: \n" << message << "\n";
        XMLString::release(&message);
        res = -1;
    }

    catch (const DOMException& toCatch) {
        char* message = XMLString::transcode(toCatch.msg);
        cout << "Exception message is: \n" << message << "\n";
        XMLString::release(&message);
        res = -1;;
    }

    //ловим ошибки, связанную с тем, что не удовлетворины
    //условия (см. Readme) оформления входного файла
    catch (int c){
        if (c == 1){
            cout << "error code : " << c << " - no processors in input file" <<endl;
        }

        else if (c == 2){
            cout << "error code : " << c << " - no programs in input file" <<endl;
        }

        else if (c == 3){
            cout << "error code : " << c << " - number of links in input file is not equal to n_prog*(n_prog-1)/2" << endl;
        }

        res = c;
    }

    catch (char *s){       
        if (s[0] == 'p'){
            cout << "error code : " << 3 << " - incorrect processor capacity value: " << (s+1) << endl;
        }

        else if (s[0] == 'r'){
            cout << "error code : " << 3 << " - incorrect program load value: " << (s+1) << endl;
        }

        else if (s[0] == 'l'){
            cout << "error code : " << 3 << " - incorrect link value: " << (s+1) << endl;
        }

        delete []s;
        res = 4;
    }

    catch (...) {
        cout << "Unexpected Exception \n" ;
        res = 4;
    }

    //закрываем все, что относилось к парсингу и выходим
    delete parser;
    delete errHandler;
    XMLPlatformUtils::Terminate();
    return;
}
