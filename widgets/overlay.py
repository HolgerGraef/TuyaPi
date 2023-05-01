from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QLabel


class Overlay(QLabel):
    def __init__(self):
        super(Overlay, self).__init__()

        self.setAlignment(Qt.AlignCenter | Qt.AlignVCenter)
        self.setStyleSheet("background: rgba(0,0,0,80%); color: #CCCCCC; font-size: 200px;")
