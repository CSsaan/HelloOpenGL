#include "csopenglwedgetlut.h"
#include <QDebug>
#include <math.h>
#include <iostream>
using namespace std;
#include <QFileDialog>

#define A_VER 0
#define T_VER 1

void CSOpenGLWedgetLut::checkGLRrror(const char *msg)
{
    int err = glGetError();
    if (err != GL_NO_ERROR)
        printf("err=%d(%s)\n", err, msg);
}

// ############################################################################################
void CSOpenGLWedgetLut::myGetLocation()
{
    texs_yuyvloc[0] = program->uniformLocation("tex_yuyv");
    checkGLRrror("[lut]glGetUniformLocation(tex_yuyv)");
    texs_lutloc[0] = program->uniformLocation("_lutTexture");
    checkGLRrror("[lut]glGetUniformLocation(_lutTexture)");

//    frameWH_loc = program->uniformLocation("frameWH");
//    checkGLRrror("[focus]glGetUniformLocation(frameWH)");
    size_lut_loc = program->uniformLocation("size_lut");
    checkGLRrror("[zebra]glGetUniformLocation(size_lut)");
    qsz_lut_loc = program->uniformLocation("qsz_lut");
    checkGLRrror("[zebra]glGetUniformLocation(qsz_lut)");
}

void CSOpenGLWedgetLut::myUniformData()
{
//    glUniform2f(frameWH_loc, width * 1.0f, height * 1.0f);
//    checkGLRrror("[focus]glUniform2f(frameWH_loc)");
    glUniform1f(size_lut_loc, lut_size * 1.0f);
    checkGLRrror("[zebra]glUniform1f(size_lut_loc)");
    glUniform1f(qsz_lut_loc, qSZ * 1.0f);
    checkGLRrror("[zebra]glUniform1f(qsz_lut_loc)");
}

CSOpenGLWedgetLut::CSOpenGLWedgetLut(QWidget *parent) : QOpenGLWidget(parent)
{
    connect(&m_timer, &QTimer::timeout, this,[&]()
    {
        this->update();
    });
    m_timer.start(1);
}

void CSOpenGLWedgetLut::initializeGL()
{
    qDebug() << "[lut]###########################################初始化###########################################";
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    program = new QOpenGLShaderProgram;
    qDebug() << "[lut]Load Vert FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Vertex, QDir::currentPath()+ QString("/Shaders/LUT.vert"));
    qDebug() << "[lut]Load Frag FIle：" <<program->addShaderFromSourceFile(QOpenGLShader::Fragment, QDir::currentPath()+ QString("/Shaders/LUT.frag"));
    checkGLRrror("[lut]program load shader");
    program->bindAttributeLocation("vertexIn", A_VER);
    program->bindAttributeLocation("textureIn", T_VER);
    qDebug() << "[lut]link shader：" << program->link();
    qDebug() << "[lut]bind shader：" << program->bind();
    checkGLRrror("[lut]program link & bind");

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
    checkGLRrror("[lut]Gen yuyv texture");
    // 创建lut
    load_3dlut_file_to_picture();
    CreateLutTexture2D();
    glGenTextures(1, texs_lutID);
    glBindTexture(GL_TEXTURE_2D, texs_lutID[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lut_size * qSZ, lut_size * qSZ, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    glBindTexture(GL_TEXTURE_2D, 0);
    checkGLRrror("[lut]Gen lut texture.");

    m_file.setFileName(fL_video_YUYV);
    if(!m_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "[lut]打开失败！";
        return;
    }
    imageData = new unsigned char[save_wid*save_hei*3];
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    time_begin();
}

void CSOpenGLWedgetLut::resizeGL(int w, int h)
{
    glViewport(0.0f, 0.0f, w, h);
    save_wid = w;
    save_hei = h;
    qDebug() << "[lut]w,h:" << w << h;
}

void CSOpenGLWedgetLut::paintGL()
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
    glBindTexture(GL_TEXTURE_2D, texs_lutID[0]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lut_size * qSZ, lut_size * qSZ, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    glUniform1i(texs_lutloc[0], 1);

    myUniformData();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();
    glBindVertexArray(VAO);
//    program->release();

    time_end();
}

CSOpenGLWedgetLut::~CSOpenGLWedgetLut()
{
    cleanup();
}

void CSOpenGLWedgetLut::cleanup()
{
    qDebug() << "[lut]close";
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
    glDeleteTextures(1, texs_yuyvID);
    glDeleteTextures(1, texs_lutID);
    if(lut_table != NULL)
    {
        free(lut_table);
        lut_table = NULL;
    }
    if(bitmap != NULL)
    {
        free(bitmap);
        bitmap = NULL;
    }
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    doneCurrent();
}

//###############################################################################################
void CSOpenGLWedgetLut::time_begin()
{
    gettimeofday(&G_tv_begin,NULL);
}

void CSOpenGLWedgetLut::time_end()
{
    gettimeofday(&G_tv_end, NULL);
    double tmspan = (G_tv_end.tv_sec - G_tv_begin.tv_sec) * 1000.0 +
                    (G_tv_end.tv_usec - G_tv_begin.tv_usec) / 1000.0;
    qDebug() << "[lut]timespan(ms):" << tmspan;
}

#include<regex>
bool CSOpenGLWedgetLut::matchRegex(const char* pattern, const char* userString)
{
    bool result = false;
    regex str_expr(pattern);
    if (regex_match(userString, str_expr))
    {
        result = true;
    }
    return result;
}

int CSOpenGLWedgetLut::load_3dlut_file_to_picture()
{
    FILE* fp = fopen((QDir::currentPath() + QString(lutImagePath)).toStdString().c_str(), "r");
    if (fp == NULL)
    {
        qDebug() << "can not open .CUBE file!";
        return -1;
    }
    char* buf = (char*)malloc(1024 * sizeof(char));
    int lines = 0;
    int LUT_3D_SIZE = 256;
    char typd[64];
    int nd = 0;
    const char* pattern = "^(-?[0-9]\\d*\\.\\d*\\s*){3}";
    while (fgets(buf, 1024, fp) != NULL)
    {
        if (buf[0] == '#')
            continue;
        for (unsigned int z = 0; z < strlen(buf); z++)
        {
            if (buf[z] == '\n' || buf[z] == '\t')
                buf[z] = ' ';
        }
        if (++lines < 10)
        {
            if (0 == matchRegex(pattern, buf))
            {
                char* token = strtok(buf, " ");
                if (token && strcmp(token, "LUT_3D_SIZE") == 0)
                {
                    token = strtok(NULL, " ");
                    LUT_3D_SIZE = atoi(token);
                    lut_table = (float*)malloc(LUT_3D_SIZE * LUT_3D_SIZE * LUT_3D_SIZE * 3 * sizeof(float));
                }
                if (token && strcmp(token, "#") == 0)
                {
                    token = strtok(NULL, " ");
                    if (token && strcmp(token, "LUT_ORDER") == 0)
                    {
                        token = strtok(NULL, " ");
                        strcpy(typd, token);
                    }
                }
                continue;
            }
        }
        char* token = strtok(buf, " ");
        float fvals[3] = { 0, 0, 0 };
        int nfval = 0;
        while (token != NULL)
        {
            double fval = atof(token);
            token = strtok(NULL, " ");
            fvals[nfval++] = (float)fval;
        }
        if (nfval == 3)
        {
            lut_table[nd++] = (fvals[0]);
            lut_table[nd++] = (fvals[1]);
            lut_table[nd++] = (fvals[2]);
        }
        else
            qDebug() << "Fail to parse " << buf << " to three float.";
    }
    lut_size = LUT_3D_SIZE;
    assert(nd == (LUT_3D_SIZE * LUT_3D_SIZE * LUT_3D_SIZE * 3));
    qDebug() << "LUT_SIZE:" << LUT_3D_SIZE << ", nd:" << nd << ", lines:" << lines;
    free(buf);
    buf = NULL;
    fclose(fp);
    return 0;
}

int CSOpenGLWedgetLut::CreateLutTexture2D()
{
    int qsz = 2;
    while (qsz * qsz < lut_size)
    {
        qsz++;
    }
    qDebug() << "qsz:" << qsz <<", lut_size:" << lut_size;
    qSZ = qsz;
    bitmap = (unsigned char*)malloc((lut_size * qsz) * (lut_size * qsz) * 4 * sizeof(unsigned char));
    for (int qj = 0; qj < qsz; qj++)
    {
        for (int qi = 0; qi < qsz; qi++)
        {
            int offx = (lut_size)*qi;
            int offy = (lut_size)*qj;
            int blu = qj * qsz + qi;
            if (blu < lut_size)
            {
                for (int y = 0; y < lut_size; y++)
                {
                    for (int x = 0; x < lut_size; x++)
                    {
                        int red = x;
                        int gre = y;
                        int pz = blu * lut_size * lut_size + gre * lut_size + red;
                        float r = lut_table[3 * pz + 0];
                        float g = lut_table[3 * pz + 1];
                        float b = lut_table[3 * pz + 2];
                        int nd = (offy + y) * lut_size * qsz + (offx + x);
                        bitmap[4 * nd + 0] = (unsigned char)(fabs(r) * 255.0f);
                        bitmap[4 * nd + 1] = (unsigned char)(fabs(g) * 255.0f);
                        bitmap[4 * nd + 2] = (unsigned char)(fabs(b) * 255.0f);
                        bitmap[4 * nd + 3] = (unsigned char)(255.0f);
                    }
                }
            }
        }
    }
    return 0;
}

void CSOpenGLWedgetLut::set_lutImagePath(const char *Path)
{
    lutImagePath = Path;
    qDebug() << lutImagePath;
    free(lut_table);
    lut_table = NULL;
    free(bitmap);
    bitmap = NULL;
    load_3dlut_file_to_picture();
    CreateLutTexture2D();
}

void CSOpenGLWedgetLut::savepicture()
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

