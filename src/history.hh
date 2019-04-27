#ifndef HISTORY_HH
#define HISTORY_HH

#include "constant.hh"

typedef struct debug_flag_info
{
    debug_drapeau ftype;
    position pos;
} debug_flag_info;

// We do not want unused debug flag info to appear in the public API, so using
// only action_hist is not enough to store the complete internal history.
typedef struct internal_action
{
    bool internal;
    union {
        debug_flag_info debug_flag;
        action_hist action;
    };
} internal_action;

#endif
