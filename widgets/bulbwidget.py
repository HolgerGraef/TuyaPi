import tinytuya

from collections import namedtuple
from functools import partial

from PyQt5.QtCore import QThread, pyqtSignal
from PyQt5.QtWidgets import QPushButton, QSizePolicy, QSpacerItem, QVBoxLayout, QWidget

from . import IconButton

BulbState = namedtuple("BulbState", ["state", "status"])


class BulbWorker(QThread):
    stateChanged = pyqtSignal(BulbState)

    def __init__(self, device: dict):
        super(BulbWorker, self).__init__()

        self.bulb_state = None
        self.device_desc = device
        self.start()
    
    def run(self):
        self.device = tinytuya.BulbDevice(
            self.device_desc["id"],
            self.device_desc["ip"],
            self.device_desc["key"],
        )
        self.device.set_version(3.3)

        while True:
            bulb_state = BulbState(state=self.device.state(), status=self.device.status())
            if bulb_state != self.bulb_state:
                self.bulb_state = bulb_state
                self.stateChanged.emit(bulb_state)

                print("Status of {}: {}".format(self.device_desc["name"], bulb_state.status))
                print("State of {}: {}".format(self.device_desc["name"], bulb_state.state))
            self.sleep(1)


class BulbWidget(QWidget):
    def __init__(self, device: dict, presets: dict):
        super(BulbWidget, self).__init__()

        self.worker = BulbWorker(device)

        self.presets = presets

        self.b_toggle = IconButton("mdi6.lightbulb-off-outline", device["name"])
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
            self.b_toggle.setIcon("mdi6.lightbulb-on")
        else:
            self.b_toggle.setIcon("mdi6.lightbulb-off")
