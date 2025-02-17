#! /bin/bash

pipewire &
pipewire-pulse &

picom --experimental-backends --config ~/.config/picom.conf &

/usr/bin/emacs --daemon &

pgrep -x copyq > /dev/null || copyq &
pgrep -x fcitx5 > /dev/null || fcitx5 &
pgrep -x flameshot > /dev/null || flameshot &
pgrep -x xfce4-power-manager > /dev/null || xfce4-power-manager &
# xsetroot -cursor_name left_ptr
nitrogen --restore &
# xset dpms 600 600 600
