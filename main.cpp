//#define dingux 1
#include "func.h"

//int main (int argc, char const* argv[])
int main(int argc, char* argv[])
{
    srand(time(NULL));

	opt_preview=5;
	opt[OPT_HOLD]=1;
	opt[OPT_NUDGE]=1;
	opt[OPT_EASYSPIN]=1;
	opt[OPT_SHADOW]=1;
	opt[OPT_SLIDE]=1;
	opt[OPT_ALTHOLD]=0;
	opt[OPT_GHOST]=0;
	opt[OPT_DARK]=0;
	opt[OPT_FLASHLIGHT]=0;
	opt[OPT_THUNDER]=0;
	opt[OPT_FLARE]=0;
	opt[OPT_TRAINING]=0;
	opt[OPT_BAG]=0;

	if (argc>1) {if (str_match((char*)argv[1],(char*)"reset-cfg")) generate_config(1);}
	else generate_config(0);

	game_init();
	#ifdef caanoo
	SDL_ShowCursor(0);
	#elif dingux
	SDL_ShowCursor(0);
	#else
	SDL_ShowCursor(1);
	#endif


	//if (main_menu(0)==0) {clearSDL(); return 0;}
	main_menu(0);

	clearSDL();

	#ifdef caanoo
	chdir("/usr/gp2x");
	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
	#endif
	return 0;
}
