
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
    ship.speed = 0.2
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
    ship.speed = 0.175
    ship.center_time = 400
    return ship
end

function lulzTrippleShot(self)
    if self.weapon_delay_modifier == nil then self.weapon_delay_modifier = 1.0 end
    if self.weapon_delay == nil then
        self.weapon_delay = random(140, 300) * self.weapon_delay_modifier
        self.weapon_state = "pre-fire"
        self.weapon_spin_counter = 0
        self.spin_direction = 1
        if random(0, 100) < 50 then self.spin_direction = -1 end
    end
    
    self.setRotation(self.weapon_spin_counter * 1.0 + 180)
    if not self.fire then return end
    
    self.weapon_spin_counter = self.weapon_spin_counter + self.spin_direction
    if self.weapon_state == "pre-fire" then
        if self.weapon_delay > 0 then
            self.weapon_delay = self.weapon_delay - 1
        else
            self.weapon_delay = random(60, 70)
            self.weapon_state = "firing"
        end
    elseif self.weapon_state == "firing" then
        if self.weapon_delay > 0 then
            self.weapon_delay = self.weapon_delay - 1
        else
            self.weapon_delay = random(140, 300) * self.weapon_delay_modifier
            self.weapon_state = "pre-fire"
        end
        if self.weapon_delay % 3 == 0 then
            self.createProjectile("PULSE", 0, 0, 0);
            self.createProjectile("PULSE", 0, 0, 120);
            self.createProjectile("PULSE", 0, 0,-120);
        end
    end
end

function createLulz_Lulz_Boss()
    local self = createEnemy("ship/lulz/core.png", 10.0)
    table.insert(all_enemies, self)
    self.setCollisionCircle(3.8)
    self.setHealth(50)
    self.setBoss()
    self.onControlUpdate(lulzBossController)
    self.onWeaponUpdate(lulzBossWeapons)
    self.onDestroy(lulzBossDestroyed)
    self.setPosition(Vector2(25, 0))

    self.slow_speed = 0.05
    self.high_speed = 0.20
    self.weapon1_delay = 120
    self.weapon4_delay = 300
    self.shields = {}
    self.target_angle = 3
    self.shield_regen_delay = 2000
    
    self.shield_regen_counter = self.shield_regen_delay
    self.weapon1_counter = self.weapon1_delay
    self.weapon2_state = 0
    self.weapon3_state = 0
    self.weapon3_delay = 0
    self.weapon4_counter = self.weapon4_delay
    self.weapon5_state = 0

    for n=1,6 do
        local shield = createEnemy("ship/lulz/core-shield.png", 5.5)
        shield.setCollisionBox(2.7, 5)
        shield.setPosition(Vector2(25, 0))
        shield.setHealth(10)
        shield.setDrawOrder(1)
        self.shields[n] = shield
    end
end

function lulzBossController(self)
    local position = self.getPosition()
    if position.x > 5 then
        position.x = position.x - self.slow_speed
    else
        position.x = position.x - self.high_speed
    end
    if position.x < -25 then
        position.x = 25
        position.y = random(-15, 15)
    end
    self.setPosition(position)
    local adiff = angleDiff(self.getRotation(), self.target_angle * 360 / 6)
    if adiff < -1 then
        self.setRotation(self.getRotation() - 0.5)
    elseif adiff > 1 then
        self.setRotation(self.getRotation() + 0.5)
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
            local offset = Vector2(-4, 0):rotate(a)
            shield.setRotation(a + 180)
            shield.setPosition(position + offset)
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
                shield.setPosition(Vector2(25, 0))
                shield.setHealth(10)
                shield.setDrawOrder(1)
                self.shields[i] = shield

                self.shield_regen_counter = self.shield_regen_delay
            end
        end
    end
end

function lulzBossWeapons(self)
    --The lulz boss has 6 types of weapons, one for each direction of the core. Only the front 3 are firing.
    --1) Period spread shot
    --2) Launch lulz fighter dive bomber
    --3! Laser blast, digitizer laser XL
    --4! Homing missile
    --5) Launch mini-octo
    --6! ?
    local a = -self.getRotation()
    local pos = self.getPosition()
    if math.abs(angleDiff(a, 0)) > 90 then
        --1) Period spread shot
        if self.weapon1_counter > 0 then
            self.weapon1_counter = self.weapon1_counter - 1
        else
            for n=-2,2 do
                self.createProjectile("PULSE", 4, n, n * 7);
            end
            self.weapon1_counter = self.weapon1_delay
        end
    end
    if math.abs(angleDiff(a, 60)) > 90 then
        --2) Launch lulz fighter dive bomber
        if self.weapon2_state == 0 then
            if pos.x < 4 then
                self.weapon2_state = 1
                local ship = createEnemy("ship/lulz/lulz.png", 3.0)
                ship.setCollisionCircle(1.2)
                ship.setHealth(2)
                ship.onControlUpdate(diveBomberController)
                ship.onWeaponUpdate(basicPulseWeapon)
                ship.speed = 0.2
                ship.setPosition(pos + Vector2(4, 0):rotate(self.getRotation() + 60))
                ship.setRotation(self.getRotation() + 60)
                ship.setDrawOrder(1)
            end
        elseif self.weapon2_state == 1 and pos.x > 20 then
            self.weapon2_state = 0
        end
    end
    if math.abs(angleDiff(a, 120)) > 90 then
        --3! Laser blast, digitizer laser XL
        if self.weapon3_state == 0 then
            if self.weapon3_delay > 0 then
                self.weapon3_delay = self.weapon3_delay - 1
            else
                local pre_blast = createEnemy("weapon/pre-laser.png", 1000.0)
                pre_blast.setPosition(pos + Vector2(500, 0):rotate(self.getRotation() + 120))
                pre_blast.setRotation(self.getRotation() + 120)
                pre_blast.setInvincible(true)
                pre_blast.setDrawOrder(-1)
                self.weapon3_object = pre_blast
                self.weapon3_state = 1
                self.weapon3_delay = 60
            end
        elseif self.weapon3_state == 1 then
            self.weapon3_object.setPosition(pos + Vector2(500, 0):rotate(self.getRotation() + 120))
            self.weapon3_object.setRotation(self.getRotation() + 120)
            if self.weapon3_delay > 0 then
                self.weapon3_delay = self.weapon3_delay - 1
            else
                self.weapon3_object.destroy()
                self.weapon3_state = 2
                self.weapon3_delay = 40

                local blast = createEnemy("weapon/laser.png", 1000.0)
                blast.setPosition(pos + Vector2(500, 0):rotate(self.getRotation() + 120))
                blast.setRotation(self.getRotation() + 120)
                blast.setCollisionBox(1000, 5)
                blast.setInvincible(true)
                blast.setDrawOrder(-1)
                self.weapon3_object = blast
            end
        elseif self.weapon3_state == 2 then
            self.weapon3_object.setPosition(pos + Vector2(500, 0):rotate(self.getRotation() + 120))
            self.weapon3_object.setRotation(self.getRotation() + 120)
            if self.weapon3_delay > 0 then
                self.weapon3_delay = self.weapon3_delay - 1
            else
                self.weapon3_object.destroy()
                self.weapon3_state = 0
                self.weapon3_delay = random(120, 300)
            end
        end
    else
        if self.weapon3_object ~= nil and self.weapon3_object.valid then
            self.weapon3_object.destroy()
        end
        self.weapon3_state = 0
    end
    if math.abs(angleDiff(a, 180)) > 90 then
        --4! Homing missile
        if self.weapon4_counter > 0 then
            self.weapon4_counter = self.weapon4_counter - 1
        else
            local ship = createEnemy("weapon/missile.png", 2.0)
            ship.setCollisionBox(2, 0.8)
            ship.setHealth(3)
            ship.setPosition(pos + Vector2(-4, 0):rotate(self.getRotation() + 180))
            ship.setRotation(self.getRotation() + 180)
            ship.speed = 0.1
            ship.onControlUpdate(function(s)
                if s.player == nil or not s.player.valid then
                    s.player = getRandomPlayer()
                end
                s.setPosition(s.getPosition() + Vector2(s.speed, 0):rotate(s.getRotation()))
                if s.player then
                    s.setRotation((s.player.getPosition() - s.getPosition()):angle())
                end
            end)
            ship.setDrawOrder(1)
            self.weapon4_counter = self.weapon4_delay
        end
    end
    if math.abs(angleDiff(a, 240)) > 90 then
        --5) Launch mini-octo
        if self.weapon5_state == 0 then
            if pos.x < 4 then
                self.weapon5_state = 1

                local ship = createEnemy("ship/lulz/octo.png", 2.5)
                ship.setCollisionCircle(1.1)
                ship.setHealth(2)
                ship.setPosition(pos + Vector2(4, 2):rotate(self.getRotation() + 240))
                ship.setRotation(self.getRotation() + 240)
                ship.setTouchDamage(1.4, "energyDrain")
                ship.speed = 0.3
                ship.onControlUpdate(aimedDiveBomberController)
                ship.setDrawOrder(1)

                local ship = createEnemy("ship/lulz/octo.png", 2.5)
                ship.setCollisionCircle(1.1)
                ship.setHealth(2)
                ship.setPosition(pos + Vector2(4, -2):rotate(self.getRotation() + 240))
                ship.setRotation(self.getRotation() + 240)
                ship.setTouchDamage(1.4, "energyDrain")
                ship.speed = 0.3
                ship.onControlUpdate(aimedDiveBomberController)
                ship.setDrawOrder(1)
            end
        elseif self.weapon5_state == 1 and pos.x > 20 then
            self.weapon5_state = 0
        end
    end
    if math.abs(angleDiff(a, 300)) > 90 then
        --6! ?
    end
end

function lulzBossDestroyed(self)
    for n=1,6 do
        if self.shields[n].valid then
            self.shields[n].destroy()
        end
    end
    if self.weapon3_object ~= nil and self.weapon3_object.valid then
        self.weapon3_object.destroy()
    end
end
