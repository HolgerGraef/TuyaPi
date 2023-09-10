[![CI](https://github.com/hgrf/TuyaPi/actions/workflows/ci.yml/badge.svg)](https://github.com/hgrf/TuyaPi/actions/workflows/ci.yml)

## Setup

Download and set up the 64-bit version of Raspberry Pi OS Lite on your Raspberry Pi 4. The
following was tested with
[this version](https://downloads.raspberrypi.org/raspios_lite_arm64/images/raspios_lite_arm64-2023-05-03/2023-05-03-raspios-bullseye-arm64-lite.img.xz).

### Install latest TuyaPi build

```sh
sudo apt install xorg libgles2-mesa libharfbuzz0b libmd4c0 libdouble-conversion3 \
        libpcre2-16-0 libxcb-icccm4 libxcb-image0 libxcb-keysyms1 libxcb-render-util0 \
        libxcb-xinerama0 libxcb-xkb1 libxcb-xinput0 libxkbcommon0 libxkbcommon-x11-0
wget -O - "https://github.com/hgrf/qtxrpi/releases/download/v5.15.3-3/qt5.15.tar.gz" | sudo tar -C / -xz
echo /opt/qtxrpi/qt5.15/lib | sudo tee /etc/ld.so.conf.d/qt5.15.conf
sudo ldconfig
sudo wget -O /usr/bin/TuyaPi https://github.com/hgrf/TuyaPi/releases/download/v0.1.1/TuyaPi
sudo chmod +x /usr/bin/TuyaPi
```

### Start X and allow control via SSH

```sh
# TODO: can we do this without sudo?
sudo startx &
sudo DISPLAY=:0 xhost +
```

### Start application

```sh
DISPLAY=:0 TuyaPi -platform xcb
```

### For autostart

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
DISPLAY=:0 TuyaPi -platform xcb
```

### Background setup

Add a file `background.png` in the home folder.

### Splashscreen setup

```sh
sudo apt install fbi
sudo su
cat <<EOF > /etc/systemd/system/splashscreen.service
[Unit]
Description=Splash screen
DefaultDependencies=no
After=local-fs.target

[Service]
ExecStart=/usr/bin/fbi -d /dev/fb0 --noverbose -a /home/pi/background.png
StandardInput=tty
StandardOutput=tty

[Install]
WantedBy=sysinit.target
EOF
systemctl enable splashscreen
```

Add `disable_splash=1` in `/boot/config.txt`.

Add `logo.nologo` to `/boot/cmdline.txt`.

### TinyTuya setup

For setup instructions, go to https://pypi.org/project/tinytuya/, section "Setup Wizard - Getting Local Keys".
Here just an extract:


```sh
sudo apt install python3-pip
pip install tinytuya
cd ~
mkdir -p tinytuya && cd tinytuya
python -m tinytuya wizard
```

### Timezone setup

e.g.:

```sh
sudo timedatectl set-timezone Europe/Paris
```

## Setup of the custom touchscreen

**NOTE:** HDMI timings are specific to the module.

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
# see https://www.raspberrypi.com/documentation/computers/config_txt.html#display_hdmi_rotate
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

## Release instructions

* update version number in `src/version.h`
* update version number in installation instructions above
* update changelog
* add release commit and tag it
* once pipeline has finished, update release notes on GitHub

## References

- https://learn.adafruit.com/using-weird-displays-with-raspberry-pi/everything-else
- https://gist.github.com/cnlohr/15d3440f72d143a26739ce8e20dcc76c
- https://www.gechic.com/en/raspberry-pi-touch-monitor-rotate-touch-screen-rotate-settings/
- https://wiki.seeedstudio.com/reTerminal-build-UI-using-Qt-for-Python/
- https://pypi.org/project/tinytuya/
- https://github.com/codetheweb/tuyapi
- https://www.cnx-software.com/2021/12/13/blink-an-led-on-esp32-board-with-tuya-link-sdk/
- https://github.com/spyder-ide/qtawesome#supported-fonts
- https://eskimon.fr/ajouter-un-splash-screen-de-boot-sur-sa-raspberrypi
- https://raspberrypi.stackexchange.com/a/3488
- https://yingtongli.me/blog/2016/12/21/splash.html
