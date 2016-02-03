#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <iostream>
#include <cmath>
#include "sdlglutils.h"
using namespace std;
namespace{
    void MultMatrix (GLdouble m[16]);

    void  Rotate (double theta,double x, double y, double z)
    {
        theta = theta * (M_PI/180);
        GLdouble m[16] = {x*x*(1 - cos(theta)) + cos (theta),x*y*(1 - cos(theta)) -z*sin(theta),x*z*(1 - cos(theta)) +y*sin(theta),0,
                          x*y*(1 - cos(theta)) +z*sin(theta),y*y*(1 - cos(theta)) + cos (theta),y*z*(1 - cos(theta)) -x*sin(theta),0,
                          x*z*(1 - cos(theta)) -y*sin(theta),y*z*(1 - cos(theta)) +x*sin(theta),z*z*(1 - cos(theta)) + cos (theta),0,
                          0,                       0,                            0,                          1};
        MultMatrix(m);
    }
    void  Translate (double x, double y, double z)
    {
        GLdouble m[16] ={1,0,0,0,
                         0,1,0,0,
                         0,0,1,0,
                         x,y,z,1};
        MultMatrix(m);
    }

    void MultMatrix (GLdouble m[16])
    {
        GLdouble CurrentMatrix[16];
        glGetDoublev(GL_MODELVIEW_MATRIX,CurrentMatrix);

        GLdouble Result[16];
        int IndiceCourant = 0;
        for (int j(0); j < 4 ;++j)
        {
            for (int i(0); i < 4; ++i)
            {
                Result[IndiceCourant] = CurrentMatrix[i]*m[4*j] + CurrentMatrix[i+4]*m[4*j+1] + CurrentMatrix[i+8]*m[4*j+2] + CurrentMatrix[i+12]*m[4*j+3];
                ++IndiceCourant;
            }
        }
        glLoadMatrixd(Result);
    }
    void Init (int y, int z)
    {
        int MatSpec [4] = {1,1,1,1};
        int position [4] = {-3,0,0,2};



        glMaterialiv(GL_FRONT_AND_BACK,GL_SPECULAR,MatSpec);
        glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,100);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);


        glLoadIdentity();

        glLightiv(GL_LIGHT0,GL_POSITION,position);

        gluPerspective(70,(double)640/480,1,1000);

        gluLookAt(1.5, y, z, 0, 0, 0, 0, 1, 0); // place la camera

    }

    void paint (GLuint Texture, float Diametre)
    {
        GLUquadric* params = gluNewQuadric();
        glBindTexture(GL_TEXTURE_2D,Texture);

        gluQuadricTexture(params,GL_TRUE);

        gluSphere(params,Diametre,20,20);
    }

    void Principal ()
    {

/******************Declaration des variables utilisées ******************************/
    double AngleRotationTerreSurY = 0;
    double RotationLune = 0;
    double RotationPlanete = 0;
    float x;
    float y;
    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time,ellapsed_time;
    GLuint texture1;
    GLuint texture2;
    GLuint texture3;
    GLuint texture4;
    GLuint texture5;
    GLuint texture6;
    GLuint texture7;
    GLuint texture8;
    GLuint texture9;
    float Mouv = 0;
    Uint32 start_time; //nouvelle variable

/***********************************************************************************/

/********Debut du code minimal pour ouvrir une fenetre SDL *************************/

    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetCaption("SDL GL Application", NULL);
    SDL_SetVideoMode(840, 680, 32, SDL_OPENGL);

    bool continuer = true;
    SDL_Event event;
/**********************************************************************************/

/*********Activation des textures, de la profondeur, des lumières******************/
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
/*********************************************************************************/

/**************************Mise en place des textures*****************************/
    texture1 = loadTexture("terre.jpg");
    texture2 = loadTexture("lune.jpg");
    texture3 = loadTexture("soleil.jpg");
    texture4 = loadTexture("venus.jpeg");
    texture5 = loadTexture("Mars.jpeg");
    texture6 = loadTexture("jupiter.jpg");
    texture7 = loadTexture("Saturne.jpg");
    texture8 = loadTexture("uranus.jpg");
    texture9 = loadTexture("neptune.jpg");
/********************************************************************************/
    while (continuer)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {

/**************Permet de quitter le programme***********************************/
             case SDL_QUIT:
                exit(0);
                break;
/******************************************************************************/

/***************Evenement si on clique sur une touche**************************/
            case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
/***************Evenement si on clique sur la flèche gauche********************/
                case SDLK_LEFT:
/*******************Debut de programme terre lune ****************************/
                    Init(1.5,5);

/******************************Terre*****************************************/
                    glPushMatrix();
                    Translate(0,0,0);
                    Rotate(AngleRotationTerreSurY, 0, 1, 0);


                    Rotate(67, 1, 0, 0);
/** rotation de la terre pour placer la jpeg a la bonne place + inclinaison de l'axe de 23° **/
                    paint (texture1, 1.5);
                    glPopMatrix();
/****************************************************************************/

/**********************Rotation eclipstique*********************************/
                    x=0.5*cos(RotationLune * (M_PI/180));
                    y=0.5*sin(RotationLune * (M_PI/180));
/***************************************************************************/

/***************************Lune*******************************************/
                    glPushMatrix();
                    Rotate(RotationLune,0,1,0);
                    Translate(x,y,3);
                    Rotate(RotationLune/27,0,1,0);
                    paint(texture2, 0.5);
                    glPopMatrix();
/************************************************************************/

/**********Rotation en fonction du temps de l'exécution du programme ****/
                    current_time = SDL_GetTicks();
                    ellapsed_time = current_time - last_time;
                    last_time = current_time;

                    AngleRotationTerreSurY += 0.2 * ellapsed_time;
                    RotationLune += (0.2/27) * ellapsed_time; // car la lune a une periode de revolution de 27 jour, donc 27 fois moins rapide que la terre

                    if (ellapsed_time < 10)
                    {
                        SDL_Delay(10 - ellapsed_time);
                    }
/***********************************************************************/
                    SDL_GL_SwapBuffers();
                    break;
/******Evenement si on clique sur la flèche droite*********************/
            case SDLK_RIGHT:
                Init(50,90);
/***********************Soleil****************************************/
                glPushMatrix();

                Translate(0,Mouv,0);

                paint(texture3, 8);
                glPopMatrix();
/*******************************************************************/

                x=0.5*cos(RotationPlanete * (M_PI/180));
                y=0.5*sin(RotationPlanete * (M_PI/180));

/************************Mercure**********************************/
                glPushMatrix();
                Translate(0,Mouv,0);
                Rotate(RotationPlanete*1.5,cos(5),1,0);
                Translate(x,y,10);
                Rotate(RotationPlanete,0,1,0);
                paint (texture2, 1.5);
                glPopMatrix();

/***************************************************************/

/************************Vénus*********************************/
                glPushMatrix();
                Translate(0,Mouv,0);
                Rotate(RotationPlanete,cos(1.5),1,0);
                Translate(x,y,15);
                paint (texture4, 2);
                glPopMatrix();
/*************************************************************/


/************************Terre*********************************/
                glPushMatrix();
                Translate(0,Mouv,0);
                Rotate(RotationPlanete/2,0,1,0);
                Translate(x,y,21);
                paint (texture1, 2.5);
                Rotate(67, 1, 0, 0);
/** rotation de la terre pour placer la jpeg a la bonne place + inclinaison de l'axe de 23° **/
                glPopMatrix();
/*************************************************************/

/************************Mars*********************************/
                glPushMatrix();
                Translate(0,Mouv,0);
                Rotate(RotationPlanete/3,0,1,0);
                Translate(x,y,26);
                paint (texture5, 2.5);
                glPopMatrix();
/*************************************************************/




/************************Jupiter*********************************/
                glPushMatrix();
                Translate(0,Mouv,0);
                Rotate(RotationPlanete/4,0,1,0);
                Translate(x,y,45);
                Rotate(90, 1, 0, 0);
                paint (texture6, 4);
                glPopMatrix();
/**************************************************************/

/*********************Saturne*********************************/
                glPushMatrix();
                Translate(0,Mouv,0);
                Rotate(RotationPlanete/5,0,1,0);
                Translate(x,y,55);
                Rotate(90, 1, 0, 0);
                paint (texture7, 3.2);
                glPopMatrix();
/*********************Anneaux********************************/
                glPushMatrix();
                Translate(0,Mouv,0);
                Rotate(RotationPlanete/5,0,1,0);
                Translate(x,y,55);
                Rotate(90, 1, 0, 0);
                GLUquadric* params = gluNewQuadric();
                gluDisk(params,4.5, 6, 20, 10);
                glPopMatrix();
/**********************************************************/

/**********************************************************/


/*********************Uranus*******************************/
                glPushMatrix();
                Translate(0,Mouv,0);
                Rotate(RotationPlanete/7,0,1,0);
                Translate(x,y,68);
                paint (texture8, 2.8);
                glPopMatrix();
/**********************************************************/


/*********************Neptune*******************************/
                glPushMatrix();
                Translate(0,Mouv,0);
                Rotate(RotationPlanete/10,0,1,0);
                Translate(x,y,73);
                paint (texture9, 2.8);
                glPopMatrix();
/**********************************************************/

                current_time = SDL_GetTicks();
                ellapsed_time = current_time - last_time;
                last_time = current_time;

                RotationPlanete += 0.2 * ellapsed_time; // car la lune a une periode de revolution de 27 jour, donc 27 fois moins rapide que la terre
                Mouv += 0.03;
                if (ellapsed_time < 10)
                {
                    SDL_Delay(10 - ellapsed_time);
                }

                SDL_GL_SwapBuffers();
                break;
            }
        }
    }


    SDL_Quit();

    }

}
int main(int argc, char *argv[])
{
    Principal();
    return 0;
}
