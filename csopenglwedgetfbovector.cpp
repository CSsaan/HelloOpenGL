#include "csopenglwedgetfbovector.h"
#include <QDebug>
#include "stb_image.h"
#include <QFileDialog>
#include<QLabel>

#define A_VER 0
#define T_VER 1
#define FBO_IMG_SIZE 260
#define FBO_IMG_SIZE_H 260

void CSOpenGLWedgetFboVector::checkGLRrror(const char *msg)
{
    int err = glGetError();
    if (err != GL_NO_ERROR)
        printf("err=%d(%s)\n", err, msg);
}

// ############################################################################################
void CSOpenGLWedgetFboVector::myGetLocation()
{
    // 从shader获取loc
    texs_yuyvloc[0] = fbo_program->uniformLocation("tex_yuyv");
    checkGLRrror("[vector]glGetUniformLocation(tex_yuyv)");
    frameWH_loc = fbo_program->uniformLocation("frameWH");
    checkGLRrror("[focus]glGetUniformLocation(frameWH)");

    m_Fbo2DTextureIdloc[0] = program_map->uniformLocation("texs_fbo");
    checkGLRrror("[vector map]glGetUniformLocation(texs_fbo)");
    texs_maploc[0] = program_map->uniformLocation("texs_map");
    checkGLRrror("[vector map]glGetUniformLocation(texs_map)");
    alphabscope_loc = program_map->uniformLocation("alphabscope");
    checkGLRrror("[hist fbo]glGetUniformLocation(alphabscope)");

}

void CSOpenGLWedgetFboVector::myUniformData()
{
    // uniform传值
    glUniform2f(frameWH_loc, width * 1.0f, height * 1.0f);
    checkGLRrror("[focus]glUniform2f(frameWH_loc)");
}

CSOpenGLWedgetFboVector::CSOpenGLWedgetFboVector(QWidget *parent) : QOpenGLWidget(parent)
{
    connect(&m_timer, &QTimer::timeout, this,[&]()
    {
        this->update();
    });
    m_timer.start(1);
}

CSOpenGLWedgetFboVector::~CSOpenGLWedgetFboVector()
{
    cleanup();
}

void CSOpenGLWedgetFboVector::cleanup()
{
    qDebug() << "[vector]close";
    if(m_file.isOpen())
    {
        m_file.close();
    }
    makeCurrent();
    if(fbo_program != NULL)
    {
        delete fbo_program;
        fbo_program = NULL;
    }
    if(program_map != NULL)
    {
        delete program_map;
        program_map = NULL;
    }
    glDeleteBuffers(1, &FBOId);
    glDeleteVertexArrays(1,&FBO_vao);
    glDeleteBuffers(1,&FBO_vbo);
    glDeleteVertexArrays(1,&D2_vao);
    glDeleteBuffers(1,&D2_vbo);
    glDeleteTextures(1, m_Fbo2DTextureId);
    glDeleteTextures(1, texs_yuyvID);
    glDeleteTextures(1, texs_mapID);
    doneCurrent();
}

void CSOpenGLWedgetFboVector::initializeGL()
{
    qDebug() << "[vector]###########################################FBO 初始化###########################################";
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    // FBO
    fbo_program = new QOpenGLShaderProgram;
    qDebug() << "[vector fbo]Load Vert FIle：" <<fbo_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QDir::currentPath()+ QString("/Shaders/FBOVECTOR.vert"));
    qDebug() << "[vector fbo]Load Frag FIle：" <<fbo_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QDir::currentPath()+ QString("/Shaders/FBOVECTOR.frag"));
    checkGLRrror("[vector fbo]program load shader");
    fbo_program->bindAttributeLocation("vertexIn", A_VER);
    fbo_program->bindAttributeLocation("textureIn", T_VER);
    qDebug() << "[vector fbo]link shader：" << fbo_program->link();
    qDebug() << "[vector fbo]bind shader：" << fbo_program->bind();
    checkGLRrror("[vector fbo]program link & bind");
    // FBO顶点坐标数据
    float vertices[] = {
            -1.0f, -1.0f,0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f,  1.0f, 0.0f
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

    // 普通2D显示
    program_map = new QOpenGLShaderProgram;
    qDebug() << "[vector map]Load Vert FIle：" <<program_map->addShaderFromSourceFile(QOpenGLShader::Vertex, QDir::currentPath()+ QString("/Shaders/VERTORFBOMAP.vert"));
    qDebug() << "[vector map]Load Frag FIle：" <<program_map->addShaderFromSourceFile(QOpenGLShader::Fragment, QDir::currentPath()+ QString("/Shaders/VERTORFBOMAP.frag"));
    checkGLRrror("[vector map]program load shader");
    program_map->bindAttributeLocation("vertexIn", A_VER);
    program_map->bindAttributeLocation("textureIn", T_VER);
    qDebug() << "[vector map]link shader：" << program_map->link();
    qDebug() << "[vector map]bind shader：" << program_map->bind();
    checkGLRrror("[vector map]program link & bind");
    // 生成绑定vao和vbo数据
    glGenVertexArrays(1, &D2_vao);
    glBindVertexArray(D2_vao);
    glGenBuffers(1, &D2_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, D2_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    checkGLRrror("[vector map]vao_vbo_bind_init");
    glVertexAttribPointer(A_VER, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(A_VER);
    glVertexAttribPointer(T_VER, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(T_VER);
    checkGLRrror("vao_vbo_enable_init");
    program_map->release();

    myGetLocation();

    // 创建一个FBO绑定的纹理（fbo输出结果，用来普通2D显示）
    glGenTextures(1, m_Fbo2DTextureId);
    glBindTexture(GL_TEXTURE_2D, m_Fbo2DTextureId[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
    // 创建map纹理
    img = QImage(mapImagePath);
    img = img.convertToFormat(QImage::Format_RGB888);
    glGenTextures(1, texs_mapID);
    glBindTexture(GL_TEXTURE_2D, texs_mapID[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width(), img.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.bits());
    glBindTexture(GL_TEXTURE_2D, 0);
    checkGLRrror("[vector map]Gen yuyv texture.");

    // 初始化FBO
    glGenFramebuffers(1, &FBOId);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOId);
    glBindTexture(GL_TEXTURE_2D, m_Fbo2DTextureId[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Fbo2DTextureId[0], 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FBO_IMG_SIZE, FBO_IMG_SIZE_H, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!= GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "PBOSample::CreateFrameBufferObj glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE";
    }
    checkGLRrror("[vector map]CreateFrameBufferObj");
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

    m_file.setFileName(fL_video_YUYV);
    if(!m_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "[vector]打开失败！";
        return;
    }

    imageData = new unsigned char[save_wid * save_hei * 3];
    fboImageData = new unsigned char[FBO_IMG_SIZE * FBO_IMG_SIZE_H * 3];
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CSOpenGLWedgetFboVector::resizeGL(int w, int h)
{
    glViewport(0.0f, 0.0f, w, h);
    save_wid = w;
    save_hei = h;
    qDebug() << "[vector]w,h:" << w << h;
}

void CSOpenGLWedgetFboVector::paintGL()
{
    time_begin();

    if(m_file.atEnd())
    {
        m_file.seek(0);
    }
    QByteArray buf = m_file.read(width * height * 2);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //*********************************************************************************************
    // FBO
    glViewport(0.0, 0.0, (GLsizei)FBO_IMG_SIZE, (GLsizei)FBO_IMG_SIZE_H);
    fbo_program->bind();
    glBindVertexArray(FBO_vao);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOId);
    checkGLRrror("[vector map]glBindFramebuffer");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texs_yuyvID[0]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
    glUniform1i(texs_yuyvloc[0], 0);
    myUniformData();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // 保存FBO结果
    memset(fboImageData, 0x00, FBO_IMG_SIZE*FBO_IMG_SIZE_H*3);
    glReadPixels(0, 0, (GLsizei)FBO_IMG_SIZE, (GLsizei)FBO_IMG_SIZE_H, GLenum(GL_RGB), GLenum(GL_UNSIGNED_BYTE), fboImageData);

//    // 显示一下FBO的图片
//    QImage image(fboImageData,FBO_IMG_SIZE, FBO_IMG_SIZE_H, QImage::Format_RGB888);
//    QLabel* label=new QLabel(" ",0);
//    QPixmap mp;
//    mp = mp.fromImage(image);
//    label->setPixmap(mp);
//    label->show();

    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(0);

    //*********************************************************************************************
//    // test
//    img = QImage("./wt000023.jpg");
//    QSize picSize(200,200);
//    img = img.convertToFormat(QImage::Format_RGB888).scaled(picSize);

//    Vectorscope_Vertexs(img.bits(), img.width(), img.height());
//    // 保存图像
//    QString filePath  = QFileDialog::getSaveFileName(this,QStringLiteral("保存图像"), "vvv.jpg");
//    if(filePath == "")
//    {
//        return ;
//    }
//    QImage image(data_out,img.height(), img.width(), QImage::Format_RGB888);
//    image = image.mirrored(false, false);
//    image.save(filePath);
    //*********************************************************************************************
    glEnable(GL_BLEND);   // 启用混合
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_COLOR); // 保证全透明时，波形图存在
    // map
    glViewport(0.0, 0.0, save_wid, save_hei);
    program_map->bind();
    glBindVertexArray(D2_vao);
    Vectorscope_Vertexs(fboImageData, FBO_IMG_SIZE, FBO_IMG_SIZE_H); // data_out

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texs_mapID[0]);
    glUniform1i(texs_maploc[0], 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_Fbo2DTextureId[0]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, FBO_IMG_SIZE, FBO_IMG_SIZE_H, GL_RGB, GL_UNSIGNED_BYTE, data_out);
    glUniform1i(m_Fbo2DTextureIdloc[0], 1);

    glUniform1f(alphabscope_loc, (100 - alphabscope) / 100.0f); // 0-100
    checkGLRrror("[hist fbo]glUniform1f(alphabscope_loc)");

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(0);

//    time_end();
    glDisable(GL_BLEND);   // 关闭混合
}

//###############################################################################################
void CSOpenGLWedgetFboVector::time_begin()
{
    gettimeofday(&G_tv_begin,NULL);
}

void CSOpenGLWedgetFboVector::time_end()
{
    gettimeofday(&G_tv_end, NULL);
    double tmspan = (G_tv_end.tv_sec - G_tv_begin.tv_sec) * 1000.0 +
                    (G_tv_end.tv_usec - G_tv_begin.tv_usec) / 1000.0;
    qDebug() << "[vector]timespan(ms):" << tmspan;
}

void CSOpenGLWedgetFboVector::set_alphabscope(int s)
{
    alphabscope = s;
}

void CSOpenGLWedgetFboVector::Vectorscope_Vertexs(unsigned char* buf_yuyv, int img_width, int img_height)
{
    if (data_out == NULL)
    {
        data_out = (unsigned char*)malloc(FBO_IMG_SIZE*FBO_IMG_SIZE_H * sizeof(unsigned char) * 3);
    }
    vectorscope(buf_yuyv, data_out, img_width, img_height);
}

int CSOpenGLWedgetFboVector::getIndex(int x, int y, int width)
{
    return ((y * width) + x) * 3; 
}
float *CSOpenGLWedgetFboVector::RGBtoUV(float R, float G, float B)
{
    static float result[2];
    result[0] = (R * (-0.168736)) + (G * (-0.331264)) + (B * 0.500000) + 128; // 0 - 255
    result[1] = (R * 0.500000) + (G * (-0.418688)) + (B * (-0.081312)) + 128; // 0 - 255
    return result;
}
float CSOpenGLWedgetFboVector::normalize(float input)
{
    return input / 255.0f;
}
float CSOpenGLWedgetFboVector::getUpdatedColor(float pixel_cur, float pixel_new, float divider, float constant)
{
    float result = pixel_cur + (constant * pixel_new / divider);
    if (result > 255.0f)
        return 255.0f;
    return result;
}
void CSOpenGLWedgetFboVector::vectorscope(unsigned char *data_in, unsigned char *data_out, int width, int height)
{
    int index = 0;;
    float x = 0.0f, y = 0.0f;
    float R = 0.0f, G = 0.0f, B = 0.0f;
    float U = 0.0f, V = 0.0f;
    memset(data_out, 0x00, FBO_IMG_SIZE*FBO_IMG_SIZE_H*3);
    for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < height; h++)
        {
//            index = getIndex(w, h, width);
            index = (h * width + w) * 3;
            R = (float)data_in[index];
            G = (float)data_in[index + 1];
            B = (float)data_in[index + 2];
            // get UV
            float *uvVal = RGBtoUV(R, G, B);
            float sizeRate = 0.85f; // 0.0-1.0
            U = uvVal[0] * sizeRate + 255.0f * (1.0f - sizeRate) * 0.5f;
            V = uvVal[1] * sizeRate + 255.0f * (1.0f - sizeRate) * 0.5f;
            // convert UV to XY
            x = normalize(U) * (FBO_IMG_SIZE - 1);
            y = (FBO_IMG_SIZE_H - 1) - (normalize(V) * (FBO_IMG_SIZE_H - 1));
            // calculate resulting pixel brightness
//            index = getIndex((int)round(x), (int)round(y), height);
            index = (int)(round(x) + round(y) * height) * 3;
            // Luma
            float result = data_out[index + 1] + 16.0f * 256.0f / FBO_IMG_SIZE_H;
//            float result = data_out[index + 1] + 16.0f * (R*0.299f + G*0.587f + B*0.114f) / height;
            if (result > 255.0f)
                result = 255.0f;
            data_out[index] = (unsigned char)result;
            data_out[index + 1] = (unsigned char)result;
            data_out[index + 2] = (unsigned char)result;
//            // RGB Color
//            data_out[index] = getUpdatedColor((float)data_out[index], R, (float)height, 16.0f);
//            data_out[index+1] = getUpdatedColor((float)data_out[index+1], G, (float)height, 16.0f);
//            data_out[index+2] = getUpdatedColor((float)data_out[index+2], B, (float)height, 16.0f);
        }
    }
}

void CSOpenGLWedgetFboVector::savepicture()
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
