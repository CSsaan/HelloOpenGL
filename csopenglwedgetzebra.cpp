#include "csopenglwedgetzebra.h"
#include <QDebug>
#include <QFileDialog>
//#include "stb_image.h"

#define A_VER 0
#define T_VER 1

void CSOpenGLWedgetZebra::checkGLRrror(const char *msg)
{
    int err = glGetError();
    if (err != GL_NO_ERROR)
        printf("err=%d(%s)\n", err, msg);
}

// ############################################################################################
void CSOpenGLWedgetZebra::myGetLocation()
{
    texs_yuyvloc[0] = program->uniformLocation("tex_yuyv");
    checkGLRrror("[zebra]glGetUniformLocation(tex_yuyv)");
    texs_zebraloc[0] = program->uniformLocation("texs_zebra");
    checkGLRrror("[zebra]glGetUniformLocation(texs_zebra)");
    zebraLow_loc = program->uniformLocation("zebraLow");
    checkGLRrror("[zebra]glGetUniformLocation(zebraLow)");
    zebraHig_loc = program->uniformLocation("zebraHig");
    checkGLRrror("[zebra]glGetUniformLocation(zebraHig)");
    iGlobalTime_loc = program->uniformLocation("iGlobalTime");
    checkGLRrror("[zebra]glGetUniformLocation(iGlobalTime)");
    frameWH_loc = program->uniformLocation("frameWH");
    checkGLRrror("[focus]glGetUniformLocation(frameWH)");
}

void CSOpenGLWedgetZebra::myUniformData()
{
    glUniform1f(zebraLow_loc, zebraLow / 100.0f);
    checkGLRrror("[zebra]glUniform1f(zebraLow_loc)");
    glUniform1f(zebraHig_loc, zebraHig / 100.0f);
    checkGLRrror("[zebra]glUniform1f(zebraHig_loc)");
    glUniform1f(iGlobalTime_loc, (float)clock() / CLOCKS_PER_SEC * 2.5);
    checkGLRrror("[zebra]glUniform1f(iGlobalTime_loc)");
    glUniform2f(frameWH_loc, width * 1.0f, height * 1.0f);
    checkGLRrror("[focus]glUniform2f(frameWH_loc)");
}

CSOpenGLWedgetZebra::CSOpenGLWedgetZebra(QWidget *parent) : QOpenGLWidget(parent)
{
    connect(&m_timer, &QTimer::timeout, this,[&]()
    {
        this->update();
    });
    m_timer.start(1);
}

CSOpenGLWedgetZebra::~CSOpenGLWedgetZebra()
{
    cleanup();
}

void CSOpenGLWedgetZebra::cleanup()
{
    qDebug() << "[zebra]close";
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
    glDeleteTextures(1, texs_zebraID);
    doneCurrent();
}

void CSOpenGLWedgetZebra::initializeGL()
{
    time_begin();
    qDebug() << "[zebra]###########################################初始化###########################################";
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    program = new QOpenGLShaderProgram;
    qDebug() << "[zebra]Load Vert FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Vertex, QDir::currentPath()+ QString("/Shaders/ZEBRA.vert"));
    qDebug() << "[zebra]Load Frag FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Fragment, QDir::currentPath()+ QString("/Shaders/ZEBRA.frag"));
    checkGLRrror("[zebra]program load shade");
    program->bindAttributeLocation("vertexIn", A_VER);
    program->bindAttributeLocation("textureIn", T_VER);
    qDebug() << "[zebra]link shader：" << program->link();
    qDebug() << "[zebra]bind shader：" << program->bind();
    checkGLRrror("[zebra]program link & bind");

//    static const GLfloat ver[] = {
//        -1.0f, -1.0f,
//        1.0f, -1.0f,
//        -1.0f, 1.0f,
//        -1.0f, 1.0f,
//        1.0f, -1.0f,
//        1.0f, 1.0f,
//    };
//    static const GLfloat tex[] = {
//        0.0f, 1.0f,
//        1.0f, 1.0f,
//        0.0f, 0.0f,
//        0.0f, 0.0f,
//        1.0f, 1.0f,
//        1.0f, 0.0f,
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
    checkGLRrror("[zebra]Gen lut texture.");
    // 创建zebra
    QImage img = QImage(zebraImagePath);
    img = img.convertToFormat(QImage::Format_RGB888);
    glGenTextures(1, texs_zebraID);
    glBindTexture(GL_TEXTURE_2D, texs_zebraID[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width(), img.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.bits());
    glBindTexture(GL_TEXTURE_2D, 0);
    checkGLRrror("[zebra]Gen yuyv texture.");

    m_file.setFileName(fL_video_YUYV);
    if(!m_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "[zebra]打开失败！";
        return;
    }
    imageData = new unsigned char[save_wid*save_hei*3];
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CSOpenGLWedgetZebra::resizeGL(int w, int h)
{
    glViewport(0.0f, 0.0f, w, h);
    save_wid = w;
    save_hei = h;
    qDebug() << "[zebra]w,h:" << w << h;
}

void CSOpenGLWedgetZebra::paintGL()
{
//    time_begin();

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
    glBindTexture(GL_TEXTURE_2D, texs_zebraID[0]);
    glUniform1i(texs_zebraloc[0], 1);

    myUniformData();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();
    glBindVertexArray(0);
//    program->release();

//    time_end();
}

//###############################################################################################
void CSOpenGLWedgetZebra::time_begin()
{
    gettimeofday(&G_tv_begin,NULL);
}

void CSOpenGLWedgetZebra::time_end()
{
    gettimeofday(&G_tv_end, NULL);
    double tmspan = (G_tv_end.tv_sec - G_tv_begin.tv_sec) * 1000.0 +
                    (G_tv_end.tv_usec - G_tv_begin.tv_usec) / 1000.0;
    qDebug() << "[zebra]timespan(ms):" << tmspan;
}

void CSOpenGLWedgetZebra::set_zebraLow(int s)
{
    zebraLow = s * 1.0f;
}
void CSOpenGLWedgetZebra::set_zebraHig(int s)
{
    zebraHig = s * 1.0f;
}

void CSOpenGLWedgetZebra::savepicture()
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
