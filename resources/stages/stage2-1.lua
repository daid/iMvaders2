--Stage 2-1
--Lulzbot area.
--"For the lulz"

include("util.lua")

function start()
    transmission(
        "[Face:marvin]Today we are heading out into|lulz territory",
        "[Face:marvin]This should be pieceful.|We allied with them a while back",
        "[Face:lola]We are the Lulz|Resistance is futile|Prepare to be extruded",
        "[Face:jaime]We should have know|cats cannot be trusted!",
        startWave1
    )
end

-- Wave 1:
-- 8 shielded enemies, 10 normal enemies, 2x4 burst shot enemies.
function startWave1()
    log("Wave 1")
    
    update = basicEndOfWaveCheck(postWave1)
    
    createEnemyGroup(8, 0, 3.5, 4, function(group, position) createLulz_Lulz_FighterBurst({}).target = position end)
    createEnemyGroup(5, 0, 3.5, 3, function(group, position) createLulz_Lulz_Fighter({}).target = position end)
    createEnemyGroup(2, 0, 3.5, 2, function(group, position) createLulz_Lulz_Fighter({}).target = position end)
    createEnemyGroup(-1, 0, 3.5, 1, function(group, position) createLulz_Lulz_Fighter({}, true).target = position end)

    local left_group = {}
    createEnemyGroup(8, 13, 3.5, 3, function(group, position) createLulz_Lulz_Fighter(left_group).target = position end)
    createEnemyGroup(5, 13, 3.5, 2, function(group, position) createLulz_Lulz_Fighter(left_group).target = position end)
    createEnemyGroup(2, 13, 3.5, 1, function(group, position) createLulz_Lulz_Fighter(left_group, true).target = position end)

    local right_group = {}
    createEnemyGroup(8,-13, 3.5, 3, function(group, position) createLulz_Lulz_Fighter(right_group).target = position end)
    createEnemyGroup(5,-13, 3.5, 2, function(group, position) createLulz_Lulz_Fighter(right_group).target = position end)
    createEnemyGroup(2,-13, 3.5, 1, function(group, position) createLulz_Lulz_Fighter(right_group, true).target = position end)
end

function postWave1()
    transmission(
        "[Face:lola]Pur the lulz!",
        "[Face:marvin]We should blast them just|for their bad puns.",
        "[Face:peter]I wanted to shoot them|for their choice of colors.",
        startWave2
    )
end

function startWave2()
    log("Wave 2")
    
    update = basicEndOfWaveCheck(postWave2)
    
    createEnemyGroup(8, 0, 3.5, 4, function(group, position) createLulz_Lulz_Spinner(true).target = position end)
end

function postWave2()
    transmission(
        "[Face:lola]Exterminate,|Exterminate,|EXTERMINATE!",
        "[Face:harma]Up yours!|     |Silly cats!",
        startWave3
    )
end

function startWave3()
    log("Wave 3")
    
    update = basicEndOfWaveCheck(postWave3)
    
    createEnemyGroup(8, 0, 6.5, 2, function(group, position) createLulz_Lulz_Spinner(true).target = position end)
    
    local main_group = {}
    createEnemyGroup(8, 0, 3.5 * 4, 2, function(group, position) createLulz_Lulz_FighterBurst({}).target = position end)
    createEnemyGroup(5, 0, 3.5 * 3, 2, function(group, position) createLulz_Lulz_Fighter({}).target = position end)
    createEnemyGroup(2, 0, 3.5 * 2, 2, function(group, position) createLulz_Lulz_Fighter({}).target = position end)
    createEnemyGroup(-1, 0, 3.5, 2, function(group, position) createLulz_Lulz_Fighter({}, true).target = position end)
    createEnemyGroup(-4, 0, 3.5, 1, function(group, position) createLulz_Lulz_Fighter({}, true).target = position end)
end

start()
--postWave2()
