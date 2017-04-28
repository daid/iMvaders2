#ifndef SAVE_DATA_H
#define SAVE_DATA_H

#include <set>

class SaveData
{
public:
    int player_count;
    
    int score;
    int pla;
    
    std::set<std::pair<int, int>> finished_stages;
    
    SaveData(int player_count);
    
    int unlockedStageLevel() const;
    
    static SaveData* instance;
};

#endif//SAVE_DATA_H
