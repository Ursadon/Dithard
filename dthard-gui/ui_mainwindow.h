/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
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
    QComboBox *comPortList;
    QPushButton *pbComPortOpen;
    QPushButton *pushButton;
    QTableWidget *tableStatus;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->resize(400, 300);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(400, 300));
        MainWindow->setMaximumSize(QSize(400, 300));
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
        groupBox->setGeometry(QRect(240, 130, 151, 161));
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
        comPortList = new QComboBox(centralWidget);
        comPortList->setObjectName(QStringLiteral("comPortList"));
        comPortList->setGeometry(QRect(20, 270, 101, 22));
        comPortList->setFocusPolicy(Qt::NoFocus);
        comPortList->setStyleSheet(QStringLiteral(""));
        pbComPortOpen = new QPushButton(centralWidget);
        pbComPortOpen->setObjectName(QStringLiteral("pbComPortOpen"));
        pbComPortOpen->setGeometry(QRect(130, 270, 75, 23));
        pbComPortOpen->setFocusPolicy(Qt::NoFocus);
        pbComPortOpen->setContextMenuPolicy(Qt::NoContextMenu);
        pbComPortOpen->setFlat(false);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(20, 210, 186, 51));
        pushButton->setFocusPolicy(Qt::NoFocus);
        pushButton->setStyleSheet(QStringLiteral(""));
        pushButton->setFlat(false);
        tableStatus = new QTableWidget(centralWidget);
        if (tableStatus->columnCount() < 2)
            tableStatus->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableStatus->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableStatus->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (tableStatus->rowCount() < 3)
            tableStatus->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableStatus->setVerticalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableStatus->setVerticalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableStatus->setVerticalHeaderItem(2, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableStatus->setItem(0, 0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableStatus->setItem(0, 1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableStatus->setItem(1, 0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableStatus->setItem(1, 1, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableStatus->setItem(2, 0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableStatus->setItem(2, 1, __qtablewidgetitem10);
        tableStatus->setObjectName(QStringLiteral("tableStatus"));
        tableStatus->setGeometry(QRect(10, 10, 231, 161));
        tableStatus->setFocusPolicy(Qt::NoFocus);
        tableStatus->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableStatus->setAutoScroll(false);
        tableStatus->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableStatus->setTabKeyNavigation(false);
        tableStatus->setProperty("showDropIndicator", QVariant(false));
        tableStatus->setDragDropOverwriteMode(false);
        tableStatus->setGridStyle(Qt::SolidLine);
        tableStatus->setCornerButtonEnabled(false);
        tableStatus->setRowCount(3);
        tableStatus->setColumnCount(2);
        tableStatus->horizontalHeader()->setVisible(true);
        tableStatus->horizontalHeader()->setCascadingSectionResizes(false);
        tableStatus->horizontalHeader()->setDefaultSectionSize(114);
        tableStatus->verticalHeader()->setVisible(false);
        tableStatus->verticalHeader()->setDefaultSectionSize(20);
        tableStatus->verticalHeader()->setHighlightSections(false);
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
        comPortList->setCurrentText(QString());
        pbComPortOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        pushButton->setText(QApplication::translate("MainWindow", "PushButton", 0));
        QTableWidgetItem *___qtablewidgetitem = tableStatus->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableStatus->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "\320\227\320\275\320\260\321\207\320\265\320\275\320\270\320\265", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableStatus->verticalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "CRC error", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableStatus->verticalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "Motor", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableStatus->verticalHeaderItem(2);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "TX queue", 0));

        const bool __sortingEnabled = tableStatus->isSortingEnabled();
        tableStatus->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem5 = tableStatus->item(0, 0);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "CRC error", 0));
        QTableWidgetItem *___qtablewidgetitem6 = tableStatus->item(0, 1);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "0", 0));
        QTableWidgetItem *___qtablewidgetitem7 = tableStatus->item(1, 0);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "Motor", 0));
        QTableWidgetItem *___qtablewidgetitem8 = tableStatus->item(1, 1);
        ___qtablewidgetitem8->setText(QApplication::translate("MainWindow", "0", 0));
        QTableWidgetItem *___qtablewidgetitem9 = tableStatus->item(2, 0);
        ___qtablewidgetitem9->setText(QApplication::translate("MainWindow", "TX queue", 0));
        QTableWidgetItem *___qtablewidgetitem10 = tableStatus->item(2, 1);
        ___qtablewidgetitem10->setText(QApplication::translate("MainWindow", "0", 0));
        tableStatus->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
