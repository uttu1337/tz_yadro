#include <string>
class Time // класс Time
{
    public:
    int timeInMinutes;
    std::string timeInStr;
    Time(std::string str) { // перевод строки формата XX:XX в секунды (т. отсчёта - 00:00)
    auto colonpos=str.find(":");
    timeInStr=str;
    
        if (colonpos!=NULL) { // ВЫЯСНИТЬ: ЧТО МОЖНО ВСТАВИТЬ ВМЕСТО NULL
            int hours = stoi(str.substr(0,colonpos)); // делим строку на 2 подстроки, 1 - часы, 2 - минуты

            int minutes = stoi(str.substr(colonpos+1));
            timeInMinutes=hours*60+minutes; // вычисляем время в минутах от начала отсчёта
        }
    }
};
