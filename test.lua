function round(x, p)
  local m = 10^(p or 0)
  return math.floor(x * m + 0.5) / m
end

if type() == "normal" then
    print("if application() == \"".. application().. "\"")
    print("    and type() == \"".. type().. "\"")
    print("then")
    if maximized() then
        print("    maximized(true)")
    else
        x, y, w, h = rect()
        print("    rect(".. round(x, 1).. ", ".. round(y, 1).. ", ".. round(w, 1).. ", ".. round(h, 1).. ")")
    end
    print("end")
    print()
end

if application() == "Xfce Terminal"
    and type() == "normal"
then
    --minimized(true)
    rect(20.0, 5.0, 80.0, 80.0)
end
