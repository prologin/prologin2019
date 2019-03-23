#include "nain_info.hh"

NainInfo::NainInfo()
{
    nain_ = { { -1, -1}, VIE_NAIN, NB_POINTS_ACTION, NB_POINTS_DEPLACEMENT, 0 };
}

void NainInfo::set_position(const position& pos)
{
    nain_.pos = pos;
}
