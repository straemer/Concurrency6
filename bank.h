#ifndef BANK_H__
#define BANK_H__

#include <vector>

_Monitor Bank {
public:
    Bank(unsigned int numStudents);
    virtual ~Bank();
    void deposit(unsigned int id, unsigned int amount);
    void withdraw(unsigned int id, unsigned int amount);
private:
    friend void testBank(); // Friend for unit tests
    std::vector<unsigned> m_balances;
    uCondition *m_withdrawWaiters;
};

#endif
