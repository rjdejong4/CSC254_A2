/* Complete recursive descent parser for the calculator language.
   Builds on figure 2.16 in the text.  Prints a trace of productions
   predicted and tokens matched.  Does no error recovery: prints
   "syntax error" and dies on invalid input.
   Michael L. Scott, 2008-2022.
*/

#include <iostream>
#include <tuple>
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::tie;

#include "scan.hpp"

const char* names[] = {"read", "write", "id", "literal", "gets", "add",
                       "sub", "mul", "div", "lparen", "rparen", "eof"};

class parser {
    token next_token;
    string token_image;
    scanner s;

    void error () {
        cerr << "syntax error" << endl;
        exit (1);
    }

    void match (token expected) {
        if (next_token == expected) {
            cout << "matched " << names[next_token];
            if (next_token == t_id || next_token == t_literal)
                cout << ": " << token_image;
            cout << endl;
            tie(next_token, token_image) = s.scan ();
        }
        else error ();
    }

public:

    parser() {
        tie(next_token, token_image) = s.scan ();
    }

    void program () {
        switch (next_token) {
            case t_id:
            case t_read:
            case t_write:
            case t_eof:
                cout << "predict program --> stmt_list eof" << endl;
                stmt_list ();
                match (t_eof);
                break;
            default: error ();
        }
    }

private:

    void stmt_list () {
        switch (next_token) {
            case t_id:
            case t_read:
            case t_write:
                cout << "predict stmt_list --> stmt stmt_list" << endl;
                stmt ();
                stmt_list ();
                break;
            case t_eof:
                cout << "predict stmt_list --> epsilon" << endl;
                break;          // epsilon production
            default: error ();
        }
    }

    void stmt () {
        switch (next_token) {
            case t_id:
                cout << "predict stmt --> id gets expr" << endl;
                match (t_id);
                match (t_gets);
                expr ();
                break;
            case t_read:
                cout << "predict stmt --> read id" << endl;
                match (t_read);
                match (t_id);
                break;
            case t_write:
                cout << "predict stmt --> write expr" << endl;
                match (t_write);
                expr ();
                break;
            default: error ();
        }
    }

    void expr () {
        switch (next_token) {
            case t_id:
            case t_literal:
            case t_lparen:
                cout << "predict expr --> term term_tail" << endl;
                term ();
                term_tail ();
                break;
            default: error ();
        }
    }

    void term_tail () {
        switch (next_token) {
            case t_add:
            case t_sub:
                cout << "predict term_tail --> add_op term term_tail" << endl;
                add_op ();
                term ();
                term_tail ();
                break;
            case t_rparen:
            case t_id:
            case t_read:
            case t_write:
            case t_eof:
                cout << "predict term_tail --> epsilon" << endl;
                break;          // epsilon production
            default: error ();
        }
    }

    void term () {
        switch (next_token) {
            case t_id:
            case t_literal:
            case t_lparen:
                cout << "predict term --> factor factor_tail" << endl;
                factor ();
                factor_tail ();
                break;
            default: error ();
        }
    }

    void factor_tail () {
        switch (next_token) {
            case t_mul:
            case t_div:
                cout << "predict factor_tail --> mul_op factor factor_tail"
                     << endl;
                mul_op ();
                factor ();
                factor_tail ();
                break;
            case t_add:
            case t_sub:
            case t_rparen:
            case t_id:
            case t_read:
            case t_write:
            case t_eof:
                cout << "predict factor_tail --> epsilon" << endl;
                break;          // epsilon production
            default: error ();
        }
    }

    void factor () {
        switch (next_token) {
            case t_literal:
                cout << "predict factor --> literal" << endl;
                match (t_literal);
                break;
            case t_id :
                cout << "predict factor --> id" << endl;
                match (t_id);
                break;
            case t_lparen:
                cout << "predict factor --> lparen expr rparen" << endl;
                match (t_lparen);
                expr ();
                match (t_rparen);
                break;
            default: error ();
        }
    }

    void add_op () {
        switch (next_token) {
            case t_add:
                cout << "predict add_op --> add" << endl;
                match (t_add);
                break;
            case t_sub:
                cout << "predict add_op --> sub" << endl;
                match (t_sub);
                break;
            default: error ();
        }
    }

    void mul_op () {
        switch (next_token) {
            case t_mul:
                cout << "predict mul_op --> mul" << endl;
                match (t_mul);
                break;
            case t_div:
                cout << "predict mul_op --> div" << endl;
                match (t_div);
                break;
            default: error ();
        }
    }

}; // parser

int main () {
    parser p;
    p.program ();
    return 0;
}
