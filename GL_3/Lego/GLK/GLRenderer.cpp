#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
#include "Tex.h"


//#pragma comment(lib, "GL\\glut32.lib")

const double M_PI = 3.141592653589793238463;

CGLRenderer::CGLRenderer(void)
{
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(3.0, 10.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	DrawLegoWall(3.0, 1.0, 2.0);

	glFlush();
	SwapBuffers(pDC->m_hDC);

	//---------------------------------	
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	double aspect = (double)w / (double)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, aspect, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawBox(double x, double y, double z)
{
	glBegin(GL_QUADS);

		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-x / 2, -y / 2, z / 2);//0
		glVertex3f(x / 2, -y / 2, z / 2);//1
		glVertex3f(x / 2, y / 2, z / 2);//2
		glVertex3f(-x / 2, y / 2, z / 2);//3

		glNormal3f(1.0, 0.0, 0.0);
		glVertex3f(x / 2, -y / 2, z / 2);//1
		glVertex3f(x / 2, -y / 2, -z / 2);//4
		glVertex3f(x / 2, y / 2, -z / 2);//7
		glVertex3f(x / 2, y / 2, z / 2);//2

		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(x / 2, -y / 2, -z / 2);//4
		glVertex3f(-x / 2, -y / 2, -z / 2);//5
		glVertex3f(-x / 2, y / 2, -z / 2);//6
		glVertex3f(x / 2, y / 2, -z / 2);//7
		
		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3f(-x / 2, -y / 2, -z / 2);//5
		glVertex3f(-x / 2, -y / 2, z / 2);//0
		glVertex3f(-x / 2, y / 2, z / 2);//3
		glVertex3f(-x / 2, y / 2, -z / 2);//6

		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(-x / 2, y / 2, z / 2);//3
		glVertex3f(x / 2, y / 2, z / 2);//2		
		glVertex3f(x / 2, y / 2, -z / 2);//7
		glVertex3f(-x / 2, y / 2, -z / 2);//6

		glNormal3f(0.0, -1.0, 0.0);
		glVertex3f(-x / 2, -y / 2, z / 2);//0
		glVertex3f(x / 2, -y / 2, z / 2);//1
		glVertex3f(x / 2, -y / 2, -z / 2);//4
		glVertex3f(-x / 2, -y / 2, -z / 2);//5

	glEnd();
}

void CGLRenderer::DrawCylinder(double r, double h, int no)
{
	glColor3f(1.0, 0.5, 0.1);

	//telo
	glBegin(GL_QUAD_STRIP);
		for (float angle = 0; angle <= 360; angle += (360 / no))
		{
			glNormal3f(cos(toRad(angle)), 0.0, sin(toRad(angle)));
			glVertex3f(r * cos(toRad(angle)), h / 2, r*sin(toRad(angle)));
			glVertex3f(r * cos(toRad(angle)), -h / 2, r*sin(toRad(angle)));
		}
	glEnd();

	//osnova gore
	glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, h / 2, 0.0);
		for (float angle = 0; angle <= 360; angle += (360 / no))
		{
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(r * cos(toRad(angle)), h / 2, r * sin(toRad(angle)));
		}
	glEnd();
}

void CGLRenderer::DrawLegoBox(double x, double y, double z)
{
	DrawBox(x, y, z);

	float moveX = x / 3;	
	float moveZ = z / 2;
	
	float cylY = (y / 2) + (0.15 * y);
	glPushMatrix();
		glTranslatef(-moveX, cylY, -moveZ / 2);
		DrawCylinder(0.15 * z, 0.3 * y, 360);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-moveX, cylY, moveZ / 2);
		DrawCylinder(0.15 * z, 0.3 * y, 360);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0, cylY, -moveZ / 2);
		DrawCylinder(0.15 * z, 0.3 * y, 360);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0, cylY, moveZ / 2);
		DrawCylinder(0.15 * z, 0.3 * y, 360);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(moveZ, cylY, -moveZ / 2);
		DrawCylinder(0.15 * z, 0.3 * y, 360);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(moveZ, cylY, moveZ / 2);
		DrawCylinder(0.15 * z, 0.3 * y, 360);
	glPopMatrix();
}

void CGLRenderer::DrawLegoWall(float x, float y, float z)
{
	float moveX = x / 3;
	float moveZ = z / 2;
	
	glColor3f(0.1,0.1,0.1);
	glPushMatrix();
		DrawLegoBox(x, y, z);
	glPopMatrix();

	glColor3f(0.2, 0.5, 0.3);
	glPushMatrix();
		glTranslatef(-moveX - moveX - (moveZ / 2), 0.0, -(moveZ / 2));
		glRotatef(90, 0.0, 1.0, 0.0);
		DrawLegoBox(x, y, z);
	glPopMatrix();
	
	glColor3f(0.89, 0.89, 0.89);
	glPushMatrix();
		glTranslatef(-moveX - moveX, y, 0.0);
		DrawLegoBox(x, y, z);
	glPopMatrix();
}

float CGLRenderer::toRad(float angle)
{
	return (angle * M_PI) / 180;
}

float CGLRenderer::toDeg(float angle) 
{
	return (angle * 180) / M_PI;
}


void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

double CGLRenderer::R(double a, double alpha, int n, double h)
{
	alpha *= 0.01745329251994329577;
	double PI2 = 6.2831853071795864769;
	double alpha1 = (PI2 / (2 * n) - alpha);
	double alpha2;
	double R1, R;

	R = a / (2 * sin(alpha1 / 2.0));
	double delta = R / 4.;
	R1 = R;
	do {
		alpha1 = 2 * asin(a / (2 * R1));
		alpha2 = 2 * asin(a / (2 * (R1 - h)));
		if ((alpha1 + alpha2) > ((PI2 / n) - (2 * alpha)))
			R1 += delta;
		else
			R1 -= delta;
		delta /= 2.0;
	} while (delta > 1e-6);
	return R;
}

void CGLRenderer::NormCrossProd(double x1, double y1, double z1, double x2, double y2, double z2, double& x, double& y, double& z)
{
	x = y1 * z2 - z1 * y2;
	y = z1 * x2 - x1 * z2;
	z = x1 * y2 - y1 * x2;
	double d = sqrt(x*x + y*y + z*z);
	if (d != 0.0)
	{
		x /= d; y /= d; z /= d;
	}
}
