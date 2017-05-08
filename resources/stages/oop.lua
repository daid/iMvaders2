
function newClass(t)
    t.__index = t or {}
    if t.__init == nil then
        t.__init = function(self) end
    end
    setmetatable(t, {__call = function(class, ...)
            local o = {}
            setmetatable(o, t)
            o:__init(...)
            return o
    end})
    return t
end
