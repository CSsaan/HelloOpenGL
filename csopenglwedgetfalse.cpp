#include "csopenglwedgetfalse.h"
#include <QDebug>
#include <QFileDialog>

#define A_VER 0
#define T_VER 1

void CSOpenGLWedgetFalse::checkGLRrror(const char *msg)
{
    int err = glGetError();
    if (err != GL_NO_ERROR)
        printf("err=%d(%s)\n", err, msg);
}

// ############################################################################################
void CSOpenGLWedgetFalse::myGetLocation()
{
    // 从shader获取loc
    texs_yuyvloc[0] = program->uniformLocation("tex_yuyv");
    checkGLRrror("[false]glGetUniformLocation(tex_yuyv)");
    frameWH_loc = program->uniformLocation("frameWH");
    checkGLRrror("[focus]glGetUniformLocation(frameWH)");
}

void CSOpenGLWedgetFalse::myUniformData()
{
    // uniform传值
    glUniform2f(frameWH_loc, width * 1.0f, height * 1.0f);
    checkGLRrror("[focus]glUniform2f(frameWH_loc)");
}

int CSOpenGLWedgetFalse::updateImage()
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

CSOpenGLWedgetFalse::CSOpenGLWedgetFalse(QWidget *parent) : QOpenGLWidget(parent)
{
    // 启动定时器
    connect(&m_timer, &QTimer::timeout, this,[&]()
    {
//        this->update();
        update();
    });
    m_timer.start(1);
}

CSOpenGLWedgetFalse::~CSOpenGLWedgetFalse()
{
    cleanup();
}

void CSOpenGLWedgetFalse::cleanup()
{
    qDebug() << "[false]close";
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

void CSOpenGLWedgetFalse::initializeGL()
{
    time_begin();
    qDebug() << "[false]###########################################初始化###########################################";
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    program = new QOpenGLShaderProgram;
    qDebug() << "[false]Load Vert FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Vertex, QDir::currentPath()+ QString("/Shaders/FALSE.vert"));
    qDebug() << "[false]Load Frag FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Fragment, QDir::currentPath()+ QString("/Shaders/FALSE.frag"));
    checkGLRrror("[false]program load shader");
    program->bindAttributeLocation("vertexIn", A_VER);
    program->bindAttributeLocation("textureIn", T_VER);
    qDebug() << "[false]link shader：" << program->link();
    qDebug() << "[false]bind shader：" << program->bind();
    checkGLRrror("[false]program link & bind");

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
    checkGLRrror("[false]Gen yuyv texture.");

    m_file.setFileName(fL_video_YUYV);
    if(!m_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "[false]打开失败！";
        return;
    }
    imageData = new unsigned char[save_wid*save_hei*3];
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
//    time_begin();
}

void CSOpenGLWedgetFalse::resizeGL(int w, int h)
{
    glViewport(0.0f, 0.0f, w, h);
    save_wid = w;
    save_hei = h;
    qDebug() << "[false]w,h:" << w << h;
}

void CSOpenGLWedgetFalse::paintGL()
{
    updateImage();

//    time_begin();

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
    program->release();

    time_end();
}

//###############################################################################################
void CSOpenGLWedgetFalse::time_begin()
{
    gettimeofday(&G_tv_begin,NULL);
}

void CSOpenGLWedgetFalse::time_end()
{
    gettimeofday(&G_tv_end, NULL);
    double tmspan = (G_tv_end.tv_sec - G_tv_begin.tv_sec) * 1000.0 +
                    (G_tv_end.tv_usec - G_tv_begin.tv_usec) / 1000.0;
    qDebug() << "[false]timespan(ms):" << tmspan;
}

void CSOpenGLWedgetFalse::savepicture()
{
    makeCurrent();
    memset(imageData, 0x00, save_wid*save_hei*3);
    glReadPixels(0,0,save_wid,save_hei,GL_RGB,GL_UNSIGNED_BYTE,imageData);
    qDebug() << "[false]save_wid, save_hei:" << save_wid << "," <<save_hei;
    QString filePath  = QFileDialog::getSaveFileName(this,tr("Open Image"), "aaa.jpg",tr("Image Files (*.png *.jpg *.bmp)"), 0, QFileDialog::DontUseNativeDialog);

    if(filePath == "")
    {
        return;
    }
    QImage image(imageData,save_wid,save_hei, QImage::Format_RGB888);
    image = image.mirrored(false, true);
    image.save(filePath);
     qDebug() << filePath;
}



// 左键、右键点击
void CSOpenGLWedgetFalse::mousePressEvent(QMouseEvent *event)
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
void CSOpenGLWedgetFalse::mouseMoveEvent(QMouseEvent *event)
{
    if(mouse_press){
        move(event->globalPos() - mousePoint);
        event->accept();
    }
}
// 松开
void CSOpenGLWedgetFalse::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_press = false;
    event->accept();
}

void CSOpenGLWedgetFalse::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0)
    {
        //放大
        resize(this->size().width() * 1.1, this->size().height() * 1.1);
        qDebug() << "max" << this->size().width() << this->size().height();
        event->accept();
    }
    else
    {
        //缩小
        resize(this->size().width() * 0.9, this->size().height() * 0.9);
        qDebug() << "min" << this->size().width() << this->size().height();
        event->accept();
    }
}
