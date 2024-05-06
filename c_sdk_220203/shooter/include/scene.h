#ifndef SCENE_H
#define SCENE_H
#include "utils.h"
#include <obj/model.h>
#include "texture.h"
#include <stdlib.h>
#include "camera.h"

typedef struct Scene
{
	int target_number;				//lelőhető gömbök száma
	int gridsize;					//gömbök helyzetének mátrixa (pozíció)
	vec3* Target_positions;			//pozíció lista, amely target_number méretű, a gömbök pozícióját menti el a térben
	Model model;					//gömb modell
	GLuint texture;					//gömb textúra
	Model map_model;				//map modell		(BANK)
	GLuint map_texture;				//map textúra
	Model weapon_model;				//fegyver modell
	GLuint weapon_texture;			//fegyver textúra
	Model ground_model;				//talaj modell
	GLuint ground_texture;			//talaj textúra
	GLuint helpmenu;				//helpmenü textúrája
	GLuint crosshair;				//célkereszt textúrája
	Material material;
	int points;						//elért pontok (találatok száma)
	int maxpoints;					//összes lövés száma (pontosság kiszámításához szükséges)
	float light;					//fény ereje
	double player_speed;			//játékos mozgási sebessége
	GLuint* hudnumbers;				//hud-on látható visszaszámlálóhoz tartozó számok textúráját menti el
	int countdown;					//visszaszámláló maximális értéke
	double countdown_timer;			//jelenlegi idő a visszaszámlálón
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting(Scene* scene);

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Render the scene objects.
 */
void render_scene(Scene* scene,Camera* camera);

/**
 * Check target hit.
 */
void checktarget(Camera* camera, Scene* scene);


/**
 * Add value to timer.
 */
void addtime(Scene* scene,double time);

/**
 * Shows the time on hud.
 */
void showtimerhud(Scene* scene);

#endif /* SCENE_H */
