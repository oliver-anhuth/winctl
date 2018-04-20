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
