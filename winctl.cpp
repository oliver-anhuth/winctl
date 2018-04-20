#include "winctl.h"


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

int WinCtl::get_application(lua_State * lua)
{
    WnckWindow * window = get_window(lua);
    const char * application = wnck_application_get_name(wnck_window_get_application(window));

    lua_pushstring(lua, application);
    return 1;
}

int WinCtl::is_fullscreen(lua_State * lua)
{
    WnckWindow * window = get_window(lua);
    bool fullscreen = wnck_window_is_fullscreen(window);

    lua_pushboolean(lua, fullscreen);
    return 1;
}

int WinCtl::set_fullscreen(lua_State * lua)
{
    bool set_fullscreen = true;
    int top = lua_gettop(lua);
    if (top > 0) {
        set_fullscreen = lua_toboolean(lua, 1);
    }

    WnckWindow * window = get_window(lua);
    wnck_window_set_fullscreen(window, set_fullscreen);

    return 0;
}

int WinCtl::is_maximized(lua_State * lua)
{
    WnckWindow * window = get_window(lua);
    bool maximized = wnck_window_is_maximized(window);

    lua_pushboolean(lua, maximized);
    return 1;
}

int WinCtl::set_maximized(lua_State * lua)
{
    bool set_maximized = true;
    int top = lua_gettop(lua);
    if (top > 0) {
        set_maximized = lua_toboolean(lua, 1);
    }

    WnckWindow * window = get_window(lua);
    if (set_maximized) {
        wnck_window_maximize(window);
    } else {
        wnck_window_unmaximize(window);
    }

    return 0;
}

int WinCtl::is_minimized(lua_State * lua)
{
    WnckWindow * window = get_window(lua);
    bool minimized = wnck_window_is_minimized(window);

    lua_pushboolean(lua, minimized);
    return 1;
}

int WinCtl::set_minimized(lua_State * lua)
{
    bool set_minimized = true;
    int top = lua_gettop(lua);
    if (top > 0) {
        set_minimized = lua_toboolean(lua, 1);
    }

    WnckWindow * window = get_window(lua);
    if (set_minimized) {
        wnck_window_minimize(window);
    } else {
        wnck_window_unminimize(window, make_timestamp());
    }

    return 0;
}

int WinCtl::get_pos(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    WnckScreen * screen = wnck_window_get_screen(window);
    double screen_width = wnck_screen_get_width(screen);
    double screen_height = wnck_screen_get_height(screen);

    int x, y, width, height;
    wnck_window_get_geometry(window, &x, &y, &width, &height);

    lua_pushnumber(lua, 100.0 * x / screen_width);
    lua_pushnumber(lua, 100.0 * y / screen_height);
    return 2;
}

int WinCtl::set_pos(lua_State * lua)
{
    double x = luaL_checknumber(lua, 1);
    double y = luaL_checknumber(lua, 2);

    WnckWindow * window = get_window(lua);

    WnckScreen * screen = wnck_window_get_screen(window);
    double screen_width = wnck_screen_get_width(screen);
    double screen_height = wnck_screen_get_height(screen);

    auto mask = WNCK_WINDOW_CHANGE_X | WNCK_WINDOW_CHANGE_Y;
    wnck_window_set_geometry(
        window,
        WNCK_WINDOW_GRAVITY_STATIC,
        static_cast<WnckWindowMoveResizeMask> (mask),
        x * screen_width / 100.0,
        y * screen_height / 100.0,
        0,
        0);

    return 0;
}

int WinCtl::get_rect(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    WnckScreen * screen = wnck_window_get_screen(window);
    double screen_width = wnck_screen_get_width(screen);
    double screen_height = wnck_screen_get_height(screen);

    int x, y, width, height;
    wnck_window_get_geometry(window, &x, &y, &width, &height);

    lua_pushnumber(lua, 100.0 * x / screen_width);
    lua_pushnumber(lua, 100.0 * y / screen_height);
    lua_pushnumber(lua, 100.0 * (x + width - 1.0) / screen_width);
    lua_pushnumber(lua, 100.0 * (y + height - 1.0) / screen_height);
    return 4;
}

int WinCtl::set_rect(lua_State * lua)
{
    double left = luaL_checknumber(lua, 1);
    double top = luaL_checknumber(lua, 2);
    double right = luaL_checknumber(lua, 3);
    double bottom = luaL_checknumber(lua, 4);

    WnckWindow * window = get_window(lua);

    WnckScreen * screen = wnck_window_get_screen(window);
    double screen_width = wnck_screen_get_width(screen);
    double screen_height = wnck_screen_get_height(screen);

    auto mask = WNCK_WINDOW_CHANGE_X | WNCK_WINDOW_CHANGE_Y | WNCK_WINDOW_CHANGE_WIDTH | WNCK_WINDOW_CHANGE_HEIGHT;
    wnck_window_set_geometry(
        window,
        WNCK_WINDOW_GRAVITY_STATIC,
        static_cast<WnckWindowMoveResizeMask> (mask),
        left * screen_width / 100.0,
        top * screen_height / 100.0,
        (right - left + 1.0) * screen_width / 100.0,
        (bottom - top + 1.0) * screen_height / 100.0);

    return 0;
}

int WinCtl::get_title(lua_State * lua)
{
    WnckWindow * window = get_window(lua);
    const char * title = wnck_window_get_name(window);

    lua_pushstring(lua, title);
    return 1;
}

int WinCtl::get_type(lua_State * lua)
{
    WnckWindow * window = get_window(lua);
    WnckWindowType type = wnck_window_get_window_type(window);

    switch (type) {
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
