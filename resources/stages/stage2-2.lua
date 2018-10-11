--Stage 2-2
--Re-hash of the origonal iMvaders
--100% filled with M Corperation enemies
--"Blast from the past"

-- Wave 1 - 8 shielded enemies, 10 normal enemies, 2x4 burst shot enemies.
-- Bonus powerup
-- Z18
-- Wave 2 - 8 shielded 4 burst shielded, 2 digitizers
-- Bonus powerup
--Start of printer field (TODO: dive bombers)
-- Wave 3 - 2 digitizers
-- Wave 4 - 3 digitizers, 8 enemies
-- Wave 5 - 4 digitizers, 8 enemies, 8 enemies
--End printer field
-- Boss

include("util.lua")

function start()
    transmission(
        "[Face:peter]Welcome heroes|of earth",
        "You misssion is to|destroy the|M corperation",
        "It will be hazardous|so take care",
        "[Face:jaime]INCOMMING ENEMIES",
        startWave1
    )
end

-- Wave 1:
-- 8 shielded enemies, 10 normal enemies, 2x4 burst shot enemies.
function startWave1()
    log("Wave 1")
    
    update = basicEndOfWaveCheck(postWave1)
    createEnemyGroup(8, 0, 2.5, 8, function(group, position) createM_M_Fighter(group, true).target = position end)
    createEnemyGroup(5, 0, 2.5, 10, function(group, position) createM_M_Fighter(group).target = position end)
    createEnemyGroup(2,-8, 2.5, 4, function(group, position) createM_M_FighterBurst(group).target = position end)
    createEnemyGroup(2, 8, 2.5, 4, function(group, position) createM_M_FighterBurst(group).target = position end)
end

function postWave1()
	transmission(
        "[Face:peter]These M fighters|are better equiped|then us",
        "Good thing they|are stupid.",
        "Better destroy some|smaller 3D printers|and grab the upgrades",
        giveBonus1
    )
end

function giveBonus1()
    -- Bonus powerup
    update = basicEndOfWaveCheck(postBonus1)
    createM_Rep2(1)
    createM_Rep2(-1)
end

function postBonus1()
    update = delayUpdate(100, function() transmission(
        "[Face:peter]Large object inbound",
        "[Face:jaime]Watch out!|It is the|Replicator Z18",
        "[Face:peter]It does not|seem to do|anything",
        "Maybe the firmware|is not installed",
        "[Face:jaime]It is on a crash|course towards us",
        launchZ18,  --We launch this thing early. Just to mess with you.
        "Blast your way|trough it!"
    ) end)
end

function launchZ18()
    -- Z18
    update = basicEndOfWaveCheck(postZ18)
    table.insert(all_enemies, createSpecial("Z18"))
end

function postZ18()
	transmission(
        "[Face:peter]More enemies inbound.",
        startWave2
    )
end

function startWave2()
    -- Wave, 8 shielded 4 burst shielded, 2 digitizers
    update = basicEndOfWaveCheck(postWave2)
    
    createEnemyGroup(8, 0, 12, 2, function(group, position) createM_Digitizer(group).target = position end)
    createEnemyGroup(3, 0, 2.5, 8, function(group, position) createM_M_Fighter(group, true).target = position end)
    
    createEnemyGroup(0.5,-8, 2.5, 2, function(group, position) createM_M_FighterBurst(group, true).target = position end)
    createEnemyGroup(0.5, 8, 2.5, 2, function(group, position) createM_M_FighterBurst(group, true).target = position end)
end

function postWave2()
    -- Bonus powerup
    update = basicEndOfWaveCheck(postBonus2)
    createM_Rep2()
end

function postBonus2()
    update = delayUpdate(100, function() transmission(
        "[Face:jaime]Watch out",
        "You are flying into|a field of abandoned|printers",
        startPrinterField
    ) end)
end

--Start of printer field (+dive bombers?)
function startPrinterField()
    update = printerFieldUpdate(delayUpdate(240, startWave3))
end

function printerFieldUpdate(next)
    return function()
        if abandoned_printer_count_down and abandoned_printer_count_down > 0 then
            abandoned_printer_count_down = abandoned_printer_count_down - 1
        else
            createSpecial("AbandondedPrinter")
            abandoned_printer_count_down = 60
        end
        next()
    end
end

function startWave3()
    -- Wave, 2 digitizers
    update = printerFieldUpdate(basicEndOfWaveCheck(function() update = printerFieldUpdate(delayUpdate(60, startWave4)) end))
    createEnemyGroup(8, 0, 12, 2, function(group, position) createM_Digitizer(group).target = position end)
end

function startWave4()
    -- Wave, 3 digitizers, 8 enemies
    update = printerFieldUpdate(basicEndOfWaveCheck(function() update = printerFieldUpdate(delayUpdate(60, startWave5)) end))
    createEnemyGroup(3, 0, 2.5, 8, function(group, position) createM_M_Fighter(group).target = position end)
    createEnemyGroup(8, 0, 10, 3, function(group, position) createM_Digitizer(group).target = position end)
end

function startWave5()
    -- Wave, 4 digitizers, 8 enemies, 8 enemies
    update = printerFieldUpdate(basicEndOfWaveCheck(function() update = delayUpdate(120, preBoss) end))
    createEnemyGroup(0, -2.5, 2.5, 6, function(group, position) createM_M_Fighter(group).target = position end)
    createEnemyGroup(3, 2.5, 2.5, 6, function(group, position) createM_M_Fighter(group).target = position end)
    createEnemyGroup(8, 0, 8, 4, function(group, position) createM_Digitizer(group).target = position end)
end

function preBoss()
    transmission(
        "[Face:jaime]I am detecting a|huge ego on|the radar",
        "[Face:peter]Oh no!|It is him!",
        "[Face:jaime]Kill IT!!!",
        startBoss
    )
end

function startBoss()
    update = basicEndOfWaveCheck(postBoss)
    createM_Bre()
end

function postBoss()
    transmission(
        "[Face:peter]You did it|You saved the universe.",
        "[Face:martijn]Thank you|            |            ",
        "[Face:martijn]But our princess|is in another castle!",
        stageDone
    )
end

--TODO: Bonus game

start()
--postWave2()
--startWave5()
--startPrinterField()
--startBoss()
