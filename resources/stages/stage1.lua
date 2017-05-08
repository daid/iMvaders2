--Stage 1
--"Welcome back to the jungle"

include("util.lua")

--Simple stage that serves as introduction
--Main enemy: Ztx
--Also makes an appearance: M Corperation

--Wave1: 2 X fighters with 2 Z wingman each
--Wave2: 4 X fighters with 2 Z wingman each, and 2 rows of single Z fighters.

function start()
    transmission(
        "[Face:marvin]Welcome back heroes of earth",
        "I hope you are well rested after the|defeat of the M Corperation",
        "Your first mission will be to cleanup|some remaining ZTX ships from|this sector",
        "It should not be a difficult task.|And it will give you some time|to get your bearings with your new ship",

        "[Face:jaime]First ships inbound!",
        startWave1
    )
end

function startWave1()
    log("Wave 1")
    
    update = basicEndOfWaveCheck(postWave1)
    createEnemyGroup(4,-8, 0, 1, function(group, position) createZTX_X_Fighter(group, true).target = position end)
    createEnemyGroup(4, 8, 0, 1, function(group, position) createZTX_X_Fighter(group, true).target = position end)
end

function postWave1()
    transmission(
        "[Face:marvin]Good start.",
        "These Z fighters are weak,|but they produced a|lot of cheap ones!",
        "Prepare for a large|group of them!",
        startWave2
    )
end

function startWave2()
    log("Wave 2")
    
    update = basicEndOfWaveCheck(postWave2)
    createEnemyGroup(3, 0, 6.6, 4, function(group, position) createZTX_X_Fighter(group, true).target = position end)
    createEnemyGroup(6,-7, 2, 6, function(group, position) createZTX_Z_Fighter(group).target = position end)
    createEnemyGroup(6, 7, 2, 6, function(group, position) createZTX_Z_Fighter(group).target = position end)
end

function postWave2()
    transmission(
        "[Face:jaime]I'm detecting electrical interference",
        "These new ships should never|have passed safety tests!",
        startWave3
    )
end

function startWave3()
    log("Wave 3")
    
    update = basicEndOfWaveCheck(postWave3)
    createZTX_T_Fighter(-5)
    createZTX_T_Fighter(5)
end

--start()
--startWave1()
--startWave2()
startWave3()
