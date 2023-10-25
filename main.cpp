/* tz for yadro made by emil garaishin hire me please T_T */
//КК - компьютерный клуб
#include <iostream> // подключение библиотек
#include <fstream>
#include <string>
#include "event.cpp" 
#include <vector>
#include <queue>
#include <algorithm> // для find()
#include "client.cpp"

    int tableNum, hourPrice; // количество столов и цена за час
    void cyberClubStart(); // функция инициализации
    std::vector<std::string> currentEvent; // вектор, в котором хранится информация о текущем событии
    std::string buf; //буфер
    int i;
    int *tableTime;
    int waitingNum;
    int numLine;
    bool *busyTables; //указатель для дин. массива
    short unsigned int tNum; //я уже не помню зачем мне она нужна
    std::ifstream fileToRead; //открываем файл для чтения
    std::string sa; // строка с которой работаем
    std::vector<std::string> clients; //имена клиентов, не состоящих в очереди
    std::vector<client*> currentClients; // содержит указатели на наших клиентов
    std::queue<std::string> waitingClients; // очередь, собственно, для очереди клиентов
    Time* openingTime; // указатели на объекты класса нужны чтоб не вызывать конструктор
    Time* closingTime;
    client* myClient;
    int *moneyPerTable; // для расчётов денег

void cyberClubStart(std::ifstream &myFile) { // функция инициализации
    numLine=0;
    //myFile.open("C:/Users/uttu/.vscode/stuff/yadro/test_file.txt"); // открываем файл
    
    if (myFile.is_open() == true) { // успешно открылся
        getline(myFile,sa); // здесь получаем и преобразовываем в int первую строку - кол-во столов
        numLine+=1;
        try {
        tableNum = stoi(sa); 
        }
        catch (std::exception &err) {
            std::cout<<numLine<<std::endl;
            std::exit(-1);
        }
        moneyPerTable = new int [tableNum];
        for (i=0; i<tableNum;i++) {
                moneyPerTable[i]=0;
            }
        busyTables = new bool[tableNum]; // создаем динамический массив bool, в котором хранятся
                                               // 2 состояния кол-ва столов в КК - свободен/занят
            for (i=0; i<tableNum;i++) {
                busyTables[i]=false;
            }
        tableTime = new int[tableNum];
        for (i=0; i<tableNum;i++) {
                tableTime[i]=0;
            }
        getline(myFile,sa); // получаем вторую строку - часы работы
        numLine+=1;
        auto pos=sa.find(' ');
        
        if (pos!=NULL) { 
            try {
            openingTime = new Time(sa.substr(0,pos)); // делим строку на 2 подстроки, куда записываем часы работы
            closingTime= new Time(sa.substr(pos+1));
            
            }
            catch (std::exception &err) {
            std::cout<<numLine<<std::endl;
            std::exit(-1);
        }
            std::cout<<openingTime->timeInStr<<std::endl;
            

        }
        
        getline(myFile,sa); // получаем и преобразовываем третью строку - часовая оплата
        numLine+=1;
        try {
        hourPrice = stoi(sa);
        }
        catch (std::exception &err) {
            std::cout<<numLine<<std::endl;
            std::exit(-1);
        }
        
    }
    else {
        std::cout<<"Error while opening file!"; // иначе выводим ошибку
        exit(-1);
    }
}
void check_event(std::ifstream &myFile){ //функция проверки события
    getline(myFile,sa); // получаем строку
    std::cout<<sa<<std::endl;
    currentEvent.clear(); // не забываем очистить вектор
    buf=""; //буфер
    for (i=0;i<sa.size()+1;i++) { // size+1, чтобы также читало последний символ строки
        if (sa[i]!=' ' && sa[i]!='\0') { // \0 - символ конца строки
            buf+=sa[i]; // добавляем в буфер
        }
        else { // если слово кончилось - засовываем его в вектор, а буфер обнуляем
            currentEvent.push_back(buf);
            buf="";
        }
        
        
    }
    Event myEvent(currentEvent); // создаем экземпляр класса Event
    bool a;
    a=myEvent.checkOpeningTime(openingTime->timeInMinutes,myEvent.eventTime.timeInMinutes,myEvent); // проверяем - открыт ли клуб
    if (a==true){ // если КК открыт
    switch (myEvent.incID) {
        case 1: { // если клиент заходит
            if (not (find(clients.begin(),clients.end(),myEvent.clientName)!=clients.end())) { // если до этого он не зашёл
        clients.push_back(myEvent.clientName); //заносим его в вектор клиентов
        }
        else { //иначе (относится к if not), пишем, что он не может войти
        myEvent.incID=13;
        myEvent.errMSG="YouShallNotPass";
        myEvent.print(myEvent); // выводим строку с исходящим событием
        }
        break;
        }
        case 2: {//клиент занимает стол
        tNum = stoi(currentEvent[3]); // номер стола, который пытаемся занять
        if (busyTables[tNum-1] == false) {
            if (not (find(clients.begin(),clients.end(),myEvent.clientName)!=clients.end())) {
                myEvent.incID=13; //если клиент, которого нет в КК пытается занять стол
                myEvent.errMSG="ClentUnknown";
                myEvent.print(myEvent);
            }

        busyTables[tNum-1]=true;
        myClient = new client(tNum,myEvent.clientName,myEvent.eventTime.timeInStr);
        currentClients.push_back(myClient);
        }
        else {
            myEvent.incID=13; // если стол занят
            myEvent.errMSG="PlaceIsBusy";
            myEvent.print(myEvent);

        }
        break;
        }
        case 3: {// клиент ожидает
        bool flagAvailable = false;
        for (i=0;i<tableNum;i++) {
            if (busyTables[i]==false){
                flagAvailable = true;
                myEvent.incID=13; //если клиент, которого нет в КК пытается занять стол
                myEvent.errMSG="ICanWaitNoLonger!";
                myEvent.print(myEvent);
                break;
            }
        }
        if (flagAvailable==false) {
            waitingNum+=1;
            waitingClients.push(myEvent.clientName);
            if (waitingNum>tableNum) {
                myEvent.incID=11;
                myEvent.printLeave(myEvent);
                
            }
        }
        break;                                               
        }
        case 4: {
            
            if (not (find(clients.begin(),clients.end(),myEvent.clientName)!=clients.end())) {
                myEvent.incID=13; //если клиент, которого самтго по себе нет в КК
                myEvent.errMSG="ClentUnknown";
                myEvent.print(myEvent);
            }
            else {
                
                short unsigned int removeTable;
                //памятник моей тупости. я писал этот код около 4-6 часов. всё ради
                for (i=0;i<currentClients.size();i++) { // этой
                    if (myEvent.clientName==currentClients[i]->clientName) { // этой
                        moneyPerTable[(currentClients[i]->table)-1] +=
                        (myEvent.eventTime.timeInMinutes - currentClients[i]->startingTime.timeInMinutes)/60*10;
                        if ((myEvent.eventTime.timeInMinutes - currentClients[i]->startingTime.timeInMinutes)%60 > 0) {
                            moneyPerTable[(currentClients[i]->table)-1]+=10;
                        }
                        tableTime[(currentClients[i]->table)-1]+=
                        myEvent.eventTime.timeInMinutes - currentClients[i]->startingTime.timeInMinutes;
                        
                        currentClients.erase(currentClients.begin()+i); // и этой строчек
                        
                    }
                }
                if (not (waitingClients.empty())) {
                    myEvent.incID=12;
                myClient= new client(removeTable+1,waitingClients.front(),myEvent.eventTime.timeInStr);
                waitingClients.pop();
                currentClients.push_back(myClient);
                
                
                myEvent.printTwelve(myEvent);
                std::cout<<myClient->clientName<<" "<<myClient->table<<std::endl;
                
                
                
                }
            }
            
            break;
        }
    }
    }
}
int main(int argc, char** argv) {
    std::ifstream fileToRead(argv[1]);
    //fileToRead.open("C:/Users/uttu/.vscode/stuff/yadro/test_file.txt"); // открываем файл
    
    cyberClubStart(fileToRead); //инициализируем работу
    while(fileToRead.good()) { // до концца файла проверяем на события
    numLine+=1;
    try{
        check_event(fileToRead);
    }
    catch (std::exception &err) {
            std::cout<<numLine<<std::endl;
            std::exit(-1);
        }
        
    }
    if (not (currentClients.empty())) { // если на конец рабочего дня в КК остались люди
        std::sort(currentClients.begin(),currentClients.end(),[](const auto& lhs, const auto& rhs) {
            return lhs->clientName > rhs->clientName; // сортируем
        });
        for (i=0;i<currentClients.size();i++) {
        std::cout<<closingTime->timeInStr<<" "<<11<<" "<<currentClients[i]->clientName<<std::endl;
        for (i=0;i<currentClients.size();i++) {
            moneyPerTable[(currentClients[i]->table)-1] +=
            (closingTime->timeInMinutes - currentClients[i]->startingTime.timeInMinutes)/60*10;
            if ((closingTime->timeInMinutes - currentClients[i]->startingTime.timeInMinutes)%60 > 0) {
                moneyPerTable[(currentClients[i]->table)-1]+=10;
                }
                tableTime[(currentClients[i]->table)-1]+=
                closingTime->timeInMinutes - currentClients[i]->startingTime.timeInMinutes;
            currentClients.erase(currentClients.begin()+i);
                        
                    
                } 
        }
        
    }
    
    std::cout<<closingTime->timeInStr<<std::endl;
    for (i=0;i<tableNum;i++) {
        std::cout<<i+1<<" "<<moneyPerTable[i]<<" ";
        if (tableTime[i]<600) {
            std::cout<<0;
        }
        std::cout<<std::to_string(tableTime[i]/60)<<":";
        if ((tableTime[i]%60)<10) {
            std::cout<<0;
        }
        std::cout<<std::to_string(tableTime[i]%60);
        std::cout<<std::endl;
        


    }
    return 0;
    }