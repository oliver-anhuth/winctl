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

> winctl --print-windows

Execute once and print code to restore all open windows to their current position.

> winctl --help

List available options.

> winctl --help-functions

List available Lua functions for each window.

# Example script
```Lua
-- Make Firefox maximized
if window.type() == "normal" and window.application() == "Firefox" then
    window.maximized(true)
end

-- Make Google Chrome span 90% of the workspace area
if window.type() == "normal" and string.match(window.application(), "Google Chrome$") then
    window.rect(1, 1, 91, 91)
end
```
