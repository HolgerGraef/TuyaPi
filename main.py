import json
import os
import signal
import tinytuya

from glob import glob

from PyQt5.QtCore import Qt, QSize, QTimer
from PyQt5.QtWidgets import (
    QApplication,
    QLabel,
    QHBoxLayout,
    QVBoxLayout,
    QPushButton,
    QSizePolicy,
    QSpacerItem,
    QWidget,
)

import qtawesome as qta

signal.signal(signal.SIGINT, signal.SIG_DFL)


class BulbWidget(QWidget):
    def __init__(self, label, device):
        super(BulbWidget, self).__init__()

        self.device = device
        self.device.set_version(3.3)
        print("Status of {}: {}".format(label, self.device.status()))
        print("Sate of {}: {}".format(label, self.device.state()))

        self.b_toggle = QPushButton(label)
        self.update_toggle_button()

        self.b_bright = QPushButton("Bright")
        self.b_comfy = QPushButton("Comfy")

        # set up layout
        l = QVBoxLayout()
        l.addWidget(self.b_toggle)
        l.addWidget(self.b_bright)
        l.addWidget(self.b_comfy)
        l.addSpacerItem(QSpacerItem(0, 0, vPolicy=QSizePolicy.Expanding))
        self.setLayout(l)

        # make connections
        self.b_toggle.clicked.connect(self.toggle)
        self.b_bright.clicked.connect(self.set_bright)
        self.b_comfy.clicked.connect(self.set_comfy)

    def toggle(self):
        if self.device.state()["is_on"]:
            self.device.turn_off()
        else:
            self.device.turn_on()
        self.update_toggle_button()

    def set_bright(self):
        try:
            # make sure bulb is on
            self.device.turn_on()

            if isinstance(self.device.state()["mode"], int):
                self.device.set_mode(255)
                self.device.set_brightness(255)
            else:
                self.device.set_mode("white")
                self.device.set_white_percentage(100, 100)
        except Exception as e:
            print("Failed to set brightness: {}".format(e))

        self.update_toggle_button()

    def set_comfy(self):
        try:
            # make sure bulb is on
            self.device.turn_on()

            if isinstance(self.device.state()["mode"], int):
                # this is actually the brightness (10%) on my lightbulb
                self.device.set_mode(48)
                # this is actually the color temperature on my lightbulb
                self.device.set_brightness(0)
            else:
                self.device.set_mode("white")
                self.device.set_white_percentage(0, 10)
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


class BluePulsePiWidget(QWidget):
    def __init__(self):
        super(BluePulsePiWidget, self).__init__()

        self.widgets = {}
        self.spacer = QSpacerItem(0, 0, vPolicy=QSizePolicy.Expanding)

        # set up layout
        self.layout = QVBoxLayout()
        self.layout.addSpacerItem(self.spacer)
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
        icon = qta.icon("fa5b.bluetooth")
        for d in devices:
            # skip devices that have already been processed
            if d in self.widgets:
                continue

            # credit: https://stackoverflow.com/a/61130602
            b = QPushButton()
            b.setIcon(icon)
            b.setIconSize(QSize(64, 64))
            # align the icon left
            b.setStyleSheet("text-align: left;")
            b.setLayout(QHBoxLayout())

            l = QLabel(d)
            l.setAlignment(Qt.AlignCenter | Qt.AlignVCenter)
            l.setAttribute(Qt.WA_TransparentForMouseEvents, True)
            # TODO: find better way to show all text
            l.setStyleSheet("margin-top: 100px; margin-bottom: 100px;")

            b.layout().addWidget(l)

            self.widgets[d] = b

        self.layout.removeItem(self.spacer)
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
        self.layout.addSpacerItem(self.spacer)


class MainWidget(QWidget):
    def __init__(self):
        super(MainWidget, self).__init__()

        self.setGeometry(0, 0, 1920, 1080)

        self.setStyleSheet("QWidget { font-size: 42pt; padding: 50px; }")

        devices = json.load(open("tinytuya/devices.json", "r"))
        self.widgets = []
        for d in devices:
            if d["category"] == "dj":  # light bulb
                self.widgets.append(BulbWidget(
                    d["name"],
                    tinytuya.BulbDevice(
                        d["id"],
                        d["ip"],
                        d["key"],
                    )
                ))

        # only for BluePulsePi
        if os.path.exists("/home/pi/BluePulsePi"):
            self.widgets.append(BluePulsePiWidget())

        # set up layout
        l = QHBoxLayout()
        for w in self.widgets:
            l.addWidget(w)
        self.setLayout(l)

        self.show()


if __name__ == "__main__":
    app = QApplication([])

    w = MainWidget()

    app.exec_()
