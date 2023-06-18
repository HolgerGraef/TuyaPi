#include "bluetoothwidget.h"

#include "main.h"

BluetoothWidget::BluetoothWidget(BluetoothManager* manager, QWidget* parent)
  : QWidget{ parent }
  , mManager(manager)
{
  setLayout(&mLayout);

  connect(mManager, SIGNAL(updated()), this, SLOT(refresh()));
  refresh();
}

void
BluetoothWidget::refresh()
{
  auto devices = mManager->devices();

  for (const auto& dev : devices) {
    if (mWidgets.contains(dev))
      continue;
    mWidgets.insert(dev, new IconButton());
    mWidgets[dev]->setText(dev);
    mWidgets[dev]->setIcon(fa::fa_bluetooth);
  }

  QList<QString> removeList;
  for (auto it = mWidgets.keyValueBegin(); it != mWidgets.keyValueEnd(); it++) {
    if (devices.contains(it->first)) {
      /* new device? */
      if (mLayout.indexOf(it->second) < 0)
        mLayout.addWidget(it->second);
    } else {
      /* device no longer present */
      mLayout.removeWidget(it->second);
      removeList.append(it->first);
    }
  }
  for (const auto& dev : removeList) {
    delete mWidgets[dev];
    mWidgets.remove(dev);
  }
}
