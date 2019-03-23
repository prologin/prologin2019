#include "rules.hh"

#include <fstream>

#include "actions.hh"

Rules::Rules(const rules::Options opt)
    : TurnBasedRules(opt), sandbox_(opt.time)
{
    if (!opt.champion_lib.empty())
    {
        champion_dll_ = std::make_unique<utils::DLL>(opt.champion_lib);

        // FIXME: register user functions
    }

    std::ifstream ifs(opt.map_file);
    if (!ifs.is_open())
        FATAL("Cannot open file: %s", opt.map_file.c_str());

    GameState* game_state = new GameState(ifs, opt.players);
    api_ = std::make_unique<Api>(game_state, opt.player);
    register_actions();
}

void Rules::register_actions()
{
    api_->actions()->register_action(
        ID_ACTION_DEPLACER,
        []() -> rules::IAction* { return new ActionDeplacer(); }
        );
    api_->actions()->register_action(
        ID_ACTION_ESCALADER,
        []() -> rules::IAction* { return new ActionEscalader(); }
        );
    api_->actions()->register_action(
        ID_ACTION_GRIMPER,
        []() -> rules::IAction* { return new ActionGrimper(); }
        );
    api_->actions()->register_action(
        ID_ACTION_LACHER,
        []() -> rules::IAction* { return new ActionLacher(); }
        );
    api_->actions()->register_action(
        ID_ACTION_MINER,
        []() -> rules::IAction* { return new ActionMiner(); }
        );
    api_->actions()->register_action(
        ID_ACTION_TIRER,
        []() -> rules::IAction* { return new ActionTirer(); }
        );
    api_->actions()->register_action(
        ID_ACTION_POSER_CORDE,
        []() -> rules::IAction* { return new ActionPoserCorde(); }
        );
    api_->actions()->register_action(
        ID_ACTION_DEBUG_AFFICHER_DRAPEAU,
        []() -> rules::IAction* { return new ActionDebugAfficherDrapeau(); }
        );
}

rules::Actions* Rules::get_actions()
{
    return api_->actions();
}

void Rules::apply_action(const rules::IAction_sptr& action)
{
    // When receiving an action, the API should have already checked that it
    // is valid. We recheck that for the current gamestate here to avoid weird
    // client/server desynchronizations and make sure the gamestate is always
    // consistent across the clients and the server.

    int err = action->check(api_->game_state());
    if (err)
        FATAL("Synchronization error: received action %d from player %d, but "
              "check() on current gamestate returned %d.",
              action->id(), action->player_id(), err);
    api_->game_state_set(action->apply(api_->game_state()));
}

bool Rules::is_finished()
{
    // FIXME
    return true;
}
