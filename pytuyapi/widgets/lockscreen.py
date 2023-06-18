import qtawesome as qta

from datetime import datetime

from PyQt5.QtCore import pyqtSignal, Qt, QTimer
from PyQt5.QtGui import QPixmap
from PyQt5.QtWidgets import QWidget

from . import Overlay, UnlockOverlay


class LockScreen(Overlay):
    hidden = pyqtSignal()

    def __init__(self, parent: QWidget, bluetoothManager, wifiManager, lockTimeoutMs: int = 5000):
        super(LockScreen, self).__init__(parent)

        self.background = QPixmap("background.png")
        self.infoOverlay = Overlay(self)
        self.infoOverlay.setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents)

        self.bluetoothManager = bluetoothManager
        self.wifiManager = wifiManager

        self.extraStyles = "<style>p { margin-bottom: 120px; }</style>"

        self.lockTimer = QTimer()
        self.lockTimer.setInterval(lockTimeoutMs)
        self.lockTimer.setSingleShot(True)
        self.lockTimer.timeout.connect(self.show)
        self.lockTimer.start()

        self.refreshTimer = QTimer()
        self.refreshTimer.setInterval(1000)
        self.refreshTimer.timeout.connect(self.refresh)
        self.refreshTimer.start()

        self.unlockOverlay = UnlockOverlay(self)

        self.refresh()

    def mousePressEvent(self, event):
        self.unlockOverlay.start(0.7)

    def mouseReleaseEvent(self, event):
        if self.unlockOverlay.isVisible():
            if self.unlockOverlay.progress() >= 1.0:
                self.hide()
                self.resetLockTimer()
            self.unlockOverlay.hide()

    def refresh(self):
        date_time_str = datetime.now().strftime(self.extraStyles + "<p>%d %b %Y</p><p>%H:%M</p>")
        self.infoOverlay.setText(
            date_time_str +
            qta.charmap(self.wifiManager.icon) +
            qta.charmap(self.bluetoothManager.icon)
        )

    def resetLockTimer(self):
        self.lockTimer.start()

    def hide(self):
        super(LockScreen, self).hide()

        # skip when called from constructor
        if hasattr(self, "infoOverlay"):
            self.infoOverlay.hide()

    def show(self):
        super(LockScreen, self).show()
        self.infoOverlay.show()

        self.setPixmap(self.background.scaled(
            self.size(),
            Qt.AspectRatioMode.KeepAspectRatioByExpanding
        ))
