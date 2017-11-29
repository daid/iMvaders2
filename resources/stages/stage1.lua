--Stage 1
--"Welcome back to the jungle"

include("util.lua")
include("stage1_pickup.lua")

--Simple stage that serves as introduction
--Main enemy: Ztx
--Also makes an appearance: M Corperation

--Wave1: 2 X fighters with 2 Z wingman each
--Wave2: 4 X fighters with 2 Z wingman each, and 2 rows of single Z fighters.
--Wave3: 2 T fighters
--Wave4: 3 T fighters, 10 m fighters
--Wave5: 4 X fighters with 2 Z wingman each, 2 T fighters, and 1 rows of single Z fighters.
--Boss

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
    
    if getPlaytroughCount() == 0 then
        update = basicEndOfWaveCheck(startExplainPickup)
    else
        update = basicEndOfWaveCheck(postWave1)
    end
    createEnemyGroup(4,-8, 0, 1, function(group, position) createZTX_X_Fighter(group, true).target = position end)
    createEnemyGroup(4, 8, 0, 1, function(group, position) createZTX_X_Fighter(group, true).target = position end)
    
    if getPlaytroughCount() == 0 then
        for _, enemy in ipairs(all_enemies) do
            enemy.onDestroy(nil)
        end
    end
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

function postWave3()
    transmission(
        "[Face:jaime]What is this?|I though we defeated the M corperation!",
        "[Face:marvin]They must be trying to make allies.",
        "Too late for that, just blast them to bits!",
        startWave4
    )
end

function startWave4()
    log("Wave 4")
    
    update = basicEndOfWaveCheck(postWave4)
    createEnemyGroup(1, 0, 2.5, 10, function(group, position) createM_M_Fighter(group).target = position end)
    
    createZTX_T_Fighter(-7)
    createZTX_T_Fighter(0)
    createZTX_T_Fighter(7)
end

function postWave4()
    update = delayUpdate(50, startWave5)
end

function startWave5()
    log("Wave 5")
    update = basicEndOfWaveCheck(postWave5)

    createEnemyGroup(8, 0, 2, 10, function(group, position) createZTX_Z_Fighter(group).target = position end)
    createEnemyGroup(5, 0, 5, 4, function(group, position) createZTX_X_Fighter(group).target = position end)
    
    createZTX_T_Fighter(-16)
    createZTX_T_Fighter(16)
end

function postWave5()
    transmission(
        "[Face:jaime]Large object inbound!",
        "[Face:marvin]Looks like they just|mashed something together.",
        "Watch out.|This could be a tough fight.",
        startBoss
    )
end

function startBoss()
    log("Boss")
    
    update = basicEndOfWaveCheck(postBoss)
    createZTX_Boss()
    if getPlaytroughCount() > 0 then
        createZTX_Boss().setPosition(52, random(-16, 16))
    end
end

function postBoss()
    transmission(
        "[Face:jaime]You did it!|The mothership is no more.",
        "[Face:marvin]Well done commander.",
        "Your mission here is complete|See you in the next round.",
        stageDone
    )
end

start()
--startExplainPickup()
--startBoss()
