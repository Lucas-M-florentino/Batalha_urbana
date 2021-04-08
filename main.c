/*****************************************************************************
 *  Universidade Federal da Grande Dourados - UFGD                           *
 *  Trabalho 1 da disciplina de Computa��o Gr�fica - Batalha Urbana          *
 *  Caio Fernandes Lima, Lucas Matheus de Moraes Florentino                  *
 *****************************************************************************/

#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define PI 3.141592
#define WIDTH 600.0
#define HEIGHT 600.0
#define WIDTH_TIJOLO 3.0
#define HEIGHT_TIJOLO (WIDTH_TIJOLO/2)
#define ORTO_X 30.0
#define ORTO_Y 30.0
#define tx 5.0
#define ty 1.5
#define TAM_CONTAINER_X 7.0
#define TAM_CONTAINER_Y 2.0
#define TAM_BARRA_X 5.0
#define TAM_BARRA_Y 0.6

float translate_slider = 0;
float translate_slider2 = 0;

float atualPosicaoSlider1_X =  - TAM_BARRA_X / 2.0;
float atualPosicaoSlider2_X =  - TAM_BARRA_X / 2.0;
float atualPosicaoSlider_Y = 0.0;

bool sliding = false;
bool sliding2 = false;

float mouse_X0;
float mouse_Y0;

float translate_slider_local_x;
float translate_slider_local_y;

float translate_slider2_local_x;
float translate_slider2_local_y;

float X_bomba = 0;
float Y_bomba = 0;
bool bomba = false;
bool colisao = false;
bool colisaoM = false;
bool posicao = false;
bool menu = false;
bool novoJogo = true;
int mapa = 0;

int tcolisao = 0;
int V01 = 30; //ms
int V02 = 30; //ms
float vetPCarM1[6][2];
float vetPCarM2[4][2];
float atualPcar1[2];
float atualPcar2[2];
float PColisao1[2];
float PColisao2[2];
float V0x;
float V0y;
float g = 10; //gravidade
float delta_tempo = 0;
int jogada = 0;
int pontoJ1 = 0;
int pontoJ2 = 0;

void redisplay(int value);
static void balisticaBomba();
void trajetoria(float teta,float V0);
void container();
void slider1();
void slider2();

int RotC1 = 300, RotC2 = 300;
int teta_tankVermelho = 30; //graus
int teta_tankAzul = 150;

void retangulos()
{
  glBegin(GL_QUADS);
  glVertex2f(-(tx / 2), ty - ty);
  glVertex2f(-(tx / 2), ty);
  glVertex2f(tx / 2, ty);
  glVertex2f(tx / 2, ty - ty);
  glEnd();

  glColor3f(0.0,0.0,0.0);
  glBegin(GL_LINE_LOOP);
  glVertex2f(-(tx / 2), ty - ty);
  glVertex2f(-(tx / 2), ty);
  glVertex2f(tx / 2, ty);
  glVertex2f(tx / 2, ty - ty);
  glEnd();
}
void triangulo()
{
  glBegin(GL_TRIANGLES);
  glVertex2f(-ty, tx -tx);
  glVertex2f(ty - ty, tx);
  glVertex2f(ty, tx - tx);
  glEnd();
}

void quickbox(float altura, float largura)
{

  glColor3f(0.0,1.0,0.0);
  glBegin(GL_LINE_LOOP);
    glVertex2f(-30.0f,-30.0);
    glVertex2f((-30.0),  -30.0f + altura);
    glVertex2f((-30.0 + largura), (-30.0 + altura));
    glVertex2f((-30.0 + largura), (-30.0));
  glEnd();
}
void quickboxCar(float altura, float largura)
{
  glBegin(GL_LINE_LOOP);
    glVertex2f(-(tx / 2), 0.5);
    glVertex2f(-(tx / 2), 0 + altura);
    glVertex2f((largura),0+ altura);
    glVertex2f((largura), 0.5);
  glEnd();
}

void canhao(){
  glBegin(GL_QUADS);
    glVertex2f(-ty/2,ty-ty);
    glVertex2f(ty/2,ty-ty);
    glVertex2f(ty/2,tx);
    glVertex2f(-ty/2,tx);
    glEnd();
}

void circulos()
{

    float teta2 = 0.0f;
    glBegin(GL_POLYGON);
    while(teta2 < 360.0)
    {
      glVertex2f(cos(teta2/180.0 * PI),sin(teta2/180.0 * PI));
      teta2 += 0.1;
    }
  glEnd();

  glBegin(GL_LINES);
  float raio = 1.0;
  float angulo = 0;
  teta2= 0.1;
  while(teta2 < 360.0)
    {
      raio = 1.1;
      glColor3f(0.0f, 0.0f, 0.0f); //cor preto
      angulo = teta2;
      glVertex2f((cos(angulo) * raio),(sin(angulo) * raio));
      raio = 1.0;

      glVertex2f((cos(angulo) * raio),(sin(angulo) * raio));
      teta2 +=1.0;
    }

  glEnd();


}
/// trata texto
void renderbitmap(float x, float y, void *font, char *string)
{
    char *c;
    glRasterPos2f(x, y);
    for(c=string; *c != '\0'; c++){
        glutBitmapCharacter(font, *c);
    }
}
///// texto de angulo de canhao
void texto()
{
    char buf[100] = {0};
    glColor3f(1.0,0.0,1.0);
    sprintf(buf,"(Jogador 1) Angulo: %d", teta_tankVermelho);
    renderbitmap(-15,73,GLUT_BITMAP_HELVETICA_12, buf);

    glColor3f(0.0,1.0,1.0);
    sprintf(buf,"(Jogador 2) Angulo: %d",-teta_tankAzul + 180);
    renderbitmap(5,73,GLUT_BITMAP_HELVETICA_12, buf);
}
//// texto de velocidade slider e botoes
void textoVelo()
{
    char buf[100] = {0};
    glColor3f(1.0,0.0,1.0);
    sprintf(buf,"Velocidade 1: %d", V01);
    renderbitmap(-29,73,GLUT_BITMAP_HELVETICA_12, buf);

    glLoadIdentity();
    glColor3f(0.0,1.0,1.0);
    sprintf(buf,"Velocidade 2: %d", V02);
    renderbitmap(21,73,GLUT_BITMAP_HELVETICA_12, buf);

    glColor3f(0.0,0.0,0.0);
    sprintf(buf,"pressione 'm' menu");
    renderbitmap(-29,64,GLUT_BITMAP_HELVETICA_18, buf);

}
/// texto exibido no menu
void textoBotoes()
{
    char buf[100] = {0};
    glColor3f(0.0,0.0,0.0);
    sprintf(buf,"MENU");
    renderbitmap(-28,66,GLUT_BITMAP_HELVETICA_12, buf);

    glColor3f(1.0,0.0,0.0);
    sprintf(buf,"Jogador 1:");
    renderbitmap(-29,63,GLUT_BITMAP_HELVETICA_12, buf);


    sprintf(buf,"Angulo: 'j' & 'l'");
    renderbitmap(-29,61,GLUT_BITMAP_HELVETICA_12, buf);

    glColor3f(0.0,0.0,1.0);
    sprintf(buf,"Jogador 2:");
    renderbitmap(-29,58,GLUT_BITMAP_HELVETICA_12, buf);


    glColor3f(0.0,0.0,1.0);
    sprintf(buf,"Angulo: 'a' & 'd'");
    renderbitmap(-29,55,GLUT_BITMAP_HELVETICA_12, buf);

    glColor3f(0.0,0.0,0.0);
    sprintf(buf,"Controles globais:");
    renderbitmap(-29,52,GLUT_BITMAP_HELVETICA_12, buf);

    glColor3f(0.0,0.7,0.0);
    sprintf(buf,"Velocidade: Slider ou 'y' & 't'");
    renderbitmap(-29,49,GLUT_BITMAP_HELVETICA_12, buf);

    glColor3f(0.0,0.7,0.0);
    sprintf(buf,"Atirar: 'b'");
    renderbitmap(-29,46,GLUT_BITMAP_HELVETICA_12, buf);

    glColor3f(0.0,0.0,0.0);
    sprintf(buf,"Novo Jogo: 'n'");
    renderbitmap(-29,43,GLUT_BITMAP_HELVETICA_12, buf);

    glColor3f(0.0,0.0,0.0);
    sprintf(buf,"retornar: 'm'");
    renderbitmap(-29,40,GLUT_BITMAP_HELVETICA_12, buf);

    glColor3f(0.0,0.0,0.0);
    sprintf(buf,"Sair: 'q'");
    renderbitmap(-29,38,GLUT_BITMAP_HELVETICA_12, buf);

}

//// texto de pontua��o para jogadores
void textoPonto()
{
    char buf[100] = {0};
    glColor3f(0.0,0.0,0.0);
    sprintf(buf,"Melhor de 3!");
    renderbitmap(-3,71,GLUT_BITMAP_HELVETICA_12, buf);
    sprintf(buf,"Score: ");
    renderbitmap(-2,68,GLUT_BITMAP_HELVETICA_18, buf);

    glColor3f(0.0,0.0,0.0);
    sprintf(buf,"Jogador 1: ");
    renderbitmap(-10,66,GLUT_BITMAP_HELVETICA_18, buf);
    glColor3f(1.0,0.0,1.0);
    sprintf(buf,"%d",pontoJ1);
    renderbitmap(-2,66,GLUT_BITMAP_HELVETICA_18, buf);

    glColor3f(0.0,0.0,0.0);
    sprintf(buf,"Jogador 2: ");
    renderbitmap(0,66,GLUT_BITMAP_HELVETICA_18, buf);
    glColor3f(0.0,1.0,1.0);
    sprintf(buf,"%d",pontoJ2);
    renderbitmap(8,66,GLUT_BITMAP_HELVETICA_18, buf);

}
//// texto de anima��o para colis�op carrinho
void textoboom(float R, float G, float B, float x, float y)
{
    char buf[100] = {0};
    glColor3f(R,G,B);
    sprintf(buf,"Boom!!");
    //renderbitmap(-3.9,-1,GLUT_BITMAP_TIMES_ROMAN_24, buf);
    renderbitmap(-2.9 + x,-1 + y,GLUT_BITMAP_HELVETICA_18,buf);
}

/// parte da anima��o para carrinho
void elipse()
{
  glBegin(GL_LINES);
  float raio = 1.0;
  float angulo = 0;
  float a = 1.5;
  float b = 2;
  float teta2 = 0.0;

  teta2= 0.1;
  while(teta2 < 360.0)
    {
      raio = 10;
      angulo = teta2;
      glVertex2f((cos(angulo) * raio)/(a*a),(sin(angulo) * raio)/(b*b));
      raio = 0;

      glVertex2f((cos(angulo) * raio)/(a*a),(sin(angulo) * raio)/(b*b));
      teta2 +=1.0;
    }

  glEnd();
}

/// anima��o de colis�o com carrinho
void impacto(float x, float y){
  int i = -90;
  int c = 0;

  glLoadIdentity();
  glColor3f(1,0.8,0);
  glTranslatef(x,y,0);
    elipse();

    while(i < 300){

    if(c%2 == 0){
    glLoadIdentity();//////////
    glColor3f(1,0.8,0);
    glTranslatef(x,y,0);
    glRotatef(i,0,0,1);
    glTranslatef(0,2,0);
    triangulo();
    glColor3f(1,0,0);
    glScaled(0.9,0.9,0);
    triangulo();
    }
    else{
    glLoadIdentity();//////////
    glTranslatef(x,y,0);
    glColor3f(1,0.8,0);
    glRotatef(i+5,0,0,1);
    glTranslatef(0,0.5,0);
    triangulo();
    glColor3f(1,0,0);

    glScaled(0.9,0.9,0);
    triangulo();

    }
    c+=1;
    i = i+20;
    }

    glLoadIdentity();
    glTranslatef(x,y,0);
    glColor3f(1,0,0);
    glScaled(0.9,0.9,0);
    elipse();


    glLoadIdentity();
    textoboom(1,1,0,x,y);

}
/// carrinho 1
void tank_Vermelho()
{
glPushMatrix();

  /////////////////////////////// escotilha
  glColor3f(1.0,0.0,0.0); ///  vermelho
  glTranslatef(-(0.1 * tx), (ty * 1.65)+0.7, 0);
  glScaled(1.3, 1.3, 0.0);
  glRotatef(RotC1, 0, 0, 1);
  circulos();
  glPopMatrix();

  /////////////////////////////// canh�o
  glPushMatrix();
  glTranslatef(-tx/10.5,1.6*ty+0.7, 0);
  glColor3f(0.0, 0.0, 0.0); ////// preto
  glScaled(0.4, 0.4, 0.0);
  glRotatef(RotC1, 0, 0, 1);
  canhao();


  ///////////////////////////// boca canh�o

  glColor3f(0.5, 0.5, 0.5);
  glTranslatef(0,3.2*ty,0);
  glScaled(0.8, 0.8, 0.0);
  circulos();
  glPopMatrix();


  /////////////////////////// basecanhao
  glPushMatrix();
  glColor3f(0.5, 0.5, 0.5);
  glTranslatef(-(0.1 * tx), (ty * 1.6)+0.7, 0);
  glScaled(0.5, 0.5, 0.0);
  glRotatef(RotC1, 0, 0, 1);
  circulos();
  glPopMatrix();

  //////////////////////////// carroceria
  glPushMatrix();
  glColor3f(1.0,0.0,0.0); //// vermelho
  glTranslatef(0,0.7,0);
  retangulos();
  glPopMatrix();

  ////////////////////////////// rodas
  glPushMatrix();
  glTranslatef(-0.2*tx, -(ty / 4)+0.7, 0);
  glColor3f(0.0, 0.0, 0.0); ////   preto
  glScaled(0.4, 0.4, 0.0);
  circulos();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.2 * tx, -(ty/4)+0.7, 0);
  glScaled(0.4, 0.4, 0.0);
  circulos();
  glPopMatrix();
}
/// carrinho 2
void tank_Azul()
{

  ////////////////////////////////  Tank azul
  glRotatef(180, 0, 1, 0); //////   rotaciona em y
  glPushMatrix();

  /////////////////////////////// escotilha
  glColor3f(0.0,1.0,1.0); ///  azul claro
  glTranslatef(-(0.1 * tx), (ty * 1.65)+0.7, 0);
  glScaled(1.3, 1.3, 0.0);
  glRotatef(RotC2, 0, 0, 1);
  circulos();
  glPopMatrix();

  /////////////////////////////// canh�o
  glPushMatrix();
  glTranslatef(-tx/10.5,1.6*ty+0.7, 0);
  glColor3f(0.0, 0.0, 0.0); ////// preto
  glScaled(0.4, 0.4, 0.0);
  glRotatef(RotC2, 0, 0, 1);
  canhao();


  ///////////////////////////// boca canh�o

  glColor3f(0.5, 0.5, 0.5);
  glTranslatef(0,3.2*ty,0);
  glScaled(0.8, 0.8, 0.0);
  circulos();
  glPopMatrix();


  /////////////////////////// basecanhao
  glPushMatrix();
  glColor3f(0.5, 0.5, 0.5);
  glTranslatef(-(0.1 * tx), (ty * 1.6)+0.7, 0);
  glScaled(0.5, 0.5, 0.0);
  glRotatef(RotC2, 0, 0, 1);
  circulos();
  glPopMatrix();

  //////////////////////////// carroceria
  glPushMatrix();
  glColor3f(0.0,1.0,1.0); //// azul claro
  glTranslatef(0,0.7,0);
  retangulos();
  glPopMatrix();

  ////////////////////////////// rodas
  glPushMatrix();
  glTranslatef(-0.2*tx, -(ty / 4)+0.7, 0);
  glColor3f(0.0, 0.0, 0.0); ////   preto
  glScaled(0.4, 0.4, 0.0);
  circulos();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.2 * tx, -(ty/4)+0.7, 0);
  glScaled(0.4, 0.4, 0.0);
  circulos();
  glPopMatrix();

  glRotatef(-180, 0, 1, 0); //////   rotaciona em y

}
//// desenha um tijolo
static void tijolo()
{
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-30.0f,-30.0);
        glVertex2f((-30.0 + WIDTH_TIJOLO),  -30.0f);
        glVertex2f((-30.0 + WIDTH_TIJOLO), (-30.0 + HEIGHT_TIJOLO));
        glVertex2f((-30.0f), (-30.0 + HEIGHT_TIJOLO));
    glEnd();

}
/// desenha peda�o de tijolos no quadro muro
static void pedacoMuro(int altura, int largura, int transladado)
{

  for(int i = 0; i < altura; i++ )
  {
    if(transladado)
    {
        if(i % 2 == 0)
        {
          glTranslatef(-WIDTH_TIJOLO/2.0, 0.0, 0.0);
        }

    }

    else
    {
      if(i % 2 == 1)
      {
        glTranslatef(-WIDTH_TIJOLO/2.0, 0.0, 0.0);
      }
    }

    for(int j = 0; j < largura; j++)
    {
      tijolo();
      glTranslatef(WIDTH_TIJOLO, 0.0, 0.0);
    }
    glTranslatef(0.0, HEIGHT_TIJOLO, 0.0);
    glTranslatef((- largura * WIDTH_TIJOLO), 0.0, 0.0);
    if(transladado)
    {
      if(i % 2 == 0)
      {
        glTranslatef(WIDTH_TIJOLO/2.0, 0.0, 0.0);
      }
    }
    else{
      if(i % 2 == 1)
      {
        glTranslatef(WIDTH_TIJOLO/2.0, 0.0, 0.0);
      }
    }

  }
}
/// retangulo muro
static void retangulo(float altura, float largura)
{
  glColor3f(0.0,0.0,0.0);
  glBegin(GL_LINE_LOOP);
    glVertex2f(-30.0f,-30.0);
    glVertex2f((-30.0),  -30.0f + altura);
    glVertex2f((-30.0 + largura), (-30.0 + altura));
    glVertex2f((-30.0 + largura), (-30.0));
  glEnd();

  glColor3f(1.0,1.0,0.0);
  glBegin(GL_QUADS);
    glVertex2f(-30.0f,-30.0);
    glVertex2f((-30.0),  -30.0f + altura);
    glColor3f(1.0,0.0,0.0);
    glVertex2f((-30.0 + largura), (-30.0 + altura));
    glVertex2f((-30.0 + largura), (-30.0));
  glEnd();

}

////// ratangulo menu
static void retMenu(float altura, float largura)
{
  glColor3f(0.0,0.0,0.0);
  glBegin(GL_LINE_LOOP);
    glVertex2f((-30.0),  -30.0f + altura);
    glVertex2f((-30.0 + largura), (-30.0 + altura));
    glVertex2f((-30.0 + largura), (-30.0));
  glEnd();

  glColor3f(0.8,0.8,0.8);
  glBegin(GL_QUADS);
    glVertex2f(-30.0f,-30.0);
    glVertex2f((-30.0),  -30.0f + altura);
    glVertex2f((-30.0 + largura), (-30.0 + altura));
    glVertex2f((-30.0 + largura), (-30.0));
  glEnd();

}

//// desenha fundo
static void Ceu(float altura, float largura)
{
  glColor3f(0.0,0.0,0.0);
  glBegin(GL_LINE_LOOP);
    glVertex2f(-30.0f,-30.0);
    glVertex2f((-30.0),  -30.0f + altura);
    glVertex2f((-30.0 + largura), (-30.0 + altura));
    glVertex2f((-30.0 + largura), (-30.0));
  glEnd();

  glBegin(GL_QUADS);
    glColor3f(1.0,0.0,0.7);
    glVertex2f(-30.0f,-30.0);
    glColor3f(0.0,0.0,0.9);
    glVertex2f((-30.0),  -30.0f + altura);
    glColor3f(0.0,0.0,0.9);
    glVertex2f((-30.0 + largura), (-30.0 + altura));
    glColor3f(1.0,0.0,0.7);
    glVertex2f((-30.0 + largura), (-30.0));
  glEnd();

}
//// desenha muro
static void muros()
{
  glLoadIdentity();
  Ceu(70*HEIGHT_TIJOLO, 20* WIDTH_TIJOLO);

  glLoadIdentity();
  retangulo( 9 * HEIGHT_TIJOLO, 20 * WIDTH_TIJOLO);
  pedacoMuro(9, 20, 0);

  glLoadIdentity();
  glTranslatef(2.5 * WIDTH_TIJOLO, 9 *  HEIGHT_TIJOLO, 0.0 );
  retangulo( 15 * HEIGHT_TIJOLO, 6.5 * WIDTH_TIJOLO);//
  glTranslatef(0.5 * WIDTH_TIJOLO, 0.0, 0.0 );
  pedacoMuro(15 , 6 , 1);

  glLoadIdentity();
  glTranslatef( (2.5 + 0.5 + 8 + 1.0)* WIDTH_TIJOLO, 9 *  HEIGHT_TIJOLO, 0.0 );
  retangulo( 5 * HEIGHT_TIJOLO, 8.5 * WIDTH_TIJOLO);///
  glTranslatef(0.5 * WIDTH_TIJOLO, 0.0, 0.0 );
  pedacoMuro(5 , 8 , 0);

  glLoadIdentity();
  glTranslatef( (2.5 + 0.5 + 8 + 1.0)* WIDTH_TIJOLO, (9 + 5) *  HEIGHT_TIJOLO, 0.0 );
  retangulo( 10 * HEIGHT_TIJOLO, 2.5 * WIDTH_TIJOLO);////
  glTranslatef(0.5 * WIDTH_TIJOLO, 0.0, 0.0 );
  pedacoMuro(10 , 2.0 , 1);

  glLoadIdentity();
  glTranslatef( (2.5 + 0.5 + 8 + 1.5 + 5)* WIDTH_TIJOLO, (9 + 5) *  HEIGHT_TIJOLO, 0.0 );
  retangulo( 10 * HEIGHT_TIJOLO, 2.5 * WIDTH_TIJOLO);
  glTranslatef(0.5 * WIDTH_TIJOLO, 0.0, 0.0 );
  pedacoMuro(10 , 2.0 , 1);

}
void muros2()
{
  glLoadIdentity();
  Ceu(70*HEIGHT_TIJOLO, 20* WIDTH_TIJOLO);

  glLoadIdentity();
  retangulo( 9 * HEIGHT_TIJOLO, 20 * WIDTH_TIJOLO);
  pedacoMuro(9, 20, 0); //altura e largura

  glLoadIdentity();
  glTranslatef(0.0, 9 *  HEIGHT_TIJOLO, 0.0 );
  retangulo( 6 * HEIGHT_TIJOLO, 2.5 * WIDTH_TIJOLO);
  glTranslatef(0.5 * WIDTH_TIJOLO, 0.0, 0.0 );
  pedacoMuro(6 , 2.0 , 1);

  glLoadIdentity();
  glTranslatef(7.5 * WIDTH_TIJOLO, 9 *  HEIGHT_TIJOLO, 0.0 );
  retangulo( 15 * HEIGHT_TIJOLO, 2.5 * WIDTH_TIJOLO);
  glTranslatef(0.5 * WIDTH_TIJOLO, 0.0, 0.0 );
  pedacoMuro(15 , 2.5 , 1);

  glLoadIdentity();
  glTranslatef( 5 * WIDTH_TIJOLO, (9 + 15.0) *  HEIGHT_TIJOLO, 0.0 );
  retangulo( 5 * HEIGHT_TIJOLO, 7.5 * WIDTH_TIJOLO);
  glTranslatef(0.5 * WIDTH_TIJOLO, 0.0, 0.0 );
  pedacoMuro(5 , 7.5 , 0);

  glLoadIdentity();
  glTranslatef( 15 * WIDTH_TIJOLO, (9 + 15) *  HEIGHT_TIJOLO, 0.0 );
  retangulo( 5 * HEIGHT_TIJOLO, 5.5 * WIDTH_TIJOLO);
  glTranslatef(0.5 * WIDTH_TIJOLO, 0.0, 0.0 );
  pedacoMuro(5 , 5.0 , 1);

}
void espaco1(){///// mapeia possiveis posi��es para carrinhos


  vetPCarM1[0][0] = -ORTO_X + ((2.5 * WIDTH_TIJOLO)/2);
  vetPCarM1[0][1] = -ORTO_Y + (9*HEIGHT_TIJOLO);

  vetPCarM1[1][0] = -ORTO_X + ((2.5*WIDTH_TIJOLO)+((6.5*WIDTH_TIJOLO)/2));
  vetPCarM1[1][1] = -ORTO_Y + (9*HEIGHT_TIJOLO)+((15*HEIGHT_TIJOLO));

  vetPCarM1[2][0] = -ORTO_X + (12*WIDTH_TIJOLO) +(8.5 *WIDTH_TIJOLO)/2;
  vetPCarM1[2][1] = -ORTO_Y + (9*HEIGHT_TIJOLO)+(5*HEIGHT_TIJOLO);

  vetPCarM1[3][0] = -ORTO_X + (12*WIDTH_TIJOLO)+(2.5*WIDTH_TIJOLO)/2;
  vetPCarM1[3][1] = -ORTO_Y + (14*HEIGHT_TIJOLO)+(10*HEIGHT_TIJOLO);

  vetPCarM1[4][0] = -ORTO_X + (17.5*WIDTH_TIJOLO)+(2.5*WIDTH_TIJOLO)/2;
  vetPCarM1[4][1] = -ORTO_Y + (14*HEIGHT_TIJOLO)+(10*HEIGHT_TIJOLO);

  vetPCarM1[5][0] = -ORTO_X + (20* WIDTH_TIJOLO)/2;
  vetPCarM1[5][1] = -ORTO_Y + (9*HEIGHT_TIJOLO);
}

void espaco2(){
  vetPCarM2[0][0] = -ORTO_X + ((2.5 * WIDTH_TIJOLO)/2);
  vetPCarM2[0][1] = -ORTO_Y + (9*HEIGHT_TIJOLO)+(6*HEIGHT_TIJOLO);

  vetPCarM2[1][0] = -ORTO_X + (7.5*WIDTH_TIJOLO)+(5*HEIGHT_TIJOLO)/2;
  vetPCarM2[1][1] = -ORTO_Y + (14*HEIGHT_TIJOLO)+(7.5*WIDTH_TIJOLO);

  vetPCarM2[2][0] = -ORTO_X + (15*WIDTH_TIJOLO) +(5 *WIDTH_TIJOLO)/2;
  vetPCarM2[2][1] = -ORTO_Y + (23*HEIGHT_TIJOLO)+(6*HEIGHT_TIJOLO);

  vetPCarM2[3][0] = -ORTO_X + (15*WIDTH_TIJOLO)-(2.5*WIDTH_TIJOLO)/2;
  vetPCarM2[3][1] = -ORTO_Y + (9*HEIGHT_TIJOLO);
}


static void Bomba(){

  float teta2 = 0.0f;
    glBegin(GL_POLYGON);
    while(teta2 < 360.0)
    {
      glVertex2f(cos(teta2/180.0 * PI),sin(teta2/180.0 * PI));
      teta2 += 0.1;
    }
  glEnd();
}
///  caixa de menu e teclado
void menuBox(){
  glLoadIdentity();
  glTranslatef(1,67,0);
  retMenu(10*WIDTH_TIJOLO,10*HEIGHT_TIJOLO);
  glLoadIdentity();
  textoBotoes();


}

void spawnCar2(){
  srand( (unsigned)time(NULL) );
  int C1 = 0;
  int C2 = 0;
  if(novoJogo){
    mapa = rand()%2;
    pontoJ1 = 0;
    pontoJ2 = 0;
    novoJogo = false;
  }

  if(mapa == 0){
  int C1 = rand()%6;
  int C2 = rand()%6;
  espaco1();

   while(C2 == C1){
    C2 = rand()%6;
  }
  atualPcar1[0] = vetPCarM1[C1][0];
  atualPcar1[1] = vetPCarM1[C1][1];
  atualPcar2[0] = vetPCarM1[C2][0];
  atualPcar2[1] = vetPCarM1[C2][1];
  PColisao1[0] = vetPCarM1[C1][0];
  PColisao1[1] = vetPCarM1[C1][1];
  PColisao2[0] = vetPCarM1[C2][0];
  PColisao2[1] = vetPCarM1[C2][1];

  }else{
    int C1 = rand()%4;
    int C2 = rand()%4;
    espaco2();
   while(C2 == C1){
    C2 = rand()%4;
  }
  atualPcar1[0] = vetPCarM2[C1][0];
  atualPcar1[1] = vetPCarM2[C1][1];
  atualPcar2[0] = vetPCarM2[C2][0];
  atualPcar2[1] = vetPCarM2[C2][1];
  PColisao1[0] = vetPCarM2[C1][0];
  PColisao1[1] = vetPCarM2[C1][1];
  PColisao2[0] = vetPCarM2[C2][0];
  PColisao2[1] = vetPCarM2[C2][1];

}
  }
void transformacao(){

  if(!posicao || novoJogo){ // sorteia posi��o para os carros e novo mapa
    spawnCar2();
    posicao = true;
  }
  if(mapa == 0){ // seleciona mapa sorteado
    muros();
  }
  else if(mapa == 1){

  muros2();
  }

  glLoadIdentity();
  glPushMatrix();
  glTranslatef(atualPcar1[0], atualPcar1[1], 0.0);
  tank_Vermelho();
  glPopMatrix();

  glLoadIdentity();
  glPushMatrix();
  glTranslatef(atualPcar2[0], atualPcar2[1] , 0.0);
  tank_Azul();
  glPopMatrix();


  if(jogada == 0){
    glLoadIdentity();
    glTranslatef(-0.66 * WIDTH_TIJOLO+atualPcar1[0],-1* HEIGHT_TIJOLO+atualPcar1[1]+0.7,0); // tra�o da trajetoria do tiro para joagdor 1
    trajetoria(teta_tankVermelho,V01);
  }else{
  glLoadIdentity();
  glTranslatef(-0.35 * WIDTH_TIJOLO+atualPcar2[0],-1* HEIGHT_TIJOLO+atualPcar2[1]+0.7,0);// tra�o da trajetoria do tiro para joagdor 2
  trajetoria(teta_tankAzul,V02);
  }

  glLoadIdentity();
  textoVelo();
  glLoadIdentity();
  textoPonto();

  if(bomba)
  {
    balisticaBomba();
  }

  glLoadIdentity();
  texto();

  if(colisao){
    if(jogada == 1){
      glLoadIdentity();
      impacto(atualPcar2[0],atualPcar2[1]);
        glutPostRedisplay();
      if(tcolisao < 100){
        tcolisao += 1;
      }
      else{
        tcolisao = 0;
        colisao = false;
      }
    }
  else{
    glLoadIdentity();

    impacto(atualPcar1[0],atualPcar1[1]);
    glutPostRedisplay();
      if(tcolisao < 100){
        tcolisao += 1;
      }
      else{
        tcolisao = 0;
        colisao = false;
      }
  }
  }else if(colisaoM){

    colisaoM = false;

  }
  if(menu){
    menuBox();
  }
  if(pontoJ1 == 3 || pontoJ2 == 3){
    novoJogo = true;
  }
  glLoadIdentity();
  slider1();
  glLoadIdentity();
  slider2();

}

static void balisticaBomba()
{
  //printf("Bomba: %f\n", delta_tempo);

  float translateX = 0.0;
  float translateY = 0.0;
  float i = 0;
  float delta_espaco_X = V0x * delta_tempo;
  float delta_espaco_Y = V0y * delta_tempo - g * delta_tempo * delta_tempo / 2;



  if(jogada == 0){
    translateX = X_bomba + delta_espaco_X + (-0.50 + atualPcar1[0]); /// posiciona objetos para jogador 1
    translateY = Y_bomba + delta_espaco_Y + (3.7+atualPcar1[1]);


    if(translateX > PColisao2[0] - tx/2 && translateX < PColisao2[0] + tx/2 // trata colis�o com jogador 2
      && translateY > PColisao2[1] &&  translateY < PColisao2[1] + ty+3.5){

    colisao = true;


    bomba = false;
    delta_tempo = 0;

      jogada = 1;
      pontoJ1 += 1;
      posicao = false;

    glutPostRedisplay();
  }
  else if(mapa == 0){
    if(translateX > -ORTO_X && translateX < -ORTO_X + 22.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y && translateY < -ORTO_Y + 9 * HEIGHT_TIJOLO ||
          translateX > -ORTO_X + 2.5 * WIDTH_TIJOLO && translateX < -ORTO_X + 2.5 * WIDTH_TIJOLO + 6.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 9 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 9 *  HEIGHT_TIJOLO + 15 * HEIGHT_TIJOLO ||
          translateX > -ORTO_X + 12 * WIDTH_TIJOLO && translateX < -ORTO_X + 12 * WIDTH_TIJOLO + 8.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 9 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 9 *  HEIGHT_TIJOLO + 4 * HEIGHT_TIJOLO ||
          translateX > -ORTO_X + 12 * WIDTH_TIJOLO && translateX < -ORTO_X + 12 * WIDTH_TIJOLO + 2.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 14 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 14 *  HEIGHT_TIJOLO + 10 * HEIGHT_TIJOLO ||
          translateX > -ORTO_X + 17.5 * WIDTH_TIJOLO && translateX < -ORTO_X + 17.5 * WIDTH_TIJOLO + 2.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 12 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 12 *  HEIGHT_TIJOLO + 10 * HEIGHT_TIJOLO ||
          translateX < -ORTO_X || translateY < -ORTO_Y || translateX > ORTO_X){  //////// trata colis�o com muro e laterais da janela para jogador 1

          colisaoM = true;

          bomba = false;
          delta_tempo = 0;
          if(jogada == 0)
          {
            jogada = 1;
          }
          else
          {
            jogada = 0;
          }
          glutPostRedisplay();
      }


      else{ ////   lan�amento de projetil

        glLoadIdentity();
        glColor3f(0.0,0.0,0.0);
        glTranslatef(translateX, translateY, 0.0);
        glScaled(0.5, 0.5, 0.0);
        Bomba();
        glScaled(1/0.5, 1/0.5, 0.0);
        glutTimerFunc(10, redisplay, X_bomba);

      }
  }
  else{
    if(translateX > -ORTO_X && translateX < -ORTO_X + 22.5 * WIDTH_TIJOLO     ////////    trata colis�o com muro e laterais da janela
          && translateY > -ORTO_Y && translateY < -ORTO_Y + 9 * HEIGHT_TIJOLO || translateX > -ORTO_X && translateX < -ORTO_X + 2.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 9 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 9 *  HEIGHT_TIJOLO +  6 * HEIGHT_TIJOLO ||
          translateX > -ORTO_X + 7.5 * WIDTH_TIJOLO && translateX < -ORTO_X + 7.5 * WIDTH_TIJOLO + 2.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 9 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 9 *  HEIGHT_TIJOLO + 15 * HEIGHT_TIJOLO ||
          translateX > -ORTO_X + 5 * WIDTH_TIJOLO && translateX < -ORTO_X + 5 * WIDTH_TIJOLO + 7.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 24 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 24 *  HEIGHT_TIJOLO + 5 * HEIGHT_TIJOLO ||
          translateX > -ORTO_X + 15 * WIDTH_TIJOLO && translateX < -ORTO_X + 15 * WIDTH_TIJOLO + 5.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 24 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 24 *  HEIGHT_TIJOLO + 5 * HEIGHT_TIJOLO ||
          translateX < -ORTO_X || translateY < -ORTO_Y || translateX > ORTO_X){

          colisaoM = true;


          bomba = false;
          delta_tempo = 0;
          if(jogada == 0)
          {
            jogada = 1;
          }
          else
          {
            jogada = 0;
          }
          glutPostRedisplay();
      }

    else{
      /////  lan�amento de projetil
      glLoadIdentity();
      glColor3f(0.0,0.0,0.0);
      glTranslatef(translateX, translateY, 0.0);
      glScaled(0.5, 0.5, 0.0);
      Bomba();
      glScaled(1/0.5, 1/0.5, 0.0);
      glutTimerFunc(10, redisplay, X_bomba);

   }
  }
}
  else{
    translateX = X_bomba + delta_espaco_X + ( atualPcar2[0]+0.4);////// posiciona objetos para jogador 2
    translateY = Y_bomba + delta_espaco_Y + (3.7+atualPcar2[1]);

    if(translateX > PColisao1[0] - tx/2 && translateX < PColisao1[0] + tx/2 //// trata colis�o com jgador 1
    && translateY > PColisao1[1] && translateY < PColisao1[1] + ty+3.5){

    colisao = true;


    bomba = false;
    delta_tempo = 0;

    jogada = 0;

    pontoJ2 += 1;
    posicao = false;

    glutPostRedisplay();
  }
  else if(mapa == 0){
      if(translateX > -ORTO_X && translateX < -ORTO_X + 22.5 * WIDTH_TIJOLO     ////////    trata colis�o com muro e laterais da janela
          && translateY > -ORTO_Y && translateY < -ORTO_Y + 9 * HEIGHT_TIJOLO || translateX > -ORTO_X + 2.5 * WIDTH_TIJOLO && translateX < -ORTO_X + 2.5 * WIDTH_TIJOLO + 6.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 9 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 9 *  HEIGHT_TIJOLO + 15 * HEIGHT_TIJOLO ||
          translateX > -ORTO_X + 12 * WIDTH_TIJOLO && translateX < -ORTO_X + 12 * WIDTH_TIJOLO + 8.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 9 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 9 *  HEIGHT_TIJOLO + 4 * HEIGHT_TIJOLO ||
          translateX > -ORTO_X + 12 * WIDTH_TIJOLO && translateX < -ORTO_X + 12 * WIDTH_TIJOLO + 2.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 14 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 14 *  HEIGHT_TIJOLO + 10 * HEIGHT_TIJOLO ||
          translateX > -ORTO_X + 17.5 * WIDTH_TIJOLO && translateX < -ORTO_X + 17.5 * WIDTH_TIJOLO + 2.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 12 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 12 *  HEIGHT_TIJOLO + 10 * HEIGHT_TIJOLO ||
          translateX < -ORTO_X || translateY < -ORTO_Y || translateX > ORTO_X){

          colisaoM = true;


          bomba = false;
          delta_tempo = 0;
          if(jogada == 0)
          {
            jogada = 1;
          }
          else
          {
            jogada = 0;
          }
          glutPostRedisplay();
      }

    else{
      /////  lan�amento de projetil
      glLoadIdentity();
      glColor3f(0.0,0.0,0.0);
      glTranslatef(translateX, translateY, 0.0);
      glScaled(0.5, 0.5, 0.0);
      Bomba();
      glScaled(1/0.5, 1/0.5, 0.0);
      glutTimerFunc(10, redisplay, X_bomba);

   }
  }
  else{
    if(translateX > -ORTO_X && translateX < -ORTO_X + 22.5 * WIDTH_TIJOLO     ////////    trata colis�o com muro e laterais da janela
          && translateY > -ORTO_Y && translateY < -ORTO_Y + 9 * HEIGHT_TIJOLO || translateX > -ORTO_X && translateX < -ORTO_X + 2.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 9 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 9 *  HEIGHT_TIJOLO +  6 * HEIGHT_TIJOLO ||
          translateX > -ORTO_X + 7.5 * WIDTH_TIJOLO && translateX < -ORTO_X + 7.5 * WIDTH_TIJOLO + 2.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 9 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 9 *  HEIGHT_TIJOLO + 15 * HEIGHT_TIJOLO ||
          translateX > -ORTO_X + 5 * WIDTH_TIJOLO && translateX < -ORTO_X + 5 * WIDTH_TIJOLO + 7.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 24 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 24 *  HEIGHT_TIJOLO + 5 * HEIGHT_TIJOLO ||
          translateX > -ORTO_X + 15 * WIDTH_TIJOLO && translateX < -ORTO_X + 15 * WIDTH_TIJOLO + 5.5 * WIDTH_TIJOLO
          && translateY > -ORTO_Y + 24 *  HEIGHT_TIJOLO && translateY < -ORTO_Y + 24 *  HEIGHT_TIJOLO + 5 * HEIGHT_TIJOLO ||
          translateX < -ORTO_X || translateY < -ORTO_Y || translateX > ORTO_X){

          colisaoM = true;


          bomba = false;
          delta_tempo = 0;
          if(jogada == 0)
          {
            jogada = 1;
          }
          else
          {
            jogada = 0;
          }
          glutPostRedisplay();
      }

    else{
      /////  lan�amento de projetil
      glLoadIdentity();
      glColor3f(0.0,0.0,0.0);
      glTranslatef(translateX, translateY, 0.0);
      glScaled(0.5, 0.5, 0.0);
      Bomba();
      glScaled(1/0.5, 1/0.5, 0.0);
      glutTimerFunc(10, redisplay, X_bomba);

   }
  }
  }

  printf("translateX: %f\n",translateX );
  printf("translateY: %f\n", translateY);


}

void trajetoria(float teta,float V0)
{
  //printf("trajetoria\n" );
  float V0x = V0 * cos(teta/180.00 * PI);
  float V0y = V0 * sin(teta/180.00 * PI);
  //printf("V0x: %f\n",V0x );
  //printf("V0y: %f\n",V0y );

  float temp = 0.0;
  float trajeX = 1.5;
  float trajeY = 4.0;
  //glLoadIdentity();
  glColor3f(0, 1, 0);
  glBegin(GL_LINE_STRIP);
  while(temp <= 1* (V0y * 2 / g)-2)
  {
    //printf("Dentro do while, tempo: %f\n", temp );
    //printf("Max %f\n", (V0y * 2 / g) );
    //glLoadIdentity();
    //glTranslatef(V0x * temp, V0y * temp - g * temp * temp /2, 0.0);
    //glTranslatef(temp, temp , 0.0);
    //glVertex2f(trajeX , trajeY  );
    glVertex2f(trajeX + V0x * temp , trajeY + V0y * temp - g * temp * temp /2.0 );
    temp += 0.1;
  }
  glEnd();
}


static void Atualiza_desenho(void)
{
  printf("Atualiza Desenho\n");
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();//inicializa matriz com a matriz identidade
  glClear(GL_COLOR_BUFFER_BIT);//Limpa janela

    transformacao();

    glutSwapBuffers();
}

static void Atualiza_tamanho(int largura, int altura)
{
    glViewport(0,0,largura,altura);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-ORTO_X, ORTO_X, -ORTO_Y, 2.5*ORTO_Y);
}

static void idle(void)
{
    glutPostRedisplay();
}

void Teclado(unsigned char tecla, int x1, int y1)
{
  switch (tecla)
  {
  case 27:
  case 'q':
    exit(0);
    break;
  case 'j':
    if (RotC1 < 450 && !bomba)
    {
      RotC1 += 3;

      teta_tankVermelho = RotC1 - 270;



      printf("Rota��o anti horario\n");
    }
    break;
  case 'l':

    if (RotC1 > 270 && !bomba)
    {
      RotC1 -= 3;
      teta_tankVermelho = RotC1 - 270;



      printf("Rota��o horario\n");
    }
    break;
  case 'd':
    if (RotC2 < 450 && !bomba)
    {
      RotC2 += 3;
      teta_tankAzul = 450 - RotC2;


    }
    break;
  case 'a':
    if (RotC2 > 270 && !bomba)
    {
      RotC2 -= 3;
      teta_tankAzul = 450 - RotC2;


    }
    break;

  case 'b':
    if(!bomba){
      bomba = true;
      if(jogada == 0)
      {
        V0x = V01 * cos(teta_tankVermelho/180.00 * PI);
        V0y = V01 * sin(teta_tankVermelho/180.00 * PI);
      }
      if(jogada == 1)
      {
        V0x = V02 * cos(teta_tankAzul/180.00 * PI);
        V0y = V02 * sin(teta_tankAzul/180.00 * PI);
      }
      delta_tempo = 0;
    }
    break;

  case 't':
    if(jogada == 0)
    {
      if(!bomba)
      {
        V01 += 2.0;
        atualPosicaoSlider1_X = (V01 - 25.0)/10.0;

      }

    }else{
      if(!bomba)
      {
        V02 += 2;
        atualPosicaoSlider2_X = (V02 - 25.0)/10.0;
      }

    }

    break;

  case 'y':
    if(jogada == 0)
    {
      if(!bomba)
      {
        V01 -= 2;
        atualPosicaoSlider1_X = (V01 - 25.0)/10.0;

      }

    }else{
      if(!bomba)
      {
        V02 -= 2;
        atualPosicaoSlider2_X = (V02 - 25.0)/10.0;
      }

    }

    break;

  case 'm':
    if(!menu){
      menu = true;
    }
    else{
      menu = false;
    }
    break;
  case 'n':
    if(!novoJogo){
      novoJogo = true;
    }
  }
  glutPostRedisplay();
}

void redisplay(int value){
  printf("X_bomba %d\n", value);
  if(bomba){
    delta_tempo += 0.1;
  }
  glutPostRedisplay();
}


void mouseFunc(int button, int state, int x, int y )
{
  printf("\n" );
  printf("mouseFunc\n");
  printf("x: %d\n",x );
  printf("y: %d\n",y );
  printf("viewport X: %f\n",(x/12.5 - 30) );
  printf("viewport Y: %i\n",(72 - y/16));
  switch (button) {
    case GLUT_LEFT_BUTTON:
      if(state == GLUT_DOWN)
      {


        if((x/12.5 - 30) <= atualPosicaoSlider1_X + translate_slider_local_x + 1.0 && (x/12.5 - 30) >= atualPosicaoSlider1_X + translate_slider_local_x - 1.0)
        {
          if((72 - y/16) <= atualPosicaoSlider_Y + translate_slider_local_y + 1.0 && (72 - y/16)>= atualPosicaoSlider_Y + translate_slider_local_y - 1.0)
          {
            sliding = true;
            mouse_X0 = x;
          }
        }

        if((x/12.5 - 30) <= atualPosicaoSlider2_X + translate_slider2_local_x + 1.0 && (x/12.5 - 30) >= atualPosicaoSlider2_X + translate_slider2_local_x - 1.0)
        {
          if((72 - y/16) <= atualPosicaoSlider_Y + translate_slider2_local_y + 1.0 && (72 - y/16)>= atualPosicaoSlider_Y + translate_slider2_local_y - 1.0)
          {
            sliding2 = true;
            mouse_X0 = x;
          }
        }

      }
      if(state == GLUT_UP)
      {
        if(sliding)
        {
          sliding = false;
          atualPosicaoSlider1_X += translate_slider;
        }

        if(slider2)
        {
          sliding2 = false;
          atualPosicaoSlider2_X += translate_slider2;
        }

        translate_slider = 0;
        translate_slider2 = 0;
        glutPostRedisplay();
      }
      break;
  }
}

void motionFunc(int x, int y)
{
    printf("\n" );
    printf("motionFunc\n" );
    printf("sliding %d\n", sliding );
    float oldTranslate_slider = translate_slider;
    float oldTranslate_slider2 = translate_slider2;

    if(sliding)
    {
        translate_slider = (x - mouse_X0)/10.0;


        if(atualPosicaoSlider1_X + translate_slider >=5.0/2.0 || atualPosicaoSlider1_X + translate_slider <= -5.0/2.0)
        {
          translate_slider = oldTranslate_slider;

        }

        printf("track_x: %f\n", x - mouse_X0 );
        printf("track_y: %f\n", y - mouse_Y0);
        printf("translate_slider: %f\n", translate_slider );

        glutPostRedisplay();

    }

    if(sliding2)
    {
      translate_slider2 = (x - mouse_X0)/10.0;


      if(atualPosicaoSlider2_X + translate_slider2 >= 5.0/2.0 || atualPosicaoSlider2_X + translate_slider2 <= -5.0/2.0)
      {
        translate_slider2 = oldTranslate_slider2;

      }

      printf("track_x: %f\n", x - mouse_X0 );
      printf("track_y: %f\n", y - mouse_Y0);
      printf("translate_slider: %f\n", translate_slider2 );

      glutPostRedisplay();
    }
}
void primitivaCirculo(){
  float tetaCirculo = 0.0f;
  glBegin(GL_POLYGON);
  while(tetaCirculo < 360.0)
  {
    glVertex2f(cos(tetaCirculo/180.0 * PI), sin(tetaCirculo/180.0 * PI));
    tetaCirculo += 0.1;
  }
  glEnd();

}

void circuloCursor(){

    glScaled(0.8, 0.8, 0.0);
    primitivaCirculo();
}

void primitivaSlider()
{
    glBegin(GL_POLYGON);
        glVertex2f(-0.5,-0.5);
        glVertex2f(-0.5, 0.5);
        glVertex2f(0.5, 0.5);
        glVertex2f(0.5, -0.5);
    glEnd();
}

void barraSlider(){
  glColor3f(0.8, 0.0, 0.5);
  glScaled(TAM_BARRA_X, TAM_BARRA_Y, 0.0);
  primitivaSlider();
}

void container()
{
  glColor3f(0.5, 0.8, 0.5);
  glScaled(TAM_CONTAINER_X, TAM_CONTAINER_Y, 0.0);
  primitivaSlider();

}

void slider1(){

  glLoadIdentity();
  glTranslatef( translate_slider_local_x,   translate_slider_local_y, 0.0); //colocar no local correto

  glPushMatrix();
    container();
  glPopMatrix();

  glPushMatrix();
    barraSlider();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(atualPosicaoSlider1_X + translate_slider, 0.0, 0.0); //mover ao longo do slider
    V01 = 10 * (atualPosicaoSlider1_X + translate_slider) + 25;
    circuloCursor();
  glPopMatrix();

}
void slider2(){

  glLoadIdentity();
  glTranslatef( translate_slider2_local_x,   translate_slider2_local_y, 0.0); //colocar no local correto

  glPushMatrix();
    container();
  glPopMatrix();

  glPushMatrix();
    barraSlider();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(atualPosicaoSlider2_X + translate_slider2, 0.0, 0.0); //mover ao longo do slider
    V02 = 10 * (atualPosicaoSlider2_X + translate_slider2) + 25;
    circuloCursor();
  glPopMatrix();

}

int main(int argc, char *argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(350,0);
    glutInitWindowSize(750,960);
    glutCreateWindow("LCG - Batalha Urbana");

    //registra fun  o callback para tratar evento de desenho
    glutDisplayFunc(Atualiza_desenho);

    //registra fun  o callback para tratar evento de redimensionamento de janela
    glutReshapeFunc(Atualiza_tamanho);

    //define a cor de limpeza da janela como sendo a branca
    glClearColor(1,1,1,0);
    glutKeyboardFunc(Teclado);
    //glutIdleFunc(idle);
    //glutTimerFunc(5000, time, 12);

    atualPosicaoSlider1_X = (V01 - 25)/12.5;
    translate_slider_local_x = -26.0;
    translate_slider_local_y = 70.0;

    atualPosicaoSlider2_X = (V02 - 25)/12.5;
    translate_slider2_local_x = 26.0;
    translate_slider2_local_y = 70.0;

    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);

    //Inicia tratamento de eventos
    glutMainLoop();

    return 0;
}
