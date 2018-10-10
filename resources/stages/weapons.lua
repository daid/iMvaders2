function basicPulseWeapon(self)
    if not self.fire then return end
    if self.weapon_delay_modifier == nil then self.weapon_delay_modifier = 1.0 end
    if self.weapon_delay == nil then self.weapon_delay = random(140, 300) * self.weapon_delay_modifier end
    if self.weapon_delay > 0 then
        self.weapon_delay = self.weapon_delay - 1
    else
        self.weapon_delay = nil
        self.createProjectile("PULSE", 0, 0, 0);
    end
end

function dualPulseWeapon(self)
    if not self.fire then return end
    if self.weapon_delay_modifier == nil then self.weapon_delay_modifier = 1.0 end
    if self.weapon_delay == nil then self.weapon_delay = random(140, 300) * self.weapon_delay_modifier end
    if self.weapon_delay > 0 then
        self.weapon_delay = self.weapon_delay - 1
    else
        self.weapon_delay = nil
        self.createProjectile("PULSE", 0, -1, 0);
        self.createProjectile("PULSE", 0, 1, 0);
    end
end

function chargeShotWeapon(self)
    if not self.fire then
        self.disableGlow()
        self.weapon_state = nil
    end
    if self.weapon_state == nil then
        self.weapon_delay = random(100, 500)
        self.weapon_state = "idle"
    elseif self.weapon_state == "idle" then
        if self.weapon_delay > 0 then
            self.weapon_delay = self.weapon_delay - 1
        else
            self.setGlow(2.0)
            self.weapon_delay = 90
            self.weapon_state = "pre-fire"
        end
    elseif self.weapon_state == "pre-fire" then
        if self.weapon_delay > 0 then
            self.weapon_delay = self.weapon_delay - 1
        else
            self.weapon_delay = 25
            self.weapon_state = "fire"
            self.disableGlow()
        end
    elseif self.weapon_state == "fire" then
        if self.weapon_delay > 0 then
            local y = math.sin(self.weapon_delay / 30.0 * math.pi * 4.0)
            self.createProjectile("PULSE", 0, y * 0.5, y * 5);
            self.weapon_delay = self.weapon_delay - 1
        else
            self.weapon_delay = random(300, 500)
            self.weapon_state = "idle"
        end
    end
end

function electroBoltWeapon(self)
    if not self.fire then
        self.disableGlow()
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
            self.weapon_delay = 140
        end
    elseif self.weapon_state == "charge" then
        if self.weapon_delay > 0 then
            self.setGlow(2.0 + (140 - self.weapon_delay) / 140 * 5)
            self.weapon_delay = self.weapon_delay - 1
        else
            self.disableGlow()
            self.weapon_state = "fire"
            self.weapon_delay = 20
        end
    elseif self.weapon_state == "fire" then
        if self.weapon_delay > 0 then
            if self.weapon_delay % 2 == 0 then
                createElectricBeam(self, Vector2(1, 0), Vector2(21, random(-4, 4)), random(8, 12))
            end
            self.weapon_delay = self.weapon_delay - 1
        else
            self.weapon_state = "pre_fire"
            self.weapon_delay = random(200, 600)
        end
    end
end

function createElectricBeam(self, start_position, end_position, split_position)
    local diff = end_position - start_position
    local distance = diff:length()
    local count = math.ceil(distance)
    local p0 = start_position
    for n=1,count do
        local p1 = start_position + (diff / distance) * n + Vector2(random(-0.3, 0.3), random(-0.3, 0.3))
        local pos = ((p0 + p1) / 2)
        self.createProjectile("ELECTRIC", pos.x, pos.y, (p1 - p0):angle());
        p0 = p1
        
        if split_position and split_position < n then
            createElectricBeam(self, p0, p0 + Vector2(10, random(-4, 4)))
            split_position = nil
        end
    end
end
