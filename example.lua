
if window.type() == "normal" or window.type() == "dialog" then
    -- Cascade windows from pos 1,1 to 20, 20
    if not cascade_current then
        cascade_start = 1
        cascade_end = 20
        cascade_step = 1
        cascade_current = cascade_start
    end

    -- Set window position to current cascade position
    window.pos(cascade_current, cascade_current)

    -- Wrap cascade position around at the end
    cascade_current = cascade_current + cascade_step
    if cascade_current > cascade_end then
        cascade_current = cascade_start
    end

    -- Make terminal windows fullscreen
    if window.application() == "Xfce Terminal" then
        window.fullscreen(true)
    end

    -- Make Firefox maximized
    if window.application() == "Firefox" then
        window.maximized(true)
    end

    -- And just in case...
    if window.application() == "iexplore.exe" then
        window.minimized(true)
    end
end
