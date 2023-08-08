#ifndef CSOPENGLWEDGETFBOHISTIN_H
#define CSOPENGLWEDGETFBOHISTIN_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QFile>
#include <QTimer>
#include<sys/time.h>
#include <QDir>

class CSOpenGLWedgetFboHistIn : public QOpenGLWidget, QOpenGLExtraFunctions //QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit CSOpenGLWedgetFboHistIn(QWidget *parent = nullptr);
    ~CSOpenGLWedgetFboHistIn();
    void savepicture();
    // 计时函数
    struct timeval G_tv_begin, G_tv_end;
    void time_begin();
    void time_end();
    void set_alphabscope(int s);
    void set_type(bool usergb);

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    void checkGLRrror(const char *msg);
    void myGetLocation();
    void myUniformData();
    void render_Mesh(const uint8_t *imgDataPtr);
    void CalHistgramPointer(const uint8_t *imgDataPtr);
    void UpdateMesh();
    QVector4D Combile(const QVector2D &VertCoord, const QVector2D &texCoord);
    int MAX_VALUE_LEVEL = 0;

    const QString fL_video_YUYV = QDir::currentPath() + QString("/YUYV/yuyv.yuv");
//    QOpenGLShaderProgram *program = NULL;
    QOpenGLShaderProgram *fbo_program = NULL;
    QOpenGLShaderProgram *program_2D = NULL;

    // FBO纹理
    GLuint texs_yuyvID[1] = {0};
    GLuint texs_yuyvloc[1] = {0};
    GLint alphabscope_loc = 0;
    GLint useRGB_loc = 0;
    // 普通2D显示纹理
    GLuint m_Fbo2DTextureId[1] = {0};
    GLuint m_Fbo2DTextureIdloc[1] = {0};

    // uniform_loc
    GLuint points_loc[4] = {0};

    QFile m_file;
    QByteArray m_buf;
    QTimer m_timer;

    // uniform_loc

    // uniform value
    int width = 1920;
    int height = 1080;

    unsigned char* imageData;
    unsigned char* fboImageData;
    int save_wid = 1920;
    int save_hei = 1080;
    int alphabscope = 10; // 透明度 (0-100)
    bool useRGB = false;

//    GLuint VAO, VBO;
    GLuint readBuffer;
    GLuint FBO_vao, FBO_vbo;
    GLuint D2_vao = 0;
    GLuint D2_vbo = 0;
    GLuint FBOId;
    int m_RenderDataSize = 256;
    QVector4D *m_pCombile = nullptr; // 每个柱子两个三角形的6个顶点(x,y, x,y)*6
    GLint *m_HistData[4] = {nullptr};
    QVector3D *m_pVerticesCoords = nullptr;
    float* vertex_coords = nullptr;

signals:

public slots:
    void cleanup();
};

#endif // CSOPENGLWEDGETFBOHISTIN_H
