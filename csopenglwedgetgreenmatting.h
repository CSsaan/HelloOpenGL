#ifndef CSOPENGLWEDGETGREENMATTING_H
#define CSOPENGLWEDGETGREENMATTING_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QFile>
#include <QTimer>
#include<sys/time.h>
#include <QDir>

struct AVFrame;

class CSOpenGLWedgetGreenMatting : public QOpenGLWidget, protected QOpenGLExtraFunctions //QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit CSOpenGLWedgetGreenMatting(QWidget *parent = nullptr);
    ~CSOpenGLWedgetGreenMatting();
    void savepicture();
    // 计时函数
    struct timeval G_tv_begin, G_tv_end;
    void time_begin();
    void time_end();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    void checkGLRrror(const char *msg);
    void myGetLocation();
    void myUniformData();

    const QString backgroundImagePath = QDir::currentPath() + QString("/img/map.jpg");
    const QString fL_video_YUYV = QDir::currentPath() + QString("/YUYV/yuyv.yuv");
    QOpenGLShaderProgram *program = NULL;

    GLuint texs_yuyvID[1] = {0};
    GLuint texs_yuyvloc[1] = {0};
    GLuint texs_backgroundID[1] = {0};
    GLuint texs_backgroundloc[1] = {0};

    QFile m_file;
    QByteArray m_buf;
    QTimer m_timer;

    // uniform_loc
    GLuint frameWH_loc = 0;

    // uniform value
    int width = 1920;
    int height = 1080;

    unsigned char* imageData;
    int save_wid = 1920;
    int save_hei = 1080;

    GLuint VAO, VBO;

    QImage img;

signals:

public slots:
    void cleanup();

};

#endif // CSOPENGLWEDGETGREENMATTING_H
