#ifndef ANSI_ESCAPE_CODES_H
#define ANSI_ESCAPE_CODES_H
//usage:
//adding a _l refers to the number of bytes in the escapesequence

//terminal buffer contols
#define INIT_ALT_TERM_BUF "\x1b[?1049h"
#define INIT_ALT_TERM_BUF_l 8

#define END_ALT_TERM_BUF "\x1b[?1049l"
#define END_ALT_TERM_BUF_l 8

#define CLR_SCR_BUF "\x1b[2J"
#define CLR_SCR_BUF_l 8

//cursor controls
#define MV_CURS_HOME "\x1b[H"
#define MV_CURS_HOME_l 3

#define MV_CURS(x , y) "\x1b["#x";"#y"H"
#define MV_CURS_l(x , y) (sizeof("\x1b["#x";"#y"H")-1)

#define HIDE_CURS "\x1b[?25l"
#define HIDE_CURS_l 6

#define SHOW_CURS "\x1b[?25h"
#define SHOW_CURS_l 6

//graphics controls
//multiple graphics at one
#define GRAPH_SEQ(...)   "\x1b[" #__VA_ARGS__ "m"
#define GRAPH_SEQ_l(...) (sizeof("\x1b[" #__VA_ARGS__ "m")-1)
//graphics modes
#define GRAPH_RST_ALL 0
#define GRAPH_BOLD 1
#define GRAPH_BOLD_RST 21
#define GRAPH_FAINT 2
#define GRAPH_FAINT_RST 22
#define GRAPH_INV 7
#define GRAPH_INV_RST 27
// ...


#endif
