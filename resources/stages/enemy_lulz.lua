
function createLulz_Lulz_Fighter(group, shield)
    local ship = createEnemy("ship/lulz/lulz.png", 3.0)
    table.insert(all_enemies, ship)
    ship.setCollisionCircle(1.2)
    ship.setHealth(3)
    ship.onControlUpdate(spaceInvaderController)
    ship.onWeaponUpdate(basicPulseWeapon)
    ship.onDestroy(spawnPickup())
    if shield then
        ship.setShield(2.0, 50, "ship/lulz/lulz_shield.png")
    end
    addEnemyToGroup(ship, group)
    ship.speed = 0.4
    return ship
end

function createLulz_Lulz_FighterBurst(group, shield)
    local ship = createLulz_Lulz_Fighter(group, shield)
    ship.onWeaponUpdate(chargeShotWeapon)
    return ship
end

function createLulz_Lulz_Spinner(shield)
    local ship = createEnemy("ship/lulz/lulz.png", 4.0)
    table.insert(all_enemies, ship)
    ship.setCollisionCircle(1.5)
    ship.setHealth(6)
    ship.onControlUpdate(inAndOutController)
    ship.onWeaponUpdate(lulzTrippleShot)
    ship.onDestroy(spawnPickup())
    if shield then
        ship.setShield(3.0, 40, "ship/lulz/lulz_shield.png")
    end
    ship.speed = 0.35
    ship.center_time = 200
    return ship
end

function lulzTrippleShot(self)
    if self.weapon_delay_modifier == nil then self.weapon_delay_modifier = 1.0 end
    if self.weapon_delay == nil then
        self.weapon_delay = random(70, 150) * self.weapon_delay_modifier
        self.weapon_state = "pre-fire"
        self.weapon_spin_counter = 0
        self.spin_direction = 1
        if random(0, 100) < 50 then self.spin_direction = -1 end
    end
    
    self.setRotation(self.weapon_spin_counter * 2.0 + 180)
    if not self.fire then return end
    
    self.weapon_spin_counter = self.weapon_spin_counter + self.spin_direction
    if self.weapon_state == "pre-fire" then
        if self.weapon_delay > 0 then
            self.weapon_delay = self.weapon_delay - 1
        else
            self.weapon_delay = random(30, 35)
            self.weapon_state = "firing"
        end
    elseif self.weapon_state == "firing" then
        if self.weapon_delay > 0 then
            self.weapon_delay = self.weapon_delay - 1
        else
            self.weapon_delay = random(70, 150) * self.weapon_delay_modifier
            self.weapon_state = "pre-fire"
        end
        self.createProjectile("PULSE", 0, 0, 0);
        self.createProjectile("PULSE", 0, 0, 120);
        self.createProjectile("PULSE", 0, 0,-120);
    end
end
