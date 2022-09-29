// Robin de Jong and Abbe Piels
// CSC 254 Assignment 2: syntax error recovery/* Simple ad-hoc scanner for the calculator language.
//   Dies on lexical errors.
 //  Michael L. Scott, 2008-2022.
//

#include <iostream>
#include <cctype>   // isalpha, isspace, isdigit
#include <tuple>
using std::cerr;
using std::cin;
using std::hex;
using std::endl;
using std::string;
using std::make_tuple;

#include "scan.hpp"

tuple<token, string> scanner::scan() {
    string token_image;

    // skip white space
    while (isspace(c)) {
        c = cin.get();
    }
    if (c == EOF)
        return make_tuple(t_eof, "");
    if (isalpha(c)) {
        do {
            token_image += c;
            c = cin.get();
        } while (isalpha(c) || isdigit(c) || c == '_');
        if (token_image == "read") return make_tuple(t_read, "");
        else if (token_image == "write") return make_tuple(t_write, "");
        else return make_tuple(t_id, token_image);
    }
    else if (isdigit(c)) {
        do {
            token_image += c;
            c = cin.get();
        } while (isdigit(c));
        return make_tuple(t_literal, token_image);
    } else switch (c) {
        case ':':
            c = cin.get();
            if (c != '=') {
                cerr << "expected '=' after ':', got '"
                     << c << "' (0x" << hex << c << ")\n";
                exit(1);
            } else {
                c = cin.get();
                return make_tuple(t_gets, "");
            }
            break;
        case '+': c = cin.get(); return make_tuple(t_add, "");
        case '-': c = cin.get(); return make_tuple(t_sub, "");
        case '*': c = cin.get(); return make_tuple(t_mul, "");
        case '/': c = cin.get(); return make_tuple(t_div, "");
        case '(': c = cin.get(); return make_tuple(t_lparen, "");
        case ')': c = cin.get(); return make_tuple(t_rparen, "");
        default:
            cerr << "unexpected character '"
                 << c << "' (0x" << hex << c << ")\n";
            exit(1);
    }
} // scan
