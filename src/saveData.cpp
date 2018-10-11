#include "saveData.h"
#include <sp2/logging.h>

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
    //TODO: Ugly hardcoded data
    if (level > 2) level = 2;
    return level;
}

void SaveData::checkForPlaytroughDone()
{
    //TODO: Ugly hardcoded data
    if (finished_stages.find(std::pair<int, int>(1, 0)) == SaveData::instance->finished_stages.end())
        return;
    if (finished_stages.find(std::pair<int, int>(2, 1)) == SaveData::instance->finished_stages.end())
        return;
    if (finished_stages.find(std::pair<int, int>(2, 2)) == SaveData::instance->finished_stages.end())
        return;

    finished_stages.clear();
    playtrough_count++;
}
