#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "glut.h"
#include <math.h>
using namespace std;

#define GL_BGR 0x80E0
#define PI acos(-1.0)

static float position = 0;

static float px = 0, pz = -5;
static int angle = 0;

GLUquadricObj *g_text; 

static GLuint t_bot, t_sun;


// ����һ��.bmp��ʽ����ͼ����
GLuint loadBMP_custom(const char * imagepath){

    printf("Reading image %s\n", imagepath);

    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    // Actual RGB data
    unsigned char * data;

    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file)                              {printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0;}
    // Read the header, i.e. the 54 first bytes

    // If less than 54 bytes are read, problem
    if ( fread(header, 1, 54, file)!=54 ){ 
        printf("Not a correct BMP file\n");
        return 0;
    }
    // A BMP files always begins with "BM"
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return 0;
    }
    // Make sure this is a 24bpp file
    if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return 0;}
    if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return 0;}

    // Read the information about the image
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);

    // Everything is in memory now, the file wan be closed
    fclose (file);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // OpenGL has now copied the data. Free our own version
    delete [] data;

    // Poor filtering, or ...
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //�����˲�Ϊ�����˲�
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);    //�����˲�

    // Return the ID of the texture we just created
    return textureID;
}

void init(void) 
{
	
    // �����Դ ���
    float light_pos[] = {0, 10, -15, 1.0};
    float light_Ka[] = {1.0, 0.5, 0.0, 1.0};
    float light_Kd[] = {1.0, 0.5, 0.0, 1.0};
    float light_Ks[] = {1.0, 0.5, 0.0, 1.0};
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);    //������Ȳ���
    glShadeModel(GL_SMOOTH);    //��ɫ����Ч��
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);   //��Դλ��
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);     //���ʵĻ�����ɫ
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);     //���ʵ�ɢ����ɫ
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);    //�ʵľ��淴����ɫ

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_TEXTURE_2D);

	g_text = gluNewQuadric();
	t_bot = loadBMP_custom("bot.bmp");
	t_sun = loadBMP_custom("sun.bmp");
}


// ����Բ��R���뾶��x��y��z��Բ�����꣬scaleX��scaleY��scaleZ��xyz������ı�����ѹ����Բ����
void drawEllipsoid(double R, double x, double y,double z, double scaleX, double scaleY, double scaleZ)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(scaleX, scaleY, scaleZ) ;
	gluSphere(g_text, R, 60, 60);	
	//glutSolidSphere(R, 60, 60);
	glPopMatrix();
}

// ����Ƕ�
int calAngle(int a)
{
	return a + 90;
}

// �Ƕ�ת����
double angleToRadians(double a)
{
	return a / 180 * PI;
}

// �����ϱ���ת�Ƕ�
double calAngleArmUp(double a)
{
	int t;
	// ���С��0����ͨ��������ʹ֮��Ϊ����
	if (a < 0) {
		t = -a / 180;
		a += 180 * (t + 1);
	}
	// ��aת������һ����
	t = int(a / 180);
	a = a - t * 180;
	t = a / 45;
	a = a - t * 45;
	// �������ں����������Ӧ�Ƕȡ�
	if (t == 0) {
		return a;
	} else if (t == 1) {
		return 45 - a;
	} else if (t == 2) {
		return -a;
	} else {
		return -45 + a;
	}
}

// �����±���ת�Ƕ�
double calAngleArmDown(double a)
{
	int t;
	if (a < 0) {
		t = -a / 180;
		a += 180 * (t + 1);
	}
	t = int(a / 180);
	a = a - t * 180;
	t = a / 45;
	a = a - t * 45;
	if (t == 0) {
		return a * 2;
	} else if (t == 1) {
		return (45 - a) * 2;
	} else if (t == 2) {
		return 0;
	} else {
		return 0;
	}
}

// ���������ת�Ƕ�
double calAngleLegUp(double a)
{
	int t;
	if (a < 0) {
		t = -a / 180;
		a += 180 * (t + 1);
	}
	t = int(a / 180);
	a = a - t * 180;
	t = a / 45;
	a = a - t * 45;
	if (t == 0) {
		return a;
	} else if (t == 1) {
		return 45 - a;
	} else {
		return 0;
	}
}

// ����С����ת�Ƕ�
double calAngleLegDown(double a)
{
	int t;
	if (a < 0) {
		t = -a / 180;
		a += 180 * (t + 1);
	}
	t = int(a / 180);
	a = a - t * 180;
	t = a / 45;
	a = a - t * 45;
	if (t == 0) {
		return -a * 2;
	} else if (t == 1) {
		return - (45 - a) * 2;
	} else {
		return 0;
	}
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gluQuadricTexture(g_text,GLU_TRUE);              //������������  
	gluQuadricDrawStyle(g_text,GLU_FILL);			 //�������

	// �����ɫ
    glColor3f(1.0, 1.0, 1.0); 
    glLoadIdentity(); 
	gluLookAt (0.0, 0.0, -20, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, t_sun);
		// ��̫�����ο�����գ�
		drawEllipsoid(1, 0, 10, 15, 1, 1, 1);
		glBindTexture(GL_TEXTURE_2D, t_bot);
		glTranslatef(-px, 0, pz);
		glRotatef(angle, 0.0, 1.0, 0.0);
		// ͷ
		drawEllipsoid(0.75, 0, 3.75, 0, 1, 1, 1);
		// ��
		drawEllipsoid(1.5, 0, 3, 0, 1, 0.2, 0.2);
		
		// �����±�
		glPushMatrix();
			// ���ϱ����Ϸ�ƽ�Ƶ�Բ��
			glTranslatef(-1.5, 3, 0);
			// ��ת
			glRotatef(-calAngleArmUp(position), 1.0, 0.0, 0.0);
			// �����ϱ�
			drawEllipsoid(1, 0, -1, 0, 0.2, 1, 0.2);
			glPushMatrix();
				// ���±����Ϸ�ƽ�Ƶ�Բ��
				glTranslatef(0, -2, 0);
				// ��ת
				glRotatef(-calAngleArmDown(position), 1.0, 0.0, 0.0);
				// �����±�
				drawEllipsoid(1, 0, -1, 0, 0.2, 1, 0.2);
			glPopMatrix();
		glPopMatrix();
		
		// �����±�
		glPushMatrix();
			glTranslatef(1.5, 3, 0);
			glRotatef(-calAngleArmUp(position + 90), 1.0, 0.0, 0.0);
			drawEllipsoid(1, 0, -1, 0, 0.2, 1, 0.2);
			glPushMatrix();
				glTranslatef(0, -2, 0);
				glRotatef(-calAngleArmDown(position + 90), 1.0, 0.0, 0.0);
				drawEllipsoid(1, 0, -1, 0, 0.2, 1, 0.2);
			glPopMatrix();
		glPopMatrix();

		// ��������
		glPushMatrix();
			// �Ҵ������Ϸ�ƽ�Ƶ�Բ��
			glTranslatef(-1, 0, 0);
			// ��ת
			glRotatef(-calAngleLegUp(position + 90), 1.0, 0.0, 0.0);
			// ���Ҵ���
			drawEllipsoid(1, 0, -1, 0, 0.2, 1, 0.2);
			glPushMatrix();
				// ��С�����Ϸ�ƽ�Ƶ�Բ��
				glTranslatef(0, -2, 0);
				// ��ת
				glRotatef(-calAngleLegDown(position + 90), 1.0, 0.0, 0.0);
				// ����С��
				drawEllipsoid(1, 0, -1, 0, 0.2, 1, 0.2);
			glPopMatrix();
		glPopMatrix();

		// ��������
		glPushMatrix();
			glTranslatef(1, 0, 0);
			glRotatef(-calAngleLegUp(position), 1.0, 0.0, 0.0);
			drawEllipsoid(1, 0, -1, 0, 0.2, 1, 0.2);
			glPushMatrix();
				glTranslatef(0, -2, 0);
				glRotatef(-calAngleLegDown(position), 1.0, 0.0, 0.0);
				drawEllipsoid(1, 0, -1, 0, 0.2, 1, 0.2);
			glPopMatrix();
		glPopMatrix();

		// ����
		drawEllipsoid(1.5, 0, 1.5, 0, 0.4, 1, 0.4);
		// ��
		drawEllipsoid(1, 0, 0, 0, 1, 0.4, 0.4);

	glPopMatrix();
    glutSwapBuffers();
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
}

void debug() {
	printf("angle = %d\n", angle);
	printf("px pz = %lf %lf\n", px, pz);
	printf("position = %lf\n", position);
}

void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 'w':
		case 'W':
			position = position + 2;
			pz += 0.1 * sin(angleToRadians(calAngle(angle)));
			px += 0.1 * cos(angleToRadians(calAngle(angle)));
			debug();
			glutPostRedisplay();
			break;
        case 's':
		case 'S':
			position = position - 2;
			pz -= 0.1 * sin(angleToRadians(calAngle(angle)));
			px -= 0.1 * cos(angleToRadians(calAngle(angle)));
			debug();
			glutPostRedisplay();
			glutPostRedisplay();
			break;
        case 'a':
		case 'A':
			angle = (angle + 20) % 360;
			glutPostRedisplay();
			break;
        case 'd':
		case 'D':
			angle = (angle - 20) % 360;
			glutPostRedisplay();
			break;
        case 'm':
	        break;
        case 27:
			exit(0);
			break;
        default:
			break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}