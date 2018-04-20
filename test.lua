local function round(x, p)
    local m = 10^(p or 0)
    return math.floor(x * m + 0.5) / m
end

local type = get_type()
local application = get_application()

if type == "normal" or type == "dialog" then
    print("if get_application() == \"".. application.. "\"")
    print("    and type == \"".. type.. "\"")
    print("then")
    if is_maximized() then
        print("    maximized(true)")
    else
        left, top, right, bottom = get_rect()
        print("    rect(".. round(left, 1).. ", ".. round(top, 1).. ", ".. round(left, 1).. ", ".. round(bottom, 1).. ")")
    end
    print("end")
    print()
end

if application == "Xfce Terminal"
    and type == "normal"
then
    --minimized(true)
    set_rect(20.0, 5.0, 80.0, 80.0)
end
