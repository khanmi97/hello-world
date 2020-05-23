#include <iostream>
#include <cstdlib>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"
#include "Tasks.h"

using namespace std;


int Tasks::check(string a)
{
    for (unsigned int i = 0; i < allSymbols.size(); i++)
    {
        if (allSymbols[i].compare(a) == 0)
        {
            return i;
        }
    }
    return 0;
}


void Tasks::storeSymbol()
{
    allSymbols.push_back("#");
    allSymbols.push_back("$");
    Token t = lexer.GetToken();

    struct Rule r;
    int inc = 0;
    while (t.token_type != DOUBLEHASH)
    {
        grammarRule.push_back(Rule());
        while (t.token_type != HASH)
        {
            if (t.token_type != ARROW)
            {
                if (check(t.lexeme) == 0)
                {
                    allSymbols.push_back(t.lexeme);
                    grammarRule[inc].lhs = allSymbols.size()-1;
                }
                else
                {
                    grammarRule[inc].lhs = check(t.lexeme);
                }
                t = lexer.GetToken();
            }
            else
            {
                t = lexer.GetToken();
                if (t.token_type != HASH){

                while (t.token_type != HASH)
                {
                    if (check(t.lexeme) == 0)
                    {
                        allSymbols.push_back(t.lexeme);
                        grammarRule[inc].rhs.push_back(allSymbols.size()-1);
                    }
                    else
                    {
                        grammarRule[inc].rhs.push_back(check(t.lexeme));
                    }
                    t = lexer.GetToken();
                }
                }
                else
                {
                    grammarRule[inc].rhs.push_back(check(t.lexeme));
                }
            }
        }
        inc++;
        t = lexer.GetToken();

        ////////////////////////////////////////////
        //while (t.token_type != HASH || t.token_type != DOUBLEHASH)
        //{
            /*if (t.token_type == ID)
            {
                if (check(t.lexeme) == 0)
                {
                    allSymbols.push_back(t.lexeme);
                    r.lhs = allSymbols.size()-1;
                }
                else
                {
                    r.lhs = check(t.lexeme);
                }
            }
            else if (t.token_type == ARROW)
            {
                t = lexer.GetToken();
                if (t.token_type != HASH)
                {
                    while (t.token_type != HASH)
                    {
                        if (check(t.lexeme) == 0)
                        {
                            allSymbols.push_back(t.lexeme);
                            r.rhs.push_back(allSymbols.size()-1);
                        }
                        else
                        {
                            r.rhs.push_back(check(t.lexeme));
                        }
                        t = lexer.GetToken();
                    }
                    grammarRule.push_back(r);
                }
                else
                {
                    r.rhs.push_back(check("#"));
                    grammarRule.push_back(r);
                }
            }
            t = lexer.GetToken();*/
        //}
    }

/*Remove the rest*/
    /*for (unsigned int i = 0; i < allSymbols.size(); i++)
    {
        cout << allSymbols[i] << " " << endl;
    }

    for (unsigned int j = 0; j < grammarRule.size(); j++)
    {
        cout << allSymbols[grammarRule[j].lhs] << " -> ";
        for (int k = 0; k < grammarRule[j].rhs.size(); k++)
        {
            cout << allSymbols[grammarRule[j].rhs[k]] << " ";
        }
        cout << endl;
    }
    cout << "---------------------------------------------------------------------------" << endl;*/
}

void Tasks::initializeBitVec()
{
    for (unsigned int i = 0; i < allSymbols.size(); i++)
    {
        bitVec.push_back(false);
    }
    bitVec[0] = true;
    for (unsigned int j = 1; j < allSymbols.size(); j++)
    {
        if (checkTVec(allSymbols[j]) == true)
        {
            bitVec[j] = true;
        }
    }
}

bool Tasks::checkNT(int x)
{
    for (unsigned int i = 0; i < grammarRule.size(); i++)
    {
        if (x == grammarRule[i].lhs)
        {
            return true;
        }
    }
    return false;
}

bool Tasks::checkNTVec(string a)
{
    for (unsigned int i = 0; i < nonterminal.size(); i++)
    {
        if (a.compare(nonterminal[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

bool Tasks::checkTVec(string b)
{
    for (unsigned int i = 0; i < terminal.size(); i++)
    {
        if (b.compare(terminal[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

void Tasks::finishNT()
{
    for (unsigned int i = 0; i < grammarRule.size(); i++)
    {
        if (checkNTVec(allSymbols[grammarRule[i].lhs]) == false)
        {
            nonterminal.push_back(allSymbols[grammarRule[i].lhs]);
        }
    }
}

void Tasks::setRules()
{
    for (unsigned int i = 0; i < grammarRule.size(); i++)
    {
        if (checkNTVec(allSymbols[grammarRule[i].lhs]) == false)
        {
            nonterminal.push_back(allSymbols[grammarRule[i].lhs]);
        }
        for (unsigned int j = 0; j < grammarRule[i].rhs.size(); j++)
        {
            if (checkNT(grammarRule[i].rhs[j]) == true)
            {
                if (checkNTVec(allSymbols[grammarRule[i].rhs[j]]) == false)
                    nonterminal.push_back(allSymbols[grammarRule[i].rhs[j]]);
            }
            else
            {
                if (grammarRule[i].rhs[j] != 0) { // not hash
                if (checkTVec(allSymbols[grammarRule[i].rhs[j]]) == false)
                    terminal.push_back(allSymbols[grammarRule[i].rhs[j]]);
                }
            }

        }
    }
    finishNT();
}

void Tasks::printSymbols()
{
    string NT;
    string T;
    for (unsigned int i = 0; i < nonterminal.size(); i++)
    {
        cout << nonterminal[i] << " ";
    }

    for (unsigned int j = 0; j < terminal.size(); j++)
    {
        cout << terminal[j] << " ";
    }

}

void Tasks::generating()
{
    vector <bool> old;
    for (unsigned int i = 0; i < bitVec.size(); i++)
    {
        old.push_back(bitVec[i]);
    }


    for (unsigned int k = 0; k < grammarRule.size(); k++)
    {
        int c = 0;
        for (unsigned int l = 0; l < grammarRule[k].rhs.size(); l++)
        {
            if (bitVec[grammarRule[k].rhs[l]] == true)
            {
                c++;
            }
        }
        if (c == grammarRule[k].rhs.size())
        {
            bitVec[grammarRule[k].lhs] = true;
        }

    }
    if (isChanged(old) == true)
        generating();
    else
        return;
}

bool Tasks::isChanged(vector<bool> temp)
{
    for (unsigned int i = 0; i < temp.size(); i++)
    {
        if (temp[i] != bitVec[i])
        {
            return true;
        }
    }
    return false;
}

bool Tasks::searchR(int m, int index)
{
    int ind = index - 1;
    int item = grammarRule[ind].lhs;
    for (unsigned int i = 0; i < grammarRule.size(); i++)
    {
        if (grammarRule[i].lhs == item)
        {
            for (unsigned int j = 0; j < grammarRule[i].rhs.size(); j++)
            {
                if (grammarRule[i].rhs[j] == m)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Tasks::findReachable(int n)
{
    bool flag = false;
    int i = 0;
    while (allSymbols[grammarRule[i].lhs].compare(nonterminal[0]) == 0)
    {
        for (unsigned int j = 0; j < grammarRule[i].rhs.size(); j++)
        {
            if (grammarRule[i].rhs[j] == n)
            {
                flag = true;
                return flag;
            }

        }
        i++;
    }
    if (flag == false)
    {
        int x = grammarRule[i].lhs;
        while (i < grammarRule.size())
        {
            if (searchR(x, i) == true)
            {
                for (unsigned int j = 0; j < grammarRule[i].rhs.size(); j++)
                {
                    if (grammarRule[i].rhs[j] == n)
                    {
                        return true;
                    }
                }
            }
            i++;
            x = grammarRule[i].lhs;
        }
    }


    return false;
}

void Tasks::reachable()
{

    for ( int i = 0; i < grammarRule.size(); i++)
    {
        if (bitVec[grammarRule[i].lhs] == true)
        {
            if (allSymbols[grammarRule[i].lhs].compare(nonterminal[0]) == 0)
            {
                for (unsigned int j = 0; j < grammarRule[i].rhs.size(); j++)
                {
                    if (bitVec[grammarRule[i].rhs[j]] == false)
                    {
                        grammarRule.erase(grammarRule.begin()+i);
                        i--;
                        break;
                    }
                }
            }
            else
            {
                if (findReachable(grammarRule[i].lhs) == true)
                {
                    for (unsigned int j = 0; j < grammarRule[i].rhs.size(); j++)
                    {
                        if (bitVec[grammarRule[i].rhs[j]] == false)
                        {
                            grammarRule.erase(grammarRule.begin()+i);
                            i--;
                            break;
                        }
                    }
                }
                else
                {
                    grammarRule.erase(grammarRule.begin()+i);
                    i--;
                }
            }
        }
        else
        {
            grammarRule.erase(grammarRule.begin()+i);
            i--;
        }
    }

}

void Tasks::printUseful()
{
    if (allSymbols[grammarRule[0].lhs].compare(nonterminal[0]) == 0)
    {
        for (unsigned int i = 0; i < grammarRule.size(); i++)
        {
            cout << allSymbols[grammarRule[i].lhs] << " -> ";
            for (unsigned int j = 0; j < grammarRule[i].rhs.size(); j++)
            {
                cout << allSymbols[grammarRule[i].rhs[j]] << " ";
            }
            cout << endl;

        }
    }

}

void Tasks::createSet()
{
    setF.push_back("#");
    setF.push_back("$");
    for (unsigned int i = 0; i < terminal.size(); i++)
    {
        setF.push_back(terminal[i]);
    }
}

void Tasks::initialize2d()
{
    for (unsigned int i = 0; i < allSymbols.size(); i++)
    {
        vector<bool> temp;
        for (unsigned int j = 0; j < setF.size(); j++)
        {
            if (allSymbols[i].compare(setF[j]) == 0 && allSymbols[i].compare("$") != 0)
            {
                temp.push_back(true);
            }
            else
            {
                temp.push_back(false);
            }
        }
        universe.push_back(temp);
    }
}

void Tasks::findFirst(int l, int r)
{
    for (unsigned int i = 0; i < universe[r].size(); i++)
    {
        if (universe[r][i] == true)
        {
            for (unsigned int j = 0; j < universe[l].size(); j++)
            {
                if (j == i)
                {
                    universe[l][j] = true;
                }
            }
        }
    }
}

void Tasks::hasEpsilon(int m)
{
    if (universe[grammarRule[m].lhs][0] == true)
    {
        for (unsigned int i = 1; i < grammarRule[m].rhs.size(); i++)
        {
            if (universe[grammarRule[m].rhs[i]][0] == true)
            {
                for (unsigned int j = 0; j < universe[grammarRule[m].rhs[i]].size(); j++)
                {
                        if (universe[grammarRule[m].rhs[i]][j] == true)
                        {
                            universe[grammarRule[m].lhs][j] = true;
                        }
                }
            }
            else
            {
                for (unsigned int j = 0; j < universe[grammarRule[m].rhs[i]].size(); j++)
                {
                    if (universe[grammarRule[m].rhs[i]][j] == true)
                    {
                        universe[grammarRule[m].lhs][j] = true;
                    }
                }
                return;
            }
        }
    }
    else if (universe[grammarRule[m].rhs[0]][0] == true)
    {
        for (unsigned int i = 0; i < grammarRule[m].rhs.size(); i++)
        {
            if (universe[grammarRule[m].rhs[i]][0] == true)
            {
                for (unsigned int j = 0; j < universe[grammarRule[m].rhs[i]].size(); j++)
                {
                    if (universe[grammarRule[m].rhs[i]][j] == true && setF[j].compare("#") != 0)
                    {
                        universe[grammarRule[m].lhs][j] = true;
                    }
                }
            }
            else
            {
                for (unsigned int j = 0; j < universe[grammarRule[m].rhs[i]].size(); j++)
                {
                    if (universe[grammarRule[m].rhs[i]][j] == true)
                    {
                        universe[grammarRule[m].lhs][j] = true;
                    }
                }
                return;
            }
        }
        universe[grammarRule[m].lhs][0] = true;
    }
}

bool Tasks::isChanged2(vector<vector<bool>> t)
{
    for (unsigned int i = 0; i < universe.size(); i++)
    {
        for (unsigned int j = 0; j < universe[i].size(); j++)
        {
            if (universe[i][j] != t[i][j])
            {
                return true;
            }
        }
    }
    return false;
}

void Tasks::calculateFirst()
{
    vector<vector<bool>> old;
    for (unsigned int i = 0; i < universe.size(); i++)
    {
        vector<bool> temp;
        for (unsigned int j = 0; j < setF.size(); j++)
        {
            temp.push_back(universe[i][j]);
        }
        old.push_back(temp);
    }
   for (unsigned int i = 0; i < universe.size(); i++)
   {
       if (checkNTVec(allSymbols[i]) == true)
       {
           for (unsigned int j = 0; j < grammarRule.size(); j++)
           {
               if (grammarRule[j].lhs == i)
               {
                   if (checkTVec(allSymbols[grammarRule[j].rhs[0]]) == true)
                   {
                       for (unsigned int k = 0; k < universe[i].size(); k++)
                       {
                           if (setF[k].compare(allSymbols[grammarRule[j].rhs[0]]) == 0)
                           {
                               universe[i][k] = true;
                           }
                       }
                   }
                   else if (allSymbols[grammarRule[j].rhs[0]].compare("#") == 0)
                   {
                        universe[i][0] = true;
                    }
                   else
                   {
                       if (universe[grammarRule[j].rhs[0]][0] == true)
                       {
                            hasEpsilon(j);
                       }
                       else
                            findFirst(grammarRule[j].lhs, grammarRule[j].rhs[0]);
                   }
               }
           }
       }
   }

   if (isChanged2(old) == true)
   {
       calculateFirst();
   }
   else
    return;
}

void Tasks::printFirst()
{

    for (unsigned int i = 0; i < universe.size(); i++)
    {
        int p = 0;
        if (checkNTVec(allSymbols[i]) == true)
        {
            cout << "FIRST(" << allSymbols[i] << ") = { ";
            for (unsigned int j = 0; j < universe[i].size(); j++)
            {
                if (universe[i][j] == true)
                {
                    if (p == 0)
                    {
                        cout << setF[j];
                        p++;
                    }
                    else
                    {
                        cout << ", " << setF[j];
                        p++;
                    }
                }
            }
            cout << " }" << endl;
        }
    }
}

void Tasks::initializeFollow()
{
    for (unsigned int i = 0; i < allSymbols.size(); i++)
    {
        vector<bool> temp;
        for (unsigned int j = 0; j < setF.size(); j++)
        {
            if (allSymbols[i].compare(nonterminal[0]) == 0 && j == 1)
            {
                temp.push_back(true);
            }
            else
            {
                temp.push_back(false);
            }
        }
        universe2.push_back(temp);
    }
}

void Tasks::firstPass()
{
    for (unsigned int i = 0; i < grammarRule.size(); i++)
    {
        if (allSymbols[grammarRule[i].rhs[0]].compare("#") != 0)
        {
            for (unsigned int j = 0; j < grammarRule[i].rhs.size(); j++)
            {
                if (checkTVec(allSymbols[grammarRule[i].rhs[j]]) == false)
                {
                    if (j+1 < grammarRule[i].rhs.size() && universe[grammarRule[i].rhs[j+1]][0] == true)
                    {
                        int a = j+1;
                        while (a < grammarRule[i].rhs.size() && universe[grammarRule[i].rhs[a]][0] == true)
                        {
                            for (unsigned int k = 0; k < universe[grammarRule[i].rhs[a]].size(); k++)
                            {
                                if (universe[grammarRule[i].rhs[a]][k] == true && setF[k].compare("#") != 0)
                                {
                                    universe2[grammarRule[i].rhs[j]][k] = true;
                                }
                            }
                            a++;
                        }
                        if (a < grammarRule[i].rhs.size())
                        {
                            for (unsigned int k = 0; k < universe[grammarRule[i].rhs[a]].size(); k++)
                            {
                                if (universe[grammarRule[i].rhs[a]][k] == true)
                                {
                                    universe2[grammarRule[i].rhs[j]][k] = true;
                                }
                            }
                        }
                    }
                    else if (j+1 >= grammarRule[i].rhs.size())
                    {
                        break;
                    }
                    else
                    {
                        for (unsigned int k = 0; k < universe[grammarRule[i].rhs[j+1]].size(); k++)
                        {
                            if (universe[grammarRule[i].rhs[j+1]][k] == true)
                            {
                                universe2[grammarRule[i].rhs[j]][k] = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

bool Tasks::isChanged3(vector<vector<bool>> vec)
{
    for (unsigned int i = 0; i < universe2.size(); i++)
    {
        for (unsigned int j = 0; j < universe2[i].size(); j++)
        {
            if (universe2[i][j] != vec[i][j])
            {
                return true;
            }
        }
    }
    return false;
}

void Tasks::calculateFollow()
{
    vector<vector<bool>> old;
    for (unsigned int i = 0; i < universe2.size(); i++)
    {
        vector<bool> temp;
        for (unsigned int j = 0; j < universe2[i].size(); j++)
        {
            temp.push_back(universe2[i][j]);
        }
        old.push_back(temp);
    }

    for (unsigned int i = 0; i < grammarRule.size(); i++)
    {
        if (checkTVec(allSymbols[grammarRule[i].rhs[grammarRule[i].rhs.size()-1]]) == false && allSymbols[grammarRule[i].rhs[grammarRule[i].rhs.size()-1]].compare("#") != 0)
        {
            for (int j = grammarRule[i].rhs.size()-1; j >= 0; j--)
            {
                if (checkTVec(allSymbols[grammarRule[i].rhs[j]]) == false)
                {
                    for (unsigned int k = 0; k < universe2[grammarRule[i].lhs].size(); k++)
                    {
                        if (universe2[grammarRule[i].lhs][k] == true)
                        {
                            universe2[grammarRule[i].rhs[j]][k] = true;
                        }
                    }
                    if (universe[grammarRule[i].rhs[j]][0] == false)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }

    if (isChanged3(old) == true)
    {
        calculateFollow();
    }
    else
        return;
}

void Tasks::printFollow()
{
    for (unsigned int i = 0; i < universe2.size(); i++)
    {
        int p =0;
        if (checkNTVec(allSymbols[i]) == true)
        {
            cout << "FOLLOW(" << allSymbols[i] << ") = { ";
            for (unsigned int j = 0; j < universe2[i].size(); j++)
                {
                    if (universe2[i][j] == true)
                    {
                        if (p == 0)
                        {
                            cout << setF[j];
                            p++;
                        }
                        else
                        {
                            cout << ", " << setF[j];
                            p++;
                        }
                    }
                }
                cout << " }" << endl;
        }
    }
}

int Tasks::counter(int l, int ind)
{
    while (grammarRule[ind].lhs == l)
    {
        ind++;
    }
    return ind;
}

int Tasks::counter2(int l)
{
    int c = 0;
    for (unsigned int i = 0; i < grammarRule.size(); i++)
    {
        if (grammarRule[i].lhs == l)
        {
            c++;
        }
    }
    return c;
}

bool Tasks::intersection1(int l)
{
    if (counter2(l) == 1)
    {
        return true;
    }
    vector<vector<bool>> match;

    for (unsigned int i = 0; i < counter2(l); i++)
    {
        vector<bool> temp;
        for (unsigned int j = 0; j < setF.size(); j++)
        {
            temp.push_back(false);
        }
        match.push_back(temp);
    }

    int t = 0;
    for (unsigned int i = 0; i < grammarRule.size(); i++)
    {
        if (grammarRule[i].lhs == l)
        {
            for (unsigned int j = 0; j < grammarRule[i].rhs.size(); j++)
            {
                if (checkTVec(allSymbols[grammarRule[i].rhs[0]]) == true || allSymbols[grammarRule[i].rhs[0]].compare("#") == 0)
                {
                    for (unsigned int k = 0; k < universe[grammarRule[i].rhs[0]].size(); k++)
                    {
                        if (universe[grammarRule[i].rhs[j]][k] == true)
                            match[t][k] = true;
                    }
                    break;
                }
                else
                {
                    if (universe[grammarRule[i].rhs[j]][0] == true)
                    {
                        for (unsigned int k = 1; k < universe[grammarRule[i].rhs[j]].size(); k++)
                        {
                            if (universe[grammarRule[i].rhs[j]][k] == true)
                                match[t][k] = true;
                        }
                    }
                    else
                    {
                        for (unsigned int k = 0; k < universe[grammarRule[i].rhs[j]].size(); k++)
                        {
                            if (universe[grammarRule[i].rhs[j]][k] == true)
                                match[t][k] = true;
                        }
                        break;
                    }
                    if (j == grammarRule[i].rhs.size()-1)
                    {
                        match[t][0] = true;
                    }
                }
            }
            t++;
        }
    }

        for (unsigned int i = 0; i < setF.size(); i++)
        {
            int c = 0;
            for (unsigned int j = 0; j < match.size(); j++)
            {
                if (match[j][i] == true)
                {
                    c++;
                }
                else
                    break;
            }
            if (c == match.size())
            {
                return false;
            }
        }

    return true;
}

bool Tasks::checkRepeat(vector<int> r, int l)
{
    for (unsigned int i = 0; i < r.size(); i++)
    {
        if (r[i] == l)
        {
            return false;
        }
    }
    return true;
}

bool Tasks::cond1()
{
    unsigned int i = 0;
    bool flag = true;
    vector<int> left;
    left.push_back(grammarRule[0].lhs);
    while (flag == true && i < grammarRule.size())
    {
        if (i == 0)
        {
            flag = intersection1(grammarRule[i].lhs);
        }
        else if (checkRepeat(left, grammarRule[i].lhs) == true)
        {
            flag = intersection1(grammarRule[i].lhs);
            left.push_back(grammarRule[i].lhs);
        }


        i = counter(grammarRule[i].lhs, i);
    }

    return flag;
}

bool Tasks::cond2()
{
    for (unsigned int i = 0; i < grammarRule.size(); i++)
    {
        if (universe[grammarRule[i].lhs][0] == true)
        {
            for (unsigned int j = 0; j < universe[grammarRule[i].lhs].size(); j++)
            {
                if (universe[grammarRule[i].lhs][j] == true && universe2[grammarRule[i].lhs][j] == true)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

void Tasks::printPred()
{
    if (allSymbols[grammarRule[0].lhs].compare(nonterminal[0]) == 0)
    {
        bool c1 = cond1();
        bool c2 = cond2();
        if (c1 == true && c2 == true)
        {
            cout << "YES";
        }
        else
        {
            cout << "NO";
        }
    }
    else
        cout << "NO";
}

