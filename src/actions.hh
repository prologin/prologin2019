#ifndef ACTIONS_HH
#define ACTIONS_HH

enum action_id
{
    ID_ACTION_DEPLACER,
    ID_ACTION_LACHER,
    ID_ACTION_AGRIPPER,
    ID_ACTION_MINER,
    ID_ACTION_TIRER,
    ID_ACTION_POSER_CORDE,
    ID_ACTION_DEBUG_AFFICHER_DRAPEAU
};

#include "action_agripper.hh"
#include "action_debug_afficher_drapeau.hh"
#include "action_deplacer.hh"
#include "action_lacher.hh"
#include "action_miner.hh"
#include "action_poser_corde.hh"
#include "action_tirer.hh"

#endif // !ACTIONS_HH
