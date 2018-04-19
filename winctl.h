#include <iostream>
#include <stdexcept>

#include <deque>
#include <string>
#include <sstream>
#include <vector>

#include "lua.hpp"

#include <gdk/gdk.h>
#include <gdk/gdkx.h>

#define WNCK_I_KNOW_THIS_IS_UNSTABLE 1
#include <libwnck/libwnck.h>


class WinCtl
{
public:
    WinCtl()
    {
        lua_ = luaL_newstate();
        luaL_openlibs(lua_);

        lua_register(lua_, "application", application);
        lua_register(lua_, "fullscreen", fullscreen);
        lua_register(lua_, "maximized", maximized);
        lua_register(lua_, "pos", pos);
        lua_register(lua_, "rect", rect);
        lua_register(lua_, "title", title);
        lua_register(lua_, "type", type);
    }

    ~WinCtl()
    {
        lua_close(lua_);
    }

    void add_file(const std::string & file)
    {
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
        int rc = luaL_loadstring(lua_, script.c_str());
        if (rc != 0) {
            throw std::runtime_error{lua_tostring(lua_, -1)};
        }
        lua_setglobal(lua_, make_chunk_token(num_chunks_).c_str());
        ++num_chunks_;
    }

    void run_once()
    {
        WnckScreen * screen = wnck_screen_get_default();
        wnck_screen_force_update(screen);
        for (GList * ptr = wnck_screen_get_windows(screen); ptr != nullptr; ptr = ptr->next)
        {
            WnckWindow * window = WNCK_WINDOW(ptr->data);
            call(window);
        }
    }

    void run()
    {
        g_signal_connect(wnck_screen_get(0), "window-opened", G_CALLBACK(on_window_opened), this);
        GMainLoop * main_loop = g_main_loop_new(nullptr, false);
        g_main_loop_run(main_loop);
        g_main_loop_unref(main_loop);
    }

private:
    static constexpr const double Scale = 100.0;

    static std::string make_chunk_token(size_t n)
    {
        std::stringstream ss;
        ss << "e5fa8a4517c442cf9bf781cd844f528e-" << n;
        return ss.str();
    }

    static std::string make_window_token()
    {
        return std::string{"dcc6e74976d141ad8c134724df770a0c"};
    }

    void call(WnckWindow * window)
    {
        lua_pushlightuserdata(lua_, window);
        lua_setglobal(lua_, make_window_token().c_str());

        for (size_t i = 0; i < num_chunks_; ++i) {
            lua_getglobal(lua_, make_chunk_token(i).c_str());
            int rc = lua_pcall(lua_, 0, LUA_MULTRET, 0);
            if (rc) {
                throw std::runtime_error{lua_tostring(lua_, -1)};
            }
        }
    }

    static void on_window_opened(WnckScreen * screen, WnckWindow * window, gpointer data)
    {
        auto & self = *static_cast<WinCtl *> (data);
        self.call(window);
    }

    static int application(lua_State * lua);
    static int fullscreen(lua_State * lua);
    static int maximized(lua_State * lua);
    static int pos(lua_State * lua);
    static int rect(lua_State * lua);
    static int title(lua_State * lua);
    static int type(lua_State * lua);

    static std::string dump_stack(lua_State * lua);
    static WnckWindow * get_window(lua_State * lua);

    lua_State * lua_;
    size_t num_chunks_ = 0;
};
