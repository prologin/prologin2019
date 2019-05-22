#include <cmath>

#include "../actions.hh"
#include "../api.hh"

#include "test-helpers.hh"

TEST_F(ApiTest, ActionDeplacer_Effective)
{
    for (auto& player : players)
    {
        for (int nain_id = 0; nain_id < NB_NAINS; nain_id++)
        {
            int player_id = player.api->moi();
            nain nain_before = player.api->info_nain(player_id, nain_id);
            direction dir = (nain_before.pos.colonne < 15) ? DROITE : GAUCHE;
            nain nain_after;

            player.api->deplacer(nain_id, dir);
            nain_after = player.api->info_nain(player_id, nain_id);
            EXPECT_EQ(abs(nain_before.pos.colonne - nain_after.pos.colonne), 1);
            EXPECT_EQ(nain_after.pm, NB_POINTS_DEPLACEMENT - COUT_DEPLACEMENT);

            player.api->deplacer(nain_id, dir);
            player.api->deplacer(nain_id, dir);
            nain_after = player.api->info_nain(player_id, nain_id);
            EXPECT_EQ(abs(nain_before.pos.colonne - nain_after.pos.colonne), 3);
            EXPECT_EQ(nain_after.pm,
                      NB_POINTS_DEPLACEMENT - 3 * COUT_DEPLACEMENT);
        }
    }
}

TEST_F(ApiTest, ActionDeplacer_NotEnoughActionPoints)
{
    for (auto& player : players)
    {
        for (int nain_id = 0; nain_id < NB_NAINS; nain_id++)
        {
            int player_id = player.api->moi();
            nain nain = player.api->info_nain(player_id, nain_id);
            direction dir = (nain.pos.colonne < 15) ? DROITE : GAUCHE;

            for (int i = 0; i < NB_POINTS_DEPLACEMENT / COUT_DEPLACEMENT; i++)
            {
                erreur err = player.api->deplacer(nain_id, dir);
                EXPECT_EQ(err, OK);
            }

            erreur err = player.api->deplacer(nain_id, dir);
            EXPECT_EQ(err, PM_INSUFFISANTS);
        }
    }
}

TEST_F(ApiTest, ActionDeplacer_InvalidDirections)
{
    for (auto& player : players)
    {
        for (int nain_id = 0; nain_id < NB_NAINS; nain_id++)
        {
            erreur err;
            int player_id = player.api->moi();

            nain nain = player.api->info_nain(player_id, nain_id);
            direction dir = (nain.pos.colonne < 15) ? GAUCHE : DROITE;

            err = player.api->deplacer(nain_id, dir);
            EXPECT_EQ(err, HORS_LIMITES);

            err = player.api->deplacer(nain_id, HAUT);
            EXPECT_EQ(err, OK);

            err = player.api->deplacer(nain_id, BAS);
            EXPECT_EQ(err, OBSTACLE_MUR);
        }
    }
}
