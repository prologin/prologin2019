#include "../actions.hh"
#include "../api.hh"
#include "../position.hh"

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
            player.api->game_state().reset_pm(
                player.api->game_state().get_player_id(player_id));
        }

        for (int i = 0; i < 3; i++)
        {
            EXPECT_EQ(OK, player.api->miner(0, BAS));
            player.api->game_state().reset_pa(
                player.api->game_state().get_player_id(player_id));
        }

        // The second dwarf joins in the hole and grabs wall
        for (int i = 0; i < 5; i++)
        {
            EXPECT_EQ(OK, player.api->deplacer(1, dir));
            player.api->game_state().reset_pm(
                player.api->game_state().get_player_id(player_id));
        }

        EXPECT_EQ(player.api->info_nain(player_id, 0).pos,
                  player.api->info_nain(player_id, 1).pos);
        EXPECT_EQ(OK, player.api->agripper(1));

        // The third dwarfs comes and puts a rope
        for (int i = 0; i < 4; i++)
        {
            EXPECT_EQ(OK, player.api->deplacer(2, dir));
            player.api->game_state().reset_pm(
                player.api->game_state().get_player_id(player_id));
        }

        EXPECT_EQ(OK, player.api->poser_corde(2, dir));
        EXPECT_EQ(true, player.api->corde_sur_case(
                            player.api->info_nain(player_id, 0).pos));
        player.api->game_state().reset_pa(
            player.api->game_state().get_player_id(player_id));

        // The third dwarf moves the rope up
        EXPECT_EQ(OK, player.api->tirer(2, dir, HAUT));
        EXPECT_EQ(player.api->info_nain(player_id, 0).pos.ligne,
                  player.api->info_nain(player_id, 1).pos.ligne + 1);
        player.api->game_state().reset_pa(
            player.api->game_state().get_player_id(player_id));

        // The third dwarf moves the rope down
        EXPECT_EQ(OK, player.api->tirer(2, dir, BAS));
        EXPECT_EQ(player.api->info_nain(player_id, 0).pos,
                  player.api->info_nain(player_id, 1).pos);
        player.api->game_state().reset_pa(
            player.api->game_state().get_player_id(player_id));

        // Witch puts him at the bottom of the map ...
        EXPECT_EQ(OK, player.api->tirer(2, dir, BAS));
        EXPECT_EQ(OK, player.api->tirer(2, dir, BAS));
        EXPECT_EQ(player.api->info_nain(player_id, 0).pos,
                  player.api->info_nain(player_id, 1).pos);
        player.api->game_state().reset_pa(
            player.api->game_state().get_player_id(player_id));

        // This is the same when moving as far as possible to top
        EXPECT_EQ(OK, player.api->tirer(2, dir, HAUT));
        EXPECT_EQ(OK, player.api->tirer(2, dir, HAUT));
        EXPECT_EQ(OK, player.api->tirer(2, dir, HAUT));
        player.api->game_state().reset_pa(
            player.api->game_state().get_player_id(player_id));
        EXPECT_EQ(nain_initial.pos.ligne,
                  player.api->info_nain(player_id, 1).pos.ligne);

        EXPECT_EQ(OK, player.api->tirer(2, dir, HAUT));
        EXPECT_EQ(nain_initial.pos.ligne,
                  player.api->info_nain(player_id, 1).pos.ligne);
        player.api->game_state().reset_pa(
            player.api->game_state().get_player_id(player_id));
    }
}

TEST_F(ApiTest, MergeCorde)
{
    for (auto& player : players)
    {
        const int player_id = player.api->moi();
        const nain nain_initial = player.api->info_nain(player_id, 0);
        const direction dir = (nain_initial.pos.colonne < 15) ? DROITE : GAUCHE;

        // The first dwarf digs a hook-shaped path
        EXPECT_EQ(OK, player.api->deplacer(0, dir));
        EXPECT_EQ(OK, player.api->deplacer(0, dir));

        for (int i = 0; i < 2; i++)
        {
            EXPECT_EQ(OK, player.api->miner(0, BAS));
            player.api->game_state().reset_pa(
                player.api->game_state().get_player_id(player_id));
        }

        while (OK == player.api->miner(0, reverse_direction(dir)))
            // I put a mineral on wanted direction, it takes time to break it
            player.api->game_state().reset_pa(
                player.api->game_state().get_player_id(player_id));

        EXPECT_EQ(OK, player.api->poser_corde(0, reverse_direction(dir)));
        player.api->game_state().reset_pa(
            player.api->game_state().get_player_id(player_id));

        // The first dwarf goes in a hole under the rope
        EXPECT_EQ(OK, player.api->deplacer(0, reverse_direction(dir)));
        EXPECT_EQ(OK, player.api->miner(0, BAS));
        EXPECT_EQ(OK, player.api->agripper(0));

        player.api->game_state().reset_pa(
            player.api->game_state().get_player_id(player_id));
        player.api->game_state().reset_pm(
            player.api->game_state().get_player_id(player_id));

        // The second dwarf puts a rope and then digs a hole above the first one
        EXPECT_EQ(OK, player.api->poser_corde(1, dir));
        player.api->game_state().reset_pa(
            player.api->game_state().get_player_id(player_id));

        EXPECT_EQ(OK, player.api->deplacer(1, dir));
        player.api->game_state().reset_pa(
            player.api->game_state().get_player_id(player_id));

        EXPECT_EQ(OK, player.api->miner(1, BAS));
        player.api->game_state().reset_pa(
            player.api->game_state().get_player_id(player_id));

        EXPECT_EQ(player.api->info_nain(player_id, 0).pos,
                  player.api->info_nain(player_id, 1).pos);

        // The third dwarf pulls the rope
        EXPECT_EQ(OK, player.api->tirer(2, dir, HAUT));
        EXPECT_EQ(OK, player.api->tirer(2, dir, HAUT));
        EXPECT_EQ(OK, player.api->tirer(2, dir, HAUT));

        EXPECT_EQ(player.api->info_nain(player_id, 0).pos.ligne,
                  player.api->info_nain(player_id, 2).pos.ligne);

        EXPECT_NE(player.api->info_nain(player_id, 1).pos.ligne,
                  player.api->info_nain(player_id, 2).pos.ligne);
    }
}
