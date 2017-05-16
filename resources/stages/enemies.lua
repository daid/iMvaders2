include("weapons.lua")
include("enemy_ztx.lua")
include("enemy_m.lua")
include("enemy_lulz.lua")

function addEnemyToGroup(ship, group)
    ship.group = group
    if group.members == nil then
        group.members = {}
    end
    table.insert(group.members, ship)
    if group.leader == nil then
        group.leader = ship
    end
end    

function createEnemyGroup(x, y, distance, count, func)
    local group = {}
    for n=0,count-1 do
        func(group, Vector2(x, y - (distance * (count - 1) * 0.5) + distance * n))
    end
    return group
end

function spawnPickup(amount)
    if amount == nil then
        amount = 1
    end
    return function(self)
        if amount > 1 then
            local size = math.sqrt(amount)
            for n=1,amount do
                createPickup((Vector2(self.getPosition()) + Vector2(random(-size, size), random(-size, size))):unpack())
            end
        else
            if random(0, 1) <= amount then
                createPickup(self.getPosition())
            end
        end
    end
end
