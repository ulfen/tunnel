#include "console.h"

#include <QScrollBar>

Console::Console(QWidget *parent) :
    QPlainTextEdit(parent)
{
    document()->setMaximumBlockCount(100);
    m_update = true;
}

void Console::putData(const QByteArray &data, const QBrush &col)
{
    if (data.length() > 4)
        m_update = false;

    if (m_update)
    {
        QTextCharFormat tf;
        tf = currentCharFormat();
        tf.setForeground(col);
        setCurrentCharFormat(tf);

        insertPlainText(data);

        QScrollBar *bar = verticalScrollBar();
        bar->setValue(bar->maximum());
    }
}

void Console::keyPressEvent(QKeyEvent *e)
{
    Q_UNUSED(e)
}

void Console::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void Console::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}

void Console::clear()
{
    QPlainTextEdit::clear();
    m_update = true;
}
