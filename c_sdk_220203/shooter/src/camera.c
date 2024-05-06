#include "camera.h"

#include <GL/gl.h>
#include <stdio.h>
#include <math.h>

void init_camera(Camera* camera)
{
    camera->position.x = 0.0;
    camera->position.y = 0.0;
    camera->position.z = 1.0;
    camera->rotation.x = 0.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = 90.0;
    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;
}

void update_camera(Camera* camera, double time)
{
    double angle;
    double side_angle;
    angle = degree_to_radian(camera->rotation.z);
    side_angle = degree_to_radian(camera->rotation.z + 90.0);

    camera->position.x += cos(angle) * camera->speed.y * time;
    camera->position.y += sin(angle) * camera->speed.y * time;
    camera->position.x += cos(side_angle) * camera->speed.x * time;
    camera->position.y += sin(side_angle) * camera->speed.x * time;
}

void set_view(const Camera* camera)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(camera->rotation.x + 90), 1.0, 0, 0);
    glRotatef(-(camera->rotation.z - 90), 0, 0, 1.0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}

void rotate_camera(Camera* camera, double horizontal, double vertical)
{
    camera->rotation.z += horizontal/10;
    camera->rotation.x += vertical/10;

    if (camera->rotation.z < 0) {
        camera->rotation.z += 360.0;
    }

    if (camera->rotation.z > 360.0) {
        camera->rotation.z -= 360.0;
    }
	
	if (camera->rotation.x < -90.0) {
        camera->rotation.x = -90.0;
    }

    if (camera->rotation.x > 90.0) {
        camera->rotation.x =90.0;
    }
	

}

void set_camera_speed(Camera* camera, double speed)
{
    camera->speed.y = speed;
}

void set_camera_side_speed(Camera* camera, double speed)
{
    camera->speed.x = speed;
}


//help menü megjelenítése hudként
void helpmenu()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-1, 1, -3);
    glTexCoord2f(1, 0);
    glVertex3f(1, 1, -3);
    glTexCoord2f(1, 1);
    glVertex3f(1, -1, -3);
    glTexCoord2f(0, 1);
    glVertex3f(-1, -1, -3);
    glEnd();

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}
//célkereszt megjelenítése hudként
void crosshair()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-1, 10, -1000);
    glTexCoord2f(1, 0);
    glVertex3f(1, 10, -1000);
    glTexCoord2f(1, 1);
    glVertex3f(1, -10, -1000);
    glTexCoord2f(0, 1);
    glVertex3f(-1, -10, -1000);
    glEnd();


	glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-10, 1, -1000);
    glTexCoord2f(1, 0);
    glVertex3f(10, 1, -1000);
    glTexCoord2f(1, 1);
    glVertex3f(10, -1, -1000);
    glTexCoord2f(0, 1);
    glVertex3f(-10, -1, -1000);
    glEnd();
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

//kamera iránya alapján kiszámítjuk az irányvektort
vec3 get_camera_dir_vec3f(Camera *camera)
{
    vec3 dir = {cos(degree_to_radian(camera->rotation.x)) *
                 cos(degree_to_radian(camera->rotation.z)), 
                 sin(degree_to_radian(camera->rotation.z)) *
                 cos(degree_to_radian(camera->rotation.x)),
                 sin(degree_to_radian(camera->rotation.x))};
                 
	//normalizáljuk az irányvektort, így kapunk belőle normálvektort
    vec3_normalize(&dir);
    return dir;
}


//irányvektor normálvektorrá számítása			 (képlet alapján)
void vec3_normalize(vec3 *v)
{
    float w = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x /= w;
    v->y /= w;
    v->z /= w;
}
