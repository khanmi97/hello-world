/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "REAL", "INT", "BOOLEAN", "STRING",
    "WHILE", "TRUE", "FALSE", "COMMA", "COLON", "SEMICOLON",
    "LBRACE", "RBRACE", "LPAREN", "RPAREN",
    "EQUAL", "PLUS", "MINUS", "MULT", "DIV","AND", "OR", "XOR", "NOT",
    "GREATER", "GTEQ", "LESS", "LTEQ", "NOTEQUAL",
    "ID", "NUM", "REALNUM", "STRING_CONSTANT", "ERROR"
};

#define KEYWORDS_COUNT 7
string keyword[] = {
    "REAL", "INT", "BOOLEAN", "STRING",
    "WHILE", "TRUE", "FALSE"
};

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

int LexicalAnalyzer::get_line_no() {
    return line_no;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanNumber()
{
    char c;

    input.GetChar(c);
    if (isdigit(c)) {
        if (c == '0') {
            tmp.lexeme = "0";
        } else {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(c)) {
                tmp.lexeme += c;
                input.GetChar(c);
            }
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }
        }
        input.GetChar(c);
        if (c == '.') {           // possibly REALNUM
            input.GetChar(c);
            if (isdigit(c)) {     // definitely REALNUM
                tmp.lexeme += '.';
                while (isdigit(c)) {
                    tmp.lexeme += c;
                    input.GetChar(c);
                }
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = REALNUM;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                input.UngetChar('.');
                tmp.token_type = NUM;
            }
        } else {
            if (!input.EndOfInput())
                input.UngetChar(c);
            tmp.token_type = NUM;
        }
        return tmp;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.token_type = ERROR;
        return tmp;
    }
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;

    input.GetChar(c);
    if (isalpha(c)) {
        tmp.lexeme = "";
      //  cout<<"hello1";
        while (!input.EndOfInput() && isalnum(c)) {
            //cout<<" . "<<c<< " ";
            tmp.lexeme += c;
      //      cout<<" .1 "<<c<< " ";
            input.GetChar(c);
          //  cout<<" .2 "<<c<< " ";
          //  cout<<c<< " ";
        }
    //    cout<<"hello2";
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
      //  cout<<"hello";
        tmp.token_type = ID;
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);

    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

Token LexicalAnalyzer::ScanStringCons()
{
    char c;
    input.GetChar(c);
    string lexeme = "";

    if (c == '"') {
        tmp.lexeme = "";
        //tmp.lexeme += '"';
        input.GetChar(c);
        while (!input.EndOfInput() && isalnum(c)) {
            lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            //input.GetChar(c);
            if (c == '"') {
                //lexeme += c;
                tmp.lexeme += lexeme;
                tmp.token_type = STRING_CONSTANT;
            }
            else{
                tmp.lexeme = "";
                tmp.token_type = ERROR;
            }

        }
        else{
            tmp.lexeme = "";
            tmp.token_type = ERROR;
        }

        tmp.line_no = line_no;

    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}
// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case ',': tmp.token_type = COMMA;       return tmp;
        case ':': tmp.token_type = COLON;       return tmp;
        case ';': tmp.token_type = SEMICOLON;   return tmp;
        case '{': tmp.token_type = LBRACE;      return tmp;
        case '}': tmp.token_type = RBRACE;      return tmp;
        case '(': tmp.token_type = LPAREN;      return tmp;
        case ')': tmp.token_type = RPAREN;      return tmp;
        case '=': tmp.token_type = EQUAL;       return tmp;
        case '+': tmp.token_type = PLUS;        return tmp;
        case '-': tmp.token_type = MINUS;       return tmp;
        case '*': tmp.token_type = MULT;        return tmp;
        case '/': tmp.token_type = DIV;        return tmp;
        case '|': tmp.token_type = OR;          return tmp;
        case '^': tmp.token_type = AND;         return tmp;
        case '&': tmp.token_type = XOR;         return tmp;
        case '~': tmp.token_type = NOT;         return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;

        //STRING_CONSTANT
        case '"':
            input.UngetChar(c);
            return ScanStringCons();

        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput()) {
                tmp.token_type = END_OF_FILE;
            } else {
                tmp.lexeme += c;
                tmp.token_type = ERROR;
            }
            return tmp;
    }
}

//--------------------------------------------------------------------------------------------------------
//Parser

void LexicalAnalyzer::parse_program()
{
    Token t = GetToken();
    if (t.token_type == LBRACE)
    {
        UngetToken(t);
        parse_scope();
    }
    else
    {
        synt = true;
        cout << "Syntax Error";
        return;
    }
}

void LexicalAnalyzer::parse_scope()
{
    Token t = GetToken();
    if (t.token_type == LBRACE)
    {
        Token t1 = GetToken();
        if (t1.token_type == LBRACE || t1.token_type == ID || t1.token_type == WHILE)
        {
            UngetToken(t1);
            index = 0;
            sym_table.push_back(tbl());
            track.push_back(LBRACE);
            if (track.size() >= 2)
            {
                sym_table[sym_table.size()-1].parent = track.size()-2;
            }
            parse_scope_list();
            if (synt == true)
                return;
            Token t2 = GetToken();
            if (t2.token_type == RBRACE)
            {
                track.pop_back();
                return;
            }
            else
            {
                synt = true;
                //syn_err = "Syntax Error";
                cout << "Syntax Error";
                return;
            }
        }
        else
        {
            synt = true;
            cout << "Syntax Error";
            return;
        }
    }
    else
    {
        synt = true;
        //syn_err = "Syntax Error";
        cout << "Syntax Error";
        return;
    }
}

void LexicalAnalyzer::parse_scope_list()
{
    Token t = GetToken();
    Token t1 = GetToken();

    if (t.token_type == LBRACE)
    {
        UngetToken(t1);
        UngetToken(t);
        parse_scope();
        if (synt == true)
            return;
        Token t2 = GetToken();
        if (t2.token_type == LBRACE || t2.token_type == ID || t2.token_type == WHILE)
        {
            UngetToken(t2);
            parse_scope_list();
        }
        else if (t2.token_type == RBRACE)
        {
            UngetToken(t2);
            return;
        }
        else
        {
            synt = true;
            //syn_err = "Syntax Error";
            cout << "Syntax Error";
            return;
        }
    }
    else if (t.token_type == ID && (t1.token_type == COLON || t1.token_type == COMMA))
    {
        UngetToken(t1);
        UngetToken(t);
        parse_var_decl();
        if (synt == true)
            return;
        Token t2 = GetToken();
        if (t2.token_type == LBRACE || t2.token_type == ID || t2.token_type == WHILE)
        {
            UngetToken(t2);
            parse_scope_list();
        }
        else if (t2.token_type == RBRACE)
        {
            UngetToken(t2);
            return;
        }
        else
        {
            synt = true;
           // syn_err = "Syntax Error";
           cout << "Syntax Error";
           return;
        }
    }
    else if ((t.token_type == ID || t.token_type == WHILE) && (t1.token_type == EQUAL || t1.token_type == LPAREN))
    {
        UngetToken(t1);
        UngetToken(t);
        parse_stmt();
        if (synt == true)
            return;
        Token t2 = GetToken();
        if (t2.token_type == LBRACE || t2.token_type == ID || t2.token_type == WHILE)
        {
            UngetToken(t2);
            parse_scope_list();
        }
        else if (t2.token_type == RBRACE)
        {
            UngetToken(t2);
            return;
        }
        else
        {
            synt = true;
           // syn_err = "Syntax Error";
           cout << "Syntax Error";
           return;
        }
    }
    else
    {
        synt = true;
        //syn_err = "Syntax Error";
        cout << "Syntax Error";
        return;
    }
}

void LexicalAnalyzer::parse_var_decl()
{
    Token t = GetToken();
    if (t.token_type == ID)
    {
        UngetToken(t);
        parse_idlist();
        if (synt == true)
            return;
        Token t1 = GetToken();
        if (t1.token_type == COLON)
        {
            Token t2 = GetToken();
            if (t2.token_type == REAL || t2.token_type == INT || t2.token_type == BOOLEAN || t2.token_type == STRING)
            {
                UngetToken(t2);
                parse_typename();
                if (synt == true)
                    return;
                Token t3 = GetToken();
                if (t3.token_type == SEMICOLON)
                    return;
                else
                {
                    synt = true;
                    //syn_err = "Syntax Error";
                    cout << "Syntax Error";
                    return;
                }

            }
            else
            {
                synt = true;
                //syn_err = "Syntax Error";
                cout << "Syntax Error";
                return;
            }
        }
        else
        {
            synt = true;
            //syn_err = "Syntax Error";
            cout << "Syntax Error";
            return;
        }
    }
    else
    {
        synt = true;
        //syn_err = "Syntax Error";
        cout << "Syntax Error";
        return;
    }
}

bool LexicalAnalyzer::check(string st)
{
    bool found = false;
    for (int i = 0; i < sym_table[sym_table.size()-1].v.size(); i++)
    {
        if (st.compare(sym_table[sym_table.size()-1].v[i]) == 0)
        {
            found = true;
        }
    }

    return found;
}

void LexicalAnalyzer::parse_idlist()
{

    Token t = GetToken();
    if (t.token_type == ID)
    {
        if (check(t.lexeme) == true)
        {
            if (dec == false)
            {
                dec = true;
                decl_err = "ERROR CODE 1.1 " + t.lexeme;
            }
        }
        sym_table[sym_table.size()-1].v.push_back(t.lexeme);
        sym_table[sym_table.size()-1].line.push_back(get_line_no());

        Token t1 = GetToken();
        if (t1.token_type == COMMA)
        {
            Token t2 = GetToken();
            if (t2.token_type == ID)
            {
                UngetToken(t2);
                parse_idlist();
            }
            else
            {
                synt = true;
                //syn_err = "Syntax Error";
                cout << "Syntax Error";
                return;
            }
        }
        else if (t1.token_type == COLON)
        {
            UngetToken(t1);
            return;
        }
        else
        {
            synt = true;
            //syn_err = "Syntax Error";
            cout << "Syntax Error";
            return;
        }
    }
    else
    {
        synt = true;
        //syn_err = "Syntax Error";
        cout << "Syntax Error";
        return;
    }
}

void LexicalAnalyzer::parse_typename()
{
    Token t = GetToken();
    if (t.token_type == REAL)
    {
        while (index < sym_table[sym_table.size()-1].v.size())
        {
            sym_table[sym_table.size()-1].t.push_back(REAL);
            index++;
        }
    }
    else if (t.token_type == INT)
    {
        while (index < sym_table[sym_table.size()-1].v.size())
        {
            sym_table[sym_table.size()-1].t.push_back(INT);
            index++;
        }
    }
    else if (t.token_type == BOOLEAN)
    {
        while (index < sym_table[sym_table.size()-1].v.size())
        {
            sym_table[sym_table.size()-1].t.push_back(BOOLEAN);
            index++;
        }
    }
    else if (t.token_type == STRING)
    {
        while (index < sym_table[sym_table.size()-1].v.size())
        {
            sym_table[sym_table.size()-1].t.push_back(STRING);
            index++;
        }
    }
    else
    {
        synt = true;
        //syn_err = "Syntax Error";
        cout << "Syntax Error";
        return;
    }
}

void LexicalAnalyzer::parse_stmt_list()
{
    Token t = GetToken();

    if (t.token_type == ID || t.token_type == WHILE)
    {
        UngetToken(t);
        parse_stmt();
        if (synt == true)
            return;
        Token t1 = GetToken();
        if (t1.token_type == ID || t1.token_type == WHILE)
        {
            UngetToken(t1);
            parse_stmt_list();
        }
        else if (t1.token_type == ID || t1.token_type == WHILE || t1.token_type == LBRACE || t1.token_type == RBRACE)
        {
            UngetToken(t1);
            return;
        }
        else
        {
            UngetToken(t1);
            synt = true;
            //syn_err = "Syntax Error";
            cout << "Syntax Error";
            return;
        }
    }
    else
    {
        synt = true;
        //syn_err = "Syntax Error";
        cout << "Syntax Error";
        return;
    }
}

void LexicalAnalyzer::parse_stmt()
{
    Token t = GetToken();
    if (t.token_type == ID)
    {
        UngetToken(t);
        parse_assign_stmt();
        if (synt == true)
            return;
    }
    else if (t.token_type == WHILE)
    {
        UngetToken(t);
        parse_while_stmt();
        if (synt == true)
            return;
    }
    else
    {
        synt = true;
        //syn_err = "Syntax Error";
        cout << "Syntax Error";
        return;
    }
}

bool LexicalAnalyzer::check_decl(string st)
{
    bool found = false;
    for (int i = 0; i < sym_table[sym_table.size()-1].v.size(); i++)
    {
        if (st.compare(sym_table[sym_table.size()-1].v[i]) == 0)
        {
            found = true;
            return found;
        }
    }

    if (found == false)
    {
        int p = sym_table[sym_table.size()-1].parent;
        while (p != -1)
        {
            for (int i = 0; i < sym_table[p].v.size(); i++)
            {
                if (st.compare(sym_table[p].v[i]) == 0)
                {
                    return true;
                }
            }
            p = sym_table[p].parent;
        }
    }

    return false;
}

int LexicalAnalyzer::line_dec(string st)
{
    bool found = false;
    for (int i = 0; i < sym_table[sym_table.size()-1].v.size(); i++)
    {
        if (st.compare(sym_table[sym_table.size()-1].v[i]) == 0)
        {
            found = true;
            return sym_table[sym_table.size()-1].line[i];
        }
    }

    if (found == false)
    {
        int p = sym_table[sym_table.size()-1].parent;
        while (p != -1)
        {
            for (int i = 0; i < sym_table[p].v.size(); i++)
            {
                if (st.compare(sym_table[p].v[i]) == 0)
                {
                    return sym_table[p].line[i];
                }
            }
            p = sym_table[p].parent;
        }
    }

    return 0;
}

void LexicalAnalyzer::parse_assign_stmt()
{
    Token id_type;
    newToken = GetToken();
    if (newToken.token_type == ID)
    {
        if (check_decl(newToken.lexeme) == false)
        {
            if (dec == false)
            {
                dec = true;
                decl_err = "ERROR CODE 1.2 " + newToken.lexeme;
                //dec = true;
            }
        }

        output.push_back(newToken.lexeme + " " + to_string(get_line_no()) + " " + to_string(line_dec(newToken.lexeme)));


        Token t1 = GetToken();
        if (t1.token_type == EQUAL)
        {
            Token t2 = GetToken();
            if (t2.token_type == NOT || t2.token_type == PLUS || t2.token_type == MINUS || t2.token_type == MULT || t2.token_type == DIV || t2.token_type == GREATER ||
                t2.token_type == GTEQ || t2.token_type == LESS || t2.token_type == NOTEQUAL || t2.token_type == LTEQ || t2.token_type == AND || t2.token_type == OR ||
                t2.token_type == XOR || t2.token_type == ID || t2.token_type == NUM || t2.token_type == REALNUM || t2.token_type == STRING_CONSTANT || t2.token_type == TRUE ||
                t2.token_type == FALSE)
            {
                UngetToken(t2);
                id_type = parse_expr();
                if (synt == true)
                    return;
                lft_refs.push_back(lhs_refs());
                lft_refs[lft_refs.size()-1].lhs = newToken.lexeme;
                lft_refs[lft_refs.size()-1].l = get_line_no();
                lft_refs[lft_refs.size()-1].l_dec = line_dec(newToken.lexeme);
                lft_refs[lft_refs.size()-1].loop = in_loop.size();

                if (lookup(newToken.lexeme) == INT)
                {
                    if (id_type.token_type == ID)
                    {
                        if (lookup(id_type.lexeme) != INT)
                        {
                            if (type_err == false)
                            {
                                type_err = true;
                                type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C1";
                                //type_err = true;
                            }
                        }
                    }
                    else if (id_type.token_type != NUM)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C1";
                            //type_err = true;
                        }
                    }
                }
                else if (lookup(newToken.lexeme) == BOOLEAN)
                {
                    if (id_type.token_type == ID)
                    {
                        if (lookup(id_type.lexeme) != BOOLEAN)
                        {
                            if (type_err == false)
                            {
                                type_err = true;
                                type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C1";
                                //type_err = true;
                            }
                        }
                    }
                    else if (id_type.token_type != TRUE && id_type.token_type != FALSE)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C1";
                            //type_err = true;
                        }
                    }
                }
                else if (lookup(newToken.lexeme) == STRING)
                {
                    if (id_type.token_type == ID)
                    {
                        if (lookup(id_type.lexeme) != STRING)
                        {
                            if (type_err == false)
                            {
                                type_err = true;
                                type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C1";
                               // type_err = true;
                            }
                        }
                    }
                    else if (id_type.token_type != STRING_CONSTANT)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C1";
                            //type_err = true;
                        }
                    }
                }
                else if (lookup(newToken.lexeme) == REAL)
                {
                    if (id_type.token_type == ID)
                    {
                        if (lookup(id_type.lexeme) != REAL && lookup(id_type.lexeme) != INT)
                        {
                            if (type_err == false)
                            {
                                type_err = true;
                                type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C2";
                               // type_err = true;
                            }
                        }
                    }
                    else if (id_type.token_type != REALNUM && id_type.token_type != NUM)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C2";
                            //type_err = true;
                        }
                    }
                }


                Token t3 = GetToken();
                if (t3.token_type == SEMICOLON)
                {
                    return;
                }
                else
                {
                    /*synt = true;
                    syn_err = "Syntax Error";*/
                    synt = true;
                    cout << "Syntax Error";
                    return;
                }
            }
            else
            {
                synt = true;
                cout << "Syntax Error";
                return;
            }
        }
        else
        {
            synt = true;
            cout << "Syntax Error";
            return;
        }
    }
    else
    {
        synt = true;
        //syn_err = "Syntax Error";
        cout << "Syntax Error";
        return;
    }
}

void LexicalAnalyzer::parse_while_stmt()
{
    Token t = GetToken();

    if (t.token_type == WHILE)
    {
        Token t1 = GetToken();
        if (t1.token_type == LPAREN)
        {
            UngetToken(t1);
            parse_condition();
            if (synt == true)
                return;
            Token t2 = GetToken();
            if (t2.token_type == LBRACE)
            {
                in_loop.push_back(WHILE);
                index = 0;
                sym_table.push_back(tbl());
                track.push_back(LBRACE);
                if (track.size() >= 2)
                {
                    sym_table[sym_table.size()-1].parent = track.size()-2;
                }
                Token t3 = GetToken();
                if (t3.token_type == ID || t3.token_type == WHILE)
                {
                    UngetToken(t3);
                    parse_stmt_list();
                    if (synt == true)
                        return;
                    Token t4 = GetToken();
                    if (t4.token_type == RBRACE)
                    {
                        in_loop.pop_back();
                        track.pop_back();
                        return;
                    }
                    else
                    {
                        synt = true;
                        //syn_err = "Syntax Error";
                        cout << "Syntax Error";
                        return;
                    }
                }
                else
                {
                    synt = true;
                    //syn_err = "Syntax Error";
                    cout << "Syntax Error";
                    return;
                }
            }
            else if (t2.token_type == ID || t2.token_type == WHILE)
            {
                in_loop.push_back(WHILE);
                UngetToken(t2);
                parse_stmt();
                in_loop.pop_back();
                return;
            }
            else
            {
                synt = true;
                cout << "Syntax Error";
                return;
            }
        }
        else
        {
            synt = true;
           // syn_err = "Syntax Error";
           cout << "Syntax Error";
           return;
        }
    }
    else
    {
        synt = true;
        //syn_err = "Syntax Error";
        cout << "Syntax Error";
        return;
    }
}

TokenType LexicalAnalyzer::lookup(string st)
{
    bool found = false;
    for (int i = 0; i < sym_table[sym_table.size()-1].v.size(); i++)
    {
        if (st.compare(sym_table[sym_table.size()-1].v[i]) == 0)
        {
            found = true;
            return sym_table[sym_table.size()-1].t[i];
        }
    }

    if (found == false)
    {
        int p = sym_table[sym_table.size()-1].parent;
        while (p != -1)
        {
            for (int i = 0; i < sym_table[p].v.size(); i++)
            {
                if (st.compare(sym_table[p].v[i]) == 0)
                {
                    return sym_table[p].t[i];
                }
            }
            p = sym_table[p].parent;
        }
    }
    return ERROR;
}

void LexicalAnalyzer::err_c1(Token tok, Token t)
{
    if (type_err == false)
    {
        if (lookup(tok.lexeme) == BOOLEAN)
        {
            if (lookup(t.lexeme) != BOOLEAN)
            {
                type_err = true;
                type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C1";
                //type_err = true;
            }
        }
        else if (lookup(tok.lexeme) == STRING)
        {
            if (lookup(t.lexeme) != STRING)
            {
                type_err = true;
                type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C1";
                //type_err = true;
            }
        }
        else if (lookup(tok.lexeme) == INT)
        {
            if (lookup(t.lexeme) != INT)
            {
                type_err = true;
                type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C1";
                //type_err = true;
            }
        }
        else
        {
            if (lookup(t.lexeme) != REAL || lookup(t.lexeme) != INT)
            {
                type_err = true;
                cout << "TYPE MISMATCH " << to_string(get_line_no()) << " " << "C2";
               // type_err = true;
            }
        }
    }
}

Token LexicalAnalyzer::parse_expr()
{
    Token tok;
    Token t = GetToken();
    if (t.token_type == PLUS || t.token_type == MINUS || t.token_type == MULT || t.token_type == DIV)
    {
        UngetToken(t);
        parse_arith_operator();
        if (synt == true)
            return tok;
        Token t1 = GetToken();
        if (t1.token_type == NOT || t1.token_type == PLUS || t1.token_type == MINUS || t1.token_type == MULT || t1.token_type == DIV || t1.token_type == GREATER ||
                t1.token_type == GTEQ || t1.token_type == LESS || t1.token_type == NOTEQUAL || t1.token_type == LTEQ || t1.token_type == AND || t1.token_type == OR ||
                t1.token_type == XOR || t1.token_type == ID || t1.token_type == NUM || t1.token_type == REALNUM || t1.token_type == STRING_CONSTANT || t1.token_type == TRUE ||
                t1.token_type == FALSE)
        {
            if (t1.token_type == ID)
            {
               if (lookup(t1.lexeme) == BOOLEAN || lookup(t1.lexeme) == STRING || lookup(t1.lexeme) == ERROR)
               {
                   if (type_err == false)
                   {
                       type_err = true;
                        type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C3";
                   }
               }
                //err_c1(newToken, t1);
            }
            else if (t1.token_type == TRUE || t1.token_type == FALSE || t1.token_type == STRING_CONSTANT)
            {
                if (type_err == false)
                {
                    type_err = true;
                    type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C3";
                }
            }
            UngetToken(t1);
            tok = parse_expr();
            Token t2 = GetToken();
            if (t2.token_type == NOT || t2.token_type == PLUS || t2.token_type == MINUS || t2.token_type == MULT || t2.token_type == DIV || t2.token_type == GREATER ||
                t2.token_type == GTEQ || t2.token_type == LESS || t2.token_type == NOTEQUAL || t2.token_type == LTEQ || t2.token_type == AND || t2.token_type == OR ||
                t2.token_type == XOR || t2.token_type == ID || t2.token_type == NUM || t2.token_type == REALNUM || t2.token_type == STRING_CONSTANT || t2.token_type == TRUE ||
                t2.token_type == FALSE)
            {
                if (t2.token_type == ID)
                {
                    if (lookup(t2.lexeme) == BOOLEAN || lookup(t2.lexeme) == STRING || lookup(t2.lexeme) == ERROR)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C3";
                        }
                    }
                    //err_c1(newToken, t2);
                }
                else if (t2.token_type == TRUE || t2.token_type == FALSE || t2.token_type == STRING_CONSTANT)
                {
                    if (type_err == false)
                    {
                        type_err = true;
                        type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C3";
                    }
                }
                UngetToken(t2);
                tok = parse_expr();
            }
            else
            {
                synt = true;
                //syn_err = "Syntax Error";
                cout << "Syntax Error";
                return tok;
            }

        }
        else
        {
            synt = true;
            //syn_err = "Syntax Error";
            cout << "Syntax Error";
            return tok;
        }
    }
    else if (t.token_type == AND || t.token_type == OR || t.token_type == XOR)
    {
        UngetToken(t);
        parse_bin_bool_operator();
        if (synt == true)
            return tok;
        Token t1 = GetToken();
        if (t1.token_type == NOT || t1.token_type == PLUS || t1.token_type == MINUS || t1.token_type == MULT || t1.token_type == DIV || t1.token_type == GREATER ||
                t1.token_type == GTEQ || t1.token_type == LESS || t1.token_type == NOTEQUAL || t1.token_type == LTEQ || t1.token_type == AND || t1.token_type == OR ||
                t1.token_type == XOR || t1.token_type == ID || t1.token_type == NUM || t1.token_type == REALNUM || t1.token_type == STRING_CONSTANT || t1.token_type == TRUE ||
                t1.token_type == FALSE)
        {
            if (t1.token_type == ID)
            {
                if (lookup(t1.lexeme) == STRING || lookup(t1.lexeme) == INT || lookup(t1.lexeme) == REAL || lookup(t1.lexeme) == ERROR)
                {
                    if (type_err == false)
                    {
                        type_err = true;
                        type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C4";
                    }
                }
                //err_c1(newToken, t1);
            }
            else if (t1.token_type == STRING_CONSTANT || t1.token_type == NUM || t1.token_type == REALNUM)
            {
                if (type_err == false)
                {
                    type_err = true;
                    type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C4";
                }
            }
            UngetToken(t1);
            tok = parse_expr();
            Token t2 = GetToken();
            if (t2.token_type == NOT || t2.token_type == PLUS || t2.token_type == MINUS || t2.token_type == MULT || t2.token_type == DIV || t2.token_type == GREATER ||
                t2.token_type == GTEQ || t2.token_type == LESS || t2.token_type == NOTEQUAL || t2.token_type == LTEQ || t2.token_type == AND || t2.token_type == OR ||
                t2.token_type == XOR || t2.token_type == ID || t2.token_type == NUM || t2.token_type == REALNUM || t2.token_type == STRING_CONSTANT || t2.token_type == TRUE ||
                t2.token_type == FALSE)
            {
                if (t2.token_type == ID)
                {
                    if (lookup(t2.lexeme) == STRING || lookup(t2.lexeme) == INT || lookup(t2.lexeme) == REAL || lookup(t2.lexeme) == ERROR)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C4";
                        }
                    }
                    err_c1(newToken, t2);
                }
                else if (t2.token_type == STRING_CONSTANT || t2.token_type == NUM || t2.token_type == REALNUM)
                {
                    if (type_err == false)
                    {
                        type_err = true;
                        type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C4";
                    }
                }
                UngetToken(t2);
                tok = parse_expr();
            }
            else
            {
                synt = true;
                //syn_err = "Syntax Error";
                cout << "Syntax Error";
                return tok;
            }
        }
        else
        {
            synt = true;
            //syn_err = "Syntax Error";
            cout << "Syntax Error";
            return tok;
        }
    }
    else if (t.token_type == GREATER || t.token_type == GTEQ || t.token_type == LESS || t.token_type == NOTEQUAL || t.token_type == LTEQ)
    {
        UngetToken(t);
        parse_rel_operator();
        if (synt == true)
        {
            return tok;
        }
        Token t1 = GetToken();
        if (t1.token_type == NOT || t1.token_type == PLUS || t1.token_type == MINUS || t1.token_type == MULT || t1.token_type == DIV || t1.token_type == GREATER ||
                t1.token_type == GTEQ || t1.token_type == LESS || t1.token_type == NOTEQUAL || t1.token_type == LTEQ || t1.token_type == AND || t1.token_type == OR ||
                t1.token_type == XOR || t1.token_type == ID || t1.token_type == NUM || t1.token_type == REALNUM || t1.token_type == STRING_CONSTANT || t1.token_type == TRUE ||
                t1.token_type == FALSE)
        {
            Token temp = GetToken();
            UngetToken(temp);
            if (t1.token_type == ID)
            {
                if (temp.token_type == ID)
                {
                    TokenType m = lookup(t1.lexeme);
                    TokenType m1 = lookup(temp.lexeme);
                    if ((m == STRING && m1 != STRING) || (m == BOOLEAN && m1 != BOOLEAN))
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C5";
                        }
                    }
                    else if(m == INT && (m1 != INT && m1 != REAL))
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C6";
                        }
                    }
                    else if (m == REAL && (m1 != INT && m1 != REAL))
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C6";
                        }
                    }
                }
                else if (temp.token_type == STRING_CONSTANT)//------------------------------------------
                {
                    if (lookup(t1.lexeme) != STRING)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C5";
                        }
                    }
                }
                else if (temp.token_type == TRUE || temp.token_type == FALSE)//------------------------------------
                {
                    if (lookup(t1.lexeme) != BOOLEAN)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C5";
                        }
                    }
                }
                else if (temp.token_type == NUM)
                {
                    if (lookup(t1.lexeme) != INT && lookup(t1.lexeme) != REAL)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C6";
                        }
                    }
                }
                else if (temp.token_type == REALNUM)
                {
                    if (lookup(t1.lexeme) != INT && lookup(t1.lexeme) != REAL)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C6";
                        }
                    }
                }
                //err_c1(newToken, t1);
            }
            else if (t1.token_type == NUM || t1.token_type == REALNUM)
            {
                if (temp.token_type == ID)
                {
                    if (lookup(temp.lexeme) != INT && lookup(temp.lexeme) != REAL)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C6";
                        }
                    }
                }
                else if (temp.token_type == STRING_CONSTANT || temp.token_type == TRUE || temp.token_type == FALSE)
                {
                    if (type_err == false)
                    {
                        type_err = true;
                        type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C6";
                    }
                }
            }
            else if (t1.token_type == STRING_CONSTANT)
            {
                if (temp.token_type == ID)
                {
                    if (lookup(temp.lexeme) != STRING)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C5";
                        }
                    }
                }
                else if (temp.token_type == TRUE || temp.token_type == FALSE || temp.token_type == NUM || temp.token_type == REALNUM)
                {
                    if (type_err == false)
                    {
                        type_err = true;
                        type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C5";
                    }
                }
            }
            else if (t1.token_type == TRUE || t1.token_type == FALSE)
            {
               if (temp.token_type == ID)
                {
                    if (lookup(temp.lexeme) != BOOLEAN)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C5";
                        }
                    }
                }
                else if (temp.token_type == STRING_CONSTANT || temp.token_type == NUM || temp.token_type == REALNUM)
                {
                    if (type_err == false)
                    {
                        type_err = true;
                        type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C5";
                    }
                }
            }
            UngetToken(t1);
            tok = parse_expr();
            Token t2 = GetToken();
            if (t2.token_type == NOT || t2.token_type == PLUS || t2.token_type == MINUS || t2.token_type == MULT || t2.token_type == DIV || t2.token_type == GREATER ||
                t2.token_type == GTEQ || t2.token_type == LESS || t2.token_type == NOTEQUAL || t2.token_type == LTEQ || t2.token_type == AND || t2.token_type == OR ||
                t2.token_type == XOR || t2.token_type == ID || t2.token_type == NUM || t2.token_type == REALNUM || t2.token_type == STRING_CONSTANT || t2.token_type == TRUE ||
                t2.token_type == FALSE)
            {
                Token temp1 = GetToken();
                UngetToken(temp1);
            if (t2.token_type == ID)
            {
                if (temp1.token_type == ID)
                {
                    TokenType m = lookup(t1.lexeme);
                    TokenType m1 = lookup(temp1.lexeme);
                    if ((m == STRING && m1 != STRING) || (m == BOOLEAN && m1 != BOOLEAN))
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C5";
                        }
                    }
                    else if(m == INT && (m1 != INT && m1 != REAL))
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C6";
                        }
                    }
                    else if (m == REAL && (m1 != INT && m1 != REAL))
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C6";
                        }
                    }
                }
                else if (temp1.token_type == STRING_CONSTANT)
                {
                    if (lookup(t2.lexeme) != STRING)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C5";
                        }
                    }
                }
                else if (temp1.token_type == TRUE || temp1.token_type == FALSE)
                {
                    if (lookup(t2.lexeme) != BOOLEAN)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C5";
                        }
                    }
                }
                else if (temp1.token_type == NUM)
                {
                    if (lookup(t2.lexeme) != INT && lookup(t2.lexeme) != REAL)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C6";
                        }
                    }
                }
                else if (temp1.token_type == REALNUM)
                {
                    if (lookup(t2.lexeme) != INT && lookup(t2.lexeme) != REAL)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C6";
                        }
                    }
                }
                //err_c1(newToken, t2);
            }
            else if (t2.token_type == NUM || t2.token_type == REALNUM)
            {
                if (temp1.token_type == ID)
                {
                    if (lookup(temp1.lexeme) != INT && lookup(temp1.lexeme) != REAL)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C6";
                        }
                    }
                }
                else if (temp1.token_type == STRING_CONSTANT || temp1.token_type == TRUE || temp1.token_type == FALSE)
                {
                    if (type_err == false)
                    {
                        type_err = true;
                        type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C6";
                    }
                }
            }
            else if (t2.token_type == STRING_CONSTANT)
            {
                if (temp1.token_type == ID)
                {
                    if (lookup(temp1.lexeme) != STRING)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C5";
                        }
                    }
                }
                else if (temp1.token_type == TRUE || temp1.token_type == FALSE || temp1.token_type == NUM || temp1.token_type == REALNUM)
                {
                    if (type_err == false)
                    {
                        type_err = true;
                        type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C5";
                    }
                }
            }
            else if (t2.token_type == TRUE || t2.token_type == FALSE)
            {
               if (temp1.token_type == ID)
                {
                    if (lookup(temp1.lexeme) != BOOLEAN)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C5";
                        }
                    }
                }
                else if (temp1.token_type == INT || temp1.token_type == REALNUM || temp1.token_type == STRING_CONSTANT)
                {
                    if (type_err == false)
                    {
                        type_err = true;
                        type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C5";
                    }
                }
            }
            UngetToken(t2);
            tok = parse_expr();
            }
            else
            {
                synt = true;
                //syn_err = "Syntax Error";
                cout << "Syntax Error";
                return tok;
            }
        }
        else
        {
            synt = true;
            //syn_err = "Syntax Error";
            cout << "Syntax Error";
            return tok;
        }
    }
    else if (t.token_type == NOT)
    {
        Token t1 = GetToken();
        if (t1.token_type == NOT || t1.token_type == PLUS || t1.token_type == MINUS || t1.token_type == MULT || t1.token_type == DIV || t1.token_type == GREATER ||
                t1.token_type == GTEQ || t1.token_type == LESS || t1.token_type == NOTEQUAL || t1.token_type == LTEQ || t1.token_type == AND || t1.token_type == OR ||
                t1.token_type == XOR || t1.token_type == ID || t1.token_type == NUM || t1.token_type == REALNUM || t1.token_type == STRING_CONSTANT || t1.token_type == TRUE ||
                t1.token_type == FALSE)
        {
            if (t1.token_type == ID || t1.token_type == NUM || t1.token_type == REALNUM || t1.token_type == STRING_CONSTANT || t1.token_type == TRUE || t1.token_type == FALSE)
            {
                if (t1.token_type == ID)
                {
                    if (lookup(t1.lexeme) != BOOLEAN)
                    {
                        if (type_err == false)
                        {
                            type_err = true;
                            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C8";
                        }
                    }
                    //err_c1(newToken, t1);
                }
                else if (t1.token_type == STRING_CONSTANT || t1.token_type == NUM || t1.token_type == REALNUM)
                {
                    if (type_err == false)
                    {
                        type_err = true;
                        type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C8";
                    }
                }
            }
            UngetToken(t1);
            tok = parse_expr();
        }
        else
        {
            synt = true;
            //syn_err = "Syntax Error";
            cout << "Syntax Error";
            return tok;
        }
    }
    else if (t.token_type == ID || t.token_type == NUM || t.token_type == REALNUM || t.token_type == STRING_CONSTANT || t.token_type == TRUE || t.token_type == FALSE)
    {
        UngetToken(t);
        tok = parse_primary();
    }
    else
    {
        synt = true;
        //syn_err = "Syntax Error";
        cout << "Syntax Error";
        return tok;
    }

    return tok;
}

void LexicalAnalyzer::parse_arith_operator()
{
    Token t = GetToken();
    if (t.token_type == PLUS)
    {
        return;
    }
    else if (t.token_type == MINUS)
    {
        return;
    }
    else if (t.token_type == MULT)
    {
        return;
    }
    else if (t.token_type == DIV)
    {
        return;
    }
    else
    {
        synt = true;
        cout << "Syntax Error";
        return;
    }
}

void LexicalAnalyzer::parse_bin_bool_operator()
{
    Token t = GetToken();
    if (t.token_type == AND)
    {
        return;
    }
    else if (t.token_type == OR)
    {
        return;
    }
    else if (t.token_type == XOR)
    {
        return;
    }
    else
    {
        synt = true;
        cout << "Syntax Error";
        return;
    }
}

void LexicalAnalyzer::parse_rel_operator()
{
    Token t = GetToken();
    if (t.token_type == GREATER)
    {
        return;
    }
    else if (t.token_type == GTEQ)
    {
        return;
    }
    else if (t.token_type == LESS)
    {
        return;
    }
    else if (t.token_type == NOTEQUAL)
    {
        return;
    }
    else if (t.token_type == LTEQ)
    {
        return;
    }
    else
    {
        synt = true;
        cout << "Syntax Error";
        return;
    }
}

bool LexicalAnalyzer::check_init(string st)
{
    for (int i = 0; i < lft_refs.size(); i++)
    {
        if (st.compare(lft_refs[i].lhs) == 0 && lft_refs[i].loop <= in_loop.size())
        {
            return true;
        }
    }
    return false;
}

Token LexicalAnalyzer::parse_primary()
{
    Token t = GetToken();
    if (t.token_type == ID)
    {
        if (check_decl(t.lexeme) == false)
        {
            if (dec == false)
            {
                dec = true;
                decl_err = "ERROR CODE 1.2 " + t.lexeme;
                //dec = true;
            }
        }
        refs.push_back(rhs_refs());
        refs[refs.size()-1].rhs = t.lexeme;
        refs[refs.size()-1].l = get_line_no();
        refs[refs.size()-1].l_dec = line_dec(t.lexeme);
        if (check_init(refs[refs.size()-1].rhs) == true)
        {
            refs[refs.size()-1].init = 1;
        }


        output.push_back(t.lexeme + " " + to_string(get_line_no()) + " " + to_string(refs[refs.size()-1].l_dec));
        return t;
    }
    else if (t.token_type == NUM)
    {
        return t;
    }
    else if (t.token_type == REALNUM)
    {
        return t;
    }
    else if (t.token_type == STRING_CONSTANT)
    {
        return t;
    }
    else if (t.token_type == TRUE || t.token_type == FALSE)
    {
        UngetToken(t);
        return parse_bool_const();
    }
}

Token LexicalAnalyzer::parse_bool_const()
{
    Token t = GetToken();
    if (t.token_type == TRUE)
    {
        return t;
    }
    else if (t.token_type == FALSE)
    {
        return t;
    }
}

void LexicalAnalyzer::parse_condition()
{
    Token t = GetToken();
    if (t.token_type == LPAREN)
    {
        Token t1 = GetToken();
        if (t1.token_type == PLUS || t1.token_type == MINUS || t1.token_type == MULT || t1.token_type == DIV || t1.token_type == NUM || t1.token_type == REALNUM || t1.token_type == STRING_CONSTANT)
        {
            type_err = true;
            type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C7";
        }
        else if (t1.token_type == ID)
        {
            if (lookup(t1.lexeme) != BOOLEAN)
            {
                type_err = true;
                type_mismatch = "TYPE MISMATCH " + to_string(get_line_no()) + " " + "C7";
            }
        }
        UngetToken(t1);
        if (t1.token_type == NOT || t1.token_type == PLUS || t1.token_type == MINUS || t1.token_type == MULT || t1.token_type == DIV || t1.token_type == GREATER ||
                t1.token_type == GTEQ || t1.token_type == LESS || t1.token_type == NOTEQUAL || t1.token_type == LTEQ || t1.token_type == AND || t1.token_type == OR ||
                t1.token_type == XOR || t1.token_type == ID || t1.token_type == NUM || t1.token_type == REALNUM || t1.token_type == STRING_CONSTANT || t1.token_type == TRUE ||
                t1.token_type == FALSE)
        {
            parse_expr();
            if (synt == true)
                return;
            Token t2 = GetToken();
            if (t2.token_type == RPAREN)
            {
                return;
            }
            else
            {
                synt = true;
                cout << "Syntax Error";
                return;
            }
        }
        else
        {
            synt = true;
            cout << "Syntax Error";
            return;
        }

    }
    else
    {
        synt = true;
        //syn_err = "Syntax Error";
        cout << "Syntax Error";
        return;
    }

}

bool LexicalAnalyzer::check_refs(string st)
{
    bool found;
    for (int i = 0; i < lft_refs.size(); i++)
    {
        if (st.compare(lft_refs[i].lhs) == 0)
        {
            return true;
        }
    }

    for (int j = 0; j < refs.size(); j++)
    {
        if (st.compare(refs[j].rhs) == 0)
        {
            return true;
        }
    }
    return false;
}

void LexicalAnalyzer::err_1_3()
{
    for (int i = 0; i < sym_table.size(); i++)
    {
        for (int j = 0; j < sym_table[i].v.size(); j++)
        {
            if (check_refs(sym_table[i].v[j]) == false)
            {
                dec =  true;
                cout << "ERROR CODE 1.3 " <<  sym_table[i].v[j];
                return;
            }
        }
    }
}

bool LexicalAnalyzer::check_lhs(string st)
{
    for (int i = 0; i < lft_refs.size(); i++)
    {
        if (st.compare(lft_refs[i].lhs) == 0)
        {
            return true;
        }
    }
    return false;
}

bool LexicalAnalyzer::get_uninitialized()
{
    for (int i = 0; i < refs.size(); i++)
    {
        if (refs[i].init == 0)
        {
            return true;
        }
    }
    return false;
}

void LexicalAnalyzer::print_uninitialized()
{
    for (int i = 0; i < refs.size(); i++)
    {
        if (refs[i].init == 0)
        {
            cout << "UNINITIALIZED " << refs[i].rhs << " " << refs[i].l << endl;
        }
    }
}

void LexicalAnalyzer::print_output()
{
    for (int i = 0; i < output.size(); i++)
    {
        cout << output[i] << endl;
    }
}

bool LexicalAnalyzer::get_synt()
{
    return synt;
}

string LexicalAnalyzer::get_syntax_error()
{
    return syn_err;
}

bool LexicalAnalyzer::get_dec()
{
    return dec;
}

string LexicalAnalyzer::get_decl_error()
{
    return decl_err;
}

bool LexicalAnalyzer::get_type_err()
{
    return type_err;
}

string LexicalAnalyzer::get_type_mismatch_error()
{
    return type_mismatch;
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    lexer.parse_program();

    if (lexer.get_synt() == false)
    {
        if (lexer.get_dec() == false)
        {
            lexer.err_1_3();
            if (lexer.get_dec() == false)
            {
                if (lexer.get_type_err() == false)
                {
                    if (lexer.get_uninitialized() == false)
                    {
                        lexer.print_output();
                    }
                    else
                        lexer.print_uninitialized();
                }
                else
                {
                    cout << lexer.get_type_mismatch_error();
                }
            }
        }
        else
        {
            cout << lexer.get_decl_error();
        }
    }

    //token.Print();
    /*while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }*/


}

