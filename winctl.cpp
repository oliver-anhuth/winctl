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

int WinCtl::above(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    int top = lua_gettop(lua);
    if (top > 0) {
        bool set_above = lua_toboolean(lua, 1);
        if (set_above) {
            wnck_window_make_above(window);
        } else {
            wnck_window_unmake_above(window);
        }
        return 0;
    }

    bool above = wnck_window_is_above(window);

    lua_pushboolean(lua, above);
    return 1;
}

int WinCtl::application(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    lua_pushstring(lua, wnck_application_get_name(wnck_window_get_application(window)));
    return 1;
}

int WinCtl::below(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    int top = lua_gettop(lua);
    if (top > 0) {
        bool set_below = lua_toboolean(lua, 1);
        if (set_below) {
            wnck_window_make_below(window);
        } else {
            wnck_window_unmake_below(window);
        }
        return 0;
    }

    bool below = wnck_window_is_below(window);

    lua_pushboolean(lua, below);
    return 1;
}

int WinCtl::fullscreen(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    int top = lua_gettop(lua);
    if (top > 0) {
        bool set_fullscreen = lua_toboolean(lua, 1);
        wnck_window_set_fullscreen(window, set_fullscreen);
        return 0;
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
        return 0;
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
        return 0;
    }

    bool minimized = wnck_window_is_minimized(window);

    lua_pushboolean(lua, minimized);
    return 1;
}

int WinCtl::close(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    wnck_window_close(window, 0);

    return 1;
}

int WinCtl::pinned(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    int top = lua_gettop(lua);
    if (top > 0) {
        bool set_pinned = lua_toboolean(lua, 1);
        if (set_pinned) {
            wnck_window_pin(window);
        } else {
            wnck_window_unpin(window);
        }
        return 0;
    }

    bool pinned = wnck_window_is_pinned(window);

    lua_pushboolean(lua, pinned);
    return 1;
}

int WinCtl::pos(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    WinCtl & self = get_self(lua);
    WnckScreen * screen = wnck_window_get_screen(window);
    const WorkArea & work_area = self.get_work_area(screen);

    int top = lua_gettop(lua);
    if (top > 0) {
        double x = luaL_checknumber(lua, 1);
        double y = luaL_checknumber(lua, 2);
        auto mask = WNCK_WINDOW_CHANGE_X | WNCK_WINDOW_CHANGE_Y;
        wnck_window_set_geometry(
            window,
            WNCK_WINDOW_GRAVITY_STATIC,
            static_cast<WnckWindowMoveResizeMask> (mask),
            work_area.left + x * (work_area.right - work_area.left) / 100.0,
            work_area.top + y * (work_area.bottom - work_area.top) / 100.0,
            0, 0);
        return 0;
    }

    int x, y, width, height;
    wnck_window_get_geometry(window, &x, &y, &width, &height);

    lua_pushnumber(lua, 100.0 * (x - work_area.left) / (work_area.right - work_area.left));
    lua_pushnumber(lua, 100.0 * (y - work_area.top) / (work_area.bottom - work_area.top));
    return 2;
}

int WinCtl::role(lua_State * lua)
{
    WnckWindow * window = get_window(lua);
    const char * role = wnck_window_get_role(window);

    lua_pushstring(lua, role);
    return 1;
}

int WinCtl::rect(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    WinCtl & self = get_self(lua);
    WnckScreen * screen = wnck_window_get_screen(window);
    const WorkArea & work_area = self.get_work_area(screen);

    int top = lua_gettop(lua);
    if (top > 0) {
        double x0 = luaL_checknumber(lua, 1);
        double y0 = luaL_checknumber(lua, 2);
        double x1 = luaL_checknumber(lua, 3);
        double y1 = luaL_checknumber(lua, 4);
        auto mask = WNCK_WINDOW_CHANGE_X | WNCK_WINDOW_CHANGE_Y | WNCK_WINDOW_CHANGE_WIDTH | WNCK_WINDOW_CHANGE_HEIGHT;
        wnck_window_set_geometry(
            window,
            WNCK_WINDOW_GRAVITY_STATIC,
            static_cast<WnckWindowMoveResizeMask> (mask),
            work_area.left + x0 * (work_area.right - work_area.left) / 100.0,
            work_area.top + y0 * (work_area.bottom - work_area.top) / 100.0,
            (x1 - x0) * (work_area.right - work_area.left + 1) / 100.0,
            (y1 - y0) * (work_area.bottom - work_area.top) / 100.0);
        return 0;
    }

    int x, y, width, height;
    wnck_window_get_geometry(window, &x, &y, &width, &height);

    lua_pushnumber(lua, 100.0 * (x - work_area.left) / (work_area.right - work_area.left));
    lua_pushnumber(lua, 100.0 * (y - work_area.top) / (work_area.bottom - work_area.top));
    lua_pushnumber(lua, 100.0 * (x + width - 1 - work_area.left) / (work_area.right - work_area.left));
    lua_pushnumber(lua, 100.0 * (y + height - 1 - work_area.top) / (work_area.bottom - work_area.top));
    return 4;
}

int WinCtl::title(lua_State * lua)
{
    WnckWindow * window = get_window(lua);
    const char * title = wnck_window_get_name(window);

    lua_pushstring(lua, title);
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

int WinCtl::windowed(lua_State * lua)
{
    WnckWindow * window = get_window(lua);

    int top = lua_gettop(lua);
    if (top > 0) {
        bool set_windowed = lua_toboolean(lua, 1);
        if (set_windowed) {
            timeval tv{0};
            gettimeofday(&tv, nullptr);
            wnck_window_set_fullscreen(window, false);
            wnck_window_unminimize(window, tv.tv_sec);
            wnck_window_unmaximize(window);
        } else {
            wnck_window_set_fullscreen(window, true);
            wnck_window_minimize(window);
            wnck_window_maximize(window);
        }
        return 0;
    }

    bool fullscreen = wnck_window_is_fullscreen(window);
    bool minimized = wnck_window_is_minimized(window);
    bool maximized = wnck_window_is_maximized(window);

    lua_pushboolean(lua, !fullscreen && !minimized && !maximized);
    return 1;
}

WinCtl::WorkArea WinCtl::calculate_work_area(WnckScreen * screen)
{
    LOG_DEBUG("Calculating work area for screen " << wnck_screen_get_number(screen));
    int left = 0;
    int top = 0;
    int right = wnck_screen_get_width(screen) - 1;
    int bottom = wnck_screen_get_height(screen) - 1;

    bool finished = false;
    while (!finished) {
        finished = true;
        LOG_DEBUG("Current work area: " << "rect(" << left << ", " << top << ", " << right << ", " << bottom << ")");
        for (GList * ptr = wnck_screen_get_windows(screen); ptr != nullptr; ptr = ptr->next)
        {
            WnckWindow * window = WNCK_WINDOW(ptr->data);
            int type = wnck_window_get_window_type(window);
            if (type == WNCK_WINDOW_DOCK) {
                int x1, y1, width, height;
                wnck_window_get_geometry(window, &x1, &y1, &width, &height);
                int x2 = x1 + width - 1;
                int y2 = y1 + height - 1;
                LOG_DEBUG("Found dock with rect(" << x1 << ", " << y1 << ", " << x2 << ", " << y2 << ")");

                if (width >= height) {
                    if (y1 <= top && top <= y2) {
                        top = y2 + 1;
                        finished = false;
                    } else if (y1 <= bottom && bottom <= x2) {
                        bottom = y1 - 1;
                        finished = false;
                    }
                } else {
                    if (x1 <= left && left <= x2) {
                        left = x2 + 1;
                        finished = false;
                    } else if (x1 <= right && right <= x2) {
                        right = x1 - 1;
                        finished = false;
                    }
                }
            } else if (type == WNCK_WINDOW_DESKTOP) {
                LOG_DEBUG("Found desktop");
            }
        }
    }

    return WorkArea{
        static_cast<double> (left),
        static_cast<double> (top),
        static_cast<double> (right),
        static_cast<double> (bottom)};
}
