
Curve = newClass({delta = 0.0, p0=Vector2(0, 0), p1=Vector2(0, 0), cp0=Vector2(0, 0), cp1=Vector2(0, 0)})

function Curve:get(offset)
    local f = self.delta + (offset or 0.0)
    local g = 1.0 - f
    return (self.p0*g*g*g) + (self.cp0*3.0*g*g*f) + (self.cp1*3.0*g*f*f) + (self.p1*f*f*f)
end

function Curve:advance(amount)
    local diff = self:get() - self:get(0.01)
    local move_dist_per_delta = diff:length() * 100.0
    self.delta = self.delta + amount / move_dist_per_delta
    if self.delta < 0.0 then self.delta = 0.0 end
    if self.delta > 1.0 then self.delta = 1.0 end
end
