#include<stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.14159265

double dt = 0.01, t = 0, g = 9.8, tai_z = 0, bvx1 = 0, bvx2 = 0, bvy1 = 0, bvy2 = 0;
   int i=0, k=0, colm_x = 0, colm_y = 0;
   
struct ball{
      int floor_y, floor_x, wall_x, Col_y, Col_x, colm_x, not_det;
      double r;
      double rx;
      double ry;
      double mem_y;
      double m;
      double f_x;
      double f_y;
      GLdouble x;
      GLdouble y;
      GLdouble tht;
      double dx;
      double dy;
      double dtht;
      double ddx;
      double ddy;
      double ddtht;
      double I;
};

struct wall{
      GLint x1, x2, y1, y2;
      GLint bw_xl, bw_yup, bw_xr, bw_yun;
};

struct wall w1[6] = {sizeof(w1)};

struct ball b1[40] = {sizeof(b1)};


void display(void)
{  
   
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
  for(k=0;k<4;k++){
  glPushMatrix();
  	glRectf( w1[k].x1, w1[k].y1, w1[k].x2, w1[k].y2 );
  glPopMatrix();
   }
  glColor3f(1.0, 1.0, 0.0);
  for(k=0;k<2;k++){
  for(i=0;i<36;i++){
    b1[i].rx = b1[k].r * cos(i*PI/18);
    b1[i].ry = b1[k].r * sin(i*PI/18);
   }
  glPushMatrix();
  	glTranslatef(b1[k].x, b1[k].y, 0.0);
  	glRotatef(b1[k].tht, 0.0, 0.0, 1.0);
   glBegin(GL_POLYGON);
   for(i=0;i<36;i++)
    glVertex2d(b1[i].rx, b1[i].ry);
   glEnd();
  glPopMatrix();
  
   }
  glutSwapBuffers();  
}

void simu(void){
for(k=0;k<2;k++){

  w1[k].bw_xl = w1[2].x1 + b1[k].r;
  w1[k].bw_xr = w1[3].x1 - b1[k].r;
  w1[k].bw_yun = w1[0].y2 + b1[k].r;
  w1[k].bw_yup = w1[1].y2 - b1[k].r;
  
  b1[k].mem_y = b1[k].y;
  b1[k].I = 0.4 * b1[k].m * b1[k].r * b1[k].r;
  
  /*Rotating motion op*/
  if(b1[k].y <= w1[k].bw_yun){
    if(b1[k].dx > (b1[k].r * b1[k].dtht)){
      tai_z = -b1[k].f_x * b1[k].r;
       }
    else if(b1[k].dx < (b1[k].r * b1[k].dtht)){
      tai_z = -b1[k].f_x * b1[k].r;
       }
    else if(b1[k].dx == 0){
     tai_z = 0;
     }
   }
  b1[k].ddtht = tai_z / b1[k].I;
  b1[k].dtht = b1[k].dtht + b1[k].ddtht * dt;
  b1[k].tht = b1[k].tht + b1[k].dtht * dt + b1[k].ddtht * dt * dt / 2.0;
  
  /*Main motion op*/
  if(b1[k].wall_x == 0 && b1[k].Col_y == 0 && b1[k].Col_x == 0){
    if((b1[k].mem_y <= w1[k].bw_yun) && (b1[k].dy < 0)){
      b1[k].y = w1[k].bw_yun;
      b1[k].dy = 0;
     }
     
     /*Y force (air)*/
    if(b1[k].y > w1[k].bw_yun){
    b1[k].f_y = -g * b1[k].m;
    b1[k].f_x = 0;
     }
    else{ b1[k].f_y = 0; }
    
    /*Y motion op*/
    b1[k].ddy = b1[k].f_y / b1[k].m;
    b1[k].dy = b1[k].dy + b1[k].ddy * dt;
    b1[k].y = b1[k].y + b1[k].dy * dt + b1[k].ddy * dt * dt / 2.0;
    
    /*X motion op*/
    
    if(b1[k].dx > 0){ 
      b1[k].ddx = b1[k].f_x / b1[k].m;
      b1[k].dx = b1[k].dx + b1[k].ddx * dt;
      b1[k].x = b1[k].x + b1[k].dx * dt + b1[k].ddx * dt * dt / 2.0;
      if(b1[k].dx > 0 && b1[k].y <= w1[k].bw_yun){
        b1[k].f_x = -0.01 * g * b1[k].m;
        if(b1[k].dx <= 0){
          b1[k].dx = 0;
          b1[k].f_x = 0;
          }
        }
     }
        
    else if(b1[k].dx < 0){
      b1[k].ddx = b1[k].f_x / b1[k].m;
      b1[k].dx = b1[k].dx + b1[k].ddx * dt;
      b1[k].x = b1[k].x + b1[k].dx * dt + b1[k].ddx * dt * dt / 2.0;
      if(b1[k].dx < 0 && b1[k].y <= w1[k].bw_yun){
        b1[k].f_x = 0.01 * g * b1[k].m;
        if(b1[k].dx >= 0){
          b1[k].dx = 0;
          b1[k].f_x = 0;
          }
        }
     }
     
    else if(b1[k].dx == 0){
      b1[k].ddx = b1[k].f_x / b1[k].m;
      b1[k].dx = b1[k].dx + b1[k].ddx * dt;
      b1[k].x = b1[k].x + b1[k].dx * dt + b1[k].ddx * dt * dt / 2.0;
      }
      
     /*Collision wall detection*/
    if(b1[k].x >= w1[k].bw_xr || b1[k].x <= w1[k].bw_xl){
        b1[k].dx = -0.8 * b1[k].dx;
        b1[k].Col_x = 1;
     }
     
    /*Collision floor or celling detection*/
    if(b1[k].y <= w1[k].bw_yun || b1[k].y >= w1[k].bw_yup){
      b1[k].dy = -0.8 * b1[k].dy;
      b1[k].Col_y = 1;
     }
   }
   
   /*Collision floor or celling*/
  if(b1[k].Col_y == 1){
    b1[k].y = b1[k].y + b1[k].dy * dt;
    b1[k].Col_y = 0;
   }
   
   /*Collision wall*/
  if(b1[k].Col_x == 1){
    b1[k].x = b1[k].x + b1[k].dx * dt;
    b1[k].Col_x = 0;
   }
   
   /*Collision detection*/
   if((hypot(b1[0].x - b1[1].x, b1[0].y - b1[1].y) <= (b1[0].r + b1[1].r)) && (b1[k].colm_x == 0)){
    if(b1[k].not_det == 1){
      b1[k].not_det = 0;
     }
    else{
    b1[k].colm_x = 1;
    bvx1 = b1[0].dx;
    bvx2 = b1[1].dx;
    bvy1 = b1[0].dy;
    bvy2 = b1[1].dy;
     }
    }
}
    
    /*Momentum two spheres*/
   if(b1[0].colm_x == 1 && b1[1].colm_x == 1){
    if(b1[0].x >= (45-b1[k].r) || b1[0].x <= (-45+b1[k].r)){
    b1[1].dx = -0.8 * b1[1].dx;
     }
    else if(b1[1].x >= (45-b1[k].r) || b1[1].x <= (-45+b1[k].r)){
    b1[0].dx = -0.8 * b1[0].dx;
     }
    else{
    b1[1].dx = ((b1[0].m * bvx1 + b1[1].m * bvx2) + (0.8 * b1[0].m * (bvx1 - bvx2))) / (b1[0].m + b1[1].m);
    b1[0].dx = ((b1[0].m * bvx1 + b1[1].m * bvx2) + (0.8 * b1[1].m * (bvx2 - bvx1))) / (b1[0].m + b1[1].m);
     
     }
     
    if(b1[0].y >= (40-b1[k].r) || b1[0].y <= (-40+b1[k].r)){
    b1[1].dy = -0.8 * b1[1].dy;
    
     }
    else if(b1[1].y >= (40-b1[k].r) || b1[1].y <= (-40+b1[k].r)){
    b1[0].dy = -0.8 * b1[0].dy;
     }
    else{
    b1[1].dy = ((b1[0].m * bvy1 + b1[1].m * bvy2) + (0.8 * b1[0].m * (bvy1 - bvy2))) / (b1[0].m + b1[1].m);
    b1[0].dy = ((b1[0].m * bvy1 + b1[1].m * bvy2) + (0.8 * b1[1].m * (bvy2 - bvy1))) / (b1[0].m + b1[1].m);
     }
    b1[0].colm_x = 0;
    b1[1].colm_x = 0;
    b1[0].not_det = 1;
    b1[1].not_det = 1;
   }
  
  
  glutPostRedisplay();

}

void init(void)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
}

void reshape(int w, int h)
{
  glViewport( 0, 0, w, h );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void mouse(int button, int state, int x, int y){
  switch (button) {
     case GLUT_LEFT_BUTTON:
     	if ( state == GLUT_DOWN)
     		glutIdleFunc(NULL);
     	break;
     case GLUT_RIGHT_BUTTON:
		if ( state == GLUT_DOWN)
     		glutIdleFunc(simu);
     	break; 
     default:
     	break; 
  }
}


void keyboard(unsigned char key, int x, int y)
{
  if ( key == '\x1b') exit(0);
}

int main(int argc, char *argv[])
{
  /* init value-------------------*/
  b1[1].x = 0;     /*ボール1のx座標の初期位置*/
  b1[0].x = 5;     /*ボール2のx座標の初期位置*/
  b1[1].y = 30;    /*ボール1のy座標の初期位置*/
  b1[0].y = 39;   /*ボール2のy座標の初期位置*/
  b1[1].dx = 7;    /*ボール1のx座標の初期速度*/
  b1[0].dx = 3;    /*ボール2のx座標の初期速度*/
  b1[1].dy = 3;    /*ボール1のy座標の初期速度*/
  b1[0].dy = 7;    /*ボール2のy座標の初期速度*/
  b1[1].ddx = 0;   /*ボール1のx座標の初期加速度*/
  b1[0].ddx = 0;   /*ボール2のx座標の初期加速度*/
  b1[1].ddy = 0;   /*ボール1のy座標の初期加速度*/
  b1[0].ddy = 0;   /*ボール2のy座標の初期加速度*/
  b1[1].r = 1;     /*ボール1の半径*/
  b1[0].r = 1;     /*ボール2の半径*/
  b1[1].m = 0.5;   /*ボール1の質量*/
  b1[0].m = 0.5;   /*ボール1の質量*/
  w1[0].x1 = -45.0; w1[0].y1 = -45.0; w1[0].x2 = 45.0; w1[0].y2 = -40.0; /* under wall*/
  w1[1].x1 = 45.0; w1[1].y1 = 45.0; w1[1].x2 = -45.0; w1[1].y2 = 40.0;   /*upper wall*/
  w1[2].x1 = -45.0; w1[2].y1 = 45.0; w1[2].x2 = -50.0; w1[2].y2 = -45.0; /*left wall*/
  w1[3].x1 = 45.0; w1[3].y1 = 45.0; w1[3].x2 = 50.0; w1[3].y2 = -45.0;   /*right wall*/
  
   /*------------------------------------*/
   
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (1000, 1000);
  glutInitWindowPosition(100,0);
  glutCreateWindow(argv[0]);
  init();
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutDisplayFunc(display);
    glutMainLoop();
  return 0;
}
