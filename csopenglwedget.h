#ifndef CSOPENGLWEDGET_H
#define CSOPENGLWEDGET_H

//#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QFile>
#include <QTimer>
#include<sys/time.h>
#include <QKeyEvent>
#include <QDir>

#include "csopenglwedgetwave.h"
#include "csopenglwedgetfalse.h"
#include "csopenglwedgetfocus.h"
#include "csopenglwedgetsharpen.h"
#include "csopenglwedgetzebra.h"
#include "csopenglwedgetlut.h"
//#include "csopenglwedgethist.h" // Hist1
//#include "csopenglwedgetfbohist.h" // Hist2
#include "csopenglwedgetfbohistin.h" // Hist3
//#include "csopenglwedgetvector.h"
#include "csopenglwedgetfbovector.h"
#include "csopenglwedgetgreenmatting.h"
#include "csopenglwedgetmag.h"

class CSOpenGLWedget : public QOpenGLWidget, QOpenGLExtraFunctions //QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    GLuint setupVertexBuffer(float vertices[], int size_of_vertices);
    GLuint genProgram(const char *vert, const char *frag);
    explicit CSOpenGLWedget(QWidget *parent = nullptr);
    unsigned char* GetTempImagData();
    ~CSOpenGLWedget();
    // 计时函数
    struct timeval G_tv_begin, G_tv_end;
    void time_begin();
    void time_end();
    double time_stake();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    QByteArray buf;

private:
    void checkGLRrror(const char *msg);
    void myGetLocation();
    void myUniformData();
    int updateImage();

    CSOpenGLWedgetWave *myCSOpenGLWedgetWave = NULL;
    CSOpenGLWedgetFalse *myCSOpenGLWedgetFalse = NULL;
    CSOpenGLWedgetFocus *myCSOpenGLWedgetFocus = NULL;
    CSOpenGLWedgetSharpen *myCSOpenGLWedgetSharpen = NULL;
    CSOpenGLWedgetZebra *myCSOpenGLWedgetZebra = NULL;
    CSOpenGLWedgetLut *myCSOpenGLWedgetLut = NULL;
//    CSOpenGLWedgetHist *myCSOpenGLWedgetHist = NULL; // Hist1
//    CSOpenGLWedgetFboHist *myCSOpenGLWedgetFboHist = NULL; // Hist2
    CSOpenGLWedgetFboHistIn *myCSOpenGLWedgetFboHistIn = NULL; // Hist3
//    CSOpenGLWedgetVector *myCSOpenGLWedgetVector = NULL;
    CSOpenGLWedgetFboVector *myCSOpenGLWedgetFboVector = NULL;
    CSOpenGLWedgetGreenMatting *myCSOpenGLWedgetGreenMatting = NULL;
    CSOpenGLWedgetMag *myCSOpenGLWedgetMag = NULL;

    const QString fL_video_YUYV = QDir::currentPath() + QString("/YUYV/yuyv.yuv");
    QOpenGLShaderProgram *program = NULL;
    QOpenGLShaderProgram *fbo_program = NULL;

    GLuint texs_yuyvID[1] = {0};
    GLuint texs_yuyvloc[1] = {0};
    // 普通2D显示纹理
    GLuint m_Fbo2DTextureId[1] = {0};
    GLuint m_Fbo2DTextureIdloc[1] = {0};

    // uniform_loc
    GLuint frameWH_loc = 0;

    int width = 1080;
    int height = 1920;

    QFile m_file;


    QTimer m_timer;

    unsigned char* imageData;
    int save_wid = 1920;
    int save_hei = 1080;

    GLuint VAO, VBO;
    GLuint FBO_vao, FBO_vbo;
    GLuint FBOId;
    GLuint m_RBO = 0;

    // PBO
    int m_FrameIndex;
    GLuint pboIds[2] = {0};
    GLubyte* bufPtr0 = NULL;
    GLubyte* bufPtr1 = NULL;

    unsigned char* fboImageData;

signals:

public slots:
    void cleanup();
    void on_pushButton_savepicture_clicked();
    // 波形图
    void on_pushButton_wave_clicked(bool use);
    void on_sliderValue_waveAlpha_clicked(int Alpha);
    void on_sliderValue_waveType_clicked(int Type);
    // 伪彩色
    void on_pushButton_false_clicked(bool use);
    // 峰值对焦
    void on_pushButton_focus_clicked(bool use);
    void on_sliderValue_focussensitivity_clicked(int Sensitivity);
    void on_sliderValue_focushighThreshold_clicked(int HighThreshold);
    // 放大镜
    void on_pushButton_magn_clicked(bool use);
    // 锐化
    void on_pushButton_sharpen_clicked(bool use);
    void on_sliderValue_sharpensharpenStr_clicked(int SharpenStr);
    // 斑马纹
    void on_pushButton_zebra_clicked(bool use);
    void on_sliderValue_zebralowThreshold_clicked(int LowThreshold);
    void on_sliderValue_zebrahigThreshold_clicked(int HigThreshold);
    // 3DLUT
    void on_pushButton_lut_clicked(bool use);
    void on_pushButton_lutFile1_clicked();
    void on_pushButton_lutFile2_clicked();
    // 直方图
    void on_pushButton_hist_clicked(bool use);
    // 矢量图
    void on_pushButton_vector_clicked(bool use);
    // greenMatting
    void on_pushButton_greenMatting_clicked(bool use);
};

#endif // CSOPENGLWEDGET_H
