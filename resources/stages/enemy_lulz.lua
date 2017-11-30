
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

function createLulz_Lulz_Boss()
    local center = createEnemy("ship/lulz/core.png", 10.0)
    table.insert(all_enemies, center)
    center.setCollisionCircle(3.8)
    center.setHealth(50)
    center.setBoss()
    center.onControlUpdate(lulzBossController)
    center.target = Vector2(3, 0)
    center.slow_speed = 0.10
    center.high_speed = 0.40
    center.setPosition(25, 0)

    center.shields = {}
    center.target_angle = 3
    center.shield_regen_delay = 1000
    
    center.shield_regen_counter = center.shield_regen_delay

    for n=1,6 do
        local shield = createEnemy("ship/lulz/core-shield.png", 5.5)
        shield.setCollisionBox(2.7, 5)
        shield.setPosition(25, 0)
        shield.setHealth(10)
        shield.setDrawOrder(1)
        center.shields[n] = shield
    end
end

function lulzBossController(self)
    local position = Vector2(self.getPosition())
    if position.x > 5 then
        position.x = position.x - self.slow_speed
    else
        position.x = position.x - self.high_speed
    end
    if position.x < -25 then
        position.x = 25
        position.y = random(-15, 15)
    end
    self.setPosition(position:unpack())
    local adiff = angleDiff(self.getRotation(), self.target_angle * 360 / 6)
    if adiff < -1 then
        self.setRotation(self.getRotation() - 1)
    elseif adiff > 1 then
        self.setRotation(self.getRotation() + 1)
    else
        self.setRotation(self.target_angle * 360 / 6)

        local shield_counts = {}
        for m=1,6 do
            local a = m * 360 / 6
            local shield_count = 0
            for n=1,6 do
                local adiff = angleDiff(-a, n * 360 / 6)
                if math.abs(adiff) < 80 and self.shields[n].valid then
                    shield_count = shield_count + 1
                end
            end
            shield_counts[m] = shield_count
        end
        
        if shield_counts[self.target_angle] < 3 then
            self.target_angle = self.target_angle + 1
            if self.target_angle == 7 then self.target_angle = 1 end
        end
    end

    local has_all_shields = true
    for n=1,6 do
        local shield = self.shields[n]
        if shield.valid then
            local a = self.getRotation() + n * 360 / 6
            local offset = Vector2(4, 0):rotate(a)
            shield.setRotation(a + 180)
            shield.setPosition((position + offset):unpack())
        else
            has_all_shields = false
        end
    end
    if not has_all_shields then
        if self.shield_regen_counter > 0 then
            self.shield_regen_counter = self.shield_regen_counter - 1
        else
            local i = irandom(1, 6)
            if not self.shields[i].valid then
                local shield = createEnemy("ship/lulz/core-shield.png", 5.5)
                shield.setCollisionBox(2.7, 5)
                shield.setPosition(25, 0)
                shield.setHealth(10)
                shield.setDrawOrder(1)
                self.shields[i] = shield

                self.shield_regen_counter = self.shield_regen_delay
            end
        end
    end
end
