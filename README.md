## Setup

Install Raspberry Pi OS Lite, then:

```sh
sudo apt install python3-pyqt5 python3-venv xorg

git clone https://github.com/HolgerGraef/TuyaPi.git
cd TuyaPi
python3 -m venv --system-site-packages venv
. venv/bin/activate
pip install tinytuya qtawesome
```

### /boot/config.txt

```
hdmi_force_hotplug=1
hdmi_timings=1080 1 26 4 36 1920 1 8 4 16 0 0 0 60 0 138000000 3
hdmi_group=2
hdmi_mode=87
hdmi_force_mode=1
display_hdmi_rotate=3
max_framebuffer_width=1080
max_framebuffer_height=1920

# Enable DRM VC4 V3D driver
# disabled for touchscreen
# dtoverlay=vc4-kms-v3d
# max_framebuffers=2
```

### /usr/share/X11/xorg.conf.d/40-libinput.conf

```
...
Section "InputClass"
        Identifier "libinput touchscreen catchall"
        MatchIsTouchscreen "on"
        MatchDevicePath "/dev/input/event*"
        Driver "libinput"
        Option "TransformationMatrix" "0 -1 1 1 0 0 0 0 1"
EndSection
...
```

## Start X and allow control via SSH

```sh
# TODO: can we do this without sudo?
sudo startx &
sudo DISPLAY=:0 xhost +
```

## Start application

```sh
DISPLAY=:0 python main.py
```

## For autostart

Set up automatic login. For instructions, see:
https://github.com/HolgerGraef/BluePulsePi#set-up-auto-login-so-that-pulseaudio-starts

Add these lines at the end of `/home/pi/.profile`:

```sh
if ! DISPLAY=:0 timeout 1s xset q &>/dev/null; then
  startx
else
  echo "X is already running :-)"
fi
```

Create `/home/pi/.xinitrc` and add the following line in the file:

```sh
cd /home/pi/TuyaPi && . venv/bin/activate && DISPLAY=:0 python main.py
```

## TinyTuya setup

For setup instructions, go to https://pypi.org/project/tinytuya/, section "Setup Wizard - Getting Local Keys".
Here just an extract:

### Using Tuya wizard

```sh
mkdir -p tinytuya && cd tinytuya
python -m tinytuya wizard
```

## References

- https://learn.adafruit.com/using-weird-displays-with-raspberry-pi/everything-else
- https://gist.github.com/cnlohr/15d3440f72d143a26739ce8e20dcc76c
- https://www.gechic.com/en/raspberry-pi-touch-monitor-rotate-touch-screen-rotate-settings/
- https://wiki.seeedstudio.com/reTerminal-build-UI-using-Qt-for-Python/
- https://pypi.org/project/tinytuya/
- https://github.com/codetheweb/tuyapi
- https://www.cnx-software.com/2021/12/13/blink-an-led-on-esp32-board-with-tuya-link-sdk/
- https://github.com/spyder-ide/qtawesome#supported-fonts
