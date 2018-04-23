local function round(x)
    local m = 10^(2 or 0)
    return math.floor(x * m + 0.5) / m
end

print("if window.application() == \"".. window.application().. "\"")
print("    and window.type() == \"".. window.type().. "\"")
print("then")
if window.maximized() then
    print("    window.maximized(true)")
else
    left, top, right, bottom = window.rect()
    print("    window.rect(".. round(left).. ", ".. round(top).. ", ".. round(right).. ", ".. round(bottom).. ")")
end
if window.minimized() then
    print("    window.minimized(true)")
end
if window.pinned() then
    print("    window.pinned(true)")
end
print("end")
print()

if window.application() == "Mousepad"
    and window.type() == "normal"
then
    --window.rect(20.0, 5.0, 80.0, 80.0)
    window.rect(10, 10, 90, 90)
end
