import time

from PyQt5.QtCore import Qt, QRectF, QTimer
from PyQt5.QtGui import QColor, QPainter, QPen
from PyQt5.QtWidgets import QLabel


class UnlockOverlay(QLabel):
    def __init__(self):
        super(UnlockOverlay, self).__init__()

        self.setStyleSheet("background: rgba(0,0,0,80%); color: #CCCCCC; font-size: 200px;")

        self.startTime = 0
        self.duration = 1.0
        self.diameter = 600

        self.timer = QTimer()
        self.timer.setInterval(33)
        self.timer.timeout.connect(self.repaint)

    def start(self, duration: float):
        self.duration = duration
        self.startTime = time.time()
        self.timer.start()

    def progress(self):
        return (time.time() - self.startTime) / self.duration

    def paintEvent(self, event):
        rect = QRectF((self.width() - self.diameter) / 2, (self.height() - self.diameter) / 2,
                      self.diameter, self.diameter)

        painter = QPainter(self)
        painter.setPen(QPen(QColor(0xCCCCCC), 200, cap=Qt.PenCapStyle.FlatCap))
        painter.drawArc(rect, 0, int(self.progress() * 360 * 16))

        return super().paintEvent(event)
