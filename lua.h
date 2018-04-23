#include "lua.hpp"

#if !defined(LUA_VERSION_NUM) || (LUA_VERSION_NUM < 502)
#error "This project requires Lua version >= 5.2"
#endif


const char * LuaPrintNormalWindows = R"lua(
    local function round(x)
        local m = 10^(2 or 0)
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
    end
)lua";

const char * LuaPrintAllWindows = R"lua(
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
)lua";

const char * LuaPrintAvailableFunctions = R"lua(
    local names = {}
    local count = 0;
    for name, value in pairs(window) do
        if type(value) == "function" then
            count = count + 1
            names[count] = name
        end
    end
    table.sort(names)

    print("Available functions for each window w (e.g. window.".. names[1].. "()):")
    for k, name in pairs(names) do
        print("\t".. name)
    end
)lua";
