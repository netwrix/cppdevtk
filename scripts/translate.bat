C:\Qt\5.6.3\msvc2015_64\bin\lupdate -recursive -locations none -no-ui-lines -no-obsolete -source-language en_US ./../cppdevtk.pro


REM lrelease does not handle SUBDIRS

C:\Qt\5.6.3\msvc2015_64\bin\lrelease ./../src/base/base.pro
C:\Qt\5.6.3\msvc2015_64\bin\lrelease ./../src/gui/gui.pro
C:\Qt\5.6.3\msvc2015_64\bin\lrelease ./../src/util/util.pro
C:\Qt\5.6.3\msvc2015_64\bin\lrelease ./../src/QtCopyDialog/QtCopyDialog.pro

C:\Qt\5.6.3\msvc2015_64\bin\lrelease ./../test/test_base/test_base.pro
C:\Qt\5.6.3\msvc2015_64\bin\lrelease ./../test/test_caps_lock_widget/test_caps_lock_widget.pro
C:\Qt\5.6.3\msvc2015_64\bin\lrelease ./../test/test_config/test_config.pro
C:\Qt\5.6.3\msvc2015_64\bin\lrelease ./../test/test_disk_space_widget/test_disk_space_widget.pro
C:\Qt\5.6.3\msvc2015_64\bin\lrelease ./../test/test_invisible_widget/test_invisible_widget.pro
C:\Qt\5.6.3\msvc2015_64\bin\lrelease ./../test/test_localization/test_localization.pro
C:\Qt\5.6.3\msvc2015_64\bin\lrelease ./../test/test_slot_throw/test_slot_throw.pro
C:\Qt\5.6.3\msvc2015_64\bin\lrelease ./../test/test_util/test_util.pro
