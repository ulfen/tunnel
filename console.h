#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>

class Console : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit Console(QWidget *parent = nullptr);

    void putData(const QByteArray &data, const QBrush &col=QBrush(Qt::black));

public slots:
    void clear();

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

private:
    bool m_update = false;
};

#endif // CONSOLE_H
