#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include "TriangleShader.h"
#include "DeferredShader.h"
#include "OpenGLMatrixManager.h"
#include "Surface.h"

extern "C" {
#include "image.h"
}

#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iup/iup.h>
#include <iup/iupgl.h>

class MainWindow {
public:
    /**
     * Construtor default da classe.
     */
    MainWindow();

    /**
     * Destrutor da classe.
     */
    virtual ~MainWindow();

    /**
     * Exibe a janela.
     */
    void show();

    /**
     * Oculta a janela.
     */
    void hide();




private:
    Ihandle* _dialog;

    int _width;
    int _height;

    // Matriz de model
    OpenGLMatrixManager _modelMatrix;

    // Matriz de View
    OpenGLMatrixManager _viewMatrix;

    // Matriz de View
    OpenGLMatrixManager _modelViewMatrix;

    //Matriz de projection
    OpenGLMatrixManager _projectionMatrix;

    //Superficies a serem desenhadas
    std::vector< Surface > _surface;

    double _eye[3];

    GLuint _deferredFBO;
    GLuint _quad_vertexbuffer;

    //Texturas
    Image* _textureImg[ 2 ];
    GLuint _textureId[ 2 ];


    GLuint _dsTextureId[ 5 ];

    DeferredShader* _deferredShader;

    std::vector < float > _lightPosition;
    std::vector < float > _lightDifuse;
    std::vector < float > _lightSpecular;
    std::vector < float > _lightAmbient;

    unsigned int _nLight;


private:

    /**
     * Cria janela da IUP e define suas configuracoes e callbacks.
     */
    void createWindow();

    /**
     * Incializa algumas propriedades do canvas OpenGL.
     */
    void initializeCanvas();

    /**
     * Realiza o redesenho da cena OpenGL.
     */
    void drawScene();

    /**
     * Trata evento de redimensionar o canvas OpenGL.
     * @param width - nova larguda do canvas.
     * @param height - nova altura do canvas.
     */
    void resizeCanvas(int width, int height);

    void createGBufferTex(GLenum texUnit, GLenum format, GLuint &texId);

    void initDeferredShader();
    void initBumpMapTexture();
    void bindBumpMapTextures();
    void bindDeferredShaderTextures();
    void renderQuad();


private:
    /**
     * Callback do botao de fechar a janela.
     * @param button - ihandle do botao de sair.
     * @return - retorna IUP_CLOSE para que a janela seja fechada.
     */
    static int exitButtonCallback(Ihandle* button);

    /**
     * Callback responsavel por receber evento de redesenho do canvas.
     * @param canvas - ponteiro para o canvas.
     * @return  - IUP_DEFAULT.
     */
    static int actionCanvasCallback(Ihandle* canvas);

    /**
     * Callback responsavel por receber eventos do whell do mouse no canvas para
     * realizar a operacao de Zoom.
     * @param canvas - ponteiro para o canvas.
     * @param delta - vale -1 ou 1 e indica a direcao da rotacao do botao whell.
     * @param x - posicao x do mouse na tela.
     * @param y - posicao y do mouse na tela.
     * @param status - status dos botoes do mouse e certas teclas do teclado no
     * momento que o evento foi gerado.
     * @return - IUP_DEFAULT.
     */
    static int wheelCanvasCallback(Ihandle* canvas, float delta, int x,
            int y, char* status);

    /**
     * Callback responsavel por receber eventos de resize do canvas.
     * @param canvas - ponteiro para o canvas.
     * @param width - nova largura, em pixeis, da janela.
     * @param heigth - nova altura, em pixeis, da janela.
     * @return - IUP_DEFAULT.
     */
    static int resizeCanvasCallback(Ihandle* canvas, int width, int height);

    /**
     * Callback responsavel por receber eventos de teclado do canvas.
     * @param canvas - ponteiro para o canvas.
     * @param button - identificador do botao, podem ser BUTTON_1, BUTTON_2, ...
     * @param pressed - 1 para o caso do botao esta sendo pressionado e 0 caso
     * contrario.
     * @param x - posicao x do mouse na tela.
     * @param y - posicao y do mouse na tela.
     * @param status - status dos botoes do mouse e certas teclas do teclado no
     * momento que o evento foi gerado.
     * @return - IUP_DEFAULT.
     */
    static int buttonCanvasCallback(Ihandle* canvas, int button, int pressed,
            int x, int y, char* status);

    static int timer_cb(Ihandle *n);
};

#endif  /* IUPGLCANVASDUMMY_H */

