import json
import os
import signal
import tinytuya

from PyQt5.QtWidgets import (
    QApplication,
    QHBoxLayout,
    QVBoxLayout,
    QSizePolicy,
    QSpacerItem,
    QWidget,
)

from widgets import BluePulsePiWidget, BulbWidget, WifiWidget

signal.signal(signal.SIGINT, signal.SIG_DFL)


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
