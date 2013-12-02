// Unit test file

#include "watcard.h"
#include "bank.h"
#include "printer.h"
#include "nameserver.h"
#include "vendingmachine.h"
#include "bottlingplant.h"
#include "parent.h"

#include "MPRNG.h"

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

_Task NameServerChecker {
public:
    NameServerChecker(NameServer &nameServer, const vector<VendingMachine*> &vendingMachines,
                      unsigned numStudents) :
        m_nameServer(nameServer),
        m_vendingMachines(vendingMachines),
        m_numStudents(numStudents) {}
private:
    void main() {
        for (size_t i=0; i<m_numStudents; ++i) {
            VendingMachine *machine = m_nameServer.getMachine(i);
            if (machine != m_vendingMachines[(i+1)%m_vendingMachines.size()]) {
                osacquire(cout) << "wrong vending machine! got: " << (void*)machine <<
                    ", expected: " << (void*)m_vendingMachines[(i+1)%m_vendingMachines.size()] <<
                    endl;
                assert(false);
            }
        }
    }
    NameServer &m_nameServer;
    const vector<VendingMachine*> &m_vendingMachines;
    const unsigned m_numStudents;
};

void testNameServer() {
    const unsigned numStudents = 13;
    const unsigned numVendingMachines = 5;
    const unsigned numCouriers = 1;
    Printer printer(numStudents, numVendingMachines, numCouriers);
    NameServer *nameServer = new NameServer(printer, numVendingMachines, numStudents);

    vector<VendingMachine*> vendingMachines(numVendingMachines);
    for (unsigned i=0; i<numVendingMachines; ++i) {
        vendingMachines[i] = new VendingMachine(printer, *nameServer, i, 1, 1);
    }
    {
        NameServerChecker checker(*nameServer, vendingMachines, numStudents);
    }
    delete nameServer;
    for (vector<VendingMachine*>::iterator it = vendingMachines.begin();
         it != vendingMachines.end();
         ++it) {
        delete *it;
    }
}

_Task VendingMachineRestocker {
public:
    VendingMachineRestocker(VendingMachine &machine, volatile bool &done) :
        m_machine(machine),
        m_done(done) {}
private:
    void main() {
        for (unsigned i=0; i<1000; ++i) {
            unsigned *inv = m_machine.inventory();
            for (unsigned i=0; i<4; ++i) {
                inv[i] = 1;
            }
            m_machine.restocked();
        }
        m_done = true;
    }
    VendingMachine &m_machine;
    volatile bool &m_done;
};

_Task VendingMachineBuyer {
public:
    VendingMachineBuyer(VendingMachine &machine, volatile bool &done) :
        m_machine(machine),
        m_done(done) {}
private:
    void main() {
        WATCard card;
        while (!m_done) {
            yield(2);
            card.deposit(1);
            m_machine.buy(VendingMachine::Flavours(g_mprng(3)), card);
        }
    }
    VendingMachine &m_machine;
    volatile bool &m_done;
};

void testVendingMachine() {
    const unsigned numStudents = 1;
    const unsigned numVendingMachines = 1;
    const unsigned numCouriers = 1;
    Printer printer(numStudents, numVendingMachines, numCouriers);
    {
        NameServer nameServer(printer, numVendingMachines, numStudents);
        VendingMachine machine(printer, nameServer, 0, 1, 1);
        {
            volatile bool done = false;
            VendingMachineRestocker restocker(machine, done);
            VendingMachineBuyer buyer(machine, done);
        }
    }
}

void testBottlingPlant() {
    const unsigned numStudents = 1;
    const unsigned numVendingMachines = 5;
    const unsigned numCouriers = 1;
    Printer printer(numStudents, numVendingMachines, numCouriers);
    NameServer *nameServer = new NameServer(printer, numVendingMachines, numStudents);
    vector<VendingMachine*> machines(numVendingMachines);
    const unsigned maxShippedPerFlavour = 3;
    const unsigned maxStockPerFlavour = 2;
    const unsigned timeBetweenShipments = 3;
    {
        BottlingPlant plant(printer, *nameServer, numVendingMachines, maxShippedPerFlavour,
                            maxStockPerFlavour, timeBetweenShipments);

        for (unsigned int i=0; i<numVendingMachines; ++i) {
            machines[i] = new VendingMachine(printer, *nameServer, i, 1, maxStockPerFlavour);
        }
        // I can't think of a better way to let this thing run for a bit of time...
        sleep(1);
    }
    delete nameServer;

    for (vector<VendingMachine*>::iterator it = machines.begin();
         it != machines.end();
         ++it) {
        delete *it;
    }
}

void testParent() {
    const unsigned numStudents = 10;
    const unsigned numVendingMachines = 5;
    const unsigned numCouriers = 1;
    Printer printer(numStudents, numVendingMachines, numCouriers);
    Bank bank(numStudents);
    const unsigned parentalDelay = 3;
    {
        Parent parent(printer, bank, numStudents, parentalDelay);
        sleep(1);
    }
}

void uMain::main() {
    uProcessor p[16] __attribute__((unused));

    testWatCard();
    testBank();
    testNameServer();
    testVendingMachine();
    testBottlingPlant();
    testParent();
}
