#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "serialendpoint.h"

#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_status(new QLabel),
    m_console(new Console),
    m_endpoint(new SerialEndpoint(this))
{
    m_ui->setupUi(this);
    m_console->setEnabled(false);
    setCentralWidget(m_console);

    m_ui->actionConnect->setEnabled(true);
    m_ui->actionDisconnect->setEnabled(false);
    m_ui->actionQuit->setEnabled(true);
    m_ui->actionConfigure->setEnabled(true);

    m_ui->statusBar->addWidget(m_status);

    initActionsConnections();

    connect(m_endpoint, &SerialEndpoint::statusMessage, this, &MainWindow::showStatusMessage);
    connect(m_endpoint, &SerialEndpoint::getData, m_console, &Console::putData);
    connect(m_console, &Console::getData, m_endpoint, &SerialEndpoint::putData);
}

MainWindow::~MainWindow()
{
    delete m_endpoint;
    delete m_ui;
}

void MainWindow::connectEndpoint()
{
    if (m_endpoint->open())
    {
        m_console->setEnabled(true);
        m_console->setLocalEchoEnabled(true);
        m_ui->actionConnect->setEnabled(false);
        m_ui->actionDisconnect->setEnabled(true);
        m_ui->actionConfigure->setEnabled(false);
    }
}

void MainWindow::disconnectEndpoint()
{
    m_endpoint->close();
    m_console->setEnabled(false);
    m_ui->actionConnect->setEnabled(true);
    m_ui->actionDisconnect->setEnabled(false);
    m_ui->actionConfigure->setEnabled(true);
}

void MainWindow::initActionsConnections()
{
    connect(m_ui->actionConnect, &QAction::triggered, this, &MainWindow::connectEndpoint);
    connect(m_ui->actionDisconnect, &QAction::triggered, this, &MainWindow::disconnectEndpoint);
    connect(m_ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(m_ui->actionConfigure, &QAction::triggered, m_endpoint, &SerialEndpoint::showDialog);
    connect(m_ui->actionClear, &QAction::triggered, m_console, &Console::clear);
}

void MainWindow::showStatusMessage(const QString &message)
{
    m_status->setText(message);
}
