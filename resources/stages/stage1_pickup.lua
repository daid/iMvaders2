
function startExplainPickup()
    transmission(
        "[Face:marvin]Well done",
        "It seems like some|polylactic acid polymers|wher dropped here.",
        function() explain_pickup = createPickup(20, 0) end,
        "Pick them up, they could|provide to be useful later on",
        function() update = function()
            if not explain_pickup.valid then
                update = nil
                if getPlaCount() == 0 then
                    explainPickupAgain()
                else
                    postWave1()
                end
            end
        end end
    )
end

function explainPickupAgain()
    transmission(
        "[Face:marvin]You missed it!",
        "Here, try again.",
        function() explain_pickup = createPickup(20, 0) end,
        function() update = function()
            if not explain_pickup.valid then
                update = nil
                if getPlaCount() == 0 then
                    nowYouAreJustTryingToFuckup()
                else
                    postWave1()
                end
            end
        end end
    )
end

function nowYouAreJustTryingToFuckup()
    transmission(
        "[Face:nyan]Ok, now you are just|doing this intentionally.",
        "Just to see if I accounted for this.",
        "Guess what. I did.",
        "But what do we do now?",
        "I really do want you to|understand the importance|of these pickups.",
        "Oh, I know.|Here you go.|You cannot miss now",
        function()
            update = basicEndOfWaveCheck(nowYouAreJustTryingToFuckupFinished)
            for n=-24, 24 do
                table.insert(all_enemies, createPickup(20, n))
            end
        end
    )
end

function nowYouAreJustTryingToFuckupFinished()
    transmission(
        "[Face:nyan]See. There you go.",
        "I knew you would see the|importance of getting|these pickups now.",
        "We do have a problem now.",
        "Because of your screwing|around, you got|an advantage of now.",
        "We should not be rewarding|this behaviour...",
        "So...|              |What shall we do with you.",
        "I have an idea.",
        "Have fun!",
        nowYouAreJustTryingToFuckupFuckYouStart
    )
end

function nowYouAreJustTryingToFuckupFuckYouStart()
    update = basicEndOfWaveCheck(nowYouAreJustTryingToFuckupFuckYouFinished)

    createEnemyGroup(3, 0, 6.6, 4, function(group, position) createZTX_X_Fighter(group, true).target = position end)
    createEnemyGroup(6,-7, 2, 6, function(group, position) createZTX_Z_Fighter(group).target = position end)
    createEnemyGroup(6, 7, 2, 6, function(group, position) createZTX_Z_Fighter(group).target = position end)
    
    createEnemyGroup(8, 0, 2.5, 12, function(group, position) createM_M_Fighter(group, true).target = position end)
    createEnemyGroup(5, 0, 2.5, 16, function(group, position) createM_M_Fighter(group, true).target = position end)
    createEnemyGroup(2,-10, 2.5, 8, function(group, position)
        local ship = createM_M_Fighter(group, true)
        ship.target = position
        ship.onWeaponUpdate(chargeShotWeapon)
    end)
    createEnemyGroup(2, 10, 2.5, 8, function(group, position)
        local ship = createM_M_Fighter(group, true)
        ship.target = position
        ship.onWeaponUpdate(chargeShotWeapon)
    end)

    for n=-20,20,5 do
        createZTX_T_Fighter(n)
    end
end

function nowYouAreJustTryingToFuckupFuckYouFinished()
    transmission(
        "[Face:nyan]You survived?",
        "Ok, I did not account for that.",
        "Whatever man. You are crazy.",
        "The secret word is 'BANANA'",
        "Have your victory.",
        stageDone
    )
end
