
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
    ship.speed = 0.225
    return ship
end

function createM_M_FighterBurst(group, shield)
    local ship = createM_M_Fighter(group, shield)
    ship.onWeaponUpdate(chargeShotWeapon)
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
        ship.speed = 0.15
    else
        ship.speed = -0.15
    end
    return ship
end

function createM_Digitizer(group)
    local ship = createEnemy("ship/m/digitizer.png", 5.5)
    table.insert(all_enemies, ship)
    ship.setCollisionBox(5, 4)
    ship.setHealth(6)
    ship.onControlUpdate(inAndOutController)
    ship.center_time = 300
    ship.onWeaponUpdate(digitizerLaserWeapon)
    ship.onDestroy(spawnPickup(2))
    ship.speed = 0.2
    return ship
end

function digitizerLaserWeapon(self)
    if not self.fire then
        self.weapon_state = nil
    end
    if self.weapon_state == nil then
        self.weapon_state = "pre_fire"
        self.weapon_delay = 40
    elseif self.weapon_state == "pre_fire" then
        if self.weapon_delay > 0 then
            self.weapon_delay = self.weapon_delay - 1
        else
            self.weapon_state = "charge"
            self.weapon_delay = 180

            local p = Vector2(-1.8,-1.6) + Vector2(-25, 0):rotate(25 + 180)
            self.createProjectile("PRELASER", p.x, p.y, 25);
            self.createProjectile("PRELASER", p.x,-p.y,-25);
        end
    elseif self.weapon_state == "charge" then
        if self.weapon_delay > 0 then
            self.weapon_delay = self.weapon_delay - 1
        else
            local p = Vector2(-1.8,-1.6) + Vector2(-25, 0):rotate(25 + 180)
            self.createProjectile("LASER", p.x, p.y, 25);
            self.createProjectile("LASER", p.x,-p.y,-25);
            self.weapon_state = "fire"
            self.weapon_delay = 20
        end
    elseif self.weapon_state == "fire" then
        if self.weapon_delay > 0 then
            self.weapon_delay = self.weapon_delay - 1
        else
            self.weapon_state = "pre_fire"
            self.weapon_delay = random(200, 600)
        end
    end
end

function createM_Bre()
    local bre = createEnemy("ship/m/bre/bre1.png", 30.0)
    table.insert(all_enemies, bre)
    bre.setHealth(100)
    bre.setHitProtectionTime(7)
    bre.setCollisionCircle(7)
    bre.mouth = createEnemy("ship/m/bre/bre2.png", 30.0)
    bre.setDrawOrder(-1)
    bre.mouth.setDrawOrder(-1)
    bre.onControlUpdate(breController)
    bre.onDestroy(breEnd)
    bre.shield_deployed = false
end

function breController(self)
    if self.state == nil then
        self.state = "flyin"
        self.mouth_offset = 0.0
        self.setPosition(Vector2(30, 0))
        self.setInvincible(true)
        self.default_shot_delay = 120 * getDifficultyTimeScaleFactor()
    end
    local position = self.getPosition()
    if not self.shield_deployed and self.getHealth() < 50 then
        createMessage("Deploying corperate|money shield").setFace("strata").show()

        local row_count = 1 + getPlaytroughCount()
        for row=0,row_count do
            local shield_count = 12 + row
            for n=0,shield_count-1 do
                local shield_part = createEnemy("ship/m/bre/money.png", 4.0);
                shield_part.setHealth(3)
                shield_part.setCollisionBox(4, 2);
                shield_part.owner = self
                shield_part.angle = n * 360 / shield_count
                shield_part.rotation_speed = 0.7
                if row % 2 == 1 then shield_part.rotation_speed = -shield_part.rotation_speed end
                shield_part.distance = 0
                shield_part.target_distance = 8.0 + row * 1.5
                shield_part.onControlUpdate(function(shield)
                    if not shield.owner.valid then
                        shield.destroy()
                        return
                    end
                    shield.angle = shield.angle + shield.rotation_speed
                    if shield.distance < shield_part.target_distance then
                        shield.distance = shield.distance + 0.2
                    end
                    shield.setRotation(shield.angle)
                    shield.setPosition(shield.owner.getPosition() + Vector2(0, shield.distance):rotate(shield.angle))
                end)
                self.shield_deployed = true
            end
        end
    end
    if self.state == "flyin" then
        if position.x < 4 then
            self.state = "moveLeftRight"
            self.setBoss()
            self.setInvincible(false)
            if random(0, 100) < 50 then
                self.move_dir = -1
            else
                self.move_dir = 1
            end
            self.shot_delay = 72
        else
            self.setPosition(position + Vector2(-0.15, 0))
        end
    elseif self.state == "moveLeftRight" then
        if position.y < -15 then self.move_dir = 1 end
        if position.y > 15 then self.move_dir = -1 end
        self.setPosition(Vector2(4, position.y + self.move_dir * 0.11))
        
        if self.shot_delay > 0 then
            self.shot_delay = self.shot_delay - 1
        else
            if random(0, 100) < 30 then
                if random(0, 100) < 50 then
                    self.state = "chargeLaser"
                    self.shot_delay = 60
                    self.setColor(1, 0.5, 0.5)
                else
                    self.state = "openMouth"
                    self.shot_delay = self.default_shot_delay
                end
            else
                self.shot_delay = 72
                for n=-2,2 do
                    self.createProjectile("PULSE", -4, n, n * 7);
                end
            end
        end
    elseif self.state == "chargeLaser" then
        if self.shot_delay > 0 then
            self.shot_delay = self.shot_delay - 1
        else
            self.setColor(1, 1, 1)
            self.state = "fireLaser"
            self.shot_delay = 60
            
            createBreSweepLaser(self, Vector2(-1, 2), -15, 15)
            createBreSweepLaser(self, Vector2(-1,-2), 15, -15)
        end
    elseif self.state == "fireLaser" then
        if self.shot_delay > 0 then
            self.shot_delay = self.shot_delay - 1
        else
            self.state = "moveLeftRight"
            self.shot_delay = self.default_shot_delay
        end
    elseif self.state == "openMouth" then
        if self.mouth_offset > -2.5 then
            self.mouth_offset = self.mouth_offset - 0.15
        else
            self.mouth_offset = -2.5
            self.state = "spawnShips"
            self.ship_count = 4 + getPlaytroughCount() * 2
            self.shot_delay = 10
        end
    elseif self.state == "spawnShips" then
        if self.shot_delay > 0 then
            self.shot_delay = self.shot_delay - 1
        else
            self.shot_delay = 10
            if self.ship_count > 0 then
                self.ship_count = self.ship_count - 1
                local group = {}
                
                --Create a single m fighter "group" and trick it into instantly flying out of bre's mouth.
                local ship = createM_M_Fighter(group, false)
                ship.setPosition(self.getPosition() + Vector2(3.0, 0))
                ship.target = Vector2(random(10, -10), random(-18, 18))
                ship.onDestroy(nil) --prevent pickups

                group.state = "flyin"
                group.start_point = self.getPosition() + Vector2(-7.0, 0)
                ship.delay = 0
            else
                self.state = "closeMouth"
            end
        end
    elseif self.state == "closeMouth" then
        if self.mouth_offset < 0.0 then
            self.mouth_offset = self.mouth_offset + 0.15
        else
            self.mouth_offset = 0.0
            self.state = "moveLeftRight"
            self.shot_delay = self.default_shot_delay
        end
    end
    
    self.mouth.setPosition(self.getPosition() + Vector2(self.mouth_offset, 0))
end

function breEnd(self)
    self.mouth.destroy()
    for _, enemy in ipairs(all_enemies) do
        if enemy.valid and enemy ~= self then
            enemy.destroy()
        end
    end
end

function createBreSweepLaser(owner, offset, angle_start, angle_end)
    local laser = createEnemy("weapon/laser.png", 40.0)
    laser.setDrawOrder(1)
    laser.owner = owner
    laser.offset = offset
    local pos = owner.getPosition() + offset
    laser.steps = 60
    laser.angle = angle_start
    laser.angle_step = (angle_end - angle_start) / laser.steps
    pos = pos + Vector2(-20, 0):rotate(laser.angle)
    laser.setPosition(pos)
    laser.setRotation(laser.angle)
    laser.onControlUpdate(function(self)
        if self.steps > 0 then
            self.steps = self.steps - 1
            self.angle = self.angle + self.angle_step

            local pos = self.owner.getPosition() + self.offset
            pos = pos + Vector2(-20, 0):rotate(self.angle)
            self.setPosition(pos)
            self.setRotation(self.angle)
        else
            self.destroy()
        end
    end)
    return laser
end
