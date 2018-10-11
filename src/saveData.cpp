#include "saveData.h"

SaveData* SaveData::instance;

SaveData::SaveData(int player_count)
{
    if (instance)
        delete instance;
    instance = this;
    
    this->player_count = player_count;
    for(int n=0; n<player_count; n++)
    {
        player_data.emplace_back();
    }
    
    score = 0;
    playtrough_count = 0;
    shop_unlocked = false;
}

int SaveData::unlockedStageLevel() const
{
    int level = 1;
    for(auto it : finished_stages)
    {
        level = std::max(level, it.first + 1);
    }
    return level;
}
