#ifndef INCLUDED_F18555B8_0EA7_4FFF_87E5_02A626D15942
#define INCLUDED_F18555B8_0EA7_4FFF_87E5_02A626D15942

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
        print("    and window.title() == \"".. window.title().. "\"")
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
    print("    and window.title() == \"".. window.title().. "\"")
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

#endif // INCLUDED_F18555B8_0EA7_4FFF_87E5_02A626D15942
