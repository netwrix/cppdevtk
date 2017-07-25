#!/bin/bash


lupdate-qt5 -recursive -locations none -no-ui-lines -source-language en_US ./../cppdevtk.pro


# lrelease does not handle SUBDIRS

lrelease-qt5 ./../src/base/base.pro
lrelease-qt5 ./../src/gui/gui.pro
lrelease-qt5 ./../src/util/util.pro

lrelease-qt5 ./../test/test_base/test_base.pro
lrelease-qt5 ./../test/test_caps_lock_widget/test_caps_lock_widget.pro
lrelease-qt5 ./../test/test_config/test_config.pro
lrelease-qt5 ./../test/test_disk_space_widget/test_disk_space_widget.pro
lrelease-qt5 ./../test/test_invisible_widget/test_invisible_widget.pro
lrelease-qt5 ./../test/test_slot_throw/test_slot_throw.pro
lrelease-qt5 ./../test/test_util/test_util.pro
