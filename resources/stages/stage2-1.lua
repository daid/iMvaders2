--STAGE 2-1: For the lulz
--Lulzbot area.

-- Wave 1: 8 shielded enemies, 10 normal enemies, 2x4 burst shot enemies.
-- Wave 2: 4 spinners
-- Wave 3: 2 spinners, empty V formation of enemies
--Start space octopusses

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

function postWave3()
    update = delayUpdate(100, function() transmission(
        "[Face:lola]Lets see how you like these!",
        "[Face:harma]What? Space octopuses?|You have to be kidding me.",
        "[Face:harma]Touching them will drain your energy,|so be careful!",
        startOctoField
    ) end)
end

function startOctoField()
    octo_group = {}
    update = octoFieldUpdate(delayUpdate(240, startWave4))
end

function octoFieldUpdate(next)
    return function()
        if octo_count_down and octo_count_down > 0 then
            octo_count_down = octo_count_down - 1
        else
            local ship = createEnemy("ship/lulz/octo.png", 5.0)
            ship.setCollisionCircle(2.2)
            ship.setHealth(2)
            ship.setPosition(Vector2(-22, 0))
            ship.setTouchDamage(0.4, "energyDrain")
            ship.speed = 0.3
            ship.onControlUpdate(aimedDiveBomberController)
            ship.setDrawOrder(-1)
            table.insert(octo_group, ship)

            octo_count_down = 160
        end
        next()
    end
end

function startWave4()
    update = octoFieldUpdate(basicEndOfWaveCheck(function() update = octoFieldUpdate(delayUpdate(60, startWave5)) end))
    createEnemyGroup(8, 0, 4, 5, function(group, position) createLulz_Lulz_Fighter(group).target = position end)
    createEnemyGroup(5, 0, 4, 3, function(group, position) createLulz_Lulz_Fighter({}, true).target = position end)
end

function startWave5()
    update = octoFieldUpdate(basicEndOfWaveCheck(function() update = octoFieldEnd(postWave5) end))
    createEnemyGroup(8, 0, 8, 2, function(group, position) createLulz_Lulz_Spinner(true).target = position end)
    createEnemyGroup(5, 0, 4, 5, function(group, position) createLulz_Lulz_Fighter(group, true).target = position end)
end

function octoFieldEnd(next)
    return function()
        for _, enemy in ipairs(octo_group) do
            if enemy.valid then
                return
            end
        end
        update = nil
        next()
    end
end

function postWave5()
    update = delayUpdate(300, function() transmission(
        "[Face:harma]There it is, the lulz-base!",
        "[Face:harma]Go get it, take it out.|No matter the cost.",
        "[Face:lola]There is no stopping us!|We will be victorious!",
        startBoss
    ) end)
end

function startBoss()
    update = basicEndOfWaveCheck(postBoss)
    createLulz_Lulz_Boss()
end

function postBoss()
    transmission(
        "[Face:harma]You did it|You eliminated the lulz force!",
        "[Face:lola]We will be back!",
        "[Face:marvin]We will drive you back|again.",
        stageDone
    )
end

start()
--startBoss()
--startWave2()
