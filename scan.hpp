/* Definitions the scanner shares with the parser.
   Michael L. Scott, 2008-2022.
*/

#include <tuple>
using std::tuple;
using std::string;

enum token {t_read, t_write, t_id, t_literal, t_gets, t_add,
            t_sub, t_mul, t_div, t_lparen, t_rparen, t_eof};

const int MAX_TOKEN_LEN = 256;
extern char token_image[MAX_TOKEN_LEN];

class scanner {
    int c = ' ';
public:
    tuple<token, string> scan();
};
