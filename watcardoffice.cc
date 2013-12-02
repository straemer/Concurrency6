#include "watcardoffice.h"

#include "bank.h"

#include "MPRNG.h"

using namespace std;

WATCardOffice::WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers) :
    m_printer(prt),
    m_bank(bank),
    m_couriers(numCouriers)
{
    for (vector<Courier*>::iterator it = m_couriers.begin();
         it != m_couriers.end();
         ++it) {
        *it = new Courier(*this);
    }
}

WATCardOffice::~WATCardOffice() {
    for (vector<Courier*>::iterator it = m_couriers.begin();
         it != m_couriers.end();
         ++it) {
        delete *it;
    }
}

WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
    m_jobs.push(new Job(m_bank, sid, amount));
    return m_jobs.back()->result;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard *card) {
    m_jobs.push(new Job(m_bank, sid, amount, card));
    return m_jobs.back()->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    Job *ret = m_jobs.front();
    m_jobs.pop();
    return ret;
}

void WATCardOffice::main() {
    for (;;) {
        _Accept(~WATCardOffice, create, transfer) {
        } or _When(!m_jobs.empty()) _Accept(requestWork) {
        }
    }
}

void WATCardOffice::Courier::main() {
    for (;;) {
        Job *job = m_office.requestWork();
        job->bank.withdraw(job->studentId, job->amount);
        WATCard *card = job->card;
        if (card == NULL) {
            card = new WATCard;
            //TODO: need to delete these guys somewhere.
        }
        card->deposit(job->amount);
        if (g_mprng(6) == 0) {
            job->result.exception(new WATCardOffice::Lost);
            //TODO: need to delete these somewhere...
        } else {
            job->result.delivery(card);
        }
        delete job;
    }
}
