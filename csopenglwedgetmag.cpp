#include "csopenglwedgetmag.h"
#include <QDebug>
#include <QFileDialog>

#define A_VER 0
#define T_VER 1

void CSOpenGLWedgetMag::checkGLRrror(const char *msg)
{
    int err = glGetError();
    if (err != GL_NO_ERROR)
        printf("err=%d(%s)\n", err, msg);
}

// ############################################################################################
void CSOpenGLWedgetMag::myGetLocation()
{
    texs_yuyvloc[0] = program->uniformLocation("tex_yuyv");
    checkGLRrror("[sharpen]glGetUniformLocation(tex_yuyv)");
    frameWH_loc = program->uniformLocation("frameWH");
    checkGLRrror("[focus]glGetUniformLocation(frameWH)");
    sharpenStr_loc = program->uniformLocation("sharpenStr");
    checkGLRrror("[focus]glGetUniformLocation(sharpenStr)");

    cunstom_Params_uniform = program->uniformLocation("cunstom_Params");
    checkGLRrror("[focus]glGetUniformLocation(cunstom_Params)");
    cunstom_Params_plus_uniform = program->uniformLocation("cunstom_Params_plus");
    checkGLRrror("[focus]glGetUniformLocation(cunstom_Params_plus)");
    MagLineWifth_uniform = program->uniformLocation("MagLineWifth");
    checkGLRrror("[focus]glGetUniformLocation(MagLineWifth)");
}

void CSOpenGLWedgetMag::myUniformData()
{
    glUniform2f(frameWH_loc, width * 1.0f, height * 1.0f);
    checkGLRrror("[focus]glUniform2f(frameWH_loc)");
    glUniform1f(sharpenStr_loc, sharpenStr);
    checkGLRrror("[focus]glUniform1f(sharpenStr_loc)");
}

float *CSOpenGLWedgetMag::drawRectVertexs(float centerX, float centerY, float ratio)
{
    static GLfloat vertexs[4] = {0.0f};
    int fw = width;
    float y = fw * ratio / 2.0f / height;
    if (centerX - ratio <= -1.0f)
        centerX = -1.0 + ratio;
    if (centerX + ratio >= 1.0f)
        centerX = 1.0 - ratio;
    if (centerY + y >= 1.0f)
        centerY = 1.0f - y;
    if (centerY - y <= -1.0f)
        centerY = -1.0f + y;
    vertexs[0] = centerX - ratio;
    vertexs[1] = centerY + y;
    vertexs[2] = centerX + ratio;
    vertexs[3] = centerY - y;
    return vertexs;
}

CSOpenGLWedgetMag::CSOpenGLWedgetMag(QWidget *parent) : QOpenGLWidget(parent)
{
    connect(&m_timer, &QTimer::timeout, this,[&]()
    {
        this->update();
    });
    m_timer.start(1);
}

CSOpenGLWedgetMag::~CSOpenGLWedgetMag()
{
    cleanup();
}

void CSOpenGLWedgetMag::cleanup()
{
    qDebug() << "[sharpen]close";
    if(m_file.isOpen())
    {
        m_file.close();
    }
    makeCurrent();
    if(program != NULL)
    {
        delete program;
        program = NULL;
    }
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteTextures(1, texs_yuyvID);
    doneCurrent();
}

void CSOpenGLWedgetMag::initializeGL()
{
    qDebug() << "[sharpen]###########################################初始化###########################################";
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    program = new QOpenGLShaderProgram;
    qDebug() << "[sharpen]Load Vert FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Vertex, QDir::currentPath()+ QString("/Shaders/MAG.vert"));
    qDebug() << "[sharpen]Load Frag FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Fragment, QDir::currentPath()+ QString("/Shaders/MAG.frag"));
    checkGLRrror("[sharpen]program load shader");
    program->bindAttributeLocation("vertexIn", A_VER);
    program->bindAttributeLocation("textureIn", T_VER);
    qDebug() << "[sharpen]link shader：" << program->link();
    qDebug() << "[sharpen]bind shader：" << program->bind();
    checkGLRrror("[sharpen]program link & bind");

//    static const GLfloat ver[] = {
//        -1.0f, -1.0f,
//        1.0f, -1.0f,
//        -1.0f, 1.0f,
//        1.0f, 1.0f
//    };
//    static const GLfloat tex[] = {
//        0.0f, 1.0f,
//        1.0f, 1.0f,
//        0.0f, 0.0f,
//        1.0f, 0.0f
//    };
//    glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
//    glEnableVertexAttribArray(A_VER);
//    glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
//    glEnableVertexAttribArray(T_VER);

    float vertices[] = {
            -1.0f, -1.0f,0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f,  1.0f, 0.0f
    };
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

    myGetLocation();
    // 创建yuyv纹理
    glGenTextures(1, texs_yuyvID);
    glBindTexture(GL_TEXTURE_2D, texs_yuyvID[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width / 2, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    checkGLRrror("[sharpen]Gen lut texture.");

    m_file.setFileName(fL_video_YUYV);
    if(!m_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "[sharpen]打开失败！";
        return;
    }
    imageData = new unsigned char[save_wid*save_hei*3];
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CSOpenGLWedgetMag::resizeGL(int w, int h)
{
    glViewport(0.0f, 0.0f, w, h);
    save_wid = w;
    save_hei = h;
    qDebug() << "[sharpen]w,h:" << w << h;
}

void CSOpenGLWedgetMag::paintGL()
{
    time_begin();

    glEnable(GL_BLEND);   // 启用混合
    glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA); // 保证全透明时，波形图存在

    if(m_file.atEnd())
    {
        m_file.seek(0);
    }
    QByteArray buf = m_file.read(width * height * 2);

    //    glUseProgram(program->programId());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    program->bind();
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texs_yuyvID[0]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
    glUniform1i(texs_yuyvloc[0], 0);

    myUniformData();

    // 传放大参数
    float centerX = 50.0f; float centerY = 50.0f;
    float partZoomRatio = 0.1f; // 0.1f,0.2f,0.5f
    float partZoomScale = 2.0f; // 2.0f,3.0f,4.0f
    //
    float _centerX = (centerX / 100.0f - 0.5f) * 2.0f;
    float _centerY = (0.5f - centerY / 100.0f) * 2.0f;
    // centerX = 0.75; centerY = 0.5;
    float ratio = partZoomRatio == 0.1f ? 0.3f : (partZoomRatio == 0.2f ? 0.4f : 0.5f);
    GLfloat *idex = drawRectVertexs(_centerX, _centerY, ratio);
    float arg1 = (idex[0] + 1.0f) / 2.0f;
    float arg2 = (1.0f + idex[2]) / 2.0f;
    float arg3 = (1.0f - idex[1]) / 2.0f;
    float arg4 = (1.0f - idex[3]) / 2.0f;
    float plusarg1 = partZoomScale;
    float plusarg2 = ((_centerX + 1.0f) / 2.0f) / (plusarg1 == 3.0f ? 1.5f : (plusarg1 == 4.0f ? 1.325f : 2.0f));
    float plusarg3 = ((-_centerY + 1.0f) / 2.0f) / (plusarg1 == 3.0f ? 1.5f : (plusarg1 == 4.0f ? 1.325f : 2.0f));
    glUniform4f(cunstom_Params_uniform, arg1, arg2, arg3, arg4);
    glUniform3f(cunstom_Params_plus_uniform, plusarg1, plusarg2, plusarg3);
    glUniform1i(MagLineWifth_uniform, 1.0f);

    qDebug() << arg1<< arg2<< arg3<< arg4;
    qDebug() << plusarg1<< plusarg2<< plusarg3;

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();
    glBindVertexArray(0);
//    program->release();

//    time_end();
    glDisable(GL_BLEND);   // 关闭混合
}

//###############################################################################################
void CSOpenGLWedgetMag::time_begin()
{
    gettimeofday(&G_tv_begin,NULL);
}

void CSOpenGLWedgetMag::time_end()
{
    gettimeofday(&G_tv_end, NULL);
    double tmspan = (G_tv_end.tv_sec - G_tv_begin.tv_sec) * 1000.0 +
                    (G_tv_end.tv_usec - G_tv_begin.tv_usec) / 1000.0;
    qDebug() << "[sharpen]timespan(ms):" << tmspan;
}

void CSOpenGLWedgetMag::set_sharpenStr(int s)
{
    sharpenStr = s / 100.0f;
}

void CSOpenGLWedgetMag::savepicture()
{
    makeCurrent();
    memset(imageData, 0x00, save_wid*save_hei*3);
    glReadPixels(0,0,save_wid,save_hei,GL_RGB,GL_UNSIGNED_BYTE,imageData);
    qDebug() << "[false]save_wid, save_hei:" << save_wid << "," <<save_hei;
    QString filePath  = QFileDialog::getSaveFileName(this,QStringLiteral("保存图像"), ".jpg");
    if(filePath == "")
    {
        return;
    }
    QImage image(imageData,save_wid,save_hei, QImage::Format_RGB888);
    image = image.mirrored(false, true);
    image.save(filePath);
}
