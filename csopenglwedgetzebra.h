#ifndef CSOPENGLWEDGETZEBRA_H
#define CSOPENGLWEDGETZEBRA_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QFile>
#include <QTimer>
#include<sys/time.h>
#include <QDir>

class CSOpenGLWedgetZebra : public QOpenGLWidget, QOpenGLExtraFunctions //QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit CSOpenGLWedgetZebra(QWidget *parent = nullptr);
    ~CSOpenGLWedgetZebra();
    void savepicture();
    // 计时函数
    struct timeval G_tv_begin, G_tv_end;
    void time_begin();
    void time_end();
    void set_zebraLow(int s);
    void set_zebraHig(int s);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    void checkGLRrror(const char *msg);
    void myGetLocation();
    void myUniformData();

    const QString fL_video_YUYV = QDir::currentPath() + QString("/YUYV/yuyv.yuv");
    const QString zebraImagePath = QDir::currentPath() + QString("/img/zebra.jpg");
    QOpenGLShaderProgram *program = NULL;

    GLuint texs_yuyvID[1] = {0};
    GLuint texs_yuyvloc[1] = {0};
    GLuint texs_zebraID[1] = {0};
    GLuint texs_zebraloc[1] = {0};

    QFile m_file;
    QByteArray m_buf;
    QTimer m_timer;

    // uniform_loc
    GLuint frameWH_loc = 0;
    GLuint zebraLow_loc = 0;
    GLuint zebraHig_loc = 0;
    GLuint iGlobalTime_loc = 0;

    // uniform value
    int width = 1920;
    int height = 1080;
    float iGlobalTime;
    float zebraLow = 50.0; // 50.0-zebraHig
    float zebraHig = 80.0; // zebraLow-100.0

    unsigned char* imageData;
    int save_wid = 1920;
    int save_hei = 1080;

    GLuint VAO, VBO;

signals:

public slots:
    void cleanup();
};

#endif // CSOPENGLWEDGETZEBRA_H
