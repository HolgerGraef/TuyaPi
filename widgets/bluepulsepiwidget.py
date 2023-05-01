import qtawesome as qta

from glob import glob

from PyQt5.QtWidgets import QVBoxLayout

from . import IconButton
from . import RefreshingWidget


class BluePulsePiWidget(RefreshingWidget):
    def __init__(self):
        self.widgets = {}

        super(BluePulsePiWidget, self).__init__()

        # set up layout
        self.layout = QVBoxLayout()
        self.setLayout(self.layout)

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
