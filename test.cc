// Unit test file

#include "watcard.h"
#include "bank.h"

#include <iostream>
#include <vector>

#include <cmath>
using namespace std;

_Task WatCardTester {
public:
    WatCardTester(WATCard &card) :
        m_card(card) {}
private:
    void main() {
        m_card.deposit(50);
        yield();
        m_card.deposit(50);
        yield();
        m_card.withdraw(50);
        yield();
        m_card.deposit(25);
        yield();
        m_card.withdraw(50);
        yield();
        m_card.withdraw(25);
    }

    WATCard &m_card;
};

void testWatCard() {
    WATCard testCard;
    assert(testCard.getBalance() == 0);
    {
        vector<WatCardTester*> testers;
        testers.reserve(32);
        for (unsigned i=0; i<32; ++i) {
            testers.push_back(new WatCardTester(testCard));
        }
        for (vector<WatCardTester*>::iterator it=testers.begin();
             it != testers.end();
             ++it) {
            delete *it;
        }
    }
    assert(testCard.getBalance() == 0);
}

_Task BankWithdrawer {
public:
    BankWithdrawer(Bank &bank, unsigned numStudents, unsigned offset) :
        m_bank(bank),
        m_numStudents(numStudents),
        m_offset(offset) {}
private:
    void main() {
        for (unsigned i=0; i<m_numStudents*100; ++i) {
            unsigned offsetI = (i + m_offset) % (m_numStudents*100);
            m_bank.withdraw(offsetI%m_numStudents, (unsigned)(100*abs(sin(offsetI))));
        }
    }
    Bank &m_bank;
    unsigned m_numStudents;
    unsigned m_offset;
};

_Task BankDepositer {
public:
    BankDepositer(Bank &bank, unsigned numStudents, unsigned offset) :
        m_bank(bank),
        m_numStudents(numStudents),
        m_offset(offset) {}
private:
    void main() {
        for (unsigned i=0; i<m_numStudents*100; ++i) {
            unsigned offsetI = (i + m_offset) % (m_numStudents*100);
            m_bank.deposit(offsetI%m_numStudents, (unsigned)(100*abs(sin(offsetI))));
        }
    }
    Bank &m_bank;
    unsigned m_numStudents;
    unsigned m_offset;
};

void testBank() {
    const unsigned numStudents = 5;
    Bank testBank(numStudents);
    vector<BankWithdrawer*> withdrawTesters;
    vector<BankDepositer*> depositTesters;
    withdrawTesters.reserve(32);
    depositTesters.reserve(32);
    for (unsigned i=0; i<32; ++i) {
        withdrawTesters.push_back(new BankWithdrawer(testBank, numStudents, i));
    }
    for (unsigned i=0; i<32; ++i) {
        depositTesters.push_back(new BankDepositer(testBank, numStudents, i+32));
    }
    for (vector<BankWithdrawer*>::iterator it=withdrawTesters.begin();
         it != withdrawTesters.end();
         ++it) {
        delete *it;
    }
    for (vector<BankDepositer*>::iterator it=depositTesters.begin();
         it != depositTesters.end();
         ++it) {
        delete *it;
    }
    for (unsigned i=0; i<numStudents; ++i) {
        assert(testBank.m_balances[i] == 0);
    }
}

void uMain::main() {
    uProcessor p[16] __attribute__((unused));

    testWatCard();
    testBank();
}
