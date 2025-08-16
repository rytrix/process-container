# process-container

A simple c program designed to relaunch processes when they are killed, designed specifically for use with linux window managers. The specific use-case I had in mind was automatically restarting waybar upon edits instead of disowning the process from the cli every time, hyprland doesn't have a good way of doing this since execute_always will start another instance of the process each time the config is reloaded.

# Usage
```bash
process-container sway swaync gammastep
```

# Requirements
```
gcc
```

# Building
```bash
make
make install
```

# Uninstall
```bash
make uninstall
```

# Closing process-container
Killing the application will close all child processes due to a signal handler that is implemented to respond to SIGV and SIGTERM signals.

```bash
pkill process-container
```
