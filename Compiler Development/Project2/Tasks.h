// Implement all tasks of the project

#ifndef __TASKS__H__
#define __TASKS__H__

#include <vector>
#include <string>

#include "inputbuf.h"
#include "lexer.h"

using namespace std;

class Tasks
{
    private:
        LexicalAnalyzer lexer;
        vector<string> allSymbols;
        struct Rule {
            int lhs;
            vector<int> rhs;
        };
        vector<Rule> grammarRule;
        vector<bool> bitVec;
        vector<string> nonterminal;
        vector<string> terminal;
        vector<string> setF;
        vector<vector<bool>> universe;
        vector<vector<bool>> universe2;


    public:
        void storeSymbol();
        int check(string a);
        void initializeBitVec();
        bool checkNT(int x);
        bool checkNTVec(string a);
        bool checkTVec(string b);
        void finishNT();
        void setRules();
        void printSymbols();
        void generating();
        bool isChanged(vector<bool> temp);
        bool searchR(int m, int index);
        bool findReachable(int n);
        void reachable();
        void printUseful();
        void createSet();
        void initialize2d();
        void findFirst(int l, int r);
        void hasEpsilon(int m);
        bool isChanged2(vector<vector<bool>> t);
        void calculateFirst();
        void printFirst();
        void initializeFollow();
        void firstPass();
        bool isChanged3(vector<vector<bool>> vec);
        void calculateFollow();
        void printFollow();
        int counter(int l, int ind);
        int counter2(int l);
        bool intersection1(int l);
        bool checkRepeat(vector<int> r, int l);
        bool cond1();
        bool cond2();
        void printPred();


};

#endif  //__TASKS__H__
