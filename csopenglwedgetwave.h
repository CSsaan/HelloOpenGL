#ifndef CSOPENGLWEDGETWAVE_H
#define CSOPENGLWEDGETWAVE_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QFile>
#include <QTimer>
#include<sys/time.h>
#include <QDir>

class CSOpenGLWedgetWave : public QOpenGLWidget, QOpenGLExtraFunctions //QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit CSOpenGLWedgetWave(QWidget *parent = nullptr);
    ~CSOpenGLWedgetWave() override;
    void savepicture();
    // 计时函数
    struct timeval G_tv_begin, G_tv_end;
    void time_begin();
    void time_end();
    void set_alphabscope(int s);
    void set_useWaveform(int s);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    void checkGLRrror(const char *msg);
    void myGetLocation();
    void myUniformData();

    const QString fL_video_YUYV = QDir::currentPath() + QString("/YUYV/yuyv.yuv");
    const QString tickImagePath = QDir::currentPath() + QString("/img/tick.jpg");
    QOpenGLShaderProgram *program = NULL;

    GLuint texs_yuyvID[1] = {0};
    GLuint texs_yuyvloc[1] = {0};
    GLuint texs_tickID[1] = {0};
    GLuint texs_tickloc[1] = {0};

    QFile m_file;
    QByteArray m_buf;
    QTimer m_timer;

    // uniform_loc
    GLuint useWaveform_loc = 0;
    GLuint alphabscope_loc = 0;
    GLuint frameWH_loc = 0;
    // uniform value
    int width = 1920;
    int height = 1080;
    int useWaveform = 1; // 波形图样式(1:Luma,2:RGB)
    int alphabscope = 10; // 透明度 (0-100)

    unsigned char* imageData;
    int save_wid = 1920;
    int save_hei = 1080;

    GLuint VAO, VBO;

signals:

public slots:
    void cleanup();
};

#endif // CSOPENGLWEDGETWAVE_H
