#ifndef STUDENT_H__
#define STUDENT_H__

#include "watcard.h"
#include "vendingmachine.h"

_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;
_Task VendingMachine;

_Task Student {
public:
    Student(Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
            unsigned int maxPurchases);

    Printer &prt;
    NameServer &nameServer;
    WATCardOffice &cardOffice;
    WATCard::FWATCard fCard;
    VendingMachine *machine;
    unsigned int id;
    unsigned int bottlesPurchased;
    unsigned int bottlesToPurchase;
    VendingMachine::Flavours favouriteFlavour;
private:
    void main();
};

#endif
