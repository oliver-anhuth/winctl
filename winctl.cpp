#include "winctl.h"

#include <sys/time.h>


std::string WinCtl::dump_stack(lua_State * lua)
{
    std::stringstream ss;
    int top = lua_gettop(lua);
    for (int i = 1; i <= top; i++)
    {
        if (i > 1) {
            ss << ", ";
        }
        int type = lua_type(lua, i);
        if (type ==LUA_TSTRING) {
            ss << "string = " << lua_tostring(lua, i);
        } else if (type == LUA_TBOOLEAN) {
            ss << "boolean = " << (lua_toboolean(lua, i) ? "true" : "false");
        } else if (type == LUA_TNUMBER) {
            ss << "number = " << lua_tonumber(lua, i);
        } else {
            ss << lua_typename(lua, type);
        }
    }
    return ss.str();
}

int WinCtl::application(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    lua_pushstring(lua, wnck_application_get_name(wnck_window_get_application(window)));
    return 1;
}

int WinCtl::fullscreen(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    int top = lua_gettop(lua);
    if (top > 0) {
        bool set_fullscreen = lua_toboolean(lua, 1);
        wnck_window_set_fullscreen(window, set_fullscreen);
    }

    bool fullscreen = wnck_window_is_fullscreen(window);

    lua_pushboolean(lua, fullscreen);
    return 1;
}

int WinCtl::maximized(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    int top = lua_gettop(lua);
    if (top > 0) {
        bool set_maximized = lua_toboolean(lua, 1);
        if (set_maximized) {
            wnck_window_maximize(window);
        } else {
            wnck_window_unmaximize(window);
        }
    }

    bool maximized = wnck_window_is_maximized(window);

    lua_pushboolean(lua, maximized);
    return 1;
}

int WinCtl::minimized(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    int top = lua_gettop(lua);
    if (top > 0) {
        bool set_minimized = lua_toboolean(lua, 1);
        if (set_minimized) {
            wnck_window_minimize(window);
        } else {
            timeval tv{0};
            gettimeofday(&tv, nullptr);
            wnck_window_unminimize(window, tv.tv_sec);
        }
    }

    bool minimized = wnck_window_is_minimized(window);

    lua_pushboolean(lua, minimized);
    return 1;
}

int WinCtl::pos(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    WnckScreen * screen = wnck_window_get_screen(window);
    double screen_width = wnck_screen_get_width(screen);
    double screen_height = wnck_screen_get_height(screen);

    int top = lua_gettop(lua);
    if (top > 0) {
        double x = luaL_checknumber(lua, 1);
        double y = luaL_checknumber(lua, 2);
        auto mask = WNCK_WINDOW_CHANGE_X | WNCK_WINDOW_CHANGE_Y;
        wnck_window_set_geometry(
            window,
            WNCK_WINDOW_GRAVITY_STATIC,
            static_cast<WnckWindowMoveResizeMask> (mask),
            x * screen_width / 100.0,
            y * screen_height / 100.0,
            0, 0);
    }

    int x, y, width, height;
    wnck_window_get_geometry(window, &x, &y, &width, &height);

    lua_pushnumber(lua, 100.0 * x / screen_width);
    lua_pushnumber(lua, 100.0 * y / screen_height);
    return 2;
}

int WinCtl::rect(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    WnckScreen * screen = wnck_window_get_screen(window);
    double screen_width = wnck_screen_get_width(screen);
    double screen_height = wnck_screen_get_height(screen);

    int top = lua_gettop(lua);
    if (top > 0) {
        double x1 = luaL_checknumber(lua, 1);
        double y1 = luaL_checknumber(lua, 2);
        double x2 = luaL_checknumber(lua, 3);
        double y2 = luaL_checknumber(lua, 4);
        auto mask = WNCK_WINDOW_CHANGE_X | WNCK_WINDOW_CHANGE_Y | WNCK_WINDOW_CHANGE_WIDTH | WNCK_WINDOW_CHANGE_HEIGHT;
        wnck_window_set_geometry(
            window,
            WNCK_WINDOW_GRAVITY_STATIC,
            static_cast<WnckWindowMoveResizeMask> (mask),
            x1 * screen_width / 100.0,
            y1 * screen_height / 100.0,
            (x2 - x1) * screen_width / 100.0,
            (y2 - y1) * screen_height / 100.0);
    }

    int x, y, width, height;
    wnck_window_get_geometry(window, &x, &y, &width, &height);

    lua_pushnumber(lua, 100.0 * x / screen_width);
    lua_pushnumber(lua, 100.0 * y / screen_height);
    lua_pushnumber(lua, 100.0 * (x + width) / screen_width);
    lua_pushnumber(lua, 100.0 * (y + height) / screen_height);
    return 4;
}

int WinCtl::title(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    lua_pushstring(lua, wnck_window_get_name(window));
    return 1;
}

int WinCtl::type(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    switch (wnck_window_get_window_type(window)) {
        case WNCK_WINDOW_DESKTOP:
            lua_pushstring(lua, "desktop");
            break;
        case WNCK_WINDOW_DIALOG:
            lua_pushstring(lua, "dialog");
            break;
        case WNCK_WINDOW_DOCK:
            lua_pushstring(lua, "dock");
            break;
        case WNCK_WINDOW_NORMAL:
            lua_pushstring(lua, "normal");
            break;
        case WNCK_WINDOW_TOOLBAR:
            lua_pushstring(lua, "toolbar");
            break;
        case WNCK_WINDOW_MENU:
            lua_pushstring(lua, "menu");
            break;
        case WNCK_WINDOW_UTILITY:
            lua_pushstring(lua, "utility");
            break;
        case WNCK_WINDOW_SPLASHSCREEN:
            lua_pushstring(lua, "splashscreen");
            break;
        default:
            lua_pushstring(lua, "unknown");
            break;
    }
    return 1;
}
