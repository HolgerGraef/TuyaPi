import qtawesome as qta

from . import IconButton


class WifiWidget(IconButton):
    def __init__(self, manager):
        super(WifiWidget, self).__init__(qta.icon("fa5s.wifi"), "WiFi")

        self.manager = manager
        self.manager.updated.connect(self.refresh)
        self.refresh()

    def refresh(self):
        self.setIcon(self.manager.icon)
        if self.manager.essid:
            self.label.setText("{}  |  {} dBm".format(self.manager.essid, self.manager.signal))
        else:
            self.label.setText("Not connected")
