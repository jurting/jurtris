#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>

#ifdef dingux
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#elif  caanoo
#include <SDL.h>
#include <SDL_mixer.h>

#else
#include <SDL.h>
#include <SDL_mixer.h>
#endif

#include <ctime>
#include <cmath>
#include <cstring>
#include "class.h"
#include "jfont.h"
#define PI 3.14159265
using namespace std;

const char VERSION[]="0.8.5";
const double D_VERSION=0.0805;

#define GAMETYPE_SP 0
#define GAMETYPE_COOP 1
int game_type=0;//GAMETYPE_COOP;

int flip_time=16;
bool time_flips=0;

//Surfaces
SDL_Surface *screen=NULL;
SDL_Surface *sur_block=NULL;
SDL_Surface *menu_back=NULL;
SDL_Surface *menu_back_dark=NULL;
SDL_Surface *textbar=NULL;

//SDL var's
SDL_Rect screenrect;
SDL_Rect rfont[256];
SDL_Rect menu_iconrect[7];
SDL_Rect block_rect[9];
SDL_Rect temp_rect;
SDL_Rect textbar_rect[3];
Uint32 alpha_col;
Uint32 background_col;
int fonth=16;
int fontw=16;

//Timing
Uint32 gameticks[10][2];
Uint32 moveticks[10][4];
Uint32 gametimer;
Uint32 FPStimer;
Uint32 fliptimer;
double FPS;

//game var's
bool quit=0;
int temp[2];
unsigned char board[32][22];
bool board_dark[32][22];
bool board_flashlight[32];
bool board_flare[32][22];
bool thunder=0;
bool flare_use=0;
unsigned char block[10][4][4];
unsigned char blockrot[8][4][4][4];
unsigned char preview[10][5];
int previewx[8];
int previewy[8];
bool loaded=0;

int hold[10];
int holdused[10];
int blocktype[10];
int rot[10];
int x[10];
int y[10];
int xstart[10];
int ystart[10];
int players;
int level_width;
int fall_lines[10];
int level[10];
int falltime[10];
int fall[10];
int place[10];

bool update_shadow;

int thunder_time=0;
int thunder_chance=0;
int easyspins[10];
int shadow_y[10];
int score=0;

//Stats
#define STAT_PIECES 4
#define STAT_DROPS 5
#define STAT_HOLDS 6
#define STAT_SCORE 7
int game_stats[8];

//sound var's
Mix_Chunk *mus_drop=NULL;
Mix_Music *mus_music=NULL;
int audio_rate = 22050;
Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
int audio_channels = 2;
int audio_buffers = 2048;

//menu var's
#define MENU_MAX 3
int menu_num[MENU_MAX]={7,0,3};
int menu_sel[MENU_MAX];
int menu_set=0;
jstr menustr[MENU_MAX][16];
double menustr_dir[MENU_MAX];
double menustr_tdir[MENU_MAX];
Jfont font;

//gameplay options
#define OPT_HOLD 0
#define OPT_NUDGE 1
#define OPT_EASYSPIN 2
#define OPT_SHADOW 3
#define OPT_DARK 4
#define OPT_FLASHLIGHT 5
#define OPT_THUNDER 6
#define OPT_TRAINING 7
#define OPT_BAG 8
char opt_name []="Save";
char opt_name2[]="Save2";
char opt_name3[]="Save3";
int opt_preview=0;
bool opt[9];


//event vars
SDL_Event event;
int keyp=-1;
bool bd[512];
bool bp[512];
bool mb=0;
bool mbp=0;
bool mbr=0;
double mouse_x;
double mouse_y;
double mouse_oldx;
double mouse_oldy;

//Controls
Jurt_Joystick joy[10];
int joynum=0;
int joydead=1600;

#define but_right 0
#define but_down 1
#define but_left 2
#define but_up 3
#define but_menuOK 4
#define but_hard_drop 5
#define but_hold 6
#define but_rot 7
#define but_pause 8

#ifdef dingux
int joystick_sel[10]={-1,-1,-1};
int scale_x=1;
int scale_y=1;
int screen_bit=16;
#ifdef OD
bool HWS=0;
#else
bool HWS=0;
#endif
int but[10][9]={{SDLK_RIGHT,SDLK_DOWN,SDLK_LEFT,SDLK_UP,SDLK_LCTRL,SDLK_LALT,SDLK_LSHIFT,SDLK_LCTRL,SDLK_RETURN},
{SDLK_RIGHT,SDLK_DOWN,SDLK_LEFT,SDLK_UP,-1,-1,-1,SDLK_LCTRL,SDLK_RETURN},
{SDLK_LCTRL,SDLK_LALT,SDLK_LSHIFT,SDLK_SPACE,-1,-1,-1,SDLK_LCTRL,SDLK_RETURN}};
int but_type[10][9]={{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};

#elif caanoo

int joystick_sel[10]={0,0,0};
int scale_x=1;
int scale_y=1;
int screen_bit=16;
bool HWS=0;
int but[10][9]={{0,1,0,1,2,1,0,2,8},{0,1,0,1,2,1,0,2,8},{0,1,0,1,2,1,0,2,8}};
int but_type[10][9]={{1,1,2,2,5,5,5,5,5},{1,1,2,2,3,3,3,3,3},{1,1,2,2,5,5,5,5,5}};
#else

int joystick_sel[10]={-1,-1,0};
int scale_x=2;
int scale_y=2;
int screen_bit=16;
bool HWS=0;
int but[10][9]={{SDLK_RIGHT,SDLK_DOWN,SDLK_LEFT,SDLK_UP,SDLK_RETURN,SDLK_SPACE,'c',SDLK_UP,SDLK_ESCAPE},
{SDLK_RIGHT,SDLK_DOWN,SDLK_LEFT,SDLK_UP,SDLK_RETURN,SDLK_SPACE,'c',SDLK_UP,SDLK_ESCAPE},
{0,1,0,1,2,1,0,2,8}};
int but_type[10][9]={{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{3,3,4,4,5,5,5,5,5}};
#endif


char parsechar[8];


//extrafile functions
	//game.cpp
void game_init();
void generate_config(bool overwrite);
void load_config();
double cfg_getdouble(char str[32]);
bool str_match(char *str,char *str2);
int str_num(char *str);
int main_menu(int menu_choice);
void options_menu();
char* get_bind_str(int Abut, int Aplayer);
char* get_bindtype_str(int Abut, int Aplayer);
int game_options();
int game_loop();
int new_block(int Aplayer);
bool place_block(int Aplayer);
int block_collide(int Ax, int Ay, int Arot, int Atype, int Aplayer);
void check_line();
void block_rot(int Aplayer);
void init_block();
int pause_menu();
void stats_screen();
double angle_difference(double ang1,double ang2);

	//sdlextra.cpp
void clearSDL();
SDL_Surface *load_img(string Afilename);
void apply_surface(int x,int y,SDL_Surface* source,SDL_Surface* destination, SDL_Rect *clip=NULL);
void DrawPixel(SDL_Surface *screen,int x, int y, Uint32 color);
Uint32 GetPixel(SDL_Surface *screen,int x, int y);

//sync.cpp
void sync_move();
void sync_drop();
void sync_rot();
void sync_hardrop();
void sync_hold();
int sync_pause();
void sync_fall();
void sync_draw();
void sync_init();
void sync_shadow();
void sync_save();
void sync_load();

void eventblock();
bool input_p(int Abut, int Aplayer);
bool input_h(int Abut, int Aplayer);

#include "jfont.cpp"
#include "game.cpp"
#include "sdlextra.cpp"
#include "sync.cpp"
#include "eventblock.cpp"
