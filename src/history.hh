#ifndef HISTORY_HH
#define HISTORY_HH

#include "constant.hh"

typedef struct debug_flag_info
{
    debug_drapeau ftype;
    position pos;
} debug_flag_info;

typedef struct fall_info
{
    int player_id;
    int nain_id;
    position goal;
} fall_info;

// We do not want unused debug flag info to appear in the public API, so using
// only action_hist is not enough to store the complete internal history.
typedef struct internal_action
{
    // 0 for debug flags
    // 1 for standart action
    // 2 for falling event
    // 3 for death event
    // 4 for ropes which reached the floor
    // 5 for respawn
    int type;

    union {
        debug_flag_info debug_flag;
        action_hist action;
        fall_info fall;
    };
} internal_action;

#endif
