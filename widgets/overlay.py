from typing import Union

from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QLabel, QWidget


class Overlay(QLabel):
    def __init__(self, parent: Union[QWidget, None] = None):
        super(Overlay, self).__init__(parent)

        self.setAlignment(Qt.AlignCenter | Qt.AlignVCenter)
        self.setStyleSheet("background: rgba(0,0,0,80%); color: #CCCCCC; font-size: 200px;")

        self.hide()

    def show(self):
        self.resize(self.parentWidget().size())
        super(Overlay, self).show()
