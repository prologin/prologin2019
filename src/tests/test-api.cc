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
        minerai minerai = player.api->info_minerai({ 10, 10 });
        EXPECT_EQ(minerai.rendement, 10);
        EXPECT_EQ(minerai.resistance, 10);
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
