import subprocess
import qtawesome as qta

from PyQt5.QtCore import QTimer

from . import IconButton


class WifiWidget(IconButton):
    def __init__(self):
        super(WifiWidget, self).__init__(qta.icon("fa5s.wifi"), "WiFi")

        # set up timer
        self.refresh_timer = QTimer()
        self.refresh_timer.setInterval(1000)
        self.refresh_timer.timeout.connect(self.refresh)
        self.refresh_timer.start()

        self.refresh()

    def refresh(self):
        completed = subprocess.run(["/usr/sbin/iwconfig", "wlan0"], capture_output=True)
        result = [
            line.strip()
            for line in completed.stdout.decode("utf-8").split("\n")
        ]
        essid_result = [line for line in result if "ESSID" in line]
        signal_result = [line for line in result if "Signal level" in line]
        if not essid_result or not signal_result:
            self.label.setText("Not connected")
            self.setIcon(qta.icon("ph.wifi-slash"))
        else:
            essid = essid_result[0].split(":")[1].strip("\"")
            signal = int(signal_result[0].split("=")[2].split(" ")[0])
            self.setIcon(self._signalToIcon(signal))
            self.label.setText("{}  |  {} dBm".format(essid, signal))

    @staticmethod
    def _signalToIcon(signal: int):
        if signal >= -30:
            return qta.icon("ph.wifi-high")
        elif signal >= -50:
            return qta.icon("ph.wifi-high")
        elif signal >= -60:
            return qta.icon("ph.wifi-medium")
        elif signal >= -67:
            return qta.icon("ph.wifi-medium")
        else:
            return qta.icon("ph.wifi-low")
