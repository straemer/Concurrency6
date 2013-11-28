#include "watcard.h"

WATCard::WATCard() :
    m_balance(0) {}

void WATCard::deposit(unsigned int amount) {
    uFetchAdd(m_balance, amount);
}

void WATCard::withdraw(unsigned int amount) {
    assert(amount < m_balance);
    uFetchAdd(m_balance, -amount);
}
