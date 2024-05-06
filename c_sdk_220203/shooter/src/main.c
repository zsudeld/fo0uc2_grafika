#include "app.h"

#include <stdio.h>

/**
 * Main function
 */
int main(int argc, char* argv[])
{
    App app;
	//fölösleges változók kiszedése, hogy ne legyen warning
	argc=argc;
	argv=argv;

	//app létrehozása 1920x1080 felbontással
	app.width=1920;
	app.heigth=1080;
    init_app(&app, app.width, app.heigth);
    while (app.is_running) {
		//inputok kezelése
        handle_app_events(&app);
		//app frissítése
        update_app(&app);
		//app megjelenítése
        render_app(&app);
    }
    destroy_app(&app);

    return 0;
}
