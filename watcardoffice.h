#ifndef WATCARD_OFFICE_H__
#define WATCARD_OFFICE_H__

#include "watcard.h"

_Monitor Printer;
_Monitor Bank;

_Task WATCardOffice {
private:
    struct Job {                           // marshalled arguments and return future
        WATCard::FWATCard result;                   // return future
        Job() {}
    };
public:
    _Event Lost {};                        // uC++ exception type, like "struct"
    WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers);
    WATCard::FWATCard create(unsigned int sid, unsigned int amount);
    WATCard::FWATCard transfer(unsigned int sid, unsigned int amount, WATCard *card);
    Job *requestWork();
private:
    _Task Courier {};                 // communicates with bank

    void main();
};

#endif
