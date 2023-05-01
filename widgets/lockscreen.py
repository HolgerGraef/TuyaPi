from datetime import datetime

from PyQt5.QtCore import pyqtSignal, QTimer

from . import Overlay, UnlockOverlay


class LockScreen(Overlay):
    hidden = pyqtSignal()

    def __init__(self):
        super(LockScreen, self).__init__()

        self.extraStyles = "<style>p { margin-bottom: 120px; }</style>"

        self.timer = QTimer()
        self.timer.setInterval(1000)
        self.timer.timeout.connect(self.refresh)
        self.timer.start()

        self.unlockOverlay = UnlockOverlay()

        self.refresh()

    def mousePressEvent(self, event):
        self.unlockOverlay.setParent(self)
        self.unlockOverlay.resize(self.size())
        self.unlockOverlay.start(0.7)
        self.unlockOverlay.show()

    def mouseReleaseEvent(self, event):
        if self.unlockOverlay.isVisible():
            if self.unlockOverlay.progress() >= 1.0:
                self.hide()
                self.hidden.emit()
            self.unlockOverlay.hide()

    def refresh(self):
        date_time_str = datetime.now().strftime(self.extraStyles + "<p>%d %b %Y</p><p>%H:%M</p>")
        self.setText(date_time_str)
