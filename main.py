import json
import os
import signal
import subprocess
import tinytuya

from functools import partial
from glob import glob

from PyQt5.QtCore import QTimer
from PyQt5.QtWidgets import (
    QApplication,
    QHBoxLayout,
    QVBoxLayout,
    QPushButton,
    QSizePolicy,
    QSpacerItem,
    QWidget,
)

import qtawesome as qta

from widgets import IconButton

signal.signal(signal.SIGINT, signal.SIG_DFL)


class BulbWidget(QWidget):
    def __init__(self, label: str, device: tinytuya.BulbDevice, presets: dict):
        super(BulbWidget, self).__init__()

        self.device = device
        self.device.set_version(3.3)
        print("Status of {}: {}".format(label, self.device.status()))
        print("Sate of {}: {}".format(label, self.device.state()))

        self.presets = presets

        self.b_toggle = QPushButton(label)
        self.update_toggle_button()
        self.b_toggle.clicked.connect(self.toggle)

        # set up layout
        l = QVBoxLayout()
        l.addWidget(self.b_toggle)

        for p in self.presets:
            w = QPushButton(p["label"])
            l.addWidget(w)
            w.clicked.connect(partial(self.handle_preset, p))

        l.addSpacerItem(QSpacerItem(0, 0, vPolicy=QSizePolicy.Expanding))
        self.setLayout(l)

    def toggle(self):
        if self.device.state()["is_on"]:
            self.device.turn_off()
        else:
            self.device.turn_on()
        self.update_toggle_button()

    def handle_preset(self, preset):
        try:
            # make sure bulb is on
            self.device.turn_on()

            if preset["mode"] == "white":
                self.device.set_mode("white")
                self.device.set_white_percentage(
                    preset["brightness"], preset["colourtemp"]
                )
            elif preset["mode"] == "dps":
                for k, v in preset["dps"].items():
                    self.device.set_value(k, v)
        except Exception as e:
            print("Failed to set brightness: {}".format(e))

        self.update_toggle_button()

    def update_toggle_button(self):
        try:
            state = self.device.state()
            is_on = state["is_on"]
        except Exception as e:
            print("Failed to get device state: {}".format(e))
            return

        if is_on:
            self.b_toggle.setStyleSheet("QPushButton { background-color: #00FF00; }")
        else:
            self.b_toggle.setStyleSheet("QPushButton { background-color: #FF0000; }")


class WifiWidget(IconButton):
    def __init__(self):
        super(WifiWidget, self).__init__(qta.icon("fa5s.wifi"), "WiFi")

        # set up timer
        self.refresh_timer = QTimer()
        self.refresh_timer.setInterval(1000)
        self.refresh_timer.timeout.connect(self.refresh)
        self.refresh_timer.start()

        self.refresh()

    def refresh(self):
        completed = subprocess.run(["/usr/sbin/iwconfig", "wlan0"], capture_output=True)
        result = [
            line.strip()
            for line in completed.stdout.decode("utf-8").split("\n")
            if "Signal level" in line
        ]
        if not result:
            self.setIcon(qta.icon("ph.wifi-slash"))
            self.label.setText("Not connected")
        else:
            signal = int(result[0].split("=")[2].split(" ")[0])
            self.setIcon(self._signalToIcon(signal))
            self.label.setText("{} dBm".format(signal))

    @staticmethod
    def _signalToIcon(signal: int):
        if signal >= -30:
            return qta.icon("ph.wifi-high")
        elif signal >= -50:
            return qta.icon("ph.wifi-high")
        elif signal >= -60:
            return qta.icon("ph.wifi-medium")
        elif signal >= -67:
            return qta.icon("ph.wifi-medium")
        else:
            return qta.icon("pa.wifi-low")

class BluePulsePiWidget(QWidget):
    def __init__(self):
        super(BluePulsePiWidget, self).__init__()

        self.widgets = {}

        # set up layout
        self.layout = QVBoxLayout()
        self.setLayout(self.layout)

        # set up timer
        self.refresh_timer = QTimer()
        self.refresh_timer.setInterval(1000)
        self.refresh_timer.timeout.connect(self.refresh)
        self.refresh_timer.start()

        self.refresh()

    def refresh(self):
        devices = glob("/var/bluetooth-handler-input*")
        devices = [open(d, "r").read() for d in devices]
        for d in devices:
            # skip devices that have already been processed
            if d in self.widgets:
                continue

            self.widgets[d] = IconButton(qta.icon("fa5b.bluetooth"), d)

        remove_list = []
        for d, w in self.widgets.items():
            if d in devices:
                # new device?
                if self.layout.indexOf(w) < 0:
                    self.layout.addWidget(w)
            else:
                # device is no longer present
                self.layout.removeWidget(w)
                remove_list.append(d)
        for d in remove_list:
            self.widgets.pop(d)


class MainWidget(QWidget):
    def __init__(self):
        super(MainWidget, self).__init__()

        self.setGeometry(0, 0, 1920, 1080)

        self.setStyleSheet("QWidget { font-size: 42pt; padding: 50px; }")

        presets = json.load(open("presets.json", "r"))

        devices = json.load(open("tinytuya/devices.json", "r"))
        self.widgets = []
        for d in devices:
            if d["id"] in presets:
                p = presets[d["id"]]
            else:
                p = presets["default"]

            if d["category"] == "dj":  # light bulb
                self.widgets.append(
                    BulbWidget(
                        d["name"],
                        tinytuya.BulbDevice(
                            d["id"],
                            d["ip"],
                            d["key"],
                        ),
                        p,
                    )
                )

        # set up layout
        l = QHBoxLayout()
        for w in self.widgets:
            l.addWidget(w)

        l2 = QVBoxLayout()
        l2.addLayout(l)
        l2.addSpacerItem(QSpacerItem(0, 0, vPolicy=QSizePolicy.Expanding))

        l2.addWidget(WifiWidget())

        # only for BluePulsePi
        if os.path.exists("/home/pi/BluePulsePi"):
            l2.addWidget(BluePulsePiWidget())

        self.setLayout(l2)

        self.show()


if __name__ == "__main__":
    app = QApplication([])

    w = MainWidget()

    app.exec_()
