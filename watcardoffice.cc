#include "watcardoffice.h"

#include "bank.h"
#include "printer.h"

#include "MPRNG.h"

using namespace std;

WATCardOffice::WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers) :
    m_printer(prt),
    m_bank(bank),
    m_couriers(numCouriers),
    m_terminating(false)
{
    for (unsigned i=0; i<numCouriers; ++i) {
        m_couriers[i] = new Courier(m_printer, *this, i);
    }
}

WATCardOffice::~WATCardOffice() {
    for (vector<Courier*>::iterator it = m_couriers.begin();
         it != m_couriers.end();
         ++it) {
        delete *it;
    }
    while(!m_jobs.empty()) {
        if (!m_jobs.front()->result.available()) {
            m_jobs.front()->result.delivery(NULL);
        }
        m_jobs.front()->result.reset();
        delete m_jobs.front();
        m_jobs.pop();
    }
}

WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
    m_jobs.push(new Job(m_bank, sid, amount));
    m_printer.print(Printer::WATCardOffice, 'C', sid, amount);
    return m_jobs.back()->result;
}

WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard *card) {
    m_jobs.push(new Job(m_bank, sid, amount, card));
    m_printer.print(Printer::WATCardOffice, 'T', sid, amount);
    return m_jobs.back()->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    if (m_terminating) {
        return NULL;
    }
    Job *ret = m_jobs.front();
    m_jobs.pop();
    m_printer.print(Printer::WATCardOffice, 'W');
    return ret;
}

void WATCardOffice::main() {
    m_printer.print(Printer::WATCardOffice, 'S');
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
    m_printer.print(Printer::Courier, m_id, 'S');
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
            if (job->card != NULL) {
                m_printer.print(Printer::Courier, m_id, 't', job->studentId, job->amount);
            }
            job->bank.withdraw(job->studentId, job->amount);
            WATCard *card = job->card;
            if (g_mprng(5) == 0) {
                delete card;
                job->result.exception(new WATCardOffice::Lost);
            } else {
                if (card == NULL) {
                    card = new WATCard;
                } else {
                    m_printer.print(Printer::Courier, m_id, 'T', job->studentId, job->amount);
                }

                card->deposit(job->amount);
                job->result.delivery(card);
            }
            delete job;
        }
    }
}
