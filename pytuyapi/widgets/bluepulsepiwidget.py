import qtawesome as qta

from PyQt5.QtWidgets import QVBoxLayout, QWidget

from . import IconButton


class BluePulsePiWidget(QWidget):
    def __init__(self, manager):
        super(BluePulsePiWidget, self).__init__()

        self.manager = manager
        self.widgets = {}

        # set up layout
        self.layout = QVBoxLayout()
        self.setLayout(self.layout)

        self.manager.updated.connect(self.refresh)
        self.refresh()

    def refresh(self):
        for d in self.manager.devices:
            # skip devices that have already been processed
            if d in self.widgets:
                continue

            self.widgets[d] = IconButton(qta.icon("fa5b.bluetooth"), d)

        remove_list = []
        for d, w in self.widgets.items():
            if d in self.manager.devices:
                # new device?
                if self.layout.indexOf(w) < 0:
                    self.layout.addWidget(w)
            else:
                # device is no longer present
                self.layout.removeWidget(w)
                remove_list.append(d)
        for d in remove_list:
            self.widgets.pop(d)
