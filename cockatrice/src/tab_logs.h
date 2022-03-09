#ifndef TAB_LOG_H
#define TAB_LOG_H

#include "tab.h"

#include <QDialog>

class AbstractClient;
class LineEditUnfocusable;

class QGroupBox;
class QPushButton;
class QSpinBox;
class QCheckBox;
class QRadioButton;
class QLabel;
class QDockWidget;
class QWidget;
class QTableWidget;
class CommandContainer;
class Response;
class AbstractClient;

class TabLog : public Tab
{
    Q_OBJECT
private:
    AbstractClient *client;
    QLabel *labelFindUserName, *labelFindIPAddress, *labelFindGameName, *labelFindGameID, *labelMessage, *labelMaximum,
        *labelDescription;
    LineEditUnfocusable *findUsername, *findIPAddress, *findGameName, *findGameID, *findMessage;
    QCheckBox *mainRoom, *gameRoom, *privateChat;
    QRadioButton *pastDays, *today, *lastHour;
    QSpinBox *maximumResults, *pastXDays;
    QDockWidget *searchDock;
    QWidget *searchDockContents;
    QPushButton *getButton, *clearButton;
    QGroupBox *criteriaGroupBox, *locationGroupBox, *rangeGroupBox, *maxResultsGroupBox, *descriptionGroupBox,
        *buttonGroupBox;
    QTableWidget *roomTable, *gameTable, *chatTable;
    QTabWidget *tabManager;

    void createDock();
signals:

private slots:
    void getClicked();
    void clearClicked();
    void viewLogHistory_processResponse(const Response &resp);
    void restartLayout();

public:
    TabLog(TabSupervisor *_tabSupervisor, AbstractClient *_client, QWidget *parent = nullptr);
    ~TabLog();
    void retranslateUi();
    QString getTabText() const
    {
        return tr("Logs");
    }
};

#endif
