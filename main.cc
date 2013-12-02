#include "printer.h"
#include "nameserver.h"
#include "bank.h"
#include "watcardoffice.h"
#include "vendingmachine.h"
#include "bottlingplant.h"
#include "parent.h"
#include "student.h"

#include "config.h"
#include "MPRNG.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

void usage(const char *name) {
    cout << "Usage: " << name << " [ config-file [ random-seed (> 0) ] ]" << endl;
    exit(1);
}

void uMain::main() {
    string configFile = "soda.config";
    int seed = getpid();
    switch (argc) {
    case 3:
        seed = atoi(argv[2]);
    case 2:
        configFile = argv[1];
    case 1:
        break;
    default:
        usage(argv[0]);
    };
    if (seed<=0) {
        usage(argv[0]);
    }

    g_mprng.seed(seed);

    ConfigParms config;
    processConfigFile(configFile.c_str(), config);

    Printer printer(config.numStudents, config.numVendingMachines, config.numCouriers);
    NameServer nameServer(printer, config.numVendingMachines, config.numStudents);
    Bank bank(config.numStudents);
    WATCardOffice office(printer, bank, config.numCouriers);
    vector<VendingMachine*> vendingMachines(config.numVendingMachines);
    for (unsigned i=0; i<config.numVendingMachines; ++i) {
        vendingMachines[i] = new VendingMachine(printer, nameServer, i, config.sodaCost,
                                                config.maxStockPerFlavour);
    }
    {
        BottlingPlant plant(printer, nameServer, config.numVendingMachines, config.maxShippedPerFlavour,
                            config.maxStockPerFlavour, config.timeBetweenShipments);
        Parent parent(printer, bank, config.numStudents, config.parentalDelay);
        vector<Student*> students(config.numStudents);
        for (unsigned i=0; i<config.numStudents; ++i) {
            students[i] = new Student(printer, nameServer, office, i, config.maxPurchases);
        }
        for (vector<Student*>::iterator it = students.begin();
             it != students.end();
             ++it) {
            delete *it;
        }
    }
    for (vector<VendingMachine*>::iterator it = vendingMachines.begin();
         it != vendingMachines.end();
         ++it) {
        delete *it;
    }
}
