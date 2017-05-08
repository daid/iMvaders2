--Stage 2-2
--Re-hash of the origonal iMvaders
--100% filled with M Corperation enemies
--"Blast from the past"

include("util.lua")

function start()
    transmission(
        "[Face:daid]Welcome heroes|of earth",
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
    
    local group = {}
    update = basicEndOfWaveCheck(postWave1)
    createEnemyGroup(8, 0, 2.5, 8, function(group, position) createM_M_Fighter(group, true).target = position end)
    createEnemyGroup(5, 0, 2.5, 10, function(group, position) createM_M_Fighter(group).target = position end)
    createEnemyGroup(2,-8, 2.5, 4, function(group, position)
        ship = createM_M_Fighter(group)
        ship.target = position
        ship.onWeaponUpdate(chargeShotWeapon)
    end)
    createEnemyGroup(2, 8, 2.5, 4, function(group, position)
        ship = createM_M_Fighter(group)
        ship.target = position
        ship.onWeaponUpdate(chargeShotWeapon)
    end)
end

function postWave1()
	transmission(
        "[Face:daid]These M fighters|are better equiped|then us",
        "Good thing they|are stupid.",
        "Better destroy some|smaller 3D printers|and grab the upgrades",
        giveBonus1
    )
end

function giveBonus1()
-- Bonus powerup
    postBonus1()
end

function postBonus1()
	transmission(
        "[Face:daid]Large object inbound",
        "[Face:jaime]Watch out!|It is the|Replicator Z18",
        "[Face:daid]It does not|seem to do|anything",
        "Maybe the firmware|is not installed",
        "[Face:jaime]It is on a crash|course towards us",
        "Blast your way|trough it!",
        launchZ18
    )
end

function launchZ18()
-- Z18
end

function postZ18()
	transmission(
        "[Face:daid]More enemies inbound.",
        startWave2
    )
end

function startWave2()
-- Wave, 8 shielded 4 burst shielded, 2 digitizers
end

function postWave2()
-- Bonus powerup
end

function postPowerup2()
	transmission(
        "[Face:jaime]Watch out",
        "You are flying into|a field of abandoned|printers",
        startPrinterField
    )
end

--Start of printer field (+dive bombers?)

function startWave3()
    -- Wave, 2 digitizers
end

function startWave4()
    -- Wave, 3 digitizers, 8 enemies
end

function startWave5()
    -- Wave, 4 digitizers, 8 enemies, 8 enemies
end

function preBoss()
    transmission(
        "[Face:jaime]I am detecting a|huge ego on|the radar",
        "[Face:daid]Oh no!|It is him!",
        "[Face:jaime]Kill IT!!!",
        startBoss
    )
end

function startBoss()
    -- BRE
end

function postBoss()
    transmission(
        "[Face:daid]You did it|You saved the universe.",
        "[Face:henk]Thank you|            |But our princess|is in another castle!",
        stageDone
    )
end

start()
