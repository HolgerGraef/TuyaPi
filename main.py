import json
import signal
import tinytuya

signal.signal(signal.SIGINT, signal.SIG_DFL)

from PyQt5.QtWidgets import (
    QApplication,
    QHBoxLayout,
    QVBoxLayout,
    QPushButton,
    QSizePolicy,
    QSpacerItem,
    QWidget,
)


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


class ControlWidget(QWidget):
    def __init__(self):
        super(ControlWidget, self).__init__()

        self.setWindowTitle("Control Widget")
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

        # set up layout
        l = QHBoxLayout()
        for w in self.widgets:
            l.addWidget(w)
        self.setLayout(l)

        self.show()


if __name__ == "__main__":
    app = QApplication([])

    w = ControlWidget()

    app.exec_()
