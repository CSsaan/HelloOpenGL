#ifndef CSOPENGLWEDGETMAG_H
#define CSOPENGLWEDGETMAG_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QFile>
#include <QTimer>
#include<sys/time.h>
#include <QDir>

class CSOpenGLWedgetMag : public QOpenGLWidget, QOpenGLExtraFunctions //QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit CSOpenGLWedgetMag(QWidget *parent = nullptr);
    ~CSOpenGLWedgetMag();
    void savepicture();
    // 计时函数
    struct timeval G_tv_begin, G_tv_end;
    void time_begin();
    void time_end();
    void set_sharpenStr(int s);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    void checkGLRrror(const char *msg);
    void myGetLocation();
    void myUniformData();
    float *drawRectVertexs(float centerX, float centerY, float ratio);

    const QString fL_video_YUYV = QDir::currentPath() + QString("/YUYV/yuyv.yuv");
    QOpenGLShaderProgram *program = NULL;

    GLuint texs_yuyvID[1] = {0};
    GLuint texs_yuyvloc[1] = {0};

    QFile m_file;
    QByteArray m_buf;
    QTimer m_timer;

    // uniform_loc
    GLuint frameWH_loc = 0;
    GLuint sharpenStr_loc = 0;

    GLuint cunstom_Params_uniform = 0;
    GLuint cunstom_Params_plus_uniform = 0;
    GLuint MagLineWifth_uniform = 0;

    // uniform value
    int width = 1920;
    int height = 1080;
    float sharpenStr = 0.9f; // 0.0-100.0

    unsigned char* imageData;
    int save_wid = 1920;
    int save_hei = 1080;

    GLuint VAO, VBO;

signals:

public slots:
    void cleanup();
};

#endif // CSOPENGLWEDGETMAG_H
