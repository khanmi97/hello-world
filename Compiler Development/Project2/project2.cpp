/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "lexer.h"
#include "inputbuf.h"
#include "Tasks.h"



using namespace std;


int main (int argc, char* argv[])
{
    Tasks t;
    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);

    // TODO: Read the input grammar at this point from standard input

    /*
       Hint: You can modify and use the lexer from previous project
       to read the input. Note that there are only 4 token types needed
       for reading the input in this project.

       WARNING: You will need to modify lexer.cc and lexer.h to only
       support the tokens needed for this project if you are going to
       use the lexer.
     */

    switch (task) {
        case 1:
            t.storeSymbol();
            t.setRules();
            t.printSymbols();
            break;

        case 2:
            t.storeSymbol();
            t.setRules();
            t.initializeBitVec();
            t.generating();
            t.reachable();
            t.printUseful();
            break;

        case 3:
            t.storeSymbol();
            t.setRules();
            t.createSet();
            t.initialize2d();
            t.calculateFirst();
            t.printFirst();
            break;

        case 4:
            t.storeSymbol();
            t.setRules();
            t.createSet();
            t.initialize2d();
            t.calculateFirst();
            t.initializeFollow();
            t.firstPass();
            t.calculateFollow();
            t.printFollow();
            break;

        case 5:
            t.storeSymbol();
            t.setRules();
            t.initializeBitVec();
            t.generating();
            t.reachable();
            t.createSet();
            t.initialize2d();
            t.calculateFirst();
            t.initializeFollow();
            t.firstPass();
            t.calculateFollow();
            t.printPred();
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

