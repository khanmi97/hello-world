/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>

#include "inputbuf.h"

// ------- token types -------------------
using namespace std;
typedef enum { END_OF_FILE = 0,
    REAL, INT, BOOLEAN, STRING,
    WHILE,TRUE, FALSE, COMMA, COLON, SEMICOLON,
    LBRACE, RBRACE, LPAREN, RPAREN,
    EQUAL, PLUS, MINUS, MULT, DIV, AND, OR, XOR, NOT,
    GREATER, GTEQ, LESS, LTEQ, NOTEQUAL,
    ID, NUM, REALNUM, STRING_CONSTANT, ERROR
} TokenType;

class Token {
  public:
    void Print();

    string lexeme;
    TokenType token_type;
    int line_no;
};

class LexicalAnalyzer {
  public:
    Token GetToken();
    TokenType UngetToken(Token);
    LexicalAnalyzer();
    int get_line_no();
    void parse_program();
    void err_1_3();
    bool get_uninitialized();
    void print_uninitialized();
    void print_output();
    bool get_synt();
    string get_syntax_error();
    bool get_dec();
    string get_decl_error();
    bool get_type_err();
    string get_type_mismatch_error();

  private:
    vector<Token> tokens;
    int line_no;
    Token tmp;
    InputBuffer input;
    struct tbl{
        vector<string> v;
        vector<TokenType> t;
        vector <int> line;
        int parent = -1;
    };
    struct tbl sym;
    vector<tbl> sym_table;
    unsigned int index;
    int track_scope = 0;
    bool flag = false;
    vector <TokenType> track;
    vector <string> output;

    Token newToken;

    bool type_err = false;
    string type_mismatch = ""; // display type mismatch error

    struct rhs_refs {
        string rhs;
        int l;
        int l_dec;
        int init = 0;
        int loop = 0;
    };
    struct rhs_refs r;
    vector <rhs_refs> refs; // all references of the variables

    struct lhs_refs{
        string lhs;
        int l;
        int l_dec;
        int loop = 0;
    };
    struct lhs_refs lft;
    vector <lhs_refs> lft_refs;

    bool synt = false; // syntax error condition
    string syn_err = ""; // display syntax error
    bool dec = false; // decl error condition
    string decl_err = ""; // display decl error

    vector <TokenType> in_loop;



    bool SkipSpace();
    bool IsKeyword(string);
    TokenType FindKeywordIndex(string);
    Token ScanIdOrKeyword();
    Token ScanStringCons();
    Token ScanNumber();

    //----------------------------------------------------------------------------------------------------------
    //void parse_program();
    void parse_scope();
    void parse_scope_list();
    void parse_var_decl();
    void parse_stmt();
    bool check(string st);
    void parse_idlist();
    void parse_typename();
    void parse_stmt_list();
    bool check_decl(string st);
    int line_dec(string st);
    void parse_assign_stmt();
    void parse_while_stmt();
    TokenType lookup(string st);
    void err_c1(Token tok, Token t);
    Token parse_expr();
    void parse_condition();
    void parse_arith_operator();
    void parse_bin_bool_operator();
    void parse_rel_operator();
    bool check_init(string st);
    Token parse_primary();
    Token parse_bool_const();
    bool check_refs(string st);
    bool check_lhs(string st);

};

#endif  //__LEXER__H__

