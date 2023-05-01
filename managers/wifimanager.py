import subprocess

from PyQt5.QtCore import pyqtSignal, QObject, QTimer


class WifiManager(QObject):
    updated = pyqtSignal()

    def __init__(self):
        super(WifiManager, self).__init__()

        self.icon = "ph.wifi-slash"
        self.essid = ""
        self.signal = -100

        self.refreshTimer = QTimer()
        self.refreshTimer.setInterval(1000)
        self.refreshTimer.timeout.connect(self.refresh)
        self.refreshTimer.start()

    def refresh(self):
        completed = subprocess.run(["/usr/sbin/iwconfig", "wlan0"], capture_output=True)
        result = [
            line.strip()
            for line in completed.stdout.decode("utf-8").split("\n")
        ]
        essid_result = [line for line in result if "ESSID" in line]
        signal_result = [line for line in result if "Signal level" in line]
        if not essid_result or not signal_result:
            self.icon = "ph.wifi-slash"
            self.essid = ""
            self.signal = -100
        else:
            self.essid = essid_result[0].split(":")[1].strip("\"")
            self.signal = int(signal_result[0].split("=")[2].split(" ")[0])
            self.icon = self._signalToIcon(self.signal)

        self.updated.emit()

    @staticmethod
    def _signalToIcon(signal: int) -> str:
        if signal >= -30:
            return "ph.wifi-high"
        elif signal >= -50:
            return "ph.wifi-high"
        elif signal >= -60:
            return "ph.wifi-medium"
        elif signal >= -67:
            return "ph.wifi-medium"
        else:
            return "ph.wifi-low"
