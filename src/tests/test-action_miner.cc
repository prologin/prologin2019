#include "../actions.hh"
#include "../api.hh"

#include "test-helpers.hh"

TEST_F(ApiTest, ActionMiner_Impossible)
{
    for (auto& player : players)
        for (int nain_id = 0; nain_id < NB_NAINS; nain_id++)
        {
            int player_id = player.api->moi();
            nain nain = player.api->info_nain(player_id, nain_id);
            direction ext_dir = (nain.pos.colonne < 15) ? GAUCHE : DROITE;

            EXPECT_EQ(OBSTACLE_MUR, player.api->miner(nain_id, BAS));
            EXPECT_EQ(PAS_DE_CIBLE, player.api->miner(nain_id, HAUT));
            EXPECT_EQ(HORS_LIMITES, player.api->miner(nain_id, ext_dir));
        }
}

TEST_F(ApiTest, ActionMiner_Recolte)
{
    for (auto& player : players)
    {
        const int player_id = player.api->moi();
        const int nain_id = 0;
        const nain nain_initial = player.api->info_nain(player_id, nain_id);
        const direction dir = (nain_initial.pos.colonne < 15) ? DROITE : GAUCHE;

        // Mine towards bottom
        EXPECT_EQ(OK, player.api->deplacer(nain_id, dir));
        EXPECT_EQ(OK, player.api->miner(nain_id, BAS));
        EXPECT_EQ(nain_initial.pos.ligne + 1,
                  player.api->info_nain(player_id, nain_id).pos.ligne);
        EXPECT_EQ(0, player.api->info_nain(player_id, nain_id).butin);

        // Damage mineral
        const nain nain = player.api->info_nain(player_id, nain_id);

        for (int hit = 0; hit < 4; hit++)
        {
            player.api->game_state()->reset_pa(
                player.api->game_state()->get_player_id(player_id));
            EXPECT_EQ(OK, player.api->miner(nain_id, BAS));
            EXPECT_EQ(0, player.api->info_nain(player_id, nain_id).butin);
            EXPECT_EQ(nain.pos, player.api->info_nain(player_id, nain_id).pos);
        }

        // Break mineral
        player.api->game_state()->reset_pa(
            player.api->game_state()->get_player_id(player_id));
        EXPECT_EQ(OK, player.api->miner(nain_id, BAS));
        EXPECT_EQ(BUTIN_MAX, player.api->info_nain(player_id, nain_id).butin);
        EXPECT_EQ(nain_initial.pos.ligne + 2,
                  player.api->info_nain(player_id, nain_id).pos.ligne);
    }
}

TEST_F(ApiTest, ActionMiner_Damage)
{
    const int NB_WARIORS = 3;

    // Move towards center
    for (auto& player : players)
    {
        const int player_id = player.api->moi();
        const nain nain_initial = player.api->info_nain(player_id, 0);
        const direction dir = (nain_initial.pos.colonne < 15) ? DROITE : GAUCHE;

        // Move forward
        for (int i = 0; i < TAILLE_MINE - 2; i++)
        {
            for (int nain_id = 0; nain_id < NB_WARIORS; nain_id++)
                EXPECT_EQ(OK, player.api->deplacer(nain_id, dir));

            player.api->game_state()->reset_pm(
                player.api->game_state()->get_player_id(player_id));
        }

        for (int nain_id = 0; nain_id < NB_WARIORS; nain_id++)
            EXPECT_EQ(OBSTACLE_NAIN, player.api->deplacer(nain_id, dir));

        // Hit
        EXPECT_EQ(OK, player.api->miner(0, dir));
        EXPECT_EQ(VIE_NAIN - DEGAT_PIOCHE,
                  player.api->info_nain(player.api->adversaire(), 0).vie);
    }
}
