import tinytuya

from functools import partial

from PyQt5.QtWidgets import QPushButton, QSizePolicy, QSpacerItem, QVBoxLayout, QWidget


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
