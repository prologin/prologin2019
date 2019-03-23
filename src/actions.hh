#ifndef ACTIONS_HH
#define ACTIONS_HH

enum action_id {
    ID_ACTION_DEPLACER,
    ID_ACTION_ESCALADER,
    ID_ACTION_GRIMPER,
    ID_ACTION_LACHER,
    ID_ACTION_MINER,
    ID_ACTION_TIRER,
    ID_ACTION_POSER_CORDE,
    ID_ACTION_DEBUG_AFFICHER_DRAPEAU
};

#include "action_deplacer.hh"
#include "action_escalader.hh"
#include "action_grimper.hh"
#include "action_lacher.hh"
#include "action_miner.hh"
#include "action_tirer.hh"
#include "action_poser_corde.hh"
#include "action_debug_afficher_drapeau.hh"

#endif // !ACTIONS_HH
