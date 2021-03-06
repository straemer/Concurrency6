#ifndef PRINTER_H__
#define PRINTER_H__

#include <map>

_Monitor Printer {
public:
    enum Kind {
        Parent,
        WATCardOffice,
        NameServer,
        Truck,
        BottlingPlant,
        Student,
        Vending,
        Courier
    };
    Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers);
    void print(Kind kind, char state);
    void print(Kind kind, char state, int value1);
    void print(Kind kind, char state, int value1, int value2);
    void print(Kind kind, unsigned int lid, char state);
    void print(Kind kind, unsigned int lid, char state, int value1);
    void print(Kind kind, unsigned int lid, char state, int value1, int value2);

private:
    std::map<unsigned int, std::string>p_map;
    unsigned int numStudents, numVendingMachines, numCouriers, entities;

    void print_impl(unsigned int position, std::string state);
    unsigned int getPosition(Kind kind, unsigned int lid);
};

#endif
