include("oop.lua")
include("vector2.lua")
include("curve.lua")
include("enemies.lua")
include("controllers.lua")

local function _transmissionFinishedFunction(next_message, functions)
    return function()
        if next_message ~= nil then
            next_message.show()
        end
        for idx, f in ipairs(functions) do
            f()
        end
    end
end

function transmission(...)
    local entries = {...}
    
    local previous = nil
    local functions = {}
    local face = nil
    for idx, value in ipairs(entries) do
        if type(value) == "function" then
            table.insert(functions, value)
        else
            local s, e = value:find("%[.*%]")
            if s == 1 then
                local data = value:sub(2, e - 1)
                s = data:find(":")
                if s ~= nil then
                    local key = data:sub(1, s - 1):lower()
                    local value = data:sub(s + 1)
                    if key == "face" then
                        face = value
                    else
                        log("Unknown data tag in transmission string:", key, value)
                    end
                end
                
                value = value:sub(e + 1)
            end
            local message = createMessage(value)
            if face ~= nil then
                message.setFace(face)
            end
            if previous == nil then
                message.show()
            else
                previous.onFinished(_transmissionFinishedFunction(message, functions))
                functions = {}
            end
            previous = message
        end
    end
    previous.onFinished(_transmissionFinishedFunction(nil, functions))
end

function basicEndOfWaveCheck(next)
    all_enemies = {}
    return function()
        for _, enemy in ipairs(all_enemies) do
            if enemy.valid then
                return
            end
        end
        update = nil
        next()
    end
end

function delayUpdate(delay, next)
    return function()
        if delay > 0 then
            delay = delay - 1
        else
            update = nil
            next()
        end
    end
end

--Function that can be used to multiply delays with, this reduces the delay the every time the game is finished, but with an acceptable difficulty curve.
function getDifficultyTimeScaleFactor()
    return 1.0 / math.log(10.0 + getPlaytroughCount() * 2.0)
end
