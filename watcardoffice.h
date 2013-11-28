#ifndef WATCARD_OFFICE_H__
#define WATCARD_OFFICE_H__

_Task WATCardOffice {
public:
    _Event Lost {};                        // uC++ exception type, like "struct"
    WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers);
    FWATCard create(unsigned int sid, unsigned int amount);
    FWATCard transfer(unsigned int sid, unsigned int amount, WATCard *card);
    Job *requestWork();
private:
    struct Job {                           // marshalled arguments and return future
        Args args;                         // call arguments (YOU DEFINE "Args")
        FWATCard result;                   // return future
        Job(Args args) : args(args) {}
    };
    _Task Courier { ... };                 // communicates with bank

    void main();
};

#endif
