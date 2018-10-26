#ifndef SAVE_DATA_H
#define SAVE_DATA_H

#include <set>
#include <vector>

#include <sp2/string.h>

class SaveData
{
public:
    int player_count;
    
    int score;
    int playtrough_count;
    bool shop_unlocked;
    
    class PlayerData
    {
    public:
        PlayerData()
        : hull(1.0), pla(0)
        {}
        
        float hull;
        int pla;
    };
    
    std::set<std::pair<int, int>> finished_stages;
    std::vector<PlayerData> player_data;
    
    SaveData(int player_count);
    
    int unlockedStageLevel() const;
    bool checkForPlaytroughDone();//returns true if the game was reset due to all stages done.

    bool stageExists(int level, int sublevel);
    sp::string getStageTitle(int level, int sublevel);
    
    static SaveData* instance;
};

#endif//SAVE_DATA_H
