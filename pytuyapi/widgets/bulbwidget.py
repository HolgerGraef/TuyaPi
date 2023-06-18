import tinytuya

from collections import namedtuple
from functools import partial
from typing import Union

from PyQt5.QtCore import QMutex, QThread, QWaitCondition, pyqtSignal, pyqtSlot
from PyQt5.QtWidgets import QSizePolicy, QSpacerItem, QVBoxLayout, QWidget

from . import IconButton

BulbState = namedtuple("BulbState", ["state", "status"])


class BulbWorker(QThread):
    stateChanged = pyqtSignal(BulbState)
    actionDone = pyqtSignal()

    def __init__(self, device: dict):
        super(BulbWorker, self).__init__()

        self._device_desc = device
        self._action_queue = []
        self._bulb_state = BulbState(state={}, status={})

        self._cond = QWaitCondition()
        self._mut = QMutex()

        self.start()

    def run(self):
        self._device = tinytuya.BulbDevice(
            self._device_desc["id"],
            self._device_desc["ip"],
            self._device_desc["key"],
            connection_timeout=1,
        )
        self._device.set_socketTimeout(1)
        self._device.set_socketRetryLimit(1)
        self._device.set_version(3.3)

        while True:
            bulb_state = BulbState(state=self._device.state(), status=self._device.status())
            if bulb_state != self._bulb_state:
                self._bulb_state = bulb_state
                self.stateChanged.emit(bulb_state)

                print("Status of {}: {}".format(self._device_desc["name"], bulb_state.status))
                print("State of {}: {}".format(self._device_desc["name"], bulb_state.state))

            self._cond.wait(self._mut, 1000)

            while self._action_queue:
                action = self._action_queue.pop(0)
                action()
                self.actionDone.emit()

    def isOn(self) -> bool:
        return "is_on" in self._bulb_state.state and self._bulb_state.state["is_on"]

    def queueAction(self, fun):
        self._action_queue.append(fun)
        self._cond.wakeAll()

    def setPreset(self, preset: dict):
        self.turnOn()
        if preset["mode"] == "white":
            self.queueAction(lambda: self._device.set_mode("white"))
            self.queueAction(lambda: self._device.set_white_percentage(
                preset["brightness"], preset["colourtemp"]
            ))
        elif preset["mode"] == "dps":
            for k, v in preset["dps"].items():
                self.queueAction(lambda: self._device.set_value(k, v))

    def toggle(self):
        if self.isOn():
            self.turnOff()
        else:
            self.turnOn()

    def turnOn(self):
        self.queueAction(self._device.turn_on)

    def turnOff(self):
        self.queueAction(self._device.turn_off)


class BulbWidget(QWidget):
    def __init__(self, device: dict, presets: dict):
        super(BulbWidget, self).__init__()

        self.worker = BulbWorker(device)
        self.worker.stateChanged.connect(self.stateChanged)
        self.worker.actionDone.connect(self.updateButtons)

        self.presets = presets
        self.button_backup = (None, None)

        self.b_toggle = IconButton("mdi6.lightbulb-off-outline", device["name"])
        self.b_toggle.clicked.connect(self.toggle)

        # set up layout
        l = QVBoxLayout()
        l.addWidget(self.b_toggle)

        for p in self.presets:
            w = IconButton(None, p["label"])
            l.addWidget(w)
            w.clicked.connect(partial(self.setPreset, w, p))

        l.addSpacerItem(QSpacerItem(0, 0, vPolicy=QSizePolicy.Expanding))
        self.setLayout(l)

        self.updateButtons()

    def setButtonsEnabled(self, b: bool=True):
        self.setEnabled(b)
        if b and self.button_backup[0] and self.button_backup[1]:
            self.button_backup[0].setIcon(self.button_backup[1])

    def setButtonWaiting(self, button: IconButton):
        self.setButtonsEnabled(False)
        self.button_backup = (button, button.icon())
        button.setIcon("fa5s.spinner")

    def toggle(self):
        self.setButtonWaiting(self.b_toggle)
        self.worker.toggle()

    def setPreset(self, button: IconButton, preset: dict):
        self.setButtonWaiting(button)
        self.worker.setPreset(preset)

    @pyqtSlot(BulbState)
    def stateChanged(self, bulb_state: BulbState):
        self.updateButtons(bulb_state)

    def updateButtons(self, bulb_state: Union[BulbState, None]=None):
        if bulb_state is not None and "Error" in bulb_state.status:
            self.setButtonsEnabled(False)
            self.b_toggle.setIcon("msc.debug-disconnect")
            return

        self.setButtonsEnabled()
        if self.worker.isOn():
            self.b_toggle.setIcon("mdi6.lightbulb-on")
        else:
            self.b_toggle.setIcon("mdi6.lightbulb-off")
