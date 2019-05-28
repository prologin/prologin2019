#include "../actions.hh"
#include "../api.hh"

#include "test-helpers.hh"

#define CHECK_CANCEL_ON_NAIN(player, nain_id, action)                          \
    {                                                                          \
        const nain nain_before_ =                                              \
            player.api->info_nain(player.api->moi(), nain_id);                 \
                                                                               \
        action;                                                                \
        EXPECT_EQ(true, player.api->annuler());                                \
                                                                               \
        EXPECT_EQ(nain_before_.pos,                                            \
                  player.api->info_nain(player.api->moi(), nain_id).pos);      \
        EXPECT_EQ(nain_before_.vie,                                            \
                  player.api->info_nain(player.api->moi(), nain_id).vie);      \
        EXPECT_EQ(nain_before_.pa,                                             \
                  player.api->info_nain(player.api->moi(), nain_id).pa);       \
        EXPECT_EQ(nain_before_.pm,                                             \
                  player.api->info_nain(player.api->moi(), nain_id).pm);       \
        EXPECT_EQ(nain_before_.accroche,                                       \
                  player.api->info_nain(player.api->moi(), nain_id).accroche); \
        EXPECT_EQ(nain_before_.butin,                                          \
                  player.api->info_nain(player.api->moi(), nain_id).butin);    \
    }

#define CHECK_CANCEL_ON_MAP(player, action)                                    \
    {                                                                          \
        const Map map_before_ = player.api->game_state()->map();               \
                                                                               \
        action;                                                                \
        EXPECT_EQ(true, player.api->annuler());                                \
                                                                               \
        for (int row = 0; row < TAILLE_MINE; row++)                            \
            for (int col = 0; col < TAILLE_MINE; col++)                        \
            {                                                                  \
                EXPECT_EQ(map_before_.get_minerai_at({row, col}).resistance,   \
                          player.api->game_state()                             \
                              ->map()                                          \
                              .get_minerai_at({row, col})                      \
                              .resistance);                                    \
                EXPECT_EQ(map_before_.get_minerai_at({row, col}).rendement,    \
                          player.api->game_state()                             \
                              ->map()                                          \
                              .get_minerai_at({row, col})                      \
                              .rendement);                                     \
                EXPECT_EQ(map_before_.get_cell_type({row, col}),               \
                          player.api->game_state()->map().get_cell_type(       \
                              {row, col}));                                    \
            }                                                                  \
    }

#define CHECK_CANCEL(player, action)                                           \
    CHECK_CANCEL_ON_MAP(player, action)                                        \
                                                                               \
    for (int nain_id__ = 0; nain_id__ < NB_NAINS; nain_id__++)                 \
    {                                                                          \
        CHECK_CANCEL_ON_NAIN(player, nain_id__, action)                        \
    }

TEST_F(ApiTest, Annuler_EmptyHist)
{
    for (auto& player : players)
    {
        const int player_id = player.api->moi();
        const nain nain_initial = player.api->info_nain(player_id, 0);
        const direction dir = (nain_initial.pos.colonne < 15) ? DROITE : GAUCHE;

        for (int nain_id = 0; nain_id < NB_NAINS; nain_id++)
        {
            EXPECT_EQ(OK, player.api->deplacer(nain_id, dir));
            EXPECT_EQ(OK, player.api->deplacer(nain_id, dir));
            EXPECT_EQ(OK, player.api->deplacer(nain_id, dir));

            EXPECT_EQ(true, player.api->annuler());
            EXPECT_EQ(true, player.api->annuler());
            EXPECT_EQ(true, player.api->annuler());
            EXPECT_EQ(false, player.api->annuler());
            EXPECT_EQ(false, player.api->annuler());
        }
    }
}

TEST_F(ApiTest, Annuler_Movements)
{
    for (auto& player : players)
    {
        const int player_id = player.api->moi();
        const nain nain_initial = player.api->info_nain(player_id, 0);
        const direction dir = (nain_initial.pos.colonne < 15) ? DROITE : GAUCHE;

        for (int nain_id = 0; nain_id < NB_NAINS; nain_id++)
        {
            CHECK_CANCEL(player,
                         EXPECT_EQ(OK, player.api->deplacer(nain_id, dir)));
            CHECK_CANCEL(player,
                         EXPECT_EQ(OK, player.api->deplacer(nain_id, HAUT)));

            EXPECT_EQ(OK, player.api->deplacer(nain_id, dir));
            CHECK_CANCEL(player,
                         EXPECT_EQ(OK, player.api->deplacer(nain_id, HAUT)));
        }
    }
}

TEST_F(ApiTest, Annuler_Miner)
{
    for (auto& player : players)
    {
        const int player_id = player.api->moi();
        const nain nain_initial = player.api->info_nain(player_id, 0);
        const direction dir = (nain_initial.pos.colonne < 15) ? DROITE : GAUCHE;

        for (int nain_id = 0; nain_id < NB_NAINS; nain_id++)
        {
            EXPECT_EQ(OK, player.api->deplacer(nain_id, dir));

            // Digs first cell on bottom
            CHECK_CANCEL(player, player.api->miner(nain_id, BAS));
            EXPECT_EQ(OK, player.api->miner(nain_id, BAS));

            // The bottom mineral has 5 hit points
            for (int i = 0; i < 5; i++)
            {
                player.api->game_state()->reset_pa(
                    player.api->game_state()->get_player_id(player_id));

                CHECK_CANCEL(player, player.api->miner(nain_id, BAS));
                EXPECT_EQ(OK, player.api->miner(nain_id, BAS));
            }

            while (player.api->annuler())
                ;
        }
    }
}

TEST_F(ApiTest, Annuler_Chute)
{
    for (auto& player : players)
    {
        const int player_id = player.api->moi();
        const nain nain_initial = player.api->info_nain(player_id, 0);
        const direction dir = (nain_initial.pos.colonne < 15) ? DROITE : GAUCHE;

        // The first dwarf digs a hole on right
        EXPECT_EQ(OK, player.api->deplacer(0, dir));

        for (int i = 0; i < 3; i++)
        {
            EXPECT_EQ(OK, player.api->miner(0, BAS));
            player.api->game_state()->reset_pa(
                player.api->game_state()->get_player_id(player_id));
        }

        // SECOND DWARF DO THE FLOP
        CHECK_CANCEL(player, player.api->deplacer(1, dir));

        // Dig more to make it deadly
        for (int i = 0; i < 10; i++)
        {
            EXPECT_EQ(OK, player.api->miner(0, BAS));
            player.api->game_state()->reset_pa(
                player.api->game_state()->get_player_id(player_id));
        }

        // SECOND DWARF DO THE (DEADLY) FLOP
        CHECK_CANCEL(player, player.api->deplacer(1, dir));
    }
}

TEST_F(ApiTest, Annuler_ActionsCorde)
{
    for (auto& player : players)
    {
        const int player_id = player.api->moi();
        const nain nain_initial = player.api->info_nain(player_id, 0);
        const direction dir = (nain_initial.pos.colonne < 15) ? DROITE : GAUCHE;

        // The first dwarf digs a hole on right with a rope on it
        CHECK_CANCEL(player, player.api->poser_corde(0, dir));
        EXPECT_EQ(OK, player.api->poser_corde(0, dir));
        player.api->game_state()->reset_pa(
            player.api->game_state()->get_player_id(player_id));

        EXPECT_EQ(OK, player.api->deplacer(0, dir));

        for (int i = 0; i < 3; i++)
        {
            CHECK_CANCEL(player, player.api->miner(0, BAS));
            EXPECT_EQ(OK, player.api->miner(0, BAS));
            player.api->game_state()->reset_pa(
                player.api->game_state()->get_player_id(player_id));
        }

        // The first dwarf extends the rope with another one
        EXPECT_EQ(OK, player.api->miner(0, dir));
        EXPECT_EQ(OK, player.api->deplacer(0, dir));
        player.api->game_state()->reset_pa(
            player.api->game_state()->get_player_id(player_id));

        EXPECT_EQ(OK, player.api->miner(0, dir));
        player.api->game_state()->reset_pa(
            player.api->game_state()->get_player_id(player_id));

        EXPECT_EQ(OK, player.api->miner(0, BAS));
        player.api->game_state()->reset_pa(
            player.api->game_state()->get_player_id(player_id));

        EXPECT_EQ(OK, player.api->miner(0, BAS));
        player.api->game_state()->reset_pa(
            player.api->game_state()->get_player_id(player_id));

        EXPECT_EQ(OK, player.api->miner(0, reverse_direction(dir)));
        player.api->game_state()->reset_pa(
            player.api->game_state()->get_player_id(player_id));

        EXPECT_EQ(OK, player.api->poser_corde(0, reverse_direction(dir)));
        player.api->game_state()->reset_pa(
            player.api->game_state()->get_player_id(player_id));

        EXPECT_EQ(OK, player.api->deplacer(0, reverse_direction(dir)));

        // Diging top should merge two ropes
        CHECK_CANCEL(player, player.api->miner(0, HAUT));
        EXPECT_EQ(OK, player.api->miner(0, HAUT));

        // First dwarf holds the rope
        CHECK_CANCEL(player, player.api->agripper(0));
        EXPECT_EQ(OK, player.api->agripper(0));

        // Second dwarf pulls the rope
        for (int i = 0; i < 10; i++)
        {
            CHECK_CANCEL(player, player.api->tirer(1, dir, HAUT));
            EXPECT_EQ(OK, player.api->tirer(1, dir, HAUT));
            player.api->game_state()->reset_pa(
                player.api->game_state()->get_player_id(player_id));
        }
    }
}

TEST_F(ApiTest, Cancel_Allie)
{
    for (auto& player : players)
    {
        const int player_id = player.api->moi();
        const nain nain_initial = player.api->info_nain(player_id, 0);
        const direction dir = (nain_initial.pos.colonne < 15) ? DROITE : GAUCHE;

        EXPECT_EQ(OK, player.api->deplacer(0, dir));

        // Hit once
        CHECK_CANCEL(player, player.api->miner(0, reverse_direction(dir)));

        // Kill !!!
        for (int i = 0; i < (VIE_NAIN + DEGAT_PIOCHE - 1) / DEGAT_PIOCHE; i++)
        {
            CHECK_CANCEL(player, player.api->miner(0, reverse_direction(dir)));
            EXPECT_EQ(OK, player.api->miner(0, reverse_direction(dir)));
            player.api->game_state()->reset_pa(
                player.api->game_state()->get_player_id(player_id));
        }

        EXPECT_EQ(PAS_DE_NAIN, player.api->miner(0, reverse_direction(dir)));
    }
}
