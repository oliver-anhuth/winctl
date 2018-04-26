# winctl
Window layout scripting utility for X11 (like devilspie2) which uses relative positions (percentages)

# How to build
Make sure to meet the requirements: Core utilities, Make, C++ compiler, pkg-config, Lua >= 5.2, gdk-3 and libwnck-3 development packages
> make

# How to install (to /usr/local/bin)
> sudo make install

# How to use
> winctl my_script.lua &

Execute Lua script for every existing window, then keep running and execute script for every new window.

# Example script
```Lua
-- Make Firefox maximized
if window.type() == "normal" and window.application() == "Firefox" then
    window.maximized(true)
end
```
