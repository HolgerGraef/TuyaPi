import qtawesome as qta

from glob import glob

from PyQt5.QtCore import QTimer
from PyQt5.QtWidgets import QVBoxLayout, QWidget

from . import IconButton


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
