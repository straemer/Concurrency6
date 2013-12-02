#include "watcardoffice.h"

#include "bank.h"

#include "MPRNG.h"

using namespace std;

WATCardOffice::WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers) :
    m_printer(prt),
    m_bank(bank),
    m_couriers(numCouriers),
    m_terminating(false)
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
    while(!m_jobs.empty()) {
        try {
            if (m_jobs.front()->result.available()) {
                delete m_jobs.front()->result();
            }
        } catch(Lost&) {
        }
        m_jobs.front()->result.reset();
        delete m_jobs.front();
        m_jobs.pop();
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
    if (m_terminating) {
        return NULL;
    }
    Job *ret = m_jobs.front();
    m_jobs.pop();
    return ret;
}

void WATCardOffice::main() {
    for (;;) {
        _Accept(~WATCardOffice) {
            m_terminating = true;
            for (unsigned i=0; i<m_couriers.size(); ++i) {
                _Accept(requestWork) {
                } _Else {
                    break;
                }
            }
            break;
        } or _Accept(create, transfer) {
        } or _When(!m_jobs.empty()) _Accept(requestWork) {
        }
    }
}

void WATCardOffice::Courier::main() {
    for (;;) {
        _Accept(~Courier) {
            break;
        } _Else {
            Job *job = m_office.requestWork();
            if (!job) {
                _Accept(~Courier){
                    break;
                }
            }
            job->bank.withdraw(job->studentId, job->amount);
            WATCard *card = job->card;
            if (g_mprng(6) == 0) {
                if (job->result.available()) {
                    delete job->result();
                }
                job->result.reset();
                job->result.exception(new WATCardOffice::Lost);
                //TODO: need to delete these somewhere...
            } else {
                if (card == NULL) {
                    card = new WATCard;
                    //TODO: need to delete these guys somewhere.
                }
                card->deposit(job->amount);
                if (job->result.available() && job->result() != card) {
                    delete job->result();
                }
                job->result.reset();
                job->result.delivery(card);
            }
            delete job;
        }
    }
}
