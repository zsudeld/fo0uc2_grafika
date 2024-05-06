#include "scene.h"
#include <GL/gl.h>
#include <obj/model.h>
#include <obj/load.h>
#include <obj/draw.h>
#include "texture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <math.h>
#include <time.h>

void init_scene(Scene* scene)
{
	//véletlen szám generátor beállítása
	srand(time(0));
	//gömbök száma
	scene->target_number=5;
	scene->points=0;
	scene->countdown=30;
	scene->countdown_timer=0.0;
	scene->maxpoints=0;
	//gömbök pozíciójának a listáját inicializáljuk
	scene->Target_positions=(vec3*)malloc(scene->target_number*sizeof(vec3));
	scene->gridsize=4;
	//modellek, textúrák betöltése
	load_model(&(scene->model), "assets/models/sphere.obj");
	scene->texture = load_texture("assets/textures/sphere.png");
	load_model(&(scene->map_model), "assets/models/bank.obj");
	scene->map_texture = load_texture("assets/textures/bank.jpg");
	load_model(&(scene->ground_model), "assets/models/ground.obj");
	scene->ground_texture = load_texture("assets/textures/ground.jpg");
	load_model(&(scene->weapon_model), "assets/models/weapon.obj");
	scene->weapon_texture = load_texture("assets/textures/weapon.jpg");
	scene->helpmenu= load_texture("assets/textures/help.png");
	scene->crosshair= load_texture("assets/textures/crosshair.png");
	//hud szám textúrák létrehozása, beolvasása
	scene->hudnumbers=(GLuint*)malloc(10*sizeof(GLuint));
	for(int i=0;i<10;i++){
		char number[2];
		itoa(i,number,10);
		char text[100]="assets/textures/";
		strcat(text,number);
		strcat(text,".png");
		scene->hudnumbers[i]=load_texture(text);
	}
	scene->light=0.8f;
	scene->player_speed=6.0f;
    scene->material.ambient.red = 1.0;
    scene->material.ambient.green = 1.0;
    scene->material.ambient.blue = 1.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 0.0;

    scene->material.specular.red = 0.0;
    scene->material.specular.green = 0.0;
    scene->material.specular.blue = 0.0;

    scene->material.shininess = 0.0;
	int i=0;
	//végigmegyünk minden gömbön
	while(i<scene->target_number){
		//a gömbök pozíciójának randomizálása
		scene->Target_positions[i].x=(rand()%scene->gridsize-(scene->gridsize/2))*3;
		scene->Target_positions[i].y=15.0;
		scene->Target_positions[i].z=(rand()%scene->gridsize-(scene->gridsize/2))*3+3.0;
		//végigmegyünk minden ezelőtt generált gömbön
		for(int k=0;k<i;k++){
			//ha ugyan azon a pozíción már létezik egy másik gömb, akkor újra generáljuk a pozíciót
			if(i!=k&&scene->Target_positions[i].x==scene->Target_positions[k].x &&scene->Target_positions[i].y==scene->Target_positions[k].y&&scene->Target_positions[i].z==scene->Target_positions[k].z){
				i--;break;
			}
		}
		i++;
	}
}


void set_lighting(Scene* scene)
{
    float ambient_light[] = { scene->light, scene->light, scene->light, 1.0f };
    float diffuse_light[] = { scene->light, scene->light, scene->light, 1.0f };
    float specular_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float position[] = { 0.0f, 0.0f, 10.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}


void render_scene(Scene* scene,Camera* camera)
{
    set_material(&(scene->material));
    set_lighting(scene);
	glBindTexture(GL_TEXTURE_2D, scene->texture);
	//végigmegyünk minden gömbön
	for(int i=0;i<scene->target_number;i++){
		glPushMatrix();
		//megjelenítjük a gömböket
		glTranslatef(scene->Target_positions[i].x,scene->Target_positions[i].y,scene->Target_positions[i].z);
		draw_model(&(scene->model));
		 glPopMatrix();
	}
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, scene->map_texture);
	glTranslatef(50.0,63.0,-6.0);
	glRotatef(90.0,1.0,0.0,0.0);
	glRotatef(210.0,0.0,1.0,0.0);
	glScalef(3.0,3.0,3.0);
	//BANK ház megjelenítése
	draw_model(&(scene->map_model));
	glPopMatrix();
	glPushMatrix();
	//fegyver megjelenítése a kamera pozíciója és iránya alapján
	float startX = camera->position.x;
	float startY = camera->position.y;
	float startZ = camera->position.z;
	vec3 dir=get_camera_dir_vec3f(camera);
	float endX = startX + dir.x;
	float endY = startY + dir.y;
	float endZ = startZ + dir.z-1;
	glBindTexture(GL_TEXTURE_2D, scene->weapon_texture);
	glTranslatef(endX,endY,endZ);
	glScalef(0.05,0.05,0.05);
	glRotatef(90.0,1.0,0.0,0.0);
	glRotatef(180.0,0.0,1.0,0.0);
	glRotatef(-camera->rotation.x,1.0,0.0,0.0);
	glRotatef(camera->rotation.z,0.0,1.0,0.0);
	//fegyver megjelenítése
	draw_model(&(scene->weapon_model));
	glPopMatrix();
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, scene->ground_texture);
	glTranslatef(-10.0,20.0,-6.0);
	glScalef(2.5,2.5,1.0);
	glRotatef(90.0,1.0,0.0,0.0);
	//talajzat megjelenítése
	draw_model(&(scene->ground_model));
	glPopMatrix();
	//visszaszámláló hud megjelenítése
	showtimerhud(scene);
}

//lövés lekezelése
void checktarget(Camera* camera, Scene* scene){
	//kiszámítjuk a kamera pozíció és iránya alapján hogy milyen irányba nézünk, így kapunk egy egyenes kezdőpontot és irányt
	float startX = camera->position.x;
	float startY = camera->position.y;
	float startZ = camera->position.z;
	vec3 dir=get_camera_dir_vec3f(camera);
	float endX = startX + dir.x;
	float endY = startY + dir.y;
	float endZ = startZ + dir.z;
	int found=0;
	//végimegyünk minden gömbön
	for(int i=0;i<scene->target_number;i++){
		//kiszámítjuk az egyenes és a gömb metszését		(képlet alapján)
		vec3 d = {endX - startX, endY - startY, endZ - startZ};
		vec3 e = {startX - scene->Target_positions[i].x, startY - scene->Target_positions[i].y, startZ - scene->Target_positions[i].z};

		float a = d.x * d.x + d.y * d.y + d.z * d.z;
		float b = 2 * (d.x * e.x + d.y * e.y + d.z * e.z);
		float c = e.x * e.x + e.y * e.y + e.z * e.z - 1.0 * 1.0;
		float discriminant = b * b - 4 * a * c;
		//ha a diszkrimináns nagyobb mint 0, akkor van metszés, tehát eltaláltuk a gömböt
		if (discriminant < 0)
			continue;
		found=1;
		int ok=0;
		while(ok==0){
			//újra randomizáljuk a gömb helyzetét
			scene->Target_positions[i].x=(rand()%scene->gridsize-(scene->gridsize/2))*3;
			scene->Target_positions[i].y=15.0;
			scene->Target_positions[i].z=(rand()%scene->gridsize-(scene->gridsize/2))*3+3.0;
			ok=1;
			for(int k=0;k<scene->target_number;k++){
				//ellenörizzük létezik-e már egy másik gömb a generált pozíción
				if(i!=k&&scene->Target_positions[i].x==scene->Target_positions[k].x &&scene->Target_positions[i].y==scene->Target_positions[k].y&&scene->Target_positions[i].z==scene->Target_positions[k].z){
					ok=0;
					break;
				}
			}
		}
	}
	//növeljül a lövések mennyiségét
	scene->maxpoints++;
	//ha eltaláltunk 1 gömböt, akkor növeljük a pontjainkat
	if(found==1){
		scene->points++;
	}
}

//visszaszámláló hud megjelenítése
void showtimerhud(Scene* scene){
	glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//a visszaszámláló alapján megkapjuk a tizes és egyes helyen szereplő számokat
	int number1=((int)(scene->countdown-scene->countdown_timer))/10;
	int number2=((int)(scene->countdown-scene->countdown_timer))%10;
	
	//megjelenítjük a tizes helyen álló számot
	glBindTexture(GL_TEXTURE_2D, scene->hudnumbers[number1]);
    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(35, -28, -50);
    glTexCoord2f(1, 0);
    glVertex3f(37, -28, -50);
    glTexCoord2f(1, 1);
    glVertex3f(37, -30, -50);
    glTexCoord2f(0, 1);
    glVertex3f(35, -30, -50);
    glEnd();

	//megjelenítjük az egyes helyen álló számot
	glBindTexture(GL_TEXTURE_2D, scene->hudnumbers[number2]);
    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(37, -28, -50);
    glTexCoord2f(1, 0);
    glVertex3f(39, -28, -50);
    glTexCoord2f(1, 1);
    glVertex3f(39, -30, -50);
    glTexCoord2f(0, 1);
    glVertex3f(37, -30, -50);
    glEnd();


    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}


//idő elmentése a visszaszámláló miatt
void addtime(Scene* scene,double time){
	if (time>=0.0){
		//elmentjük az időt
		scene->countdown_timer+=time;
	}
	//ha az idő túlmegy a maximális időn, akkor
	if(scene->countdown_timer>(double)scene->countdown){
		//reseteljük a jelenlegi időt, pontjainkat és lövések számát
		scene->countdown_timer=0.0;
		float pontossag=scene->points;
		if(scene->maxpoints!=0)
			pontossag/=scene->maxpoints;
		//kiírjuk a pontjainkat és a pontosságunkat százalékban
		printf("pontok: %d\t\tpontossag: %.0f%%\n",scene->points,pontossag*100);
		scene->points=0;
		scene->maxpoints=0;
		
	}
}