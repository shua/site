<pmeta id="created">2020 October 31</pmeta>
<pmeta id="title">Auslander aus Wayland</pmeta>

Trying to figure out how to get a window open in [wayland].

Debugging with `WAYLAND_DEBUG=1` really helps track protocol.
Terminology and what it corresponds to:
Output:for our purposes, the screen
Seat: inputs, keyboard, touch, mouse etc
Compositor: for our purposes, what we use to get a surface
Surface: define visuals, positioning
Shell: grouping of surface, 

XdgSurface: 
Toplevel: 
Popup:
XdgPositioner: 

[wayland]: https://wayland.freedesktop.org/
[wayland book]: https://wayland-book.com/
[wayland-rs]: https://github.com/Smithay/wayland-rs
