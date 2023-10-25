#include "time.h"
class client {
    public:
    Time startingTime;
    std::string clientName;
    short unsigned int table;
    client(short unsigned int t,std::string name, std::string timestr) :startingTime(timestr) {
    table = t;
    clientName = name;
    }
    getTable(const client &Client1) {
        return Client1.table;
    }
    friend bool operator== (const client client1, const client &client2) {
        return client1.clientName == client2.clientName;
    }

};
