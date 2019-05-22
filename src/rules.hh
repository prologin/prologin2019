#ifndef RULES_RULES_HH_
#define RULES_RULES_HH_

#include <memory>

#include <rules/actions.hh>
#include <rules/client-messenger.hh>
#include <rules/options.hh>
#include <rules/player.hh>
#include <rules/rules.hh>
#include <rules/server-messenger.hh>
#include <utils/dll.hh>
#include <utils/sandbox.hh>

#include "api.hh"

typedef void (*f_champion_partie_init)();
typedef void (*f_champion_jouer_tour)();
typedef void (*f_champion_partie_fin)();

class Rules : public rules::TurnBasedRules
{
public:
    explicit Rules(const rules::Options opt);
    virtual ~Rules() {}

    rules::Actions* get_actions() override;
    void apply_action(const rules::IAction_sptr& action) override;
    bool is_finished() override;

    void at_player_start(rules::ClientMessenger_sptr) override;
    void at_spectator_start(rules::ClientMessenger_sptr) override;
    void at_player_end(rules::ClientMessenger_sptr) override;
    void at_spectator_end(rules::ClientMessenger_sptr) override;

    void player_turn() override;
    void spectator_turn() override;

    void start_of_player_turn(unsigned int player_id) override;
    void end_of_player_turn(unsigned int player_id) override;

    void start_of_round() override;
    void end_of_round() override;

    void dump_state(std::ostream& out) override;

    GameState* get_game_state() const;

protected:
    f_champion_partie_init champion_partie_init_;
    f_champion_jouer_tour champion_jouer_tour_;
    f_champion_partie_fin champion_partie_fin_;

private:
    void register_actions();

    std::unique_ptr<utils::DLL> champion_dll_;
    std::unique_ptr<Api> api_;
    utils::Sandbox sandbox_;
};

#endif // !RULES_RULES_HH_
