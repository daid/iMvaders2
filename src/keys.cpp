#include "keys.h"

PlayerKeys* player_keys[2];

PlayerKeys::PlayerKeys(int index)
: up("player_up_" + sp::string(index))
, down("player_down_" + sp::string(index))
, left("player_left_" + sp::string(index))
, right("player_right_" + sp::string(index))
, start("player_start_" + sp::string(index))
, primary_fire("player_primary_fire_" + sp::string(index))
, secondary_fire("player_secondary_fire_" + sp::string(index))
, skip("player_skip_" + sp::string(index))
, brake("player_brake_" + sp::string(index))
, hold_rotation("player_hold_rotation_" + sp::string(index))
{
}

void PlayerKeys::init()
{
    player_keys[0] = new PlayerKeys(0);
    player_keys[0]->up.setKey("Up");
    player_keys[0]->up.addKey("Keypad 8");
    player_keys[0]->down.setKey("Down");
    player_keys[0]->down.addKey("Keypad 2");
    player_keys[0]->left.setKey("Left");
    player_keys[0]->left.addKey("Keypad 4");
    player_keys[0]->right.setKey("Right");
    player_keys[0]->right.addKey("Keypad 6");
    player_keys[0]->start.setKey("1");
    player_keys[0]->primary_fire.setKey("Space");
    player_keys[0]->secondary_fire.setKey("Z");
    player_keys[0]->skip.setKey("X");
    player_keys[0]->brake.setKey("C");
    player_keys[0]->hold_rotation.setKey("V");
    //player_keys[0]->secondary_fire.setKey("B");
    
    player_keys[1] = new PlayerKeys(1);
    player_keys[1]->up.setKey("W");
    player_keys[1]->down.setKey("S");
    player_keys[1]->left.setKey("A");
    player_keys[1]->right.setKey("D");
    player_keys[1]->start.setKey("2");
    player_keys[1]->primary_fire.setKey("Q");
    player_keys[1]->secondary_fire.setKey("E");
    player_keys[1]->skip.setKey("R");
    player_keys[1]->brake.setKey("F");
    player_keys[1]->hold_rotation.setKey("T");
    //player_keys[1]->secondary_fire.setKey("G");
}
