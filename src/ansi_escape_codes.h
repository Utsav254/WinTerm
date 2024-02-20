#ifndef ANSI_ESCAPE_CODES_H
#define ANSI_ESCAPE_CODES_H

#define LARGE_CURS_VAL 999

//usage:
//adding a _l refers to the number of bytes in the escapesequence

//terminal buffer contols
#define INIT_ALT_TERM_BUF "\x1b[?1049h"
#define INIT_ALT_TERM_BUF_l 8

#define END_ALT_TERM_BUF "\x1b[?1049l"
#define END_ALT_TERM_BUF_l 8

#define CLR_SCR_BUF "\x1b[2J"
#define CLR_SCR_BUF_l 8

#define CLR_CURR_LINE "\x1b[K"
#define CLR_CURR_LINE_l 3

//cursor controls
#define MV_CURS_HOME "\x1b[H"
#define MV_CURS_HOME_l 3

#define MV_CURS(l, c) "\x1b["#l";"#c"H"
#define MV_CURS_l(l, c) (sizeof("\x1b["#l";"#c"H")-1)

#define HIDE_CURS "\x1b[?25l"
#define HIDE_CURS_l 6

#define SHOW_CURS "\x1b[?25h"
#define SHOW_CURS_l 6

#define MV_CURS_NEXT_LINE "\x1b[E"
#define MV_CURS_NEXT_LINE_l 3


//graphics controls
//multiple graphics at one
#define GRAPH_SEQ(...)   "\x1b["#__VA_ARGS__"m"
#define GRAPH_SEQ_l(...) (sizeof("\x1b["#__VA_ARGS__"m")-1)

#endif
