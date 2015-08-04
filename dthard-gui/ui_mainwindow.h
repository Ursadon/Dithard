/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QProgressBar *battery_main;
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *lb_DOWN;
    QLabel *lb_UP;
    QLabel *lb_LEFT;
    QLabel *lb_RIGHT;
    QLabel *lbBattery_main;
    QLabel *lbSignal_Strenght;
    QProgressBar *signal_strenght;
    QTableWidget *tableStatus;
    QCheckBox *checkBox;
    QCheckBox *cb_acks;
    QCheckBox *checkBox_2;
    QGroupBox *groupBox_2;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_2;
    QPushButton *pbComPortOpen;
    QRadioButton *rbIP;
    QLineEdit *leIPaddress;
    QComboBox *comPortList;
    QRadioButton *rbSerial;
    QPushButton *pbIpOpen;
    QButtonGroup *buttonGroup;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->resize(400, 350);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(400, 350));
        MainWindow->setMaximumSize(QSize(400, 350));
        QFont font;
        font.setFamily(QStringLiteral("Segoe UI"));
        MainWindow->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/graphics/img/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setTabShape(QTabWidget::Rounded);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        battery_main = new QProgressBar(centralWidget);
        battery_main->setObjectName(QStringLiteral("battery_main"));
        battery_main->setEnabled(false);
        battery_main->setGeometry(QRect(260, 30, 121, 20));
        battery_main->setStyleSheet(QLatin1String("QProgressBar::chunk {\n"
"background-color: #00FF00;\n"
"}\n"
"QProgressBar {\n"
"border: 1px solid grey;\n"
"border-radius: 2px;\n"
"text-align: center;\n"
"background: #eeeeee;\n"
"}"));
        battery_main->setMaximum(3700);
        battery_main->setValue(0);
        battery_main->setInvertedAppearance(false);
        battery_main->setTextDirection(QProgressBar::TopToBottom);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(240, 180, 151, 161));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 10, 151, 151));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        lb_DOWN = new QLabel(layoutWidget);
        lb_DOWN->setObjectName(QStringLiteral("lb_DOWN"));
        lb_DOWN->setMaximumSize(QSize(40, 40));
        lb_DOWN->setFrameShape(QFrame::NoFrame);
        lb_DOWN->setPixmap(QPixmap(QString::fromUtf8(":/graphics/img/circle_green.png")));
        lb_DOWN->setScaledContents(false);
        lb_DOWN->setWordWrap(false);
        lb_DOWN->setIndent(0);

        gridLayout->addWidget(lb_DOWN, 2, 1, 1, 1);

        lb_UP = new QLabel(layoutWidget);
        lb_UP->setObjectName(QStringLiteral("lb_UP"));
        lb_UP->setMaximumSize(QSize(40, 40));
        lb_UP->setPixmap(QPixmap(QString::fromUtf8(":/graphics/img/circle_green.png")));

        gridLayout->addWidget(lb_UP, 0, 1, 1, 1);

        lb_LEFT = new QLabel(layoutWidget);
        lb_LEFT->setObjectName(QStringLiteral("lb_LEFT"));
        lb_LEFT->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lb_LEFT->sizePolicy().hasHeightForWidth());
        lb_LEFT->setSizePolicy(sizePolicy1);
        lb_LEFT->setMinimumSize(QSize(40, 40));
        lb_LEFT->setMaximumSize(QSize(40, 40));
        lb_LEFT->setPixmap(QPixmap(QString::fromUtf8(":/graphics/img/circle_green.png")));

        gridLayout->addWidget(lb_LEFT, 1, 0, 1, 1);

        lb_RIGHT = new QLabel(layoutWidget);
        lb_RIGHT->setObjectName(QStringLiteral("lb_RIGHT"));
        lb_RIGHT->setMaximumSize(QSize(40, 40));
        lb_RIGHT->setPixmap(QPixmap(QString::fromUtf8(":/graphics/img/circle_green.png")));

        gridLayout->addWidget(lb_RIGHT, 1, 3, 1, 1);

        lbBattery_main = new QLabel(centralWidget);
        lbBattery_main->setObjectName(QStringLiteral("lbBattery_main"));
        lbBattery_main->setEnabled(false);
        lbBattery_main->setGeometry(QRect(260, 10, 91, 20));
        lbSignal_Strenght = new QLabel(centralWidget);
        lbSignal_Strenght->setObjectName(QStringLiteral("lbSignal_Strenght"));
        lbSignal_Strenght->setEnabled(false);
        lbSignal_Strenght->setGeometry(QRect(260, 60, 101, 20));
        signal_strenght = new QProgressBar(centralWidget);
        signal_strenght->setObjectName(QStringLiteral("signal_strenght"));
        signal_strenght->setEnabled(false);
        signal_strenght->setGeometry(QRect(260, 80, 121, 20));
        signal_strenght->setStyleSheet(QLatin1String("QProgressBar::chunk {\n"
"background-color: #00FF00;\n"
"}\n"
"QProgressBar {\n"
"border: 1px solid grey;\n"
"border-radius: 2px;\n"
"text-align: center;\n"
"background: #eeeeee;\n"
"}"));
        signal_strenght->setMaximum(5);
        signal_strenght->setValue(0);
        tableStatus = new QTableWidget(centralWidget);
        if (tableStatus->columnCount() < 2)
            tableStatus->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableStatus->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableStatus->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (tableStatus->rowCount() < 4)
            tableStatus->setRowCount(4);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableStatus->setVerticalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableStatus->setVerticalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableStatus->setVerticalHeaderItem(2, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableStatus->setVerticalHeaderItem(3, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableStatus->setItem(0, 0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableStatus->setItem(0, 1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableStatus->setItem(1, 0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableStatus->setItem(1, 1, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableStatus->setItem(2, 0, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableStatus->setItem(2, 1, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableStatus->setItem(3, 0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableStatus->setItem(3, 1, __qtablewidgetitem13);
        tableStatus->setObjectName(QStringLiteral("tableStatus"));
        tableStatus->setGeometry(QRect(10, 10, 232, 161));
        tableStatus->setFocusPolicy(Qt::NoFocus);
        tableStatus->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableStatus->setAutoScroll(false);
        tableStatus->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableStatus->setTabKeyNavigation(false);
        tableStatus->setProperty("showDropIndicator", QVariant(false));
        tableStatus->setDragDropOverwriteMode(false);
        tableStatus->setGridStyle(Qt::SolidLine);
        tableStatus->setCornerButtonEnabled(false);
        tableStatus->setRowCount(4);
        tableStatus->setColumnCount(2);
        tableStatus->horizontalHeader()->setVisible(false);
        tableStatus->horizontalHeader()->setCascadingSectionResizes(false);
        tableStatus->horizontalHeader()->setDefaultSectionSize(114);
        tableStatus->verticalHeader()->setVisible(false);
        tableStatus->verticalHeader()->setDefaultSectionSize(20);
        tableStatus->verticalHeader()->setHighlightSections(false);
        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(20, 180, 91, 17));
        checkBox->setMouseTracking(false);
        checkBox->setFocusPolicy(Qt::NoFocus);
        checkBox->setChecked(false);
        cb_acks = new QCheckBox(centralWidget);
        cb_acks->setObjectName(QStringLiteral("cb_acks"));
        cb_acks->setGeometry(QRect(140, 180, 91, 17));
        cb_acks->setMouseTracking(false);
        cb_acks->setFocusPolicy(Qt::NoFocus);
        cb_acks->setChecked(true);
        cb_acks->setTristate(false);
        checkBox_2 = new QCheckBox(centralWidget);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setGeometry(QRect(20, 200, 121, 17));
        checkBox_2->setMouseTracking(false);
        checkBox_2->setFocusPolicy(Qt::NoFocus);
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 219, 211, 121));
        gridLayoutWidget = new QWidget(groupBox_2);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(9, 9, 191, 111));
        gridLayout_2 = new QGridLayout(gridLayoutWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        pbComPortOpen = new QPushButton(gridLayoutWidget);
        pbComPortOpen->setObjectName(QStringLiteral("pbComPortOpen"));
        pbComPortOpen->setEnabled(false);
        pbComPortOpen->setFocusPolicy(Qt::NoFocus);
        pbComPortOpen->setContextMenuPolicy(Qt::NoContextMenu);
        pbComPortOpen->setFlat(false);

        gridLayout_2->addWidget(pbComPortOpen, 3, 1, 1, 1);

        rbIP = new QRadioButton(gridLayoutWidget);
        buttonGroup = new QButtonGroup(MainWindow);
        buttonGroup->setObjectName(QStringLiteral("buttonGroup"));
        buttonGroup->addButton(rbIP);
        rbIP->setObjectName(QStringLiteral("rbIP"));
        rbIP->setChecked(true);

        gridLayout_2->addWidget(rbIP, 0, 0, 1, 1);

        leIPaddress = new QLineEdit(gridLayoutWidget);
        leIPaddress->setObjectName(QStringLiteral("leIPaddress"));

        gridLayout_2->addWidget(leIPaddress, 1, 0, 1, 1);

        comPortList = new QComboBox(gridLayoutWidget);
        comPortList->setObjectName(QStringLiteral("comPortList"));
        comPortList->setEnabled(false);
        comPortList->setFocusPolicy(Qt::NoFocus);
        comPortList->setStyleSheet(QStringLiteral(""));

        gridLayout_2->addWidget(comPortList, 3, 0, 1, 1);

        rbSerial = new QRadioButton(gridLayoutWidget);
        buttonGroup->addButton(rbSerial);
        rbSerial->setObjectName(QStringLiteral("rbSerial"));

        gridLayout_2->addWidget(rbSerial, 2, 0, 1, 1);

        pbIpOpen = new QPushButton(gridLayoutWidget);
        pbIpOpen->setObjectName(QStringLiteral("pbIpOpen"));

        gridLayout_2->addWidget(pbIpOpen, 1, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Dithard-GUI", 0));
        battery_main->setFormat(QApplication::translate("MainWindow", "%v \320\274\320\222", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "\320\224\320\262\320\270\320\266\320\265\320\275\320\270\320\265", 0));
        lb_DOWN->setText(QString());
        lb_UP->setText(QString());
        lb_LEFT->setText(QString());
        lb_RIGHT->setText(QString());
        lbBattery_main->setText(QApplication::translate("MainWindow", "\320\235\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \320\220\320\232\320\221:", 0));
        lbSignal_Strenght->setText(QApplication::translate("MainWindow", "\320\243\321\200\320\276\320\262\320\265\320\275\321\214 \321\201\320\270\320\263\320\275\320\260\320\273\320\260:", 0));
        QTableWidgetItem *___qtablewidgetitem = tableStatus->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableStatus->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "\320\227\320\275\320\260\321\207\320\265\320\275\320\270\320\265", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableStatus->verticalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "CRC error", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableStatus->verticalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "Range", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableStatus->verticalHeaderItem(2);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "TX queue", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tableStatus->verticalHeaderItem(3);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "Ping", 0));

        const bool __sortingEnabled = tableStatus->isSortingEnabled();
        tableStatus->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem6 = tableStatus->item(0, 0);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "CRC error", 0));
        QTableWidgetItem *___qtablewidgetitem7 = tableStatus->item(0, 1);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "0", 0));
        QTableWidgetItem *___qtablewidgetitem8 = tableStatus->item(1, 0);
        ___qtablewidgetitem8->setText(QApplication::translate("MainWindow", "Range", 0));
        QTableWidgetItem *___qtablewidgetitem9 = tableStatus->item(1, 1);
        ___qtablewidgetitem9->setText(QApplication::translate("MainWindow", "0", 0));
        QTableWidgetItem *___qtablewidgetitem10 = tableStatus->item(2, 0);
        ___qtablewidgetitem10->setText(QApplication::translate("MainWindow", "TX queue", 0));
        QTableWidgetItem *___qtablewidgetitem11 = tableStatus->item(2, 1);
        ___qtablewidgetitem11->setText(QApplication::translate("MainWindow", "0", 0));
        QTableWidgetItem *___qtablewidgetitem12 = tableStatus->item(3, 0);
        ___qtablewidgetitem12->setText(QApplication::translate("MainWindow", "Ping", 0));
        QTableWidgetItem *___qtablewidgetitem13 = tableStatus->item(3, 1);
        ___qtablewidgetitem13->setText(QApplication::translate("MainWindow", "0", 0));
        tableStatus->setSortingEnabled(__sortingEnabled);

        checkBox->setText(QApplication::translate("MainWindow", "RangeMeter", 0));
        cb_acks->setText(QApplication::translate("MainWindow", "Enable ACKs", 0));
        checkBox_2->setText(QApplication::translate("MainWindow", "Disable TX Queue", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\320\241\320\262\321\217\320\267\321\214", 0));
        pbComPortOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        rbIP->setText(QApplication::translate("MainWindow", "IP", 0));
        comPortList->setCurrentText(QString());
        rbSerial->setText(QApplication::translate("MainWindow", "Serial", 0));
        pbIpOpen->setText(QApplication::translate("MainWindow", "Connect", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
