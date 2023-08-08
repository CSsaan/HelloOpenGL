#include "csopenglwedgetfbohistin.h"
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <vector>
#include <iostream>
using namespace std;

#define A_VER 0
#define T_VER 1
#define FBOHIST_IMG_SIZE 300

void CSOpenGLWedgetFboHistIn::checkGLRrror(const char *msg)
{
    int err = glGetError();
    if (err != GL_NO_ERROR)
        printf("err=%d(%s)\n", err, msg);
}

// ############################################################################################
void CSOpenGLWedgetFboHistIn::myGetLocation()
{
    // 从shader获取loc
    texs_yuyvloc[0] = fbo_program->uniformLocation("tex_yuyv");
    checkGLRrror("[hist fbo]glGetUniformLocation(tex_yuyv)");
    alphabscope_loc = program_2D->uniformLocation("alphabscope");
    checkGLRrror("[hist fbo]glGetUniformLocation(alphabscope)");
    useRGB_loc = program_2D->uniformLocation("useRGB");
    checkGLRrror("[hist fbo]glGetUniformLocation(useRGB)");

    points_loc[0] = program_2D->uniformLocation("pointsR");
    checkGLRrror("[hist fbo]glGetUniformLocation(pointsR)");
    points_loc[1] = program_2D->uniformLocation("pointsG");
    checkGLRrror("[hist fbo]glGetUniformLocation(pointsG)");
    points_loc[2] = program_2D->uniformLocation("pointsB");
    checkGLRrror("[hist fbo]glGetUniformLocation(pointsB)");
    points_loc[3] = program_2D->uniformLocation("pointsY");
    checkGLRrror("[hist fbo]glGetUniformLocation(pointsY)");
}

void CSOpenGLWedgetFboHistIn::myUniformData()
{
    // uniform传值
    glUniform1i(useRGB_loc, useRGB);
    checkGLRrror("[hist fbo]glUniform1f(useRGB_loc)");
    glUniform1f(alphabscope_loc, (100 - alphabscope) / 100.0f); // 0-100
    checkGLRrror("[hist fbo]glUniform1f(alphabscope_loc)");
}

CSOpenGLWedgetFboHistIn::CSOpenGLWedgetFboHistIn(QWidget *parent) : QOpenGLWidget(parent)
{
    connect(&m_timer, &QTimer::timeout, this, [&]()
            { this->update(); });
    m_timer.start(1);
}

CSOpenGLWedgetFboHistIn::~CSOpenGLWedgetFboHistIn()
{
    cleanup();
}

void CSOpenGLWedgetFboHistIn::cleanup()
{
    qDebug() << "[hist fbo]close";
    if (m_file.isOpen())
    {
        m_file.close();
    }
    makeCurrent();
    if (fbo_program != NULL)
    {
        delete fbo_program;
        fbo_program = NULL;
    }
    if (program_2D != NULL)
    {
        delete program_2D;
        program_2D = NULL;
    }
    glDeleteBuffers(1, &FBOId);
    glDeleteVertexArrays(1, &FBO_vao);
    glDeleteBuffers(1, &FBO_vbo);
    glDeleteVertexArrays(1, &D2_vao);
    glDeleteBuffers(1, &D2_vbo);
    glDeleteTextures(1, m_Fbo2DTextureId);
    glDeleteTextures(1, texs_yuyvID);
    doneCurrent();
}

void CSOpenGLWedgetFboHistIn::initializeGL()
{
    //    glGenBuffers(1, &readBuffer);
    //    glBindBuffer(GL_ARRAY_BUFFER, readBuffer);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_UNSIGNED_BYTE) * FBOHIST_IMG_SIZE * FBOHIST_IMG_SIZE * 3, NULL, GL_DYNAMIC_DRAW);
    //    glBindBuffer(GL_ARRAY_BUFFER, 0);

    qDebug() << "[hist fbo]###########################################初始化###########################################";
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    //    m_pVerticesCoords = new QVector3D[m_RenderDataSize * 6]; //(x,y,z) * 6 points
    m_pCombile = new QVector4D[m_RenderDataSize * 6]; // (x,y, x,y) * 6 points
    m_HistData[0] = new GLint[m_RenderDataSize];         // Hist R
    m_HistData[1] = new GLint[m_RenderDataSize];         // Hist G
    m_HistData[2] = new GLint[m_RenderDataSize];         // Hist B
    m_HistData[3] = new GLint[m_RenderDataSize];         // Hist Y

    // FBO
    fbo_program = new QOpenGLShaderProgram;
    qDebug() << "[hist fbo]Load Vert FIle：" << fbo_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QDir::currentPath()+ QString("/Shaders/FBOVECTOR.vert"));
    qDebug() << "[hist fbo]Load Frag FIle：" << fbo_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QDir::currentPath()+ QString("/Shaders/FBOVECTOR.frag"));
    checkGLRrror("[hist fbo]program load shader");
    fbo_program->bindAttributeLocation("vertexIn", A_VER);
    fbo_program->bindAttributeLocation("textureIn", T_VER);
    qDebug() << "[hist fbo]link shader：" << fbo_program->link();
    qDebug() << "[hist fbo]bind shader：" << fbo_program->bind();
    checkGLRrror("[hist fbo]program link & bind");
    // FBO顶点坐标数据
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f};
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

    // 普通2D显示
    program_2D = new QOpenGLShaderProgram;
    qDebug() << "[hist 2d]Load Vert FIle：" << program_2D->addShaderFromSourceFile(QOpenGLShader::Vertex, QDir::currentPath()+ QString("/Shaders/HISTIN.vert"));
    qDebug() << "[hist 2d]Load Frag FIle：" << program_2D->addShaderFromSourceFile(QOpenGLShader::Fragment, QDir::currentPath()+ QString("/Shaders/HISTIN.frag"));
    checkGLRrror("[hist 2d]program load shader");
    program_2D->bindAttributeLocation("vertexIn", A_VER);
    program_2D->bindAttributeLocation("textureIn", T_VER);
    qDebug() << "[hist 2d]link shader：" << program_2D->link();
    qDebug() << "[hist 2d]bind shader：" << program_2D->bind();
    checkGLRrror("[hist 2d]program link & bind");
    // 生成绑定vao和vbo数据
    glGenVertexArrays(1, &D2_vao);
    glBindVertexArray(D2_vao);
    glGenBuffers(1, &D2_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, D2_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    checkGLRrror("[hist 2d]vao_vbo_bind_init");
    glVertexAttribPointer(A_VER, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(A_VER);
    glVertexAttribPointer(T_VER, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(T_VER);
    checkGLRrror("vao_vbo_enable_init");
    program_2D->release();

    myGetLocation();

    // 创建一个FBO绑定的纹理（fbo输出结果，用来普通2D显示）
    glGenTextures(1, m_Fbo2DTextureId);
    glBindTexture(GL_TEXTURE_2D, m_Fbo2DTextureId[0]);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FBOHIST_IMG_SIZE, FBOHIST_IMG_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    // 创建yuyv纹理
    glGenTextures(1, texs_yuyvID);
    glBindTexture(GL_TEXTURE_2D, texs_yuyvID[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width / 2, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // 初始化FBO
    glGenFramebuffers(1, &FBOId);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOId);
    glBindTexture(GL_TEXTURE_2D, m_Fbo2DTextureId[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Fbo2DTextureId[0], 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "PBOSample::CreateFrameBufferObj glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE";
    }
    checkGLRrror("[vector map]CreateFrameBufferObj");
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

    m_file.setFileName(fL_video_YUYV);
    if (!m_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "[hist fbo]打开失败！";
        return;
    }
    imageData = new unsigned char[save_wid * save_hei * 3];
    fboImageData = new unsigned char[FBOHIST_IMG_SIZE * FBOHIST_IMG_SIZE * 3];
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
//    time_begin();
}

void CSOpenGLWedgetFboHistIn::resizeGL(int w, int h)
{
    glViewport(0.0f, 0.0f, w, h);
    save_wid = w;
    save_hei = h;
    qDebug() << "[hist fbo]w,h:" << w << h;
}

void CSOpenGLWedgetFboHistIn::paintGL()
{
    time_begin();

    if (m_file.atEnd())
    {
        m_file.seek(0);
    }
    QByteArray buf = m_file.read(width * height * 2);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //*********************************************************************************************
    // FBO
    glViewport(0.0, 0.0, (GLsizei)FBOHIST_IMG_SIZE, (GLsizei)FBOHIST_IMG_SIZE);
    fbo_program->bind();
    glBindVertexArray(FBO_vao);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOId);
    checkGLRrror("[vector map]glBindFramebuffer");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texs_yuyvID[0]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
    glUniform1i(texs_yuyvloc[0], 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

//    // 保存FBO结果
//    #include<qopenglfunctions_3_0.h>
//    glBindBuffer(GL_ARRAY_BUFFER, readBuffer);
//    glReadPixels(0, 0, FBOHIST_IMG_SIZE, FBOHIST_IMG_SIZE, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//    float *data = (unsigned char *)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
//    memcpy(fboImageData, data, sizeof(data));



    memset(fboImageData, 0x00, FBOHIST_IMG_SIZE * FBOHIST_IMG_SIZE * 3);
    glReadPixels(0, 0, (GLsizei)FBOHIST_IMG_SIZE, (GLsizei)FBOHIST_IMG_SIZE, GLenum(GL_RGB), GLenum(GL_UNSIGNED_BYTE), fboImageData);
    fbo_program->release();
    // // 显示一下FBO的图片
    // QImage image(fboImageData, FBOHIST_IMG_SIZE, FBOHIST_IMG_SIZE, QImage::Format_RGB888);
    // QLabel *label = new QLabel(" ", 0);
    // QPixmap mp;
    // mp = mp.fromImage(image);
    // label->setPixmap(mp);
    // label->show();

    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(0);

    //*********************************************************************************************
    glEnable(GL_BLEND);   // 启用混合
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_COLOR); // 保证全透明时，波形图存在
    // map
    glViewport(0.0, 0.0, save_wid, save_hei);
    program_2D->bind();

    // 计算直方图的顶点数组
    render_Mesh(fboImageData);

    glBindVertexArray(D2_vao);
    glUniform1iv(points_loc[0], m_RenderDataSize, m_HistData[0]); // R
    glUniform1iv(points_loc[1], m_RenderDataSize, m_HistData[1]); // G
    glUniform1iv(points_loc[2], m_RenderDataSize, m_HistData[2]); // B
    glUniform1iv(points_loc[3], m_RenderDataSize, m_HistData[3]); // Y
    myUniformData();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();
    glBindVertexArray(0);
    program_2D->release();

//    time_end();
    glDisable(GL_BLEND); // 关闭混合模式
}

// ###############################################################################################
void CSOpenGLWedgetFboHistIn::time_begin()
{
    gettimeofday(&G_tv_begin, NULL);
}

void CSOpenGLWedgetFboHistIn::time_end()
{
    gettimeofday(&G_tv_end, NULL);
    double tmspan = (G_tv_end.tv_sec - G_tv_begin.tv_sec) * 1000.0 +
                    (G_tv_end.tv_usec - G_tv_begin.tv_usec) / 1000.0;
    qDebug() << "[hist fbo]timespan(ms):" << tmspan;
}

void CSOpenGLWedgetFboHistIn::set_alphabscope(int s)
{
    alphabscope = s;
}

void CSOpenGLWedgetFboHistIn::set_type(bool usergb)
{
    useRGB = usergb;
}

void CSOpenGLWedgetFboHistIn::render_Mesh(const uint8_t *imgDataPtr)
{
    // 计算直方图
    CalHistgramPointer(imgDataPtr);
    //    UpdateMesh();
}

// 计算直方图（通过图像指针）
void CSOpenGLWedgetFboHistIn::CalHistgramPointer(const uint8_t *imgDataPtr)
{
    if (imgDataPtr == nullptr)
    {
        memset(m_HistData[0], 0, sizeof(GLint) * m_RenderDataSize);
        memset(m_HistData[1], 0, sizeof(GLint) * m_RenderDataSize);
        memset(m_HistData[2], 0, sizeof(GLint) * m_RenderDataSize);
        memset(m_HistData[3], 0, sizeof(GLint) * m_RenderDataSize);
        return;
    }

    vector<vector<int>> Histogram(256, {0}); // 初始化为0
    for (unsigned int i = 0; i < Histogram.size(); i++)
        Histogram[i].resize(4);

    for (int row = 0; row < FBOHIST_IMG_SIZE; row++)
    {
        for (int col = 0; col < FBOHIST_IMG_SIZE; col++)
        {
            Histogram[imgDataPtr[3 * (row * FBOHIST_IMG_SIZE + col) + 2]][2]++; // 当前位置像素R值
            Histogram[imgDataPtr[3 * (row * FBOHIST_IMG_SIZE + col) + 1]][1]++; // 当前位置像素G值
            Histogram[imgDataPtr[3 * (row * FBOHIST_IMG_SIZE + col) + 0]][0]++; // 当前位置像素B值
//             Histogram[(unsigned long)(imgDataPtr[3 * (row * FBOHIST_IMG_SIZE + col) + 2] * 0.2989 + imgDataPtr[3 * (row * FBOHIST_IMG_SIZE + col) + 1] * 0.5870 + imgDataPtr[3 * (row * FBOHIST_IMG_SIZE + col) + 0] * 0.1140)][3]++;
            Histogram[(int)((imgDataPtr[3 * (row * FBOHIST_IMG_SIZE + col) + 2] * 0.2126 + imgDataPtr[3 * (row * FBOHIST_IMG_SIZE + col) + 1] * 0.7152 + imgDataPtr[3 * (row * FBOHIST_IMG_SIZE + col) + 0] * 0.0722))][3]++;
        }
    }

    for (int n = 0; n < m_RenderDataSize; n++)
    {
        m_HistData[0][n] = (GLint)Histogram[n][0]; // R通道
        m_HistData[1][n] = (GLint)Histogram[n][1]; // G通道
        m_HistData[2][n] = (GLint)Histogram[n][2]; // B通道
        m_HistData[3][n] = (GLint)Histogram[n][3]; // 灰度直方图
    }
}

// 顶点坐标+纹理坐标
QVector4D CSOpenGLWedgetFboHistIn::Combile(const QVector2D &VertCoord, const QVector2D &texCoord)
{
    return QVector4D(VertCoord[0], VertCoord[1], texCoord[0], texCoord[1]);
}

void CSOpenGLWedgetFboHistIn::savepicture()
{
    makeCurrent();
    memset(imageData, 0x00, save_wid * save_hei * 3);
    glReadPixels(0, 0, save_wid, save_hei, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    qDebug() << "[false]save_wid, save_hei:" << save_wid << "," << save_hei;
    QString filePath = QFileDialog::getSaveFileName(this, QStringLiteral("保存图像"), ".jpg");
    if (filePath == "")
    {
        return;
    }
    QImage image(imageData, save_wid, save_hei, QImage::Format_RGB888);
    image = image.mirrored(false, true);
    image.save(filePath);
}
