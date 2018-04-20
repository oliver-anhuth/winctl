#include "lua.hpp"


const char * LuaPrintNormalWindows = R"lua(
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
            x, y, w, h = window.rect()
            print("    window.rect(".. round(x, 1).. ", ".. round(y, 1).. ", ".. round(w, 1).. ", ".. round(h, 1).. ")")
        end
        print("end")
        print()
    end
)lua";

const char * LuaPrintAllWindows = R"lua(
    local function round(x, p)
        local m = 10^(p or 0)
        return math.floor(x * m + 0.5) / m
    end

    print("if window.application() == \"".. window.application().. "\"")
    print("    and window.type() == \"".. window.type().. "\"")
    print("then")
    if window.maximized() then
        print("    window.maximized(true)")
    else
        x, y, w, h = window.rect()
        print("    window.rect(".. round(x, 1).. ", ".. round(y, 1).. ", ".. round(w, 1).. ", ".. round(h, 1).. ")")
    end
    print("end")
    print()
)lua";

const char * LuaPrintWindowFunctions = R"lua(
    local names = {}
    local count = 0;
    for name, value in pairs(window) do
        if type(value) == "function" then
            count = count + 1
            names[count] = name
        end
    end
    table.sort(names)

    print("Available functions for each window w (e.g. w.".. names[1].. "()):")
    for k, name in pairs(names) do
        print("\t".. name)
    end
)lua";
