#include <cmath>
#include <GL/freeglut.h>
#include <ctime>
#include "loadTGA.h"


#define NUM_CIRCLES 30
#define NUM_SPIRALS 5

float cam_X = 0,  cam_Y = 2, cam_Z = 100, cam_angle = 0;	// Rotation angles about x, y axes
float length = 0.992;  // length of the pendulum arm
float mass = 10;
float theta = 2.0 * M_PI / 180.0; // initial angle of pendulum arm
float omega = 0;   // angle of the pendulum
float gravity = 9.81;
float delta = 1.0f;   // amount to change the angle each frame
float alpha = 0; // initial angular acceleration
float ball_radius = 0.1f; // radius of the pendulum ball

float ames_angle = 0.0f;

float circle_x = 1, circle_y = 0, circle_angle_x = 0, circle_angle_y = 0,circle_speed = 4;
int circle_angles[NUM_CIRCLES];
float tube_outer_rad = 20,  tube_inner_rad=5;

GLuint txId[4];

void loadTextures()
{   
    glGenTextures(5, txId); 
    glBindTexture(GL_TEXTURE_2D, txId[0]);
	loadTGA("textures/ClockWood.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[1]);
	loadTGA("textures/Clock.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[2]);
	loadTGA("textures/Floor.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // https://www.freepik.com/free-vector/psychedelic-optical-illusion-background_8398808.htm#query=optical%20illusion&position=6&from_view=search&track=ais Image by freepik
    
    glBindTexture(GL_TEXTURE_2D, txId[3]);
	loadTGA("textures/opt1.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // https://www.freepik.com/free-vector/realistic-optical-illusion-background_11797144.htm#query=optical%20illusion&position=48&from_view=search&track=ais Image by pikisuperstar on Freepik
        
    glBindTexture(GL_TEXTURE_2D, txId[4]);
	loadTGA("textures/opt2.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}

void drawCircle(float radius) {
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glTranslatef(0,0,-0.2);
        glBindTexture(GL_TEXTURE_2D, txId[1]);
        glBegin(GL_TRIANGLE_FAN);
        glTranslatef(0,0,-0.03);
        glTexCoord2f(0.5f, 0.5f); // center of texture
        glVertex2f(0.0, 0.0);
        for (int i = 0; i <= 360; i++) {
            float angle = i * 3.14159265 / 180;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            glTexCoord2f(-x / (2.0f * radius) + 0.5f, y / (2.0f * radius) + 0.5f); // flip x-axis
            glVertex2f(x, y);
        }
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawRec() {
    
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, txId[0]);
        glTranslatef(0.0, 0.3, -0.01); // Move the rectangle behind the clock
        
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_QUADS);

        glTexCoord2f(0.0f, 0.333f); glVertex3f(-1, -1, 0.0);
        glTexCoord2f(0.333f, 0.333f); glVertex3f(-1, 1, 0.0);
        glTexCoord2f(0.333f, 0.666f); glVertex3f(1, 1, 0.0);
        glTexCoord2f(0.0f, 0.666f); glVertex3f(1, -1, 0.0);

        // Back face
        glTexCoord2f(0.828f, 0.33f); glVertex3f(-1, -1, -1.0);
        glTexCoord2f(0.495f, 0.333f); glVertex3f(-1, 1, -1.0);
        glTexCoord2f(0.495f, 0.666f); glVertex3f(1, 1, -1.0);
        glTexCoord2f(0.828f, 0.666f); glVertex3f(1, -1, -1.0);

        // Left face
        glTexCoord2f(0.0, 0.166); glVertex3f(-1, -1, 0.0);
        glTexCoord2f(0.333f, 0.166); glVertex3f(-1, 1, 0.0);
        glTexCoord2f(0.333f, 0.333); glVertex3f(-1, 1, -1.0);
        glTexCoord2f(0.0f, 0.333f); glVertex3f(-1, -1, -1.0);

        // Right face
        glTexCoord2f(0.0f, 0.666f); glVertex3f(1, -1, 0.0);
        glTexCoord2f(0.333f, 0.666f); glVertex3f(1, 1, 0.0);
        glTexCoord2f(0.333f, 0.825f); glVertex3f(1, 1, -1.0);
        glTexCoord2f(0.0f, 0.825f); glVertex3f(1, -1, -1.0);

        // Top face
        glTexCoord2f(0.333f, 0.333f); glVertex3f(-1, 1, 0.0);
        glTexCoord2f(0.333f, 0.499f); glVertex3f(-1, 1, -1.0);
        glTexCoord2f(0.499f, 0.666f); glVertex3f(1, 1, -1.0);
        glTexCoord2f(0.333f, 0.666f); glVertex3f(1, 1, 0.0);

        // Bottom face
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1, -1, 0.0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, -1, -1.0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(1, -1, -1.0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1, -1, 0.0);
        glEnd();
    glPopMatrix();
    }

void drawClock() {
    glPushMatrix();
        glTranslatef(0,0,0.2);
        glColor3f(1.0, 1.0, 1.0);
        drawCircle(1.0);
    glPopMatrix();

    time_t t = time(NULL);
    struct tm* tm = localtime(&t);

    glPushMatrix();
        // hour hand
        glPushMatrix();
            glColor3f(0.0, 0.0, 0.0);
             glTranslatef(0,0,0.01);
            glRotatef(30 * tm->tm_hour + tm->tm_min / 2.0, 0.0, 0.0, 1.0);
            glBegin(GL_QUADS);
                glVertex3f(-0.05, 0.0,0);
                glVertex3f(0.05, 0.0,0);
                glVertex3f(0.04, 0.6,0);
                glVertex3f(-0.04, 0.6,0);
            glEnd();
        glPopMatrix();

        // Draw the minute hand

        glPushMatrix();
            glColor3f(0.0, 0.0, 0.0);
            glTranslatef(0,0,0.02);
            glRotatef(6 * tm->tm_min + tm->tm_sec / 10.0, 0.0, 0.0, 1.0);
            glBegin(GL_QUADS);
                glVertex2f(-0.04, 0.0);
                glVertex2f(0.04, 0.0);
                glVertex2f(0.03, 0.8);
                glVertex2f(-0.03, 0.8);
            glEnd();
        glPopMatrix();

        // Draw the second hand
        
        glPushMatrix();
            glTranslatef(0.0, 0.0, 0.03);
            glColor3f(1.0, 0.0, 0.0);
            glRotatef(6 * tm->tm_sec, 0.0, 0.0, 1.0);
            glBegin(GL_QUADS);
                glVertex2f(-0.03, 0.0);
                glVertex2f(0.03, 0.0);
                glVertex2f(0.02, 0.9);
                glVertex2f(-0.02, 0.9);
            glEnd();
        glPopMatrix();
    glPopMatrix();
}
void updatePendulum(int value) {
    // calculate angular acceleration
    float alpha = -9.81/length * sin(theta);

    // update angular velocity and angle
    omega += alpha * 0.01;
    theta += omega * 0.01;

    glutPostRedisplay(); 
    glutTimerFunc(10, updatePendulum, 0);
}


// Special key function for camera control
void special(int key, int x, int y)
{
    const float angle = 0.06f;   // rotation angle in degrees
    const float step = 0.5f;    // translation step
    switch(key) {
        case GLUT_KEY_LEFT:
            cam_angle -= angle;  // rotate left
            break;
        case GLUT_KEY_RIGHT:
            cam_angle += angle;  // rotate right
            break;
 case GLUT_KEY_UP:
            cam_X += step * sin(cam_angle);   // move forward in X direction
            cam_Z -= step * cos(cam_angle);   // move forward in Z direction
            break;
        case GLUT_KEY_DOWN:
            cam_X -= step * sin(cam_angle);   // move backward in X direction
            cam_Z += step * cos(cam_angle);   // move backward in Z direction
            break;
        default:
            break;
    }
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case '1':
            cam_X = 1,  cam_Y = 2, cam_Z = -2, cam_angle = -45.54;	
			break;
        case '2':
            cam_X = 0,  cam_Y = 2, cam_Z = 5, cam_angle = 0;
            break;
        case '3':
            cam_X = 2,  cam_Y = 2, cam_Z = -4.5, cam_angle = 77;
            break;

        case '0':
            cam_X = 0,  cam_Y = 2, cam_Z = 100, cam_angle = 0;
            break;
	}
	glutPostRedisplay();
}

void myTimer(int value)
{
theta += M_PI/180;
glutPostRedisplay();
glutTimerFunc(50, myTimer, 0);
}



void drawPendulum() {
    float x = length * sin(theta);
    float y = -length * cos(theta);

    // Draw the pendulum arm
    glPushMatrix();
    	float lpos[4] = {0., 10., 10., 1.0};  //light's position
        glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position
        glEnable(GL_LIGHTING);
        glColor3f(0.8, 0.6, 0.0);
        glTranslatef(0,-0.8,0);
        glBegin(GL_LINES);
            glVertex2f(0.0, 0.0);
            glVertex2f(x, y);
        glEnd();

        // Draw the sphere
        glTranslatef(x, y, 0.0);
        
        glutSolidSphere(0.1, 20, 20);

        // Draw the sphere shadow
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.0, 0.0, 0.0, 0.5);
        glTranslatef(x, y, -0.3); // move the shadow slightly below the sphere
        glutSolidSphere(0.1, 20, 20);
        glEnable(GL_LIGHTING);
        glDisable(GL_BLEND);
    glPopMatrix();
}

// ames window!!

void drawTrapezoid()
{
    glPushMatrix();
        // cyan piece (middle)
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_QUADS);
        glColor4f(0.5f, 0.5f, 1.0f, 1.0f);   
        glVertex3f(0.336, 0.0, 0.0);      // bottom left
        glVertex3f(8.095, 0.0, 0.0);     // bottom right
        glVertex3f(8.095, 0.33, 0.0);     // top right
        glVertex3f(0.336, 0.17, 0.0);      // top left
        glEnd();

        // green piece (left)
        glBegin(GL_QUADS);
        glColor4f(0.5f, 0.5f, 1.0f, 1.0f);    
        glVertex3f(0.0, 0.0, 0.0);      // bottom left
        glVertex3f(0.336, 0, 0.0);      // bottom right
        glVertex3f(0.336, 1.275, 0.0);      // top right
        glVertex3f(0.0, 1.56, 0.0);      // top left
        glEnd();

        // blue-purple piece (top)
        glBegin(GL_QUADS);
        glColor4f(0.5f, 0.5f, 1.0f, 1.0f);      
        glVertex3f(0.334, 1.27, 0.0);      // bottom left
        glVertex3f(8.1, 2.363, 0.0);     // bottom right
        glVertex3f(8.75, 3.099, 0.0);     // top right
        glVertex3f(0, 1.56, 0.0);      // top left
        glEnd();

        // Yellow piece (right/4th)
        glBegin(GL_QUADS);
        glColor4f(0.5f, 0.5f, 1.0f, 1.0f); 
        glVertex3f(8.095, 0.0, 0.0);     // bottom left
        glVertex3f(8.745, 0.0, 0.0);     // bottom right
        glVertex3f(8.745, 3.099, 0.0);     // top right
        glVertex3f(8.095, 2.364, 0.0);     // top left
        glEnd();

        // yellow piece (1st)
        glBegin(GL_QUADS);
        glColor3f(0.678, 0.847, 0.902);
        glVertex3f(0.336, 0.168, 0.0);      // bottom left
        glVertex3f(0.678, 0.175, 0.0);      // bottom right
        glVertex3f(0.678, 1.159, 0.0);      // top right
        glVertex3f(0.336, 1.275, 0.0);      // top left
        glEnd();


        // yellow piece (2nd)
        glBegin(GL_QUADS);
        glColor3f(0.678, 0.847, 0.902); 
        glVertex3f(2.562, 0.214, 0.0);      // bottom left
        glVertex3f(2.991, 0.223, 0.0);      // bottom right
        glVertex3f(2.991, 1.437, 0.0);      // top right
        glVertex3f(2.562, 1.592, 0.0);      // top left
        glEnd();

        // yellow piece (3rd)
        glBegin(GL_QUADS);
        glColor3f(0.678, 0.847, 0.902);    
        glVertex3f(5.296, 0.27, 0.0);     // bottom left
        glVertex3f(5.82, 0.281, 0.0);     // bottom right
        glVertex3f(5.82, 1.776, 0.0);     // top right
        glVertex3f(5.296, 1.971, 0.0);       // top left
        glEnd();

        // red piece (1st)
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.5f);     
        glVertex3f(0.678, 1.159, 0.0);      // bottom left
        glVertex3f(2.15, 1.336, 0.0);      // bottom right
        glVertex3f(2.15, 1.53, 0.0);      // top right
        glVertex3f(0.336, 1.275, 0.0);      // top left
        glEnd();

        // red piece (2nd)
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.5f); 
        glVertex3f(2.991, 1.437, 0.0);      // bottom left
        glVertex3f(4.82, 1.656, 0.0);     // bottom right
        glVertex3f(4.82, 1.904, 0.0);     // top right
        glVertex3f(2.562, 1.592, 0.0);      // top left
        glEnd();

        // red piece (3rd)
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.5f);
        glVertex3f(5.82, 1.776, 0.0);     // bottom left
        glVertex3f(8.095, 2.049, 0.0);       // bottom right
        glVertex3f(8.095, 2.364, 0.0);     // top right
        glVertex3f(5.296, 1.971, 0.0);       // top left
        glEnd();
        
        // red piece (4th)
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.5f);     
        glVertex3f(8.745, 0.0, 0.0);     // bottom left
        glVertex3f(9.39, 0.0, 0.0);     // bottom right
        glVertex3f(9.39, 2.88, 0.0);     // top right
        glVertex3f(8.745, 3.099, 0.0);     // top left
        glEnd();

        // magenta piece (1st)
        glBegin(GL_QUADS);
        glColor4f(0.5f, 0.5f, 1.0f, 1.0f); 
        glVertex3f(2.15, 0.206, 0.0);      // bottom left
        glVertex3f(2.562, 0.214, 0.0);      // bottom right
        glVertex3f(2.562, 1.592, 0.0);        // top right
        glVertex3f(2.15, 1.53, 0.0);      // top left
        glEnd();

        // magenta piece (2nd)
        glBegin(GL_QUADS);
        glColor4f(0.5f, 0.5f, 1.0f, 1.0f);
        glVertex3f(4.82, 0.26, 0.0);     // bottom left
        glVertex3f(5.296, 0.27, 0.0);     // bottom right
        glVertex3f(5.296, 1.971, 0.0);       // top right
        glVertex3f(4.82, 1.904, 0.0);     // top left
        glEnd();
    glPopMatrix();
}
void displayTrapezoid(void) 
{ 
    float centerX = (0.336 + 8.095) / 2.0;
    float centerY = (0.17 + 0.33) / 2.0;
    glPushMatrix(); 
        glDisable(GL_LIGHTING);
        glTranslatef(centerX, centerY, 0.0);
        glRotatef(ames_angle, 0.0, 1.0, 0.0);
        glTranslatef(-centerX, -centerY, 0.0);
        drawTrapezoid();
        glPopMatrix();
        glPushMatrix();  
            glScalef(1.0, -1.0, 1.0);  // mirror the object along the x-axis
            glTranslatef(centerX, centerY, 0.0);
            glRotatef(ames_angle, 0.0, 1.0, 0.0);
            glTranslatef(-centerX, -centerY, 0.0);
            drawTrapezoid();
        glPopMatrix();
    glPopMatrix();
}


void amesTimer(int value)
{
    ames_angle += 1.0;
    glutPostRedisplay();
    glutTimerFunc(50, amesTimer, 0);
}

// circles

void displayCircle(void) 
{ 
    glPushMatrix(); 
        
        glRotatef(circle_angle_x, 1.0f, 0.0f, 0.0f);
        glRotatef(circle_angle_y, 0.0f, 1.0f, 0.0f);

        
        for (int i = 0; i < NUM_CIRCLES; i++)
        {
            glPushMatrix();
        
                
                glRotatef(i * (360 / NUM_CIRCLES), 0, 1, 0);
                glTranslatef(0, 0, tube_outer_rad);
                
                glRotatef((i * (360*NUM_SPIRALS)/ NUM_CIRCLES)+ circle_speed,1,0,0); 
                glTranslatef(0,0,tube_inner_rad);
                
                glColor3f(1.0, 1.0, 0.5);
                glutSolidSphere(1, 12, 12);
            

            glPopMatrix();
        }
        circle_speed +=5;
    glPopMatrix();

}

void circleTimer(int value)
{

    glutPostRedisplay();
    glutTimerFunc(10, circleTimer, 0);
}
void drawFloor() {
    glPushMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);

        glScalef(10,10,10);
        glDisable(GL_LIGHTING);			
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txId[2]);
        float scale = 10;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Draw the floor
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0);
            glVertex3f(-10.0, 0.0, -10.0);
            glTexCoord2f(10.0*scale, 0.0);
            glVertex3f(10.0, 0.0, -10.0);
            glTexCoord2f(10.0*scale, 10.0*scale);
            glVertex3f(10.0, 0.0, 10.0);
            glTexCoord2f(0.0, 10.0*scale);
            glVertex3f(-10.0, 0.0, 10.0);
        glEnd();
        glEnable(GL_LIGHTING);	

        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawSkybox()
{
   
	glPushMatrix();
        
        glScalef(30,30,30);
        glColor3f(0.2, 0.2, 0.2); 
		// // front
		// glBegin(GL_QUADS);
		// glNormal3f(0, 0, -1);
		// glVertex3f(1, -1, 1); // bottom left
		// glVertex3f(-1, -1, 1); // bottom right
		// glVertex3f(-1, 1, 1); // top right
		// glVertex3f(1, 1, 1); // top left
		// glEnd();
		
		// back

		glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glVertex3f(-1, -1, -1); // bottom left
		glVertex3f(1, -1, -1); // bottom right
		glVertex3f(1, 1, -1); // top right
		glVertex3f(-1, 1, -1); // top left
		glEnd();
		
		// left
		glBegin(GL_QUADS);
		glNormal3f(-1, 0, 0);
		glVertex3f(1, -1, -1); // bottom left
		glVertex3f(1, -1, 1); // bottom right
		glVertex3f(1, 1, 1); // top right
		glVertex3f(1, 1, -1); // top left
		glEnd();
		
		// right
		glBegin(GL_QUADS);
		glNormal3f(1, 0, 0);
		glVertex3f(-1, -1, 1); // bottom left
		glVertex3f(-1, -1, -1); // bottom right
		glTexCoord2f(1, 1);
		glVertex3f(-1, 1, -1); // top right
		glVertex3f(-1, 1, 1); // top left
		glEnd();
		
		// bottom
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glVertex3f(-1, -1, 1); // bottom left
		glVertex3f(-1, -1, -1); // bottom right
		glVertex3f(1, -1, -1); // top right
		glVertex3f(1, -1, 1); // top left
		glEnd();
		
		// TOP
		glBegin(GL_QUADS);
		glNormal3f(0, -1, 0);
		glVertex3f(-1, 1, -1); // bottom left
		glVertex3f(-1, 1, 1); // bottom right
		glVertex3f(1, 1, 1); // top right
		glVertex3f(1, 1, -1); // top left
		glEnd();
	glPopMatrix();
}


void drawPainting1()
{   
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glDisable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D,txId[3]);
        glBegin(GL_QUADS);
        glColor3f(1,1,1);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f); // bottom-left vertex
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f); // bottom-right vertex
        glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f); // top-right vertex
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f); // top-left vertex
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawPainting2()
{   
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glDisable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D,txId[4]);
        glBegin(GL_QUADS);
        glColor3f(1,1,1);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f); // bottom-left vertex
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f); // bottom-right vertex
        glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f); // top-right vertex
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f); // top-left vertex
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void displayGallery(void) 
{ 
	float lpos[4] = {0., 10., 10., 1.0};
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


    float look_at_X = cam_X + sin(cam_angle);
    float look_at_Z = cam_Z - cos(cam_angle);

    gluLookAt(cam_X, cam_Y, cam_Z,     
              look_at_X, cam_Y, look_at_Z, 
              0, 1, 0);                   

	glLightfv(GL_LIGHT0,GL_POSITION, lpos);   

	
    drawFloor();
	
    
    drawSkybox();
    glPushMatrix();
        glTranslatef(15, 3, -4);
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        glDisable(GL_LIGHTING);	
        drawPendulum();
        
        glPushMatrix();
            glScalef(-1,1,1);
            glScalef(0.5f, 0.5f, 0.5f);
            glDisable(GL_LIGHTING);	
            drawClock();
        glPopMatrix();
        
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glDisable(GL_LIGHTING);
            drawRec();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        
    glPopMatrix();

    glPushMatrix();
        glScalef(0.5f, 0.5f, 0.5f);
        glTranslatef(-30,4,-4);
        displayTrapezoid();
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_LIGHTING);
        
        glTranslatef(0,2,-10);
        glScalef(0.1,0.1,0.1);
        glRotatef(30, 1.0f, 0.0f, 0.0f);
        displayCircle();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(-7,0,-10);
        glScalef(3,3,3);
        drawPainting2();
    glPopMatrix();

    
    glPushMatrix();
        glTranslatef(7,0,-10);
        glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
        glScalef(3,3,3);
        drawPainting1();
    glPopMatrix();

	glFlush(); 
} 


void initialize(void)
{
    loadTextures();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
	glEnable(GL_LIGHTING);	
	glEnable(GL_LIGHT0);
 	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, 1, 5, 1000.0);
}

int main(int argc, char **argv) 
{ 
	glutInit(&argc, argv);          
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);  
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Teapot");	
	initialize();
	glutDisplayFunc(displayGallery);
	glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
	glutTimerFunc(10, updatePendulum, 0);
    glutTimerFunc(50, amesTimer, 0);
	glutMainLoop();
	
	return 0; 
}

