#include "event.h"
#include "time.h"
#include <vector>
class Event {
    public:
    short unsigned int incID;
    std::string clientName;
    short unsigned int outID;
    std::string errMSG;
    Time eventTime;
    Event(std::vector <std::string> vector)
    : eventTime(vector[0]) {
        
        incID = stoi(vector[1]);
        clientName = vector[2];
        
        errMSG="";
        
        auto size = vector.size();
        if (size>3) {
            outID = stoi(vector[3]);
            
        }

    }
    int checkOpeningTime(int openMinutes,int givenMinutes, Event myEvent) {
        
    if (openMinutes>givenMinutes) { // если минут с 00:00 до 09:00 прошло больше, чем когда зашёл клиент
        //myEvent.setError(myEvent,"NotOpenYet");
        myEvent.errMSG="NotOpenYet";
        myEvent.incID=13;
        myEvent.print(myEvent); // выводим
        return 0;
    }
    else {
        return 1;
    }
}
/*void setError(Event myEvent, std::stirng error) {
    myEvent.incID=13;
    myEvent.errMSG=error;
}*/
void print(Event ev) {
    std::cout<< ev.eventTime.timeInStr <<" "<< ev.incID <<" "<< ev.errMSG<<std::endl;
}
void printLeave(Event ev) {
    std::cout<< ev.eventTime.timeInStr <<" "<< ev.incID <<" "<< ev.clientName<<std::endl;
}
void printTwelve(Event ev) {
    std::cout<<ev.eventTime.timeInStr<<" "<< ev.incID <<" ";
}
};