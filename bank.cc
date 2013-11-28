#include "bank.h"

Bank::Bank(unsigned int numStudents) :
    m_balances(numStudents, 0),
    m_withdrawWaiters(new uCondition[numStudents]) {}

Bank::~Bank() {
    delete [] m_withdrawWaiters;
}

void Bank::deposit(unsigned int id, unsigned int amount) {
    m_balances[id] += amount;
    m_withdrawWaiters[id].signal();
}

void Bank::withdraw(unsigned int id, unsigned int amount) {
    if (!m_withdrawWaiters[id].empty()) {
        m_withdrawWaiters[id].wait();
    }
    while (amount > m_balances[id]) {
        m_withdrawWaiters[id].wait();
    }
    assert(m_balances[id] >= amount);
    m_balances[id] -= amount;

    // Allow anyone that was waiting on this withdrawal to go now.
    m_withdrawWaiters[id].signal();
}
