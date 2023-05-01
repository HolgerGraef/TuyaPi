from PyQt5.QtCore import QTimer
from PyQt5.QtWidgets import QWidget


class RefreshingWidget(QWidget):
    def __init__(self, intervalMs: int = 1000):
        super(RefreshingWidget, self).__init__()

        self.refresh_timer = QTimer()
        self.refresh_timer.setInterval(intervalMs)
        self.refresh_timer.timeout.connect(self.refresh)
        self.refresh_timer.start()

        self.refresh()

    def refresh(self):
        pass
