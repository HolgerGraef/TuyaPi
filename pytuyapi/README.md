## Setup

```sh
sudo apt install python3-pyqt5 python3-venv xorg

git clone https://github.com/HolgerGraef/TuyaPi.git
cd TuyaPi/pytuyapi
python3 -m venv --system-site-packages venv
. venv/bin/activate
pip install tinytuya qtawesome
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
