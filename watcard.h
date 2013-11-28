#ifndef WATCARD_H__
#define WATCARD_H__

#include "noncopyable.h"

#include <uFuture.h>

class WATCard : private Noncopyable {
public:
    WATCard();
    typedef Future_ISM<WATCard *> FWATCard; // future watcard pointer
    void deposit(unsigned int amount);
    void withdraw(unsigned int amount);
    inline unsigned int getBalance() { return m_balance; }
private:
    unsigned m_balance;
};

#endif
