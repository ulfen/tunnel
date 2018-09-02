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
    m_console(new Console)
{
    m_endpoint[0] = new SerialEndpoint(this);
    m_endpoint[1] = new SerialEndpoint(this);

    m_ui->setupUi(this);
    m_console->setEnabled(false);
    setCentralWidget(m_console);

    m_ui->actionConnect->setEnabled(true);
    m_ui->actionDisconnect->setEnabled(false);
    m_ui->actionQuit->setEnabled(true);
    m_ui->actionConfigure->setEnabled(true);

    m_ui->statusBar->addWidget(m_status);

    initActionsConnections();

    connect(m_endpoint[0], &Endpoint::statusMessage, this, &MainWindow::showStatusMessage);
    connect(m_endpoint[0], &Endpoint::getData, m_endpoint[1], &Endpoint::putData);
    connect(m_endpoint[0], &Endpoint::getData,
            [=]( const QByteArray &data ) { m_console->putData(data, QBrush(Qt::blue)); }
    );
    connect(m_endpoint[1], &Endpoint::statusMessage, this, &MainWindow::showStatusMessage);
    connect(m_endpoint[1], &Endpoint::getData, m_endpoint[0], &Endpoint::putData);
    connect(m_endpoint[1], &Endpoint::getData,
            [=]( const QByteArray &data ) { m_console->putData(data, QBrush(Qt::red)); }
    );
}

MainWindow::~MainWindow()
{
    delete m_endpoint[0];
    delete m_endpoint[1];
    delete m_ui;
}

void MainWindow::connectEndpoint()
{
    if (m_endpoint[0]->open() && m_endpoint[1]->open())
    {
        m_console->setEnabled(true);
        m_ui->actionConnect->setEnabled(false);
        m_ui->actionDisconnect->setEnabled(true);
        m_ui->actionConfigure->setEnabled(false);
    }
    else
    {
        m_endpoint[0]->close();
        m_endpoint[1]->close();
    }
}

void MainWindow::disconnectEndpoint()
{
    m_endpoint[0]->close();
    m_endpoint[1]->close();
    m_console->setEnabled(false);
    m_ui->actionConnect->setEnabled(true);
    m_ui->actionDisconnect->setEnabled(false);
    m_ui->actionConfigure->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
}

void MainWindow::initActionsConnections()
{
    connect(m_ui->actionConnect, &QAction::triggered, this, &MainWindow::connectEndpoint);
    connect(m_ui->actionDisconnect, &QAction::triggered, this, &MainWindow::disconnectEndpoint);
    connect(m_ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(m_ui->actionConfigure, &QAction::triggered, m_endpoint[0], &Endpoint::showDialog);
    connect(m_ui->actionConfigure, &QAction::triggered, m_endpoint[1], &Endpoint::showDialog);
    connect(m_ui->actionClear, &QAction::triggered, m_console, &Console::clear);
}

void MainWindow::showStatusMessage(const QString &message)
{
    m_status->setText(message);
}
