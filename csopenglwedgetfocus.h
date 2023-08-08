#ifndef CSOPENGLWEDGETFOCUS_H
#define CSOPENGLWEDGETFOCUS_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QFile>
#include <QTimer>
#include <QMouseEvent> // 鼠标拖动
#include <QWheelEvent> //鼠标滚轮
#include <QDir>
#include<sys/time.h>

class CSOpenGLWedgetFocus : public QOpenGLWidget, QOpenGLExtraFunctions //QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit CSOpenGLWedgetFocus(QWidget *parent = nullptr);
    ~CSOpenGLWedgetFocus();
    void savepicture();
    // 计时函数
    struct timeval G_tv_begin, G_tv_end;
    void time_begin();
    void time_end();
    void set_sensitivity(int s);
    void set_highThreshold(int s);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    // 鼠标拖动事件
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void checkGLRrror(const char *msg);
    void myGetLocation();
    void myUniformData();

    // 鼠标拖动事件
    QPoint mousePoint;
    bool mouse_press;

    const QString fL_video_YUYV = QDir::currentPath() + QString("/YUYV/yuyv.yuv");
    QOpenGLShaderProgram *program = NULL;

    GLuint texs_yuyvID[1] = {0};
    GLuint texs_yuyvloc[1] = {0};

    QFile m_file;
    QByteArray m_buf;
    QTimer m_timer;

    // uniform_loc
    GLuint crgb_loc = 0;
    GLuint sensitivity_loc = 0;
    GLuint highThreshold_loc = 0;
    GLuint frameWH_loc = 0;

    // uniform value
    // 红:0xFFFF0000,绿:0xFF00FF00,蓝:0xFF0000FF,白:0xFFFFFFFF,黄:0xFFFFFF00
    int crgb = 0xFFFF0000; // crgb为color. color参数以argb的形式，例如0xFFFF0000代表不透明的红色，即alpha为0xFF，红色0xFF，绿色蓝色均为0x00.
    int width = 1080;
    int height = 1920;
    float sensitivity = 0.3; // 0.0-1.0
    float highThreshold = 0.5; // 0.0-1.0

    unsigned char* imageData;
    int save_wid = 1920;
    int save_hei = 1080;

    GLuint VAO, VBO;

signals:

public slots:
    void cleanup();

};

#endif // CSOPENGLWEDGETFOCUS_H
