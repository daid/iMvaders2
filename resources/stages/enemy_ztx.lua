
function createZTX_X_Fighter(group, has_wingman)
    local ship = createEnemy("ship/ztx/x.png", 2.5)
    table.insert(all_enemies, ship)
    ship.setCollisionBox(2.5, 2.5)
    ship.setHealth(4)
    ship.onControlUpdate(spaceInvaderController)
    ship.onWeaponUpdate(dualPulseWeapon)
    ship.onDestroy(spawnPickup())
    addEnemyToGroup(ship, group)
    ship.speed = 0.175
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
    ship.speed = 0.25
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
    ship.speed = 0.25
    return ship
end

function createZTX_T_Fighter(y)
    local ship = createEnemy("ship/ztx/t.png", 3.5)
    table.insert(all_enemies, ship)
    ship.setCollisionBox(3.5, 2.2)
    ship.setHealth(10)
    ship.onControlUpdate(inAndOutController)
    ship.center_time = 300
    ship.onWeaponUpdate(electroBoltWeapon)
    ship.onDestroy(spawnPickup(2))
    ship.speed = 0.25
    if y ~= nil then
        ship.target = Vector2(4, y)
    end
    return ship
end

function createZTX_Boss()
    --The ZTX boss is like an union, it has layers.
    --The center core is a large T.
    --Next to it are two large X parts.
    --The X parts are protected by Z parts.

    local center = createEnemy("ship/ztx/t.png", 6.5)
    table.insert(all_enemies, center)
    center.setCollisionBox(5, 2.8)
    center.setHealth(8)
    center.setBoss()
    center.onControlUpdate(ztxBossController)
    center.onWeaponUpdate(electroBoltWeapon)
    center.target = Vector2(3, 0)
    center.speed = 0.10

    center.left_x = createEnemy("ship/ztx/x.png", 3.5)
    table.insert(all_enemies, center.left_x)
    center.left_x.setCollisionBox(3.8, 3)
    center.left_x.setHealth(8)
    center.left_x.setBoss()
    center.left_x.onControlUpdate(wingmanController)
    center.left_x.onWeaponUpdate(dualPulseWeapon)
    center.left_x.weapon_delay_modifier = 0.5
    center.left_x.offset = Vector2(-1, 3)
    center.left_x.master = center

    center.right_x = createEnemy("ship/ztx/x.png", 3.5)
    table.insert(all_enemies, center.right_x)
    center.right_x.setCollisionBox(3.8, 3)
    center.right_x.setHealth(8)
    center.right_x.setBoss()
    center.right_x.onControlUpdate(wingmanController)
    center.right_x.onWeaponUpdate(dualPulseWeapon)
    center.right_x.weapon_delay_modifier = 0.5
    center.right_x.offset = Vector2(-1, -3)
    center.right_x.master = center

    center.left_z = createEnemy("ship/ztx/z.png", 1.5)
    table.insert(all_enemies, center.left_z)
    center.left_z.setCollisionBox(1, 1)
    center.left_z.setHealth(4)
    center.left_z.setBoss()
    center.left_z.onControlUpdate(wingmanController)
    center.left_z.onWeaponUpdate(basicPulseWeapon)
    center.left_z.weapon_delay_modifier = 0.4
    center.left_z.offset = Vector2(-1.5, 5.2)
    center.left_z.master = center

    center.right_z = createEnemy("ship/ztx/z.png", 1.5)
    table.insert(all_enemies, center.right_z)
    center.right_z.setCollisionBox(1, 1)
    center.right_z.setHealth(4)
    center.right_x.setBoss()
    center.right_z.onControlUpdate(wingmanController)
    center.right_z.onWeaponUpdate(basicPulseWeapon)
    center.right_z.weapon_delay_modifier = 0.4
    center.right_z.offset = Vector2(-1.5, -5.2)
    center.right_z.master = center

    center.left_z2 = createEnemy("ship/ztx/z.png", 1.5)
    table.insert(all_enemies, center.left_z2)
    center.left_z2.setCollisionBox(1, 1)
    center.left_z2.setHealth(4)
    center.left_z2.setBoss()
    center.left_z2.onControlUpdate(wingmanController)
    center.left_z2.onWeaponUpdate(basicPulseWeapon)
    center.left_z2.weapon_delay_modifier = 0.4
    center.left_z2.offset = Vector2(1.7, 2.2)
    center.left_z2.master = center

    center.right_z2 = createEnemy("ship/ztx/z.png", 1.5)
    table.insert(all_enemies, center.right_z2)
    center.right_z2.setCollisionBox(1, 1)
    center.right_z2.setHealth(4)
    center.right_z2.setBoss()
    center.right_z2.onControlUpdate(wingmanController)
    center.right_z2.onWeaponUpdate(basicPulseWeapon)
    center.right_z2.weapon_delay_modifier = 0.4
    center.right_z2.offset = Vector2(1.7, -2.2)
    center.right_z2.master = center

    center.setPosition(Vector2(22, 0))
    
    center.onDestroy(spawnPickup(12))
    return center
end

function ztxBossController(self)
    local speed = self.speed
    local position = self.getPosition()
    if self.weapon_state == "charge" then speed = speed * 0.5 end
    if self.weapon_state == "fire" then speed = speed * 0.0 end
    if position.x < 0 then speed = speed * (1.0 - position.x / 10) end
    
    self.setPosition(position + Vector2(speed, 0):rotate(self.getRotation()))
    if position.x < -22 then
        self.setPosition(Vector2(22, random(-10, 10)))
        self.setRotation(random(160, 200))
    end
    self.fire = position.x < 15

    self.setInvincible(self.left_x.valid or self.right_x.valid)
    if self.left_x.valid then
        self.left_x.setInvincible(self.left_z.valid or self.left_z2.valid)
    end
    if self.right_x.valid then
        self.right_x.setInvincible(self.right_z.valid or self.right_z2.valid)
    end
end
