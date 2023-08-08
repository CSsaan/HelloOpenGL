#include "csopenglwedgetfocus.h"
#include <QDebug>
#include <QFileDialog>

#define A_VER 0
#define T_VER 1

void CSOpenGLWedgetFocus::checkGLRrror(const char *msg)
{
    int err = glGetError();
    if (err != GL_NO_ERROR)
        printf("err=%d(%s)\n", err, msg);
}

// ############################################################################################
void CSOpenGLWedgetFocus::myGetLocation()
{
    // 从shader获取loc
    texs_yuyvloc[0] = program->uniformLocation("tex_yuyv");
    checkGLRrror("[focus]glGetUniformLocation(tex_yuyv)");
    crgb_loc = program->uniformLocation("crgb");
    checkGLRrror("[focus]glGetUniformLocation(crgb)");
    sensitivity_loc = program->uniformLocation("sensitivity");
    checkGLRrror("[focus]glGetUniformLocation(sensitivity)");
    highThreshold_loc = program->uniformLocation("highThreshold");
    checkGLRrror("[focus]glGetUniformLocation(highThreshold)");
    frameWH_loc = program->uniformLocation("frameWH");
    checkGLRrror("[focus]glGetUniformLocation(frameWH)");
}

void CSOpenGLWedgetFocus::myUniformData()
{
    // uniform传值
    glUniform3f(crgb_loc,((crgb >> 16) & 0xFF) / 255.0f, ((crgb >> 8) & 0xFF) / 255.0f, (crgb & 0xFF) / 255.0f);
    checkGLRrror("[focus]glUniform3f(crgb_loc)");
    glUniform1f(sensitivity_loc, (1.0 - sensitivity) * 0.8);
    checkGLRrror("[focus]glUniform1f(sensitivity_loc)");
    glUniform1f(highThreshold_loc, 1.0 - highThreshold);
    checkGLRrror("[focus]glUniform1f(highThreshold_loc)");
    glUniform2f(frameWH_loc, width * 1.0f, height * 1.0f);
    checkGLRrror("[focus]glUniform2f(frameWH_loc)");
}

CSOpenGLWedgetFocus::CSOpenGLWedgetFocus(QWidget *parent) : QOpenGLWidget(parent)
{
    connect(&m_timer, &QTimer::timeout, this,[&]()
    {
        this->update();
    });
    m_timer.start(1);
}

CSOpenGLWedgetFocus::~CSOpenGLWedgetFocus()
{
    cleanup();
}

void CSOpenGLWedgetFocus::cleanup()
{
    qDebug() << "[focus]close";
    makeCurrent();
    if(m_file.isOpen())
    {
        m_file.close();
    }
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

void CSOpenGLWedgetFocus::initializeGL()
{
    qDebug() << "[focus]###########################################初始化###########################################";
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    program = new QOpenGLShaderProgram;
    qDebug() << "[focus]Load Vert FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Vertex, QDir::currentPath()+ QString("/Shaders/FOCUS.vert"));
    qDebug() << "[focus]Load Frag FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Fragment, QDir::currentPath()+ QString("/Shaders/FOCUS.frag"));
    checkGLRrror("[focus]program load shader");
    program->bindAttributeLocation("vertexIn", A_VER);
    program->bindAttributeLocation("textureIn", T_VER);
    qDebug() << "[focus]link shader：" << program->link();
    qDebug() << "[focus]bind shader：" << program->bind();
    checkGLRrror("[focus]program link & bind");

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
    checkGLRrror("[focus]Gen yuyv texture.");

    m_file.setFileName(fL_video_YUYV);
    if(!m_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "[focus]打开失败！";
        return;
    }
    imageData = new unsigned char[save_wid*save_hei*3];
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    time_begin();
}

void CSOpenGLWedgetFocus::resizeGL(int w, int h)
{
    glViewport(0.0f, 0.0f, w, h);
    save_wid = w;
    save_hei = h;
    qDebug() << "[focus]w,h:" << w << h;
}

void CSOpenGLWedgetFocus::paintGL()
{
//    time_begin();
    glEnable(GL_BLEND);   // 启用混合
    glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA); // 保证全透明时，波形图存在

    if(m_file.atEnd())
    {
        m_file.seek(0);
    }
    QByteArray buf = m_file.read(width * height * 2);

//    time_end();
//    qDebug() << "[focus]file" ;

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

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();
    glBindVertexArray(0);
//    program->release();

    time_end();
//    qDebug() << "[focus]end" ;
    glDisable(GL_BLEND);   // 关闭混合
}

//###############################################################################################
void CSOpenGLWedgetFocus::time_begin()
{
    gettimeofday(&G_tv_begin,NULL);
}

void CSOpenGLWedgetFocus::time_end()
{
    gettimeofday(&G_tv_end, NULL);
    double tmspan = (G_tv_end.tv_sec - G_tv_begin.tv_sec) * 1000.0 +
                    (G_tv_end.tv_usec - G_tv_begin.tv_usec) / 1000.0;
    qDebug() << "[focus]timespan(ms):" << tmspan;
}

void CSOpenGLWedgetFocus::set_sensitivity(int s)
{
    sensitivity = s / 100.0f;
}
void CSOpenGLWedgetFocus::set_highThreshold(int s)
{
    highThreshold = s / 100.0f;
}

void CSOpenGLWedgetFocus::savepicture()
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

// 左键、右键点击
void CSOpenGLWedgetFocus::mousePressEvent(QMouseEvent *event)
{
    if( (event->button() == Qt::LeftButton) ) // 左键拖动
    {
        mouse_press = true;
        mousePoint = event->globalPos() - this->pos();
    }
    else if(event->button() == Qt::RightButton) // 右键关闭
    {
        //如果是右键
        this->close();
        this->cleanup();

    }
    event->accept();
}
// 按住
void CSOpenGLWedgetFocus::mouseMoveEvent(QMouseEvent *event)
{
    if(mouse_press){
        move(event->globalPos() - mousePoint);
        event->accept();
    }
}
// 松开
void CSOpenGLWedgetFocus::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_press = false;
    event->accept();
}

