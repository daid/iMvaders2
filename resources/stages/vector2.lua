
Vector2 = newClass({x=0.0, y=0.0})
function Vector2:__init(x, y)
    self.x = x or 0.0
    self.y = y or 0.0
end
function Vector2:__unm()
    return Vector2(-self.x, -self.y)
end
function Vector2:__add(other)
    return Vector2(self.x + other.x, self.y + other.y)
end
function Vector2:__sub(other)
    return Vector2(self.x - other.x, self.y - other.y)
end
function Vector2:__mul(other)
    return Vector2(self.x * other, self.y * other)
end
function Vector2:__div(other)
    return Vector2(self.x / other, self.y / other)
end
function Vector2:__tostring()
    return "{x="..self.x..",y="..self.y.."}"
end

function Vector2:length()
    return math.sqrt(self.x * self.x + self.y * self.y)
end

function Vector2:rotate(angle)
    local s = math.sin(angle / 180 * math.pi)
    local c = math.cos(angle / 180 * math.pi)
    return Vector2(-c * self.x + -s * self.y, c * self.y + -s * self.x)
end

function Vector2:unpack()
    return self.x, self.y
end

function Vector2:angle()
    return math.atan2(self.y, self.x) / math.pi * 180
end
