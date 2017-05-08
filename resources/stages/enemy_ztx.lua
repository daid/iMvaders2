
function createZTX_X_Fighter(group, has_wingman)
    local ship = createEnemy("ship/ztx/x.png", 2.5)
    table.insert(all_enemies, ship)
    ship.setCollisionBox(2.5, 2.5)
    ship.setHealth(4)
    ship.onControlUpdate(spaceInvaderController)
    ship.onWeaponUpdate(dualPulseWeapon)
    ship.onDestroy(spawnPickup())
    addEnemyToGroup(ship, group)
    ship.speed = 0.35
    if has_wingman then
        createZTX_Z_FighterWingman(ship, Vector2(0.5,-2))
        createZTX_Z_FighterWingman(ship, Vector2(0.5, 2))
    end
    return ship
end

function createZTX_Z_FighterWingman(dady, offset)
    local ship = createEnemy("ship/ztx/z.png", 1.5)
    table.insert(all_enemies, ship)
    ship.setCollisionBox(1.5, 1.2)
    ship.setHealth(2)
    ship.onControlUpdate(wingmanController)
    ship.onWeaponUpdate(basicPulseWeapon)
    ship.onDestroy(spawnPickup(0.5))
    ship.offset = offset
    ship.master = dady
    ship.speed = 0.5
    return ship
end

function createZTX_Z_Fighter(group)
    local ship = createEnemy("ship/ztx/z.png", 1.5)
    table.insert(all_enemies, ship)
    ship.setCollisionBox(1.5, 1.2)
    ship.setHealth(2)
    ship.onControlUpdate(spaceInvaderController)
    ship.onWeaponUpdate(basicPulseWeapon)
    ship.onDestroy(spawnPickup(0.5))
    addEnemyToGroup(ship, group)
    ship.speed = 0.5
    return ship
end

function createZTX_T_Fighter(y)
    local ship = createEnemy("ship/ztx/t.png", 3.5)
    table.insert(all_enemies, ship)
    ship.setCollisionBox(3.5, 2.2)
    ship.setHealth(10)
    ship.onControlUpdate(inAndOutController)
    ship.center_time = 150
    ship.onWeaponUpdate(electroBoltWeapon)
    ship.onDestroy(spawnPickup(2))
    ship.speed = 0.5
    if y ~= nil then
        ship.target = Vector2(4, y)
    end
    return ship
end
