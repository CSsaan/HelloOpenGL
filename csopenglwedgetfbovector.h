#ifndef CSOPENGLWEDGETFBOVECTOR_H
#define CSOPENGLWEDGETFBOVECTOR_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QFile>
#include <QTimer>
#include<sys/time.h>
#include <math.h>
#include <QDir>

class CSOpenGLWedgetFboVector : public QOpenGLWidget, QOpenGLExtraFunctions //QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit CSOpenGLWedgetFboVector(QWidget *parent = nullptr);
    ~CSOpenGLWedgetFboVector() override;
    void savepicture();
    // 计时函数
    struct timeval G_tv_begin, G_tv_end;
    void time_begin();
    void time_end();
    void set_alphabscope(int s);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    void checkGLRrror(const char *msg);
    void myGetLocation();
    void myUniformData();
    void Vectorscope_Vertexs(unsigned char* buf_yuyv, int img_width, int img_height);

    int getIndex(int x, int y, int width);
    float *RGBtoUV(float R, float G, float B);
    float normalize(float input);
    float getUpdatedColor(float pixel_cur, float pixel_new, float divider, float constant);
    void vectorscope(unsigned char *data_in, unsigned char *data_out, int width, int height);
    unsigned char*data_out = NULL;


    const QString mapImagePath = QDir::currentPath() + QString("/img/map.jpg");
    const QString fL_video_YUYV = QDir::currentPath() + QString("/YUYV/yuyv.yuv");
    QOpenGLShaderProgram *fbo_program = NULL;
    QOpenGLShaderProgram *program_map = NULL;

    // FBO纹理
    GLuint texs_yuyvID[1] = {0};
    GLuint texs_yuyvloc[1] = {0};
    // 普通2D显示纹理
    GLuint texs_mapID[1] = {0};
    GLuint texs_maploc[1] = {0};
    GLuint m_Fbo2DTextureId[1] = {0};
    GLint m_Fbo2DTextureIdloc[1] = {0};
    GLint alphabscope_loc = 0;

    QFile m_file;
    QByteArray m_buf;
    QTimer m_timer;

    // uniform_loc
    GLuint frameWH_loc = 0;

    // uniform value
    int width = 1920;
    int height = 1080;
    float* vertex_coords = NULL;

    QVector<QVector2D> vertices;
    QVector<QVector2D> coords;

    unsigned char* imageData;
    unsigned char* fboImageData;
    int save_wid = 1920;
    int save_hei = 1080;
    int alphabscope = 10; // 透明度 (0-100)

    GLuint FBO_vao, FBO_vbo;
    GLuint D2_vao, D2_vbo;
    GLuint FBOId;

    QImage img;

signals:

public slots:
    void cleanup();
};

#endif // CSOPENGLWEDGETFBOVECTOR_H
