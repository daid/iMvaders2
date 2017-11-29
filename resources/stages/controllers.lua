
function spaceInvaderController(self)
    if self.state == nil then self.state = "wait" end
    if self.target == nil then
        log("Warning, space invader controller without a target position!")
        self.target = Vector2(0, 0)
    end
    if not self.group.leader.valid then
        self.group.leader = self
    end
    
    local move_offset = (getGlobalTime() % 140) / 140 * 2
    if move_offset > 1.0 then
        move_offset = 2.0 - move_offset
    end
    move_offset = (move_offset - 0.5) * 6.0
    
    if self.group.leader == self then
        if self.group.state == nil then
            self.group.state = "wait"
        end
        local members_state = nil
        for _, member in ipairs(self.group.members) do
            if member.valid then
                if members_state == nil then members_state = member.state end
                if members_state ~= member.state then members_state = "different" end
            end
        end
        if self.group.state == "wait" or self.group.state == "flyout" then
            if members_state == "wait" then
                self.group.state = "flyin"
                self.group.start_point = Vector2(20, random(-20, 20))
                for index, member in ipairs(self.group.members) do
                    member.delay = index * 5
                end
            end
        elseif self.group.state == "flyin" then
            if members_state == "center" then
                self.group.state = "center"
                if self.group.not_initial_center_delay then
                    self.group.delay = random(300, 500)
                else
                    self.group.not_initial_center_delay = true
                    self.group.delay = random(100, 500)
                end
            end
        elseif self.group.state == "center" then
            if self.group.delay > 0 then
                self.group.delay = self.group.delay - 1
            else
                self.group.state = "flyout"
                self.group.end_point = Vector2(-25, random(-20, 20))
                for index, member in ipairs(self.group.members) do
                    member.delay = index * 5
                end
            end
        end
    end
    
    if self.state == "wait" then
        self.setPosition(-100, -100) --Place the enemy way off screen
        if self.group.state == "flyin" then
            if self.delay > 0 then
                self.delay = self.delay - 1
            else
                self.state = "flyin"
                self.curve = Curve()
                self.curve.p0 = self.group.start_point
                self.curve.p1 = self.target
                self.curve.cp0 = self.curve.p0 + Vector2(-10, 0)
                self.curve.cp1 = self.curve.p1 + Vector2(10, 0)
            end
        end
    elseif self.state == "flyin" then
        self.curve.p1 = self.target + Vector2(0, move_offset)
        self.curve:advance(self.speed)
        self.setPosition(self.curve:get():unpack())
        self.setRotation((self.curve:get(0.1) - self.curve:get()):angle())
        if self.curve.delta >= 1.0 then
            self.state = "center"
            self.setRotation(180)
        end
    elseif self.state == "center" then
        self.setPosition((self.target + Vector2(0, move_offset)):unpack())
        self.fire = true
        if self.group.state == "flyout" then
            if self.delay > 0 then
                self.delay = self.delay - 1
            else
                self.fire = false
                self.state = "flyout"
                self.curve.delta = 0.0
                self.curve.p0 = self.target + Vector2(0, move_offset)
                self.curve.p1 = self.group.end_point
                self.curve.cp0 = self.curve.p0 + Vector2(-10, 0)
                self.curve.cp1 = self.curve.p1 + Vector2(10, 0)
            end
        end
    elseif self.state == "flyout" then
        self.curve:advance(self.speed)
        self.setPosition(self.curve:get():unpack())
        self.setRotation((self.curve:get(0.1) - self.curve:get()):angle())
        if self.curve.delta >= 1.0 then
            self.state = "wait"
        end
    end
end

function wingmanController(self)
    if self.master.valid then
        self.fire = self.master.fire
        self.setPosition((Vector2(self.master.getPosition()) + self.offset:rotate(self.master.getRotation())):unpack())
        self.setRotation(self.master.getRotation())
    else
        diveBomberController(self)
    end
end

function diveBomberController(self)
    self.setPosition((Vector2(self.getPosition()) + Vector2(-self.speed, 0):rotate(self.getRotation())):unpack())
    if self.getPosition() < -20 then
        self.setPosition(20, random(-20, 20))
        self.setRotation(random(160, 200))
    end
    self.fire = self.getPosition() > -5
end

function aimedDiveBomberController(self)
    local position = Vector2(self.getPosition())
    self.setPosition((position + Vector2(-self.speed, 0):rotate(self.getRotation())):unpack())
    if position.x < -20 or position.y < -25 or position.y > 25 then
        self.setPosition(20, random(-20, 20))
        
        position = Vector2(self.getPosition())
        player = getRandomPlayer()
        if player then
            self.setRotation((Vector2(player:getPosition()) - position):angle())
        else
            self.setRotation(random(160, 200))
        end
    end
    self.fire = self.getPosition() > -5
end

function inAndOutController(self)
    if self.state == nil then
        self.state = "flyin"
        self.setPosition(20, self.target.y)
        if self.target == nil then
            self.target = Vector2(4, random(-16, 16))
        end
        if self.center_time == nil then
            self.center_time = 100
        end
    end
    local x, y = self.getPosition()
    if self.state == "flyin" then
        if x > self.target.x then
            x = x - self.speed
            self.setPosition(x, self.target.y)
        else
            self.fire = true
            self.state = "center"
            self.delay = self.center_time
        end
    elseif self.state == "center" then
        if self.delay > 0 then
            self.delay = self.delay - 1
        else
            self.fire = false
            self.state = "flyout"
        end
    elseif self.state == "flyout" then
        if x < 20 then
            x = x + self.speed
            self.setPosition(x, self.target.y)
        else
            self.state = "flyin"
            self.target.y = random(-16, 16)
        end
    end
end

function sinusWaveController(self)
    local x, y = self.getPosition()
    y = y + self.speed
    if y > 25 then y = -25 end
    if y < -25 then y = 25 end
    self.setPosition(math.sin(y / 3.0) * 3.0, y)
    self.fire = true
end
