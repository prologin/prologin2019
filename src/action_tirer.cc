#include "actions.hh"

#include <algorithm>

#include "position.hh"

int ActionTirer::check(const GameState* st) const
{
    if (!st->is_init())
        FATAL("action: you cannot use action outside jouer_tour");

    // Check nain
    if (id_nain_ < 0 || id_nain_ >= NB_NAINS)
        return ID_NAIN_INVALIDE;

    const nain nain = st->get_nain(player_id_, id_nain_);

    if (nain.vie <= 0)
        return NAIN_MORT;

    if (nain.pa < COUT_TIRER)
        return PA_INSUFFISANTS;

    // Check positions
    if (dir_corde_ != GAUCHE && dir_corde_ != DROITE)
        return DIRECTION_INVALIDE;

    if (sens_ != HAUT && sens_ != BAS)
        return DIRECTION_INVALIDE;

    position dest = get_position_offset(nain.pos, dir_corde_);

    if (!inside_map(dest))
        return HORS_LIMITES;

    if (st->map().get_cell_type(dest) != LIBRE)
        return OBSTACLE_MUR;

    // Check rope
    if (!st->map().has_rope_at(dest))
        return PAS_DE_CORDE;

    return OK;
}

void ActionTirer::apply_on(GameState* st) const
{
    internal_action action;
    action.type = 1;
    action.action = {ACTION_TIRER, id_nain_, dir_corde_, sens_};
    st->add_to_internal_history(player_id_, action);

    const nain nain = st->get_nain(player_id_, id_nain_);
    position dest = get_position_offset(nain.pos, dir_corde_);

    st->reduce_pa(player_id_, id_nain_, COUT_TIRER);

    const Rope& rope = st->map().get_rope_at(dest);
    position curr_pos = (sens_ == HAUT) ? rope.get_anchor() : rope.get_bottom();
    curr_pos = get_position_offset(curr_pos, reverse_direction(sens_));

    // Start moving dwarfs to their next position
    while (inside_map(curr_pos) && st->map().has_rope_at(curr_pos))
    {
        const int curr_occupant = st->map().get_cell_occupant(curr_pos);
        const position destination = get_position_offset(curr_pos, sens_);

        if (st->map().get_cell_occupant(destination) == 1 - curr_occupant)
        {
            curr_pos = get_position_offset(curr_pos, reverse_direction(sens_));
            continue;
        }

        const auto nains = st->map().get_nains_ids_at(curr_pos);

        for (int nain_id : nains)
            if (st->get_nain(curr_occupant, nain_id).accroche)
            {
                internal_action action;
                action.type = 7;
                action.fall = {curr_occupant, nain_id, destination};
                st->add_to_internal_history(player_id_, action);

                // Kill nain if on the opponent spawn
                if (destination == st->map().get_spawn_point(1 - curr_occupant))
                {
                    const int butin =
                        st->get_nain(curr_occupant, nain_id).butin;
                    if (butin > 0)
                        st->increase_score(1 - curr_occupant, nain.butin);
                    st->reduce_pv(curr_occupant, nain_id, VIE_NAIN, player_id_);
                    continue;
                }

                st->set_nain_position(curr_occupant, nain_id, destination);
            }

        curr_pos = get_position_offset(curr_pos, reverse_direction(sens_));
    }
}
