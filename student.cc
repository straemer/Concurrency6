#include "student.h"
#include "printer.h"
#include "nameserver.h"
#include "watcardoffice.h"
#include "MPRNG.h"

Student::Student(Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
                 unsigned int maxPurchases) : prt(prt), nameServer(nameServer), cardOffice(cardOffice), id(id), bottlesPurchased(0) {
    bottlesToPurchase = g_mprng(1, maxPurchases);   // set bottles to purchase between 1 and maxPurchases
    favouriteFlavour = (VendingMachine::Flavours)g_mprng(0, 3);   // set random flavour
    fCard = cardOffice.create(id, 5);   // create a WATCard (returns a FWATCard)
    prt.print(Printer::Student, id, 'S', (unsigned int)favouriteFlavour, bottlesToPurchase);
    machine = nameServer.getMachine(id);    // obtain vending machine
    prt.print(Printer::Student, id, 'V', machine->getId());
}

void Student::main() {
    while (bottlesPurchased < bottlesToPurchase) {
        yield(g_mprng(1, 10));  // yield between 1 and 10
        bool retrying = false;
        VendingMachine::Status status;
        do {
            try {
                status = machine->buy(favouriteFlavour, *fCard());   // attempt to buy and store status
            } catch (WATCardOffice::Lost) {
                fCard = cardOffice.create(id, 5);
                retrying = true;
            }
        } while (retrying);
        switch (status) {
            case VendingMachine::BUY:
                prt.print(Printer::Student, id, 'B', fCard()->getBalance());
                bottlesPurchased++;     // increment bottles purchased
                break;
            case VendingMachine::STOCK:
                machine = nameServer.getMachine(id);     // get new machine
                prt.print(Printer::Student, id, 'V', machine->getId());
                break;
            case VendingMachine::FUNDS:
                fCard = cardOffice.transfer(id, machine->cost() + 5, fCard());  // transfer 5 + cost to watcard
                break;
        }
    }
    prt.print(Printer::Student, id, 'F');
}
