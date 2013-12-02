#ifndef WATCARD_OFFICE_H__
#define WATCARD_OFFICE_H__

#include "watcard.h"

#include <queue>
#include <vector>

_Monitor Printer;
_Monitor Bank;

_Task WATCardOffice {
private:
    struct Job {
        WATCard::FWATCard result;
        Bank &bank;
        unsigned studentId;
        unsigned amount;
        WATCard *card;
        Job(Bank &bank, unsigned studentId, unsigned amount, WATCard *card = NULL) :
            bank(bank),
            studentId(studentId),
            amount(amount),
            card(card) {}
    };
public:
    _Event Lost {};
    WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers);
    virtual ~WATCardOffice();
    WATCard::FWATCard create(unsigned int sid, unsigned int amount);
    WATCard::FWATCard transfer(unsigned int sid, unsigned int amount, WATCard *card);
    Job *requestWork();
private:
    _Task Courier {
    public:
        Courier(WATCardOffice &office) : m_office(office) {}
    private:
        void main();

        WATCardOffice &m_office;
    };                 // communicates with bank

    void main();

    std::queue<Job*> m_jobs;
    Printer &m_printer;
    Bank &m_bank;
    std::vector<Courier*>m_couriers;
    bool m_terminating;
};

#endif
