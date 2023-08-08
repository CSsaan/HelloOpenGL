#ifndef CSOPENGLWEDGETLUT_H
#define CSOPENGLWEDGETLUT_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QFile>
#include <QTimer>
#include<sys/time.h>
#include<regex>
#include <assert.h>
#include <QDir>

class CSOpenGLWedgetLut : public QOpenGLWidget, QOpenGLExtraFunctions //QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit CSOpenGLWedgetLut(QWidget *parent = nullptr);
    ~CSOpenGLWedgetLut();
    void savepicture();
    // 计时函数
    struct timeval G_tv_begin, G_tv_end;
    void time_begin();
    void time_end();
    void set_lutImagePath(const char *Path);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    void checkGLRrror(const char *msg);
    void myGetLocation();
    void myUniformData();

    const char* lutImagePath = "/lut/Canon C-Log.cube";

    float* lut_table = NULL;
    unsigned char* bitmap = NULL;
    bool matchRegex(const char* pattern, const char* userString);
    int load_3dlut_file_to_picture();
    int CreateLutTexture2D();

    const QString fL_video_YUYV = QDir::currentPath() + QString("/YUYV/yuyv.yuv");
    QOpenGLShaderProgram *program = NULL;

    GLuint texs_yuyvID[1] = {0};
    GLuint texs_yuyvloc[1] = {0};
    GLuint texs_lutID[1] = {0};
    GLuint texs_lutloc[1] = {0};

    QFile m_file;
    QByteArray m_buf;
    QTimer m_timer;

    // uniform_loc
    GLuint frameWH_loc = 0;
    GLuint size_lut_loc = 0;
    GLuint qsz_lut_loc = 0;

    // uniform value
    int width = 1920;
    int height = 1080;
    int lut_size;
    int qSZ;

    unsigned char* imageData;
    int save_wid = 1920;
    int save_hei = 1080;

    GLuint VAO, VBO;

signals:

public slots:
    void cleanup();
};

#endif // CSOPENGLWEDGETLUT_H
