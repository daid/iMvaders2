#include "saveData.h"
#include <sp2/logging.h>
#include <sp2/io/resourceProvider.h>


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
#ifdef DEBUG
    level += 10;
#endif
    return level;
}

bool SaveData::checkForPlaytroughDone()
{
    for(int level = 1; stageExists(level, 0) || stageExists(level, 1); level++)
    {
        if (stageExists(level, 0))
        {
            if (finished_stages.find(std::pair<int, int>(level, 0)) == SaveData::instance->finished_stages.end())
                return false;
        }
        else
        {
            for(int sublevel=1; stageExists(level, sublevel); sublevel++)
            {
                if (finished_stages.find(std::pair<int, int>(level, sublevel)) == SaveData::instance->finished_stages.end())
                    return false;
            }
        }
    }

    finished_stages.clear();
    playtrough_count++;
    return true;
}

bool SaveData::stageExists(int level, int sublevel)
{
    if (sublevel == 0)
        return sp::io::ResourceProvider::get("stages/stage" + sp::string(level) + ".lua") != nullptr;
    return sp::io::ResourceProvider::get("stages/stage" + sp::string(level) + "-" + sp::string(sublevel) + ".lua") != nullptr;
}

sp::string SaveData::getStageTitle(int level, int sublevel)
{
    sp::io::ResourceStreamPtr stream;
    stream = sp::io::ResourceProvider::get("stages/stage" + sp::string(level) + "-" + sp::string(sublevel) + ".lua");
    if (!stream)
        stream = sp::io::ResourceProvider::get("stages/stage" + sp::string(level) + ".lua");
    if (!stream)
        return "?";
    return stream->readLine().substr(2);
}
