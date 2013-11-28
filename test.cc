// Unit test file

#include "watcard.h"

#include <iostream>
#include <vector>
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

void uMain::main() {
    uProcessor p[16] __attribute__((unused));

    testWatCard();
}
