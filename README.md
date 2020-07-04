# Simple Terminal

![](/st.png)

##### This is my personal build of [Suckless's](https://st.suckless.org) Simple Terminal. 
Each branch of this repository contains a feature implemented on top of the base build of St from Suckless.  

All desired features are then merged into the ***current_config*** branch. 
The current build contains patches from all other branches, other than the alpha patch.

## Patches
All patches that I have applied are located in [patches](./patches).

## Fonts and colors
**Fonts:**

+ [Cherry](https://github.com/turquoise-hexagon/cherry)
+ [JoyPixels](https://www.joypixels.com)

Cherry is a bitmap font. If there are any issues with displaying this font, make sure the file `/etc/fonts/conf.d/70-no-bitmaps.conf` does not exist. 

See [below](#use-my-build) for notes about making sure JoyPixels, and emoji fonts in general, work properly.

**Colors:**

My colorschemes are available in [colors](./colors).

The current build also supports reading Xresources. This is not necessary to replicate my build, but provides added convenience. If colors/settings are defined in Xresources, those settings will take precedence to the build in options.

## Bindings
+ **Follow URLs:** `alt-l`
+ **Copy URLs:** `alt-y`
+ **Copy the output of commands:** `alt-o`

+ **Scrollback:** `alt-↑/↓` or `alt-pageup/pagedown` or by scrolling while holding `shift`
+ **Scrollback with vim keys:** `alt-k/j`; faster scrolling with `alt-u/d`
+ **Zoom:** `shift + alt-j/k` `alt-home` returns zoom to default.
+ **Copy text:** `alt-c` 
+ **Paste text:** `alt-v` or `shift-insert`

## Use my build
Since my current build supports emoji's, it is required replace libXft with the patched version until the fix gets implemented in the official build.  

[libxft-bgra](https://aur.archlinux.org/packages/libxft-bgra/) is available in the AUR.

After replacing the standard libXft, you can build and install St by cloning this repository (insuring you are on the *current_config* branch).

Finally, run:
```shell
# make install
```

After making changes, be sure to include the clean target when rebuilding St:
```shell
# make clean install
```
