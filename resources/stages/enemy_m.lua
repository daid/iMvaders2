
function createM_M_Fighter(group, shield)
    local ship = createEnemy("ship/m/m.png", 3.0)
    table.insert(all_enemies, ship)
    ship.setCollisionCircle(1.4)
    ship.setHealth(2)
    ship.onControlUpdate(spaceInvaderController)
    ship.onWeaponUpdate(basicPulseWeapon)
    ship.onDestroy(spawnPickup(0.7))
    if shield then
        ship.setShield(1.0, 30, "ship/m/m_shield.png")
    end
    addEnemyToGroup(ship, group)
    ship.speed = 0.45
    return ship
end

function createM_Rep2(direction)
    local ship = createEnemy("ship/m/rep2.png", 4.0)
    table.insert(all_enemies, ship)
    ship.onControlUpdate(sinusWaveController)
    ship.setCollisionBox(3.0, 4.0)
    ship.setHealth(3)
    ship.onDestroy(spawnPickup(3))
    direction = direction or random(-1, 1)
    if direction > 0 then
        ship.speed = 0.3
    else
        ship.speed = -0.3
    end
    return ship
end

function createM_Digitizer(group)
    local ship = createEnemy("ship/m/digitizer.png", 5.5)
    table.insert(all_enemies, ship)
    ship.setCollisionBox(5, 4)
    ship.setHealth(6)
    ship.onControlUpdate(inAndOutController)
    ship.center_time = 150
    ship.onWeaponUpdate(digitizerLaserWeapon)
    ship.onDestroy(spawnPickup(2))
    ship.speed = 0.4
    return ship
end

function digitizerLaserWeapon(self)
    if not self.fire then
        self.weapon_state = nil
    end
    if self.weapon_state == nil then
        self.weapon_state = "pre_fire"
        self.weapon_delay = 20
    elseif self.weapon_state == "pre_fire" then
        if self.weapon_delay > 0 then
            self.weapon_delay = self.weapon_delay - 1
        else
            self.weapon_state = "charge"
            self.weapon_delay = 90

            local p = Vector2(-1.8,-1.6) + Vector2(25, 0):rotate(25 + 180)
            self.createProjectile("PRELASER", p.x, p.y, 25);
            self.createProjectile("PRELASER", p.x,-p.y,-25);
        end
    elseif self.weapon_state == "charge" then
        if self.weapon_delay > 0 then
            self.weapon_delay = self.weapon_delay - 1
        else
            local p = Vector2(-1.8,-1.6) + Vector2(25, 0):rotate(25 + 180)
            self.createProjectile("LASER", p.x, p.y, 25);
            self.createProjectile("LASER", p.x,-p.y,-25);
            self.weapon_state = "fire"
            self.weapon_delay = 10
        end
    elseif self.weapon_state == "fire" then
        if self.weapon_delay > 0 then
            self.weapon_delay = self.weapon_delay - 1
        else
            self.weapon_state = "pre_fire"
            self.weapon_delay = random(100, 300)
        end
    end
end
