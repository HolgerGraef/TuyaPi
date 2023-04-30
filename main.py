import json
import os
import signal

from datetime import datetime
from PyQt5.QtCore import Qt, QTimer, pyqtSignal
from PyQt5.QtWidgets import (
    QApplication,
    QLabel,
    QHBoxLayout,
    QVBoxLayout,
    QSizePolicy,
    QSpacerItem,
    QWidget,
)

from widgets import BluePulsePiWidget, BulbWidget, WifiWidget

signal.signal(signal.SIGINT, signal.SIG_DFL)


class Overlay(QLabel):
    hidden = pyqtSignal()

    def __init__(self):
        super(Overlay, self).__init__()

        self.setAlignment(Qt.AlignCenter | Qt.AlignVCenter)
        self.setStyleSheet("background: rgba(0,0,0,80%); color: #CCCCCC; font-size: 200px;")

        self.extraStyles = "<style>p { margin-bottom: 120px; }</style>"

        self.timer = QTimer()
        self.timer.setInterval(1000)
        self.timer.timeout.connect(self.refresh)
        self.timer.start()

        self.refresh()

    def mousePressEvent(self, event):
        self.hide()
        self.hidden.emit()

    def refresh(self):
        date_time_str = datetime.now().strftime(self.extraStyles + "<p>%d %b %Y</p><p>%H:%M:%S</p>")
        self.setText(date_time_str)


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
                self.widgets.append(BulbWidget(d, p))

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

        # set up overlay
        self.overlay = Overlay()

        # set up overlay timer
        self.overlayTimer = QTimer()
        self.overlayTimer.setInterval(5000)
        self.overlayTimer.timeout.connect(self.overlayTimeout)
        self.overlayTimer.setSingleShot(True)

        self.overlay.hidden.connect(self.overlayTimer.start)
        self.overlayTimer.start()

        self.show()

    def mousePressEvent(self, event):
        self.overlayTimer.start()

    def overlayTimeout(self):
        self.overlay.setParent(self)
        self.overlay.resize(self.size())
        self.overlay.show()



if __name__ == "__main__":
    app = QApplication([])

    w = MainWidget()

    app.exec_()
