#include "lua.hpp"


const char * LuaPrintNormalWindows = R"lua(
    local function round(x, p)
        local m = 10^(p or 0)
        return math.floor(x * m + 0.5) / m
    end

    local type = get_type()
    local application = get_application()

    if type == "normal" or type == "dialog" then
        print("if get_application() == \"".. application.. "\"")
        print("    and get_type() == \"".. type.. "\"")
        print("then")
        if is_maximized() then
            print("    maximize(true)")
        else
            left, top, right, bottom = get_rect()
            print("    set_rect(".. round(left, 1).. ", ".. round(top, 1).. ", ".. round(left, 1).. ", ".. round(bottom, 1).. ")")
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

    local type = get_type()
    local application = get_application()

    print("if application() == \"".. application.. "\"")
    print("    and type() == \"".. type.. "\"")
    print("then")
    if is_maximized() then
        print("    set_maximized(true)")
    else
        left, top, right, bottom = get_rect()
        print("    set_rect(".. round(left, 1).. ", ".. round(top, 1).. ", ".. round(left, 1).. ", ".. round(bottom, 1).. ")")
    end
    print("end")
    print()
)lua";
