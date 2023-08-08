#include "csopenglwedgetwave.h"
#include <QDebug>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <QFileDialog>

#define GET_STR(x) #x
#define A_VER 0
#define T_VER 1

void CSOpenGLWedgetWave::checkGLRrror(const char *msg)
{
    int err = glGetError();
    if (err != GL_NO_ERROR)
        printf("err=%d(%s)\n", err, msg);
}

// ############################################################################################
void CSOpenGLWedgetWave::myGetLocation()
{
    texs_yuyvloc[0] = program->uniformLocation("tex_yuyv");
    checkGLRrror("[wave]glGetUniformLocation(tex_yuyv)");
    texs_tickloc[0] = program->uniformLocation("Tick_sampler");
    checkGLRrror("[wave]glGetUniformLocation(Tick_sampler)");
    useWaveform_loc = program->uniformLocation("useWaveform");
    checkGLRrror("[wave]glGetUniformLocation(useWaveform)");
    texs_yuyvloc[0] = program->uniformLocation("tex_yuyv");
    checkGLRrror("[wave]glGetUniformLocation(tex_yuyv)");
    alphabscope_loc = program->uniformLocation("alphabscope");
    checkGLRrror("[wave]glGetUniformLocation(alphabscope)");
    frameWH_loc = program->uniformLocation("frameWH");
    checkGLRrror("[wave]glGetUniformLocation(frameWH)");
}

void CSOpenGLWedgetWave::myUniformData()
{
    glUniform2f(frameWH_loc, width * 1.0f, height * 1.0f);
    checkGLRrror("[wave]glUniform2f(frameWH_loc)");
    glUniform1f(alphabscope_loc, (100 - alphabscope) / 100.0f); // 0-100
    checkGLRrror("[wave]glUniform1f(alphabscope_loc)");
    glUniform1i(useWaveform_loc, useWaveform); // 1:Luma,2:RGB
    checkGLRrror("[wave]glUniform1f(useWaveform_loc)");
}

CSOpenGLWedgetWave::CSOpenGLWedgetWave(QWidget *parent) : QOpenGLWidget(parent)
{
    connect(&m_timer, &QTimer::timeout, this,[&]()
    {
        this->update();
    });
    m_timer.start(1);
}

CSOpenGLWedgetWave::~CSOpenGLWedgetWave()
{
    cleanup();
}

void CSOpenGLWedgetWave::cleanup()
{
    qDebug() << "[wave]close";
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
    glDeleteTextures(1, texs_tickID);
    doneCurrent();
}

void CSOpenGLWedgetWave::initializeGL()
{
//    time_begin();
    qDebug() << "[wave]###########################################初始化###########################################";
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    program = new QOpenGLShaderProgram;
    qDebug() << "[wave]Load Vert FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Vertex, QDir::currentPath()+ QString("/Shaders/WAVE.vert"));
    qDebug() << "[wave]Load Frag FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Fragment, QDir::currentPath()+ QString("/Shaders/WAVE.frag"));
    checkGLRrror("[wave]program load shader");
    program->bindAttributeLocation("vertexIn", A_VER);
    program->bindAttributeLocation("textureIn", T_VER);
    qDebug() << "[wave]link shader：" << program->link();
    qDebug() << "[wave]bind shader：" << program->bind();
    checkGLRrror("[wave]program link & bind");

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
    checkGLRrror("[wave]Gen lut texture.");
    // 创建Tick
    QImage img = QImage(tickImagePath);
    img = img.convertToFormat(QImage::Format_Grayscale8);
    glGenTextures(1, texs_tickID);
    glBindTexture(GL_TEXTURE_2D, texs_tickID[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, img.width(), img.height(), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, img.bits());
    glBindTexture(GL_TEXTURE_2D, 0);
    checkGLRrror("[wave]Gen tick texture.");

    m_file.setFileName(fL_video_YUYV);
    if(!m_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "[wave]打开失败！";
        return;
    }
    imageData = new unsigned char[save_wid*save_hei*3];
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CSOpenGLWedgetWave::resizeGL(int w, int h)
{
    glViewport(0.0f, 0.0f, w, h);
    save_wid = w;
    save_hei = h;
    qDebug() << "[wave]w,h:" << w << h;
}

void CSOpenGLWedgetWave::paintGL()
{
    time_begin();
    glEnable(GL_BLEND);   // 启用混合
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_COLOR); // 保证全透明时，波形图存在

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

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texs_tickID[0]);
    glUniform1i(texs_tickloc[0], 1);

    myUniformData();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();
    glBindVertexArray(0);
//    program->release();

    time_end();
    glDisable(GL_BLEND);
}

//###############################################################################################
void CSOpenGLWedgetWave::time_begin()
{
    gettimeofday(&G_tv_begin,NULL);
}

void CSOpenGLWedgetWave::time_end()
{
    gettimeofday(&G_tv_end, NULL);
    double tmspan = (G_tv_end.tv_sec - G_tv_begin.tv_sec) * 1000.0 +
                    (G_tv_end.tv_usec - G_tv_begin.tv_usec) / 1000.0;
    qDebug() << "[wave]timespan(ms):" << tmspan;
}

void CSOpenGLWedgetWave::set_alphabscope(int s)
{
    alphabscope = s;
}
void CSOpenGLWedgetWave::set_useWaveform(int s)
{
    useWaveform = s;
}

void CSOpenGLWedgetWave::savepicture()
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
