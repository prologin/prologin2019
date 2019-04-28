#include "../actions.hh"
#include "../api.hh"

#include "test-helpers.hh"

TEST_F(ApiTest, Api_spawn_obsidienne)
{
    for (auto& player: players)
    {
        int id_player = player.api->moi();
        position spawn = player.api->position_taverne(id_player);
        position under_spawn = {spawn.ligne + 1, spawn.colonne};
        if (!inside_map(under_spawn))
            continue;
        EXPECT_EQ(player.api->type_case(under_spawn), OBSIDIENNE);
    }
}
