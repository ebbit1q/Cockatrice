#ifndef SHORTCUTSTAB_H
#define SHORTCUTSTAB_H

#include "ui_shortcutstab.h"
#include <QWidget>

#include "../dlg_settings.h"

class ShortcutsTab : public AbstractSettingsPage
{
    Q_OBJECT
public:
    ShortcutsTab();
    void retranslateUi();
    ~ShortcutsTab();

private slots:
    void resetShortcuts();
    void refreshEdits();
    void clearShortcuts();
    void afterClear();

private:
    Ui::shortcutsTab *ui;
};

#endif // SHORTCUTSTAB_H
