local function round(x, p)
    local m = 10^(p or 0)
    return math.floor(x * m + 0.5) / m
end

if window.type() == "normal" or window.type() == "dialog" then
    print("if window.application() == \"".. window.application().. "\"")
    print("    and window.type() == \"".. window.type().. "\"")
    print("then")
    if window.maximized() then
        print("    window.maximized(true)")
    else
        left, top, right, bottom = window.rect()
        print("    window.rect(".. round(left, 1).. ", ".. round(top, 1).. ", ".. round(right, 1).. ", ".. round(bottom, 1).. ")")
    end
    print("end")
    print()
end

if window.application() == "Mousepad"
    and window.type() == "normal"
then
    --window.rect(20.0, 5.0, 80.0, 80.0)
    window.rect(10, 10, 90, 90)
end
