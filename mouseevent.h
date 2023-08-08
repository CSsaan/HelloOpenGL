#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H
#include <QMouseEvent> // 鼠标拖动

class MouseEvent
{
public:
    MouseEvent();

protected:
    // 鼠标拖动事件
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    // 鼠标拖动事件
    QPoint mousePoint;
    bool mouse_press;
};

#endif // MOUSEEVENT_H
