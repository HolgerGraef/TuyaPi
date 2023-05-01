import time

from PyQt5.QtCore import Qt, QRectF, QTimer
from PyQt5.QtGui import QColor, QPainter, QPen
from PyQt5.QtWidgets import QWidget

from . import Overlay


class UnlockOverlay(Overlay):
    def __init__(self, parent: QWidget):
        super(UnlockOverlay, self).__init__(parent)

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
        self.show()

    def progress(self):
        return (time.time() - self.startTime) / self.duration

    def paintEvent(self, event):
        rect = QRectF((self.width() - self.diameter) / 2, (self.height() - self.diameter) / 2,
                      self.diameter, self.diameter)

        painter = QPainter(self)
        painter.setPen(QPen(QColor(0xCCCCCC), 200, cap=Qt.PenCapStyle.FlatCap))
        painter.drawArc(rect, 0, int(self.progress() * 360 * 16))

        return super().paintEvent(event)
