#include "../actions.hh"
#include "../api.hh"

#include "test-helpers.hh"

TEST_F(ApiTest, ActionAgripperLacher)
{
    for (auto& player : players)
    {
        for (int nain_id = 0; nain_id < NB_NAINS; nain_id++)
        {
            int player_id = player.api->moi();
            nain nain = player.api->info_nain(player_id, nain_id);
            direction dir = (nain.pos.colonne < 15) ? DROITE : GAUCHE;

            // Try to let go
            EXPECT_EQ(COUT_DEPLACEMENT,
                      player.api->cout_deplacement(nain_id, dir));
            EXPECT_EQ(PAS_ACCROCHE, player.api->lacher(nain_id));

            // Grab
            EXPECT_EQ(OK, player.api->agripper(nain_id));
            EXPECT_EQ(NB_POINTS_ACTION - COUT_AGRIPPER,
                      player.api->info_nain(player_id, nain_id).pa);
            EXPECT_EQ(COUT_ESCALADER,
                      player.api->cout_deplacement(nain_id, dir));
            EXPECT_EQ(DEJA_ACCROCHE, player.api->agripper(nain_id));

            // Move right (next column has a rope)
            EXPECT_EQ(OK, player.api->deplacer(nain_id, dir));
            EXPECT_EQ(COUT_ESCALADER_CORDE,
                      player.api->cout_deplacement(nain_id, dir));

            // Move up
            nain = player.api->info_nain(player_id, nain_id);
            EXPECT_EQ(OK, player.api->deplacer(nain_id, HAUT));
            EXPECT_NE(nain.pos, player.api->info_nain(player_id, nain_id).pos);

            // Let go
            EXPECT_EQ(OK, player.api->lacher(nain_id));
            EXPECT_EQ(nain.pos, player.api->info_nain(player_id, nain_id).pos);
        }
    }
}
