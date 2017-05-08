
function createM_M_Fighter(group, shield)
    local ship = createEnemy("ship/m/m.png", 3.0)
    table.insert(all_enemies, ship)
    ship.setCollisionCircle(1.4)
    ship.setHealth(2)
    ship.onControlUpdate(spaceInvaderController)
    ship.onWeaponUpdate(basicPulseWeapon)
    ship.onDestroy(spawnPickup())
    if shield then
        ship.setShield(1.0, 30, "ship/m/m_shield.png")
    end
    addEnemyToGroup(ship, group)
    ship.speed = 0.45
    return ship
end
