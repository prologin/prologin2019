#include "../actions.hh"
#include "../api.hh"

#include "test-helpers.hh"

TEST_F(ApiTest, ActionPoserTirerCorde)
{
    for (auto& player : players)
    {
        const int player_id = player.api->moi();
        const nain nain_initial = player.api->info_nain(player_id, 0);
        const direction dir = (nain_initial.pos.colonne < 15) ? DROITE : GAUCHE;

        // The first dwarf digy-digy-digs a hole
        for (int i = 0; i < 5; i++)
        {
            EXPECT_EQ(OK, player.api->deplacer(0, dir));
            player.api->game_state()->reset_pm(
                player.api->game_state()->get_player_id(player_id));
        }

        for (int i = 0; i < 3; i++)
        {
            EXPECT_EQ(OK, player.api->miner(0, BAS));
            player.api->game_state()->reset_pa(
                player.api->game_state()->get_player_id(player_id));
        }

        // The second dwarf joins in the hole and grabs wall
        for (int i = 0; i < 5; i++)
        {
            EXPECT_EQ(OK, player.api->deplacer(1, dir));
            player.api->game_state()->reset_pm(
                player.api->game_state()->get_player_id(player_id));
        }

        EXPECT_EQ(player.api->info_nain(player_id, 0).pos,
                  player.api->info_nain(player_id, 1).pos);
        EXPECT_EQ(OK, player.api->agripper(1));

        // The third dwarfs comes and puts a rope
        for (int i = 0; i < 4; i++)
        {
            EXPECT_EQ(OK, player.api->deplacer(2, dir));
            player.api->game_state()->reset_pm(
                player.api->game_state()->get_player_id(player_id));
        }

        EXPECT_EQ(OK, player.api->poser_corde(2, dir));
        EXPECT_EQ(true, player.api->corde_sur_case(
                            player.api->info_nain(player_id, 0).pos));
        player.api->game_state()->reset_pa(
            player.api->game_state()->get_player_id(player_id));

        // The third dwarf moves the rope up
        EXPECT_EQ(OK, player.api->tirer(2, dir, HAUT));
        EXPECT_EQ(player.api->info_nain(player_id, 0).pos.ligne,
                  player.api->info_nain(player_id, 1).pos.ligne + 1);
        player.api->game_state()->reset_pa(
            player.api->game_state()->get_player_id(player_id));

        // The third dwarf moves the rope down
        EXPECT_EQ(OK, player.api->tirer(2, dir, BAS));
        EXPECT_EQ(player.api->info_nain(player_id, 0).pos,
                  player.api->info_nain(player_id, 1).pos);
        player.api->game_state()->reset_pa(
            player.api->game_state()->get_player_id(player_id));

        // Witch puts him at the bottom of the map ...
        EXPECT_EQ(OK, player.api->tirer(2, dir, BAS));
        EXPECT_EQ(OK, player.api->tirer(2, dir, BAS));
        EXPECT_EQ(player.api->info_nain(player_id, 0).pos,
                  player.api->info_nain(player_id, 1).pos);
        player.api->game_state()->reset_pa(
            player.api->game_state()->get_player_id(player_id));

        // This is the same when moving as far as possible to top
        EXPECT_EQ(OK, player.api->tirer(2, dir, HAUT));
        EXPECT_EQ(OK, player.api->tirer(2, dir, HAUT));
        EXPECT_EQ(OK, player.api->tirer(2, dir, HAUT));
        player.api->game_state()->reset_pa(
            player.api->game_state()->get_player_id(player_id));
        EXPECT_EQ(nain_initial.pos.ligne,
                  player.api->info_nain(player_id, 1).pos.ligne);

        EXPECT_EQ(OK, player.api->tirer(2, dir, HAUT));
        EXPECT_EQ(nain_initial.pos.ligne,
                  player.api->info_nain(player_id, 1).pos.ligne);
        player.api->game_state()->reset_pa(
            player.api->game_state()->get_player_id(player_id));
    }
}
