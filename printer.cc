#include "printer.h"
#include <iostream>
#include <sstream>

using namespace std;

Printer::Printer(unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers) :
    numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers) {
    entities = numStudents + numVendingMachines + numCouriers + 5;              // get the number of columns
    cout << "Parent\t" << "WATOff\t" << "Names\t" << "Truck\t" << "Plant\t";
    for (unsigned int i = 0; i < numStudents; i++) {
        cout << "Stud" << i << "\t";                                            // print students
    }
    for (unsigned int i = 0; i < numVendingMachines; i++) {
        cout << "Mach" << i << "\t";                                            // print vending machines
    }
    for (unsigned int i = 0; i < numCouriers; i++) {
        cout << "Cour" << i;                                                    // print couriers
        if (i != numCouriers - 1) {
            cout << "\t";
        }
    }
    cout << endl;
    for (unsigned int i = 0; i < entities; i++) {
        cout << "*******";                                                      // print the stars
        if (i != entities - 1) {
            cout << "\t";
        }
    }
    cout << endl;
}

unsigned int Printer::getPosition(Kind kind, unsigned int lid) {                // method to give column number based off kind and id
    unsigned int position;
    switch (kind) {
    case Student:
        position = 5;                                                       // if student, skip first 5 columns
        break;
    case Vending:
        position = 5 + numStudents;                                         // if vending machine, skip first 5 + students columns
        break;
    case Courier:
        position = 5 + numStudents + numVendingMachines;                    // if courier, skip first 5 + students + vending machines columns
        break;
    default:
        assert(false);
    }
    position += lid;                                                            // add id to go to correct column
    return position;
}

void Printer::print_impl(unsigned int position, std::string state) {            // method that prints using column position and string
    if (state == "F" || p_map.insert(make_pair(position, state)).second == false) {
        unsigned int last = 0;
        for (map<unsigned int, string>::iterator iter = p_map.begin(); iter != p_map.end(); ++iter) {
            for (unsigned int i = last; i < iter->first; i++) {
                cout <<"\t";
            }
            cout << iter->second;
            last = iter->first;
        }
        cout << endl;
        p_map.clear();
    }
    if(state == "F") {
        for (unsigned int i = 0; i < entities; i++) {
            if (i == position) {
                cout << "F";
            }
            else {
                cout << "...";
            }
            if (i != entities - 1) {
                cout << "\t";
            }
        }
        cout << endl;
    }
    else {
        p_map.insert(make_pair(position, state));
    }
}

void Printer::print(Kind kind, char state) {
    unsigned int position = (unsigned int)kind;                                 // change kind to column position
    assert(position<5);
    string s_state = string(1, state);                                          // change char state to string s_state
    print_impl(position, s_state);                                              // call the convenience method to print
}

void Printer::print(Kind kind, char state, int value1) {
    unsigned int position = (unsigned int)kind;
    assert(position<5);
    stringstream sout;
    string s_state;
    sout << state << value1;                                                    // combine the char and int
    s_state = sout.str();                                                       // get the combined string
    print_impl(position, s_state);
}

void Printer::print(Kind kind, char state, int value1, int value2) {
    unsigned int position = (unsigned int)kind;
    assert(position<5);
    stringstream sout;
    string s_state;
    sout << state << value1 << "," << value2;                                   // combine the char and the two ints
    s_state = sout.str();
    print_impl(position, s_state);
}

void Printer::print(Kind kind, unsigned int lid, char state) {
    unsigned int position = getPosition(kind, lid);                             // get the column position using getPosition
    string s_state = string(1, state);
    print_impl(position, s_state);
}

void Printer::print(Kind kind, unsigned int lid, char state, int value1) {
    unsigned int position = getPosition(kind, lid);
    stringstream sout;
    string s_state;
    sout << state << value1;
    s_state = sout.str();
    print_impl(position, s_state);
}

void Printer::print(Kind kind, unsigned int lid, char state, int value1, int value2) {
    unsigned int position = getPosition(kind, lid);
    stringstream sout;
    string s_state;
    sout << state << value1 << "," << value2;
    s_state = sout.str();
    print_impl(position, s_state);
}
