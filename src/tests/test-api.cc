#include "../actions.hh"
#include "../api.hh"

#include "test-helpers.hh"

// TODO add test for type_case

TEST_F(ApiTest, Api_Moi)
{
    for (auto& player : players)
        EXPECT_EQ(player.id, player.api->moi());
}

TEST_F(ApiTest, Api_Adversaire)
{
    for (int player_id = 0; player_id < 2; player_id++)
    {
        int expected = players[(player_id + 1) % 2].id;
        EXPECT_EQ(expected, players[player_id].api->adversaire());
    }
}

TEST_F(ApiTest, Api_TourActuel)
{
    for (int round = 0; round < NB_TOURS; round++)
    {
        for (auto& player : players)
            EXPECT_EQ(round, player.api->tour_actuel());
        st->increment_round();
    }
}

TEST_F(ApiTest, Api_info_minerai)
{
    for (auto& player : players)
    {
        minerai minerai = player.api->info_minerai({10, 10});
        EXPECT_EQ(minerai.rendement, 10);
        EXPECT_EQ(minerai.resistance, 10);
    }
}

TEST_F(ApiTest, Api_list_minerais)
{
    for (auto& player : players)
    {
        for (position pos : player.api->liste_minerais())
        {
            minerai minerai = player.api->info_minerai(pos);
            EXPECT_EQ(player.api->type_case(pos), GRANITE);
            EXPECT_NE(minerai.resistance, -1);
            EXPECT_NE(minerai.rendement, -1);
        }

        // Ensure that the spawn is not a mineral
        int id_player = player.api->moi();
        position spawn = player.api->position_taverne(id_player);
        minerai minerai = player.api->info_minerai(spawn);
        EXPECT_EQ(minerai.resistance, -1);
        EXPECT_EQ(minerai.rendement, -1);
    }
}

TEST_F(ApiTest, Api_nain_sur_case)
{
    for (auto& player : players)
    {
        int moi = player.api->moi();
        position spawn = player.api->position_taverne(moi);
        EXPECT_EQ(player.api->nain_sur_case(spawn), moi);
    }
}

TEST_F(ApiTest, Api_info_nain)
{
    for (auto& player : players)
    {
        for (int nain_id = 0; nain_id < NB_NAINS; nain_id++)
        {
            int moi = player.api->moi();
            nain nain = player.api->info_nain(moi, nain_id);
            EXPECT_EQ(nain.vie, VIE_NAIN);
            EXPECT_EQ(nain.pa, NB_POINTS_ACTION);
            EXPECT_EQ(nain.pm, NB_POINTS_DEPLACEMENT);
            EXPECT_EQ(nain.accroche, false);
            EXPECT_EQ(nain.butin, 0);
        }
    }
}

TEST_F(ApiTest, Api_cout_de_deplacement)
{
    for (auto& player : players)
    {
        for (int nain_id = 0; nain_id < NB_NAINS; nain_id++)
        {
            int player_id = player.api->moi();
            nain nain = player.api->info_nain(player_id, nain_id);

            // Try moving towards correct direction
            direction dir = (nain.pos.colonne < 15) ? DROITE : GAUCHE;
            EXPECT_EQ(player.api->cout_de_deplacement(nain_id, dir),
                      COUT_DEPLACEMENT);

            // Try moving towards the ground, moving an invalid dwarf id
            EXPECT_EQ(player.api->cout_de_deplacement(nain_id, BAS), -1);
            EXPECT_EQ(player.api->cout_de_deplacement(NB_NAINS, dir), -1);
        }
    }
}

TEST_F(ApiTest, Api_position_taverne)
{
    for (auto& player : players)
    {
        position pos = player.api->position_taverne(-1);
        EXPECT_EQ(pos.ligne, -1);
        EXPECT_EQ(pos.colonne, -1);
    }
}

TEST_F(ApiTest, Api_historique)
{
    for (auto& player : players)
    {
        EXPECT_EQ(player.api->historique().size(), (size_t)0);
    }
}

TEST_F(ApiTest, Api_score)
{
    for (auto& player : players)
    {
        int player_id = player.api->moi();
        EXPECT_EQ(player.api->score(player_id), 0);
    }
}

TEST_F(ApiTest, Api_moi_adversaire)
{
    int player1_id, player2_id;

    player1_id = players[0].api->moi();
    player2_id = players[0].api->adversaire();

    EXPECT_NE(player1_id, player2_id);
    EXPECT_EQ(players[1].api->moi(), player2_id);
    EXPECT_EQ(players[1].api->adversaire(), player1_id);
}

TEST_F(ApiTest, Api_annuler)
{
    for (auto& player : players)
    {
        EXPECT_EQ(player.api->annuler(), false);
    }
}

TEST_F(ApiTest, Api_tour_actuel)
{
    for (auto& player : players)
    {
        EXPECT_EQ(player.api->tour_actuel(), 0);
    }
}
