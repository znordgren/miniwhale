#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

#include "texload.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define NUM_PATTERNS 32
#define FLOOR_FILE "floor.rgb"

enum {
	PASS_NORMAL, PASS_CAUSTIC
};

enum {
	M_POSITIONAL, M_DIRECTIONAL, M_GREENISH_LIGHT, M_WHITE_LIGHT,
	M_NO_CAUSTICS, M_WITH_CAUSTICS,
	M_INCREASE_RIPPLE_SIZE, M_DECREASE_RIPPLE_SIZE
};

static GLboolean HaveTexObj = GL_FALSE;
static int reportSpeed = 0;
static GLfloat step = 1;
static int dinoDisplayList;
static GLfloat causticScale = 1.0;

static GLfloat lightAmbient[] = { 0.5, 0.5, 0.5, 1.0 };
static GLfloat lightDiffuse[] = { 0.5, 0.5, 0.5, 0.5 };
static GLfloat lightSpecular[] = { 0.5, 0.5, 0.5, 0.5 };
static GLfloat lightPosition[4];
static GLfloat lightDiffuseColor[] = { 1.0, 1.5, 1.0, 1.0 }; 
static GLfloat defaultDiffuseMaterial[] = { 0.8, 0.8, 0.8, 1.0 };

static int directionalLight = 1;
static int showCaustics = 1, causticMotion = 1;
static int useMipmaps = 1;
static int currentCaustic = 0;

static float lightAngle = 40.0, lightHeight = 30;
static GLfloat angle = 0;   /* in degrees */
static GLfloat angle2 = 0;   /* in degrees */

static int tankWidth = 40;
static int tankDepth = 20;
static int tankHeight = 20;

static int moving = 0, startx, starty;
static GLfloat whalePosition[3] = { 0.0f,0.0f,0.0f };
static GLfloat wayPoints[3][4];



static GLfloat floorVertices[4][3] = {
  { -40.0, -10.0, 20.0 },
  { 40.0, -10.0, 20.0 },
  { 40.0, -10.0, -20.0 },
  { -40.0, -10.0, -20.0 },
};

GLfloat getCurve(const GLfloat t, const GLfloat x[4])
{
	GLfloat xt = pow(1.0f - t, 3.0f) * x[0] + 3.0f * t * pow(1.0f - t, 2.0f) * x[1] +
		3.0f * pow(t, 2.0f) * (1.0f - t) * x[2] + pow(t, 3.0f) * x[3];
	return xt;
}

void generateNewWaypoints()
{
	wayPoints[0][0] = whalePosition[0]; wayPoints[0][1] = whalePosition[1]; wayPoints[0][2] = whalePosition[2];
	wayPoints[1][0] = wayPoints[1][0] * 0.3 + wayPoints[2][0] * 0.7;
	wayPoints[1][1] = wayPoints[1][1] * 0.3 + wayPoints[2][1] * 0.7;
	wayPoints[1][2] = wayPoints[1][2] * 0.3 + wayPoints[2][2] * 0.7;
	wayPoints[2][0] = wayPoints[2][0] * 0.3 + wayPoints[3][0] * 0.6 + (GLfloat)(rand() % tankWidth * 2  - tankWidth)*0.1;
	wayPoints[2][1] = wayPoints[2][1] * 0.3 + wayPoints[3][1] * 0.6 + (GLfloat)(rand() % tankHeight * 2 - tankHeight + 10)*0.1;
	wayPoints[2][2] = wayPoints[2][2] * 0.3 + wayPoints[3][2] * 0.6 + (GLfloat)(rand() % tankDepth * 2  - tankDepth)*0.1;
	wayPoints[3][0] = wayPoints[3][0] * 0.3 + (GLfloat)(rand() % tankWidth * 2 - tankWidth)*0.7;
	wayPoints[3][1] = wayPoints[3][1] * 0.3 + (GLfloat)(rand() % tankHeight * 2 - tankHeight + 10)*0.7;
	wayPoints[3][2] = wayPoints[3][2] * 0.3 + (GLfloat)(rand() % tankDepth * 2 - tankDepth)*0.7;
}

void updateWhalePosition()
{
	GLfloat bezx[4] = { wayPoints[0][0],wayPoints[1][0],wayPoints[2][0],wayPoints[3][0] };
	GLfloat bezy[4] = { wayPoints[0][1],wayPoints[1][1],wayPoints[2][1],wayPoints[3][1] };
	GLfloat bezz[4] = { wayPoints[0][2],wayPoints[1][2],wayPoints[2][2],wayPoints[3][2] };

	step += 0.0005f;
	if (step >= 0.3f)
	{
		generateNewWaypoints();
		step = 0;
	}
	whalePosition[0] = getCurve(step, bezx);
	whalePosition[1] = getCurve(step, bezy);
	whalePosition[2] = getCurve(step, bezz);
}

GLfloat mp(const GLfloat x0, const GLfloat x1)
{
	GLfloat xt = (x0 + (x1 - x0) * step);
	return xt;
}

static void drawFloor(int pass)
{
	if (pass == PASS_NORMAL) {
		if (HaveTexObj)
			glBindTexture(GL_TEXTURE_2D, 100);
		else
			glCallList(100);
	}

	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3fv(floorVertices[0]);
	glTexCoord2f(0.0, 4.0);
	glVertex3fv(floorVertices[1]);
	glTexCoord2f(8.0, 4.0);
	glVertex3fv(floorVertices[2]);
	glTexCoord2f(8.0, 0.0);
	glVertex3fv(floorVertices[3]);
	glEnd();
}

void drawWhale(int pass)
{
	if (pass == PASS_NORMAL) {
		glDisable(GL_TEXTURE_2D);
	}
	glColor3f(1, 1, 0);
	glLineWidth(10.0f);
	glPushMatrix();
	glTranslatef(0.0, 10.0, 0.0);
	glScalef(40.0f, 20.0f, 20.0f);
	glutWireCube(2.0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultDiffuseMaterial);
	glPopMatrix();


	glPushMatrix();

	glTranslatef(whalePosition[0], whalePosition[1], whalePosition[2]);
	glutSolidSphere(2.0, 12, 12);

	glPopMatrix();

	if (pass == PASS_NORMAL) {
		glEnable(GL_TEXTURE_2D);
	}
}

void drawScene(int pass)
{

	updateWhalePosition();

	GLfloat sPlane[4] = { 0.05, 0.03, 0.0, 0.0 };
	GLfloat tPlane[4] = { 0.0, 0.03, 0.05, 0.0 };

	// The causticScale determines how large the caustic "ripples" will be.

	sPlane[0] = 0.05 * causticScale;
	sPlane[1] = 0.03 * causticScale;

	tPlane[1] = 0.03 * causticScale;
	tPlane[2] = 0.05 * causticScale;

	if (pass == PASS_CAUSTIC) {
		glColor3f(1.0, 1.0, 1.0);
		glDisable(GL_LIGHTING);

		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
		glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);

		if (HaveTexObj) {
			glBindTexture(GL_TEXTURE_2D, currentCaustic + 1);
		}
		else {
			glCallList(currentCaustic + 101);
		}
	}

	drawFloor(pass);
	drawWhale(pass);

	if (pass == PASS_CAUSTIC) {
		glEnable(GL_LIGHTING);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
}

void display(void)
{
	int startTime, endTime;

	if (reportSpeed) {
		startTime = glutGet(GLUT_ELAPSED_TIME);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// position the light source.
	lightPosition[0] = 12 * cos(lightAngle);
	lightPosition[1] = lightHeight;
	lightPosition[2] = 12 * sin(lightAngle);
	if (directionalLight) {
		lightPosition[3] = 0.0;
	}
	else {
		lightPosition[3] = 1.0;
	}

	glPushMatrix();

	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	lightPosition[3] = -lightPosition[3];
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
	glEnable(GL_LIGHT1);

	// Normal pass rendering the scene
	drawScene(PASS_NORMAL);

	if (showCaustics) {
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);

		// Multiply the source color (from the caustic luminance texture) with the previous color from the normal pass.
		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
		glEnable(GL_BLEND);
		drawScene(PASS_CAUSTIC);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}
	glPopMatrix();

	glutSwapBuffers();

	if (reportSpeed) {
		glFinish();
		endTime = glutGet(GLUT_ELAPSED_TIME);
		printf("Speed %.3g frames/sec (%d ms)\n", 1000.0 / (endTime - startTime), endTime - startTime);
		fflush(stdout);
	}
}

void
idle(void)
{
	// Advance the caustic pattern.
	currentCaustic = (currentCaustic++) % NUM_PATTERNS;
	glutPostRedisplay();
}

void
updateIdleFunc(void)
{
	// Must be both displaying the caustic patterns and have the caustics in rippling motion to need an idle callback.
	if (showCaustics && causticMotion) {
		glutIdleFunc(idle);
	}
	else {
		glutIdleFunc(NULL);
	}
}

void
visible(int vis)
{
	// Stop the animation when the window is not visible.
	if (vis == GLUT_VISIBLE)
		updateIdleFunc();
	else
		glutIdleFunc(NULL);
}

static void keyboard(unsigned char c, int x, int y)
{
	switch (c) {
	case 27:  // Escape quits.
		exit(0);
		break;
	case 'R':  // Simple benchmarking
	case 'r':
		reportSpeed = !reportSpeed;
		break;
	case ' ':  // Spacebar toggles caustic rippling motion.
		causticMotion = !causticMotion;
		updateIdleFunc();
		break;
	}
}

void loadCaustics()
{
	// Load the caustic ripple textures.
	int width, height;
	int i;
	GLubyte *imageData;
	printf("loading caustics:");
	for (i = 0; i < NUM_PATTERNS; i++) {
		char filename[80];

		sprintf_s(filename, sizeof(filename), "caust%02d.bw", i);
		printf(" %d", i);
		fflush(stdout);
		imageData = read_alpha_texture(filename, &width, &height);
		if (imageData == NULL) {
			fprintf(stderr, "\n%s: could not load image file\n", filename);
			exit(1);
		}
		if (HaveTexObj)
			glBindTexture(GL_TEXTURE_2D, i + 1);
		else
			glNewList(i + 101, GL_COMPILE);
		if (useMipmaps) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_LUMINANCE, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageData);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, height, width, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageData);
		}
		free(imageData);
		if (!HaveTexObj)
			glEndList();
	}
	printf(".\n");
}

void loadFloorRGB()
{
	/* Load an RGB file for the floor texture. */
	int width, height;
	char floorFilename[] = "floor.rgb";
	GLubyte *imageData;
	printf("loading RGB textures: floor");
	fflush(stdout);
	imageData = read_rgb_texture(floorFilename, &width, &height);
	if (imageData == NULL) {
		fprintf(stderr, "%s: could not load image file\n", FLOOR_FILE);
		exit(1);
	}
	printf(".\n");

	if (HaveTexObj)
		glBindTexture(GL_TEXTURE_2D, 100);
	else
		glNewList(100, GL_COMPILE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	if (useMipmaps) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB,
			GL_UNSIGNED_BYTE, imageData);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, imageData);
	}
	free(imageData);
	if (!HaveTexObj)
		glEndList();
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(700, 500);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("miniwhale");

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	loadCaustics();
	loadFloorRGB();

	// Setup the projection and view.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, 1.0, 20.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 30.0, 110.0,  // eye
			  0.0, 8.0, 0.0,      // center
			  0.0, 1.0, 0.);      // up

	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuseColor);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glutDisplayFunc(display);
	glutVisibilityFunc(visible);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}
