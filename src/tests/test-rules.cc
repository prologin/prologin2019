#include "test-helpers.hh"

TEST_F(RulesTest, Rules_Finish)
{
    int round = 0;
    while (!rules->is_finished())
    {
        rules->start_of_round();

        rules->start_of_player_turn(PLAYER_ID_1);
        rules->end_of_player_turn(PLAYER_ID_1);

        EXPECT_FALSE(rules->is_finished());

        rules->start_of_player_turn(PLAYER_ID_2);
        rules->end_of_player_turn(PLAYER_ID_2);

        EXPECT_EQ(round, rules->get_game_state().get_round());
        rules->end_of_round();

        round++;
    }
    EXPECT_EQ(NB_TOURS, round);
}
