#ifndef INCLUDED_0C5D6352_8F57_43C5_93B6_CF2408A5B072
#define INCLUDED_0C5D6352_8F57_43C5_93B6_CF2408A5B072

#include "logger.h"

#include <iostream>
#include <stdexcept>

#include <deque>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#include <lua.hpp>

#define WNCK_I_KNOW_THIS_IS_UNSTABLE 1
#include <libwnck/libwnck.h>


class WinCtl
{
public:
    WinCtl()
    {
        LOG_DEBUG("Initializing Lua");
        lua_ = luaL_newstate();
        luaL_openlibs(lua_);

        lua_pushlightuserdata(lua_, this);
        lua_setglobal(lua_, make_self_token());

        register_window_functions();
    }

    ~WinCtl()
    {
        LOG_DEBUG("Shutting down Lua");
        lua_close(lua_);
    }

    void add_file(const std::string & file)
    {
        LOG_DEBUG("Loading Lua script from file " << file);
        int rc = luaL_loadfile(lua_, file.c_str());
        if (rc != 0) {
            throw std::runtime_error{lua_tostring(lua_, -1)};
        }
        lua_setglobal(lua_, make_chunk_token(num_chunks_).c_str());
        ++num_chunks_;
    }

    template <typename Container>
    void add_files(const Container & files)
    {
        for (const std::string & file : files) {
            add_file(file);
        }
    }

    void add_script(const std::string & script)
    {
        LOG_DEBUG("Loading Lua script from string");
        int rc = luaL_loadstring(lua_, script.c_str());
        if (rc != 0) {
            throw std::runtime_error{lua_tostring(lua_, -1)};
        }
        lua_setglobal(lua_, make_chunk_token(num_chunks_).c_str());
        ++num_chunks_;
    }

    void run_once()
    {
        LOG_DEBUG("Processing existing windows once");
        WnckScreen * screen = wnck_screen_get_default();
        wnck_screen_force_update(screen);
        for (GList * ptr = wnck_screen_get_windows(screen); ptr != nullptr; ptr = ptr->next)
        {
            WnckWindow * window = WNCK_WINDOW(ptr->data);
            call_chunks(window);
        }
    }

    void run()
    {
        LOG_DEBUG("Waiting for new windows");
        WnckScreen * screen = wnck_screen_get_default();
        wnck_screen_force_update(screen);
        g_signal_connect(wnck_screen_get(0), "window-opened", G_CALLBACK(on_window_opened), this);
        GMainLoop * main_loop = g_main_loop_new(nullptr, false);
        g_main_loop_run(main_loop);
        g_main_loop_unref(main_loop);
    }

    void run_script(const std::string & script)
    {
        LOG_DEBUG("Run script from string once");
        int rc = luaL_loadstring(lua_, script.c_str());
        if (rc != 0) {
            throw std::runtime_error{lua_tostring(lua_, -1)};
        }

        rc = lua_pcall(lua_, 0, LUA_MULTRET, 0);
        if (rc != 0) {
            throw std::runtime_error{lua_tostring(lua_, -1)};
        }
    }

    static void print_functions_and_exit()
    {
        std::cout << "\tLua functions available for each window (e.g. window.title() or window.fullscreen(true)).\n"
            << "\n"
            << "above:\n"
            << "\tIs/Set window above other windows (above(), above(true) or above(false)).\n"
            << "\n"
            << "application:\n"
            << "\tGet application of the window.\n"
            << "\n"
            << "below:\n"
            << "\tIs/Set window below other windows.\n"
            << "\n"
            << "fullscreen:\n"
            << "\tIs/Set window fullscreen.\n"
            << "\n"
            << "maximized:\n"
            << "\tIs/Set window maximized.\n"
            << "\n"
            << "minimized:\n"
            << "\tIs/Set window minimized.\n"
            << "\n"
            << "pinned:\n"
            << "\tIs/Set window visible on all workspaces.\n"
            << "\n"
            << "pos:\n"
            << "\tGet/Set window position (pos() or pos(x, y)).\n"
            << "\tWindow positions are specified in percentages of the work area.\n"
            << "\n"
            << "rect:\n"
            << "\tGet/Set window rectangle (rect() or rect(x0, y0, x1, y1)).\n"
            << "\tWindow positions are specified in percentages of the work area.\n"
            << "\n"
            << "role:\n"
            << "\tGet window role.\n"
            << "\n"
            << "title:\n"
            << "\tGet window title.\n"
            << "\n"
            << "type:\n"
            << "\tGet window type as string.\n"
            << "\n";
        std::exit(EXIT_SUCCESS);
    };

private:
    static const char * make_self_token()
    {
        return "a2f7faddb923497aa4e9f11ea8ebef1d";
    }

    static std::string make_chunk_token(size_t n)
    {
        std::stringstream ss;
        ss << "e5fa8a4517c442cf9bf781cd844f528e-" << n;
        return ss.str();
    }

    static const char * make_window_token()
    {
        return "dcc6e74976d141ad8c134724df770a0c";
    }

    static WinCtl & get_self(lua_State * lua)
    {
        lua_getglobal(lua, make_self_token());
        WinCtl * self = static_cast<WinCtl *> (lua_touserdata(lua, -1));
        lua_pop(lua, 1);
        return *self;
    }

    static WnckWindow * get_window(lua_State * lua)
    {
        lua_getglobal(lua, make_window_token());
        WnckWindow * window = static_cast<WnckWindow *> (lua_touserdata(lua, -1));
        lua_pop(lua, 1);
        return window;
    }

    void call_chunks(WnckWindow * window)
    {
        lua_pushlightuserdata(lua_, window);
        lua_setglobal(lua_, make_window_token());

        for (size_t i = 0; i < num_chunks_; ++i) {
            lua_getglobal(lua_, make_chunk_token(i).c_str());
            int rc = lua_pcall(lua_, 0, LUA_MULTRET, 0);
            if (rc != 0) {
                throw std::runtime_error{lua_tostring(lua_, -1)};
            }
        }
    }

    static void on_window_opened(WnckScreen * screen, WnckWindow * window, gpointer data)
    {
        LOG_INFO("New window opened on screen " << wnck_screen_get_number(screen));
        auto & self = *static_cast<WinCtl *> (data);
        self.call_chunks(window);
    }

    static int above(lua_State * lua);
    static int application(lua_State * lua);
    static int below(lua_State * lua);
    static int fullscreen(lua_State * lua);
    static int maximized(lua_State * lua);
    static int minimized(lua_State * lua);
    static int pinned(lua_State * lua);
    static int pos(lua_State * lua);
    static int rect(lua_State * lua);
    static int role(lua_State * lua);
    static int title(lua_State * lua);
    static int type(lua_State * lua);

    void register_window_functions()
    {
        LOG_DEBUG("Registering Lua functions");
        static luaL_Reg window_functions[] =
        {
            "above", above,
            "application", application,
            "below", below,
            "fullscreen", fullscreen,
            "maximized", maximized,
            "minimized", minimized,
            "pinned", pinned,
            "pos", pos,
            "rect", rect,
            "role", role,
            "title", title,
            "type", type,
            nullptr, nullptr
        };
        luaL_newlib(lua_, window_functions);
        lua_setglobal(lua_, "window");
    }

    static std::string dump_stack(lua_State * lua);

    struct WorkArea
    {
        double left, top, right, bottom;
        WorkArea(double l, double t, double r, double b) : left{l}, top{t}, right{r}, bottom{b} {}
    };

    const WorkArea & get_work_area(WnckScreen * screen)
    {
        int screen_number = wnck_screen_get_number(screen);
        auto iter = work_areas_.find(screen_number);
        if (iter == work_areas_.end())
        {
            WorkArea work_area = calculate_work_area(screen);
            iter = work_areas_.insert(std::make_pair(screen_number, work_area)).first;
        }
        return iter->second;
    }

    WorkArea calculate_work_area(WnckScreen * screen);

    lua_State * lua_;
    size_t num_chunks_ = 0;
    std::map<int, WorkArea> work_areas_;
};

#endif // INCLUDED_0C5D6352_8F57_43C5_93B6_CF2408A5B072
