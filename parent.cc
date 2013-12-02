#include "parent.h"

#include "bank.h"
#include "printer.h"

#include "MPRNG.h"

Parent::Parent(Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay) :
    m_printer(prt),
    m_bank(bank),
    m_numStudents(numStudents),
    m_delay(parentalDelay) {}

void Parent::main() {
    m_printer.print(Printer::Parent, 'S');
    for (;;) {
        _Accept(~Parent) {
            break;
        } _Else {
            yield(m_delay);

            unsigned giftAmount = g_mprng(1,3);
            unsigned giftee = g_mprng(m_numStudents-1);

            m_printer.print(Printer::Parent, 'D', giftee, giftAmount);

            m_bank.deposit(giftee, giftAmount);
        }
    }
    m_printer.print(Printer::Parent, 'F');
}
