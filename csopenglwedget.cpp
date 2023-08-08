#include "csopenglwedget.h"
#include <QDebug>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <QFileDialog>
#include<QLabel>

#define GET_STR(x) #x
#define A_VER 0
#define T_VER 1

void CSOpenGLWedget::checkGLRrror(const char *msg)
{
    int err = glGetError();
    if (err != GL_NO_ERROR)
        printf("err=%d(%s)\n", err, msg);
}

CSOpenGLWedget::CSOpenGLWedget(QWidget *parent) : QOpenGLWidget(parent)
{
    connect(&m_timer, &QTimer::timeout, this,[&]()
    {
        this->update();
    });
    m_timer.start(1);
}

CSOpenGLWedget::~CSOpenGLWedget()
{
    cleanup();
}

void CSOpenGLWedget::cleanup()
{
    qDebug() << "[原图]close";
    makeCurrent();
    m_file.close();
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);

    if(imageData != NULL)
    {
        delete imageData;
        imageData = NULL;
    }

    if(program != NULL)
    {
        delete program;
        program = NULL;
    }
    glDeleteTextures(1, texs_yuyvID);

    if(myCSOpenGLWedgetWave != NULL)
    {
        delete myCSOpenGLWedgetWave;
        myCSOpenGLWedgetWave = NULL;
    }
    if(myCSOpenGLWedgetFalse != NULL)
    {
        delete myCSOpenGLWedgetFalse;
        myCSOpenGLWedgetFalse = NULL;
    }
    if(myCSOpenGLWedgetFocus != NULL)
    {
        delete myCSOpenGLWedgetFocus;
        myCSOpenGLWedgetFocus = NULL;
    }
    if(myCSOpenGLWedgetSharpen != NULL)
    {
        delete myCSOpenGLWedgetSharpen;
        myCSOpenGLWedgetSharpen = NULL;
    }
    if(myCSOpenGLWedgetZebra != NULL)
    {
        delete myCSOpenGLWedgetZebra;
        myCSOpenGLWedgetZebra = NULL;
    }
    if(myCSOpenGLWedgetLut != NULL)
    {
        delete myCSOpenGLWedgetLut;
        myCSOpenGLWedgetLut = NULL;
    }
    if(myCSOpenGLWedgetFboHistIn != NULL)
    {
        delete myCSOpenGLWedgetFboHistIn;
        myCSOpenGLWedgetFboHistIn = NULL;
    }
    if(myCSOpenGLWedgetFboVector != NULL)
    {
        delete myCSOpenGLWedgetFboVector;
        myCSOpenGLWedgetFboVector = NULL;
    }
    if(myCSOpenGLWedgetMag != NULL)
    {
        delete myCSOpenGLWedgetMag;
        myCSOpenGLWedgetMag = NULL;
    }

    doneCurrent();
}

void CSOpenGLWedget::myGetLocation()
{
    // 从shader获取loc
    texs_yuyvloc[0] = fbo_program->uniformLocation("tex_yuyv");
    checkGLRrror("[原图]glGetUniformLocation(tex_yuyv)");
    frameWH_loc = fbo_program->uniformLocation("frameWH");
    checkGLRrror("[原图]glGetUniformLocation(frameWH)");
    m_Fbo2DTextureIdloc[0] = program->uniformLocation("m_Fbo2DTextureId");
    checkGLRrror("[原图]glGetUniformLocation(m_Fbo2DTextureId)");

}

void CSOpenGLWedget::myUniformData()
{
    // uniform传值
    glUniform2f(frameWH_loc, width * 1.0f, height * 1.0f);
    checkGLRrror("[focus]glUniform2f(frameWH_loc)");
}

int CSOpenGLWedget::updateImage()
{
    if(!m_file.isOpen())
    {
        return 0;
    }
    if(m_file.atEnd())
    {
        m_file.seek(0);
    }
    buf = m_file.read(width * height * 2);
    return 1;
}

void CSOpenGLWedget::initializeGL()
{
    qDebug() << "[原图]###########################################初始化###########################################";
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &CSOpenGLWedget::cleanup);
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    // FBO
    fbo_program = new QOpenGLShaderProgram;
    qDebug() << "[hist fbo]Load Vert FIle：" << fbo_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QDir::currentPath()+ QString("/Shaders/FBOYUYV.vert"));
    qDebug() << "[hist fbo]Load Frag FIle：" << fbo_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QDir::currentPath()+ QString("/Shaders/FBOYUYV.frag"));
    checkGLRrror("[hist fbo]program load shader");
    fbo_program->bindAttributeLocation("vertexIn", A_VER);
    fbo_program->bindAttributeLocation("textureIn", T_VER);
    qDebug() << "[hist fbo]link shader：" << fbo_program->link();
    qDebug() << "[hist fbo]bind shader：" << fbo_program->bind();
    checkGLRrror("[hist fbo]fbo_program link & bind");
    // FBO顶点坐标数据
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &FBO_vao);
    glGenBuffers(1, &FBO_vbo);
    glBindVertexArray(FBO_vao);
    glBindBuffer(GL_ARRAY_BUFFER, FBO_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(A_VER, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(A_VER);
    // color attribute
    glVertexAttribPointer(T_VER, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(T_VER);
    fbo_program->release();

    program = new QOpenGLShaderProgram;
//    qDebug() << "[原图]Load Vert FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Vertex, QDir::currentPath()+ QString("/Shaders/YUYV.vert"));
//    qDebug() << "[原图]Load Frag FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Fragment, QDir::currentPath() + QString("/Shaders/YUYV.frag"));
    qDebug() << "[原图]Load Vert FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Vertex, QDir::currentPath()+ QString("/Shaders/YUYV2D.vert"));
    qDebug() << "[原图]Load Frag FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Fragment, QDir::currentPath() + QString("/Shaders/YUYV2D.frag"));
    program->bindAttributeLocation("vertexIn", A_VER);
    qDebug() << QDir::currentPath()+ QString("/Shaders/YUYV2D.vert");
    program->bindAttributeLocation("textureIn", T_VER);
    qDebug() << "[原图]link shader：" << program->link();
    qDebug() << "[原图]bind shader：" << program->bind();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(A_VER, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(A_VER);
    // color attribute
    glVertexAttribPointer(T_VER, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(T_VER);
    program->release();

    myGetLocation();

    // 创建一个FBO绑定的纹理（fbo输出结果，用来普通2D显示）
    glGenTextures(1, m_Fbo2DTextureId);
    glBindTexture(GL_TEXTURE_2D, m_Fbo2DTextureId[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width / 2, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    checkGLRrror("[原图]Gen m_Fbo2DTextureId.");

    glGenTextures(1, texs_yuyvID);
    glBindTexture(GL_TEXTURE_2D, texs_yuyvID[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width / 2, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    checkGLRrror("[原图]Gen yuyv texture.");

    GLint flag;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &flag);
    qDebug() << "compress flag:" << flag;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &flag);
    qDebug() << "compress zize:"  << flag;

    // 初始化FBO
    glGenFramebuffers(1, &FBOId);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOId);
    glBindTexture(GL_TEXTURE_2D, m_Fbo2DTextureId[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Fbo2DTextureId[0], 0);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    checkGLRrror("[原图]glFramebufferTexture2D.");
//    // renderbuffer
//    glGenRenderbuffers(1, &m_RBO);
//    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width / 2, height);
//    glBindRenderbuffer(GL_RENDERBUFFER, GL_NONE);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
    // check error
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "[原图]glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE";
    }
    checkGLRrror("[原图]CreateFrameBufferObj");
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);


    m_file.setFileName(fL_video_YUYV);
    if(!m_file.open(QIODevice::ReadOnly))
    {
        qDebug()<< "[原图]打开失败：" << fL_video_YUYV ;
        return;
    }
    imageData = new unsigned char[save_wid*save_hei*3];   //截一个尺寸为save_wid, save_hei像素的图像
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    time_begin();
    fboImageData = new unsigned char[width * height * 3];
}

void CSOpenGLWedget::resizeGL(int w, int h)
{
    glViewport(0.0f, 0.0f, w, h);
    save_wid = w;
    save_hei = h;
    qDebug() << "[原图]w,h:" << w << h;
}

void CSOpenGLWedget::paintGL()
{
    updateImage();
//    time_begin();

    glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT
//    glEnable(GL_DEPTH_TEST);

    /************ FBO ********************************************/
    // FBO
    fbo_program->bind();
    glBindVertexArray(FBO_vao);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOId);
    checkGLRrror("[vector map]glBindFramebuffer");
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.translate(0.0f, 0.0f, 0.0f);
    matrix.rotate(0.0f, 0.0f, 0.0f, 1.0f);
    matrix.scale(1.0f);
    fbo_program->setUniformValue("transform",matrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texs_yuyvID[0]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
    glUniform1i(texs_yuyvloc[0], 0);

    myUniformData();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(GL_NONE);
    fbo_program->release();
    /********** 关键之处 ***********/
    GLuint fb = context()->defaultFramebufferObject();
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //*********************************************************************************************
    // 2D
    glViewport(0.0f, 0.0f, width/2, height);
    program->bind();
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Fbo2DTextureId[0]);
    glUniform1i(m_Fbo2DTextureIdloc[0], 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(GL_NONE);
    program->release();
//    time_end();
}

// ###########################################################################################
void CSOpenGLWedget::time_begin()
{
    gettimeofday(&G_tv_begin,NULL);
}

void CSOpenGLWedget::time_end()
{
    gettimeofday(&G_tv_end, NULL);
    double tmspan = (G_tv_end.tv_sec - G_tv_begin.tv_sec) * 1000.0 +
                    (G_tv_end.tv_usec - G_tv_begin.tv_usec) / 1000.0;
    qDebug() << "[原图]timespan(ms):" << tmspan;
}

unsigned char* CSOpenGLWedget::GetTempImagData()
{
    return imageData;
}

// ###########################################################################################
void CSOpenGLWedget::on_pushButton_savepicture_clicked()
{
    if(myCSOpenGLWedgetFalse != NULL)
    {
        myCSOpenGLWedgetFalse->savepicture();
    }
}

// 波形图
void CSOpenGLWedget::on_pushButton_wave_clicked(bool use)
{
    /*显示CSOpenGLWedgetWave对象窗口*/
    if(use == true)
    {
        if(myCSOpenGLWedgetWave == NULL)
        {
            myCSOpenGLWedgetWave = new CSOpenGLWedgetWave;
            myCSOpenGLWedgetWave->setWindowModality(Qt::NonModal);
            myCSOpenGLWedgetWave->setWindowTitle(tr("波形图"));
            myCSOpenGLWedgetWave->resize(480, 270);
//            myCSOpenGLWedgetWave->setWindowFlags(Qt::FramelessWindowHint);
            myCSOpenGLWedgetWave->setAttribute(Qt::WA_TranslucentBackground, true);
            myCSOpenGLWedgetWave->show();
        //    myCSOpenGLWedgetWave.showFullScreen();
        }
    }
    else
    {
        if(myCSOpenGLWedgetWave != NULL)
        {
            delete myCSOpenGLWedgetWave;
            myCSOpenGLWedgetWave = NULL;
        }
    }
}
void CSOpenGLWedget::on_sliderValue_waveAlpha_clicked(int Alpha)
{
    if(myCSOpenGLWedgetWave != NULL)
    {
        myCSOpenGLWedgetWave->set_alphabscope(Alpha);
    }
}
void CSOpenGLWedget::on_sliderValue_waveType_clicked(int Type)
{
    if(myCSOpenGLWedgetWave != NULL)
    {
        myCSOpenGLWedgetWave->set_useWaveform(Type);
    }
}

// 伪彩色
void CSOpenGLWedget::on_pushButton_false_clicked(bool use)
{
    /*显示CSOpenGLWedgetFalse对象窗口*/
    if(use == true)
    {
        if(myCSOpenGLWedgetFalse == NULL)
        {
            myCSOpenGLWedgetFalse = new CSOpenGLWedgetFalse;
            myCSOpenGLWedgetFalse->setWindowModality(Qt::NonModal);
            myCSOpenGLWedgetFalse->setWindowTitle(tr("伪彩色"));
            myCSOpenGLWedgetFalse->setWindowFlags(Qt::FramelessWindowHint | windowFlags()); // 无边框 (另加拖动窗口)
            myCSOpenGLWedgetFalse->resize(480, 270);
            myCSOpenGLWedgetFalse->show();
            //    myCSOpenGLWedgetFalse.showFullScreen();
        }
    }
    else
    {
        if(myCSOpenGLWedgetFalse != NULL)
        {
            delete myCSOpenGLWedgetFalse;
            myCSOpenGLWedgetFalse = NULL;
        }
    }
}

// 峰值对焦
void CSOpenGLWedget::on_pushButton_focus_clicked(bool use)
{
    /*显示CSOpenGLWedgetFocus对象窗口*/
    if(use == true)
    {
        if(myCSOpenGLWedgetFocus == NULL)
        {
            myCSOpenGLWedgetFocus = new CSOpenGLWedgetFocus;
            myCSOpenGLWedgetFocus->setWindowModality(Qt::NonModal);
            myCSOpenGLWedgetFocus->setWindowTitle(tr("峰值对焦"));
            myCSOpenGLWedgetFocus->setWindowFlags(Qt::FramelessWindowHint | windowFlags()); // 无边框 (另加拖动窗口)
            myCSOpenGLWedgetFocus->setAttribute(Qt::WA_TranslucentBackground, true);
            myCSOpenGLWedgetFocus->show();
        //    myCSOpenGLWedgetFocus->showFullScreen();
        }
    }
    else
    {
        if(myCSOpenGLWedgetFocus != NULL)
        {
            delete myCSOpenGLWedgetFocus;
            myCSOpenGLWedgetFocus = NULL;
        }
    }
}
void CSOpenGLWedget::on_sliderValue_focussensitivity_clicked(int Sensitivity)
{
    if(myCSOpenGLWedgetFocus != NULL)
    {
        myCSOpenGLWedgetFocus->set_sensitivity(Sensitivity);
    }
}
void CSOpenGLWedget::on_sliderValue_focushighThreshold_clicked(int HighThreshold)
{
    if(myCSOpenGLWedgetFocus != NULL)
    {
        myCSOpenGLWedgetFocus->set_highThreshold(HighThreshold);
    }
}

// 放大镜
void CSOpenGLWedget::on_pushButton_magn_clicked(bool use)
{
    /*显示CSOpenGLWedgetSharpen对象窗口*/
    if(use == true)
    {
        if(myCSOpenGLWedgetMag == NULL)
        {
            myCSOpenGLWedgetMag = new CSOpenGLWedgetMag;
            myCSOpenGLWedgetMag->setWindowModality(Qt::NonModal);
            myCSOpenGLWedgetMag->setWindowTitle(tr("放大镜"));
//            myCSOpenGLWedgetMag->setWindowFlags(Qt::FramelessWindowHint | windowFlags()); // 无边框 (另加拖动窗口)
            myCSOpenGLWedgetMag->setAttribute(Qt::WA_TranslucentBackground, true);
            myCSOpenGLWedgetMag->resize(480, 270);
            myCSOpenGLWedgetMag->show();
        //    myCSOpenGLWedgetMag.showFullScreen();
        }
    }
    else
    {
        if(myCSOpenGLWedgetMag != NULL)
        {
            delete myCSOpenGLWedgetMag;
            myCSOpenGLWedgetMag = NULL;
        }
    }
}

// 锐化
void CSOpenGLWedget::on_pushButton_sharpen_clicked(bool use)
{
    /*显示CSOpenGLWedgetSharpen对象窗口*/
    if(use == true)
    {
        if(myCSOpenGLWedgetSharpen == NULL)
        {
            myCSOpenGLWedgetSharpen = new CSOpenGLWedgetSharpen;
            myCSOpenGLWedgetSharpen->setWindowModality(Qt::NonModal);
            myCSOpenGLWedgetSharpen->setWindowTitle(tr("锐化"));
            myCSOpenGLWedgetSharpen->setAttribute(Qt::WA_TranslucentBackground, true);
            myCSOpenGLWedgetSharpen->show();
        //    myCSOpenGLWedgetSharpen.showFullScreen();
        }
    }
    else
    {
        if(myCSOpenGLWedgetSharpen != NULL)
        {
            delete myCSOpenGLWedgetSharpen;
            myCSOpenGLWedgetSharpen = NULL;
        }
    }
}
void CSOpenGLWedget::on_sliderValue_sharpensharpenStr_clicked(int SharpenStr)
{
    if(myCSOpenGLWedgetSharpen != NULL)
    {
        myCSOpenGLWedgetSharpen->set_sharpenStr(SharpenStr);
    }
}

// 斑马纹
void CSOpenGLWedget::on_pushButton_zebra_clicked(bool use)
{
    /*显示CSOpenGLWedgetZebra对象窗口*/
    if(use == true)
    {
        if(myCSOpenGLWedgetZebra == NULL)
        {
            myCSOpenGLWedgetZebra = new CSOpenGLWedgetZebra;
            myCSOpenGLWedgetZebra->setWindowModality(Qt::NonModal);
            myCSOpenGLWedgetZebra->setWindowTitle(tr("斑马纹"));
            myCSOpenGLWedgetZebra->show();
        //    myCSOpenGLWedgetZebra.showFullScreen();
        }
    }
    else
    {
        if(myCSOpenGLWedgetZebra != NULL)
        {
            delete myCSOpenGLWedgetZebra;
            myCSOpenGLWedgetZebra = NULL;
        }
    }
}
void CSOpenGLWedget::on_sliderValue_zebralowThreshold_clicked(int LowThreshold)
{
    if(myCSOpenGLWedgetZebra != NULL)
    {
        myCSOpenGLWedgetZebra->set_zebraLow(LowThreshold);
    }
}
void CSOpenGLWedget::on_sliderValue_zebrahigThreshold_clicked(int HigThreshold)
{
    if(myCSOpenGLWedgetZebra != NULL)
    {
        myCSOpenGLWedgetZebra->set_zebraHig(HigThreshold);
    }
}

// 3DLUT
void CSOpenGLWedget::on_pushButton_lut_clicked(bool use)
{
    /*显示CSOpenGLWedgetLut对象窗口*/
    if(use == true)
    {
        if(myCSOpenGLWedgetLut == NULL)
        {
            myCSOpenGLWedgetLut = new CSOpenGLWedgetLut;
            myCSOpenGLWedgetLut->setWindowModality(Qt::NonModal);
            myCSOpenGLWedgetLut->setWindowTitle(tr("3D LUT"));
            myCSOpenGLWedgetLut->show();
        //    myCSOpenGLWedgetLut.showFullScreen();
        }
    }
    else
    {
        if(myCSOpenGLWedgetLut != NULL)
        {
            delete myCSOpenGLWedgetLut;
            myCSOpenGLWedgetLut = NULL;
        }
    }
}
void CSOpenGLWedget::on_pushButton_lutFile1_clicked()
{
    if(myCSOpenGLWedgetLut != NULL)
    {

        myCSOpenGLWedgetLut->set_lutImagePath("/lut/Canon C-Log.cube");
    }
}
void CSOpenGLWedget::on_pushButton_lutFile2_clicked()
{
    if(myCSOpenGLWedgetLut != NULL)
    {
        myCSOpenGLWedgetLut->set_lutImagePath("/lut/Sony S-Log2.cube");
    }
}

// 直方图
void CSOpenGLWedget::on_pushButton_hist_clicked(bool use)
{
//    /*(1)显示CSOpenGLWedgetHist对象窗口*/
//    if(use == true)
//    {
//        if(myCSOpenGLWedgetHist == NULL)
//        {
//            myCSOpenGLWedgetHist = new CSOpenGLWedgetHist;
//            myCSOpenGLWedgetHist->setWindowModality(Qt::NonModal);
//            myCSOpenGLWedgetHist->setWindowTitle(tr("直方图"));
//            myCSOpenGLWedgetHist->show();
//        //    myCSOpenGLWedgetHist.showFullScreen();
//        }
//    }
//    else
//    {
//        if(myCSOpenGLWedgetHist != NULL)
//        {
//            delete myCSOpenGLWedgetHist;
//            myCSOpenGLWedgetHist = NULL;
//        }
//    }

//    /*(2)显示CSOpenGLWedgetFboHist对象窗口*/
//    if(use == true)
//    {
//        if(myCSOpenGLWedgetFboHist == NULL)
//        {
//            myCSOpenGLWedgetFboHist = new CSOpenGLWedgetFboHist;
//            myCSOpenGLWedgetFboHist->setWindowModality(Qt::NonModal);
//            myCSOpenGLWedgetFboHist->setWindowTitle(tr("直方图"));
//            myCSOpenGLWedgetFboHist->show();
//        //    myCSOpenGLWedgetHist.showFullScreen();
//        }
//    }
//    else
//    {
//        if(myCSOpenGLWedgetFboHist != NULL)
//        {
//            delete myCSOpenGLWedgetFboHist;
//            myCSOpenGLWedgetFboHist = NULL;
//        }
//    }

    /*(3)显示CSOpenGLWedgetFboHistIn对象窗口,着色器内部画柱状图*/
    if(use == true)
    {
        if(myCSOpenGLWedgetFboHistIn == NULL)
        {
            myCSOpenGLWedgetFboHistIn = new CSOpenGLWedgetFboHistIn;
            myCSOpenGLWedgetFboHistIn->setWindowModality(Qt::NonModal);
            myCSOpenGLWedgetFboHistIn->setWindowTitle(tr("直方图"));
            myCSOpenGLWedgetFboHistIn->resize(480, 270);
//            myCSOpenGLWedgetFboHistIn->setWindowFlags(Qt::FramelessWindowHint | windowFlags()); // 无边框 (另加拖动窗口)
            myCSOpenGLWedgetFboHistIn->setAttribute(Qt::WA_TranslucentBackground, true); // 窗口透明度
            myCSOpenGLWedgetFboHistIn->show();
        //    myCSOpenGLWedgetHist.showFullScreen();
        }
    }
    else
    {
        if(myCSOpenGLWedgetFboHistIn != NULL)
        {
            delete myCSOpenGLWedgetFboHistIn;
            myCSOpenGLWedgetFboHistIn = NULL;
        }
    }
}

// 矢量图
void CSOpenGLWedget::on_pushButton_vector_clicked(bool use)
{
//    /*显示CSOpenGLWedgetVector对象窗口*/
//    if(use == true)
//    {
//        if(myCSOpenGLWedgetVector == NULL)
//        {
//            myCSOpenGLWedgetVector = new CSOpenGLWedgetVector;
//            myCSOpenGLWedgetVector->setWindowModality(Qt::NonModal);
//            myCSOpenGLWedgetVector->setWindowTitle(tr("矢量图"));
//            myCSOpenGLWedgetVector->show();
//        //    myCSOpenGLWedgetHist.showFullScreen();
//        }
//    }
//    else
//    {
//        if(myCSOpenGLWedgetVector != NULL)
//        {
//            delete myCSOpenGLWedgetVector;
//            myCSOpenGLWedgetVector = NULL;
//        }
//    }

    /*显示CSOpenGLWedgetFboVector对象窗口*/
    if(use == true)
    {
        if(myCSOpenGLWedgetFboVector == NULL)
        {
            myCSOpenGLWedgetFboVector = new CSOpenGLWedgetFboVector;
            myCSOpenGLWedgetFboVector->setWindowModality(Qt::NonModal);
            myCSOpenGLWedgetFboVector->setWindowTitle(tr("矢量图"));
            myCSOpenGLWedgetFboVector->resize(300, 300); // 初始大小
            myCSOpenGLWedgetFboVector->setAttribute(Qt::WA_TranslucentBackground, true);
            myCSOpenGLWedgetFboVector->show();
        //    myCSOpenGLWedgetHist.showFullScreen();
        }
    }
    else
    {
        if(myCSOpenGLWedgetFboVector != NULL)
        {
            delete myCSOpenGLWedgetFboVector;
            myCSOpenGLWedgetFboVector = NULL;
        }
    }
}

// GreenMatting
void CSOpenGLWedget::on_pushButton_greenMatting_clicked(bool use)
{
    /*显示CSOpenGLWedgetGreenMatting对象窗口*/
    if(use == true)
    {
        if(myCSOpenGLWedgetGreenMatting == NULL)
        {
            myCSOpenGLWedgetGreenMatting = new CSOpenGLWedgetGreenMatting;
            myCSOpenGLWedgetGreenMatting->show();
        }
    }
    else
    {
        if(myCSOpenGLWedgetGreenMatting != NULL)
        {
            delete myCSOpenGLWedgetGreenMatting;
            myCSOpenGLWedgetGreenMatting = NULL;
        }
    }
}

