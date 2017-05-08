#ifndef SAVE_DATA_H
#define SAVE_DATA_H

#include <set>
#include <vector>

class SaveData
{
public:
    int player_count;
    
    int score;
    int pla;
    
    class PlayerData
    {
    public:
        PlayerData()
        : hull(1.0)
        {}
        
        float hull;
    };
    
    std::set<std::pair<int, int>> finished_stages;
    std::vector<PlayerData> player_data;
    
    SaveData(int player_count);
    
    int unlockedStageLevel() const;
    
    static SaveData* instance;
};

#endif//SAVE_DATA_H
