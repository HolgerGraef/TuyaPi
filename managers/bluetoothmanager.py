from glob import glob

from PyQt5.QtCore import pyqtSignal, QObject, QTimer


class BluetoothManager(QObject):
    updated = pyqtSignal()

    def __init__(self):
        super(BluetoothManager, self).__init__()

        self.devices = []
        self.icon = "ph.bluetooth-x"

        self.refreshTimer = QTimer()
        self.refreshTimer.setInterval(1000)
        self.refreshTimer.timeout.connect(self.refresh)
        self.refreshTimer.start()

        self.refresh()

    def refresh(self):
        devices = glob("/var/bluetooth-handler-input*")
        self.devices = [open(d, "r").read() for d in devices]

        self.icon = "ph.bluetooth-fill" if self.devices else "ph.bluetooth-x"

        self.updated.emit()
