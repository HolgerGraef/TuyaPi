import qtawesome as qta

from typing import Union

from PyQt5.QtCore import Qt, QSize
from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import QHBoxLayout, QLabel, QPushButton


class IconButton(QPushButton):
    def __init__(self, icon: Union[str, QIcon], text: str):
        super(IconButton, self).__init__()

        # credit: https://stackoverflow.com/a/61130602
        self.setIcon(icon)
        self.setIconSize(QSize(64, 64))
        # align the icon left
        self.setStyleSheet("text-align: left;")
        self.setLayout(QHBoxLayout())

        self.label = QLabel(text)
        self.label.setAlignment(Qt.AlignCenter | Qt.AlignVCenter)
        self.label.setAttribute(Qt.WA_TransparentForMouseEvents, True)
        # TODO: find better way to show all text
        self.label.setStyleSheet("margin-top: 100px; margin-bottom: 100px;")

        self.layout().addWidget(self.label)

    def setIcon(self, icon: Union[str, QIcon]):
        if isinstance(icon, str):
            icon = qta.icon(icon)

        super(IconButton, self).setIcon(icon)
