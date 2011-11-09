void game_init()
{
	load_config();

	#ifdef dingux
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
	joynum=0;

	#elif caanoo
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	joynum=SDL_NumJoysticks();

	#else
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	joynum=SDL_NumJoysticks();
	#endif

	if (HWS) screen=SDL_SetVideoMode(screenrect.w,screenrect.h,screen_bit,SDL_HWSURFACE|SDL_DOUBLEBUF);
	else screen=SDL_SetVideoMode(screenrect.w,screenrect.h,screen_bit,SDL_SWSURFACE);

	//if (!HWSURFACE)
        //cout << Hardware surface failed << endl;
	alpha_col=SDL_MapRGB(screen->format,255,0,255);
	background_col=SDL_MapRGB(screen->format,48,48,48);
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
		cout << "sound error: mix_open (" << __FILE__ << " " << __LINE__ << ")" << endl;

	mus_drop=Mix_LoadWAV("sound/drop.wav");
	mus_music=Mix_LoadMUS("music/allsort.mod");
	sur_block=load_img("gfx/block.bmp");
	textbar=load_img("gfx/textbar.bmp");
	menu_back=load_img("gfx/menu_background.bmp");
	menu_back_dark=load_img("gfx/menu_background_dark.bmp");
	Mix_AllocateChannels(16);

	for (int i=0; i<joynum; i++) joy[i].joy=SDL_JoystickOpen(i);

	for (int i=0; i<7; i++)
	{
		menu_iconrect[i].x=(i*64)*scale_x;
		menu_iconrect[i].w=64*scale_x;
		menu_iconrect[i].h=64*scale_y;
	}
	for (int i=0; i<9; i++)
	{
		block_rect[i].x=(i*8)*scale_x;
		block_rect[i].y=0;
		block_rect[i].w=8*scale_x;
		block_rect[i].h=8*scale_y;
	}
	for (int i=0; i<3; i++)
	{
	    textbar_rect[i].x=0;
	    textbar_rect[i].y=i*16*scale_y;;
        textbar_rect[i].w=160*scale_x;
        textbar_rect[i].h=16*scale_y;
	}

    for (int i=0; i<MENU_MAX; i++) for (int i2=0; i2<menu_num[i]; i2++) menustr[i][i2].ch=new char[32];
    font.load("font/font.bmp",2*scale_x,5*scale_x,screen,255,0,255);

	init_block();
	return;
}

void generate_config(bool overwrite)
{
	if (!overwrite)
	{
		ifstream fin;
		fin.open("config.cfg");
		if (fin.is_open() && fin.good() && !fin.eof() && !fin.fail() && !fin.bad()) {fin.close(); return;}
	}

	ofstream fout;
	fout.open("config.cfg",ios_base::trunc);
	#ifdef dingux
	fout << VERSION << endl <<
	      D_VERSION << endl <<
    "//Note about control binding:" << endl <<
    "//Types:" << endl <<
    "//0: Keyboard. button=ascii id" << endl <<
    "//1: Joystick +axis. button=axis" << endl <<
    "//2: Joystick -axis. button=axis" << endl <<
    "//3: Joystick button. button=button id" << endl <<
	"scaling_x=" << 1 << endl <<
	"scaling_y=" << 1 << endl <<
	"scaling=" << 1 << endl <<
	"screen_bit=" << 16 << endl <<
	#ifdef OD
	"HWS=" << 0 << endl <<
	#else
	"HWS=" << 0 << endl <<
	#endif
	"joystick=" << -1 << endl <<
	"joystick_deadzone=" << 1600 << endl <<
	"button_right=" << SDLK_RIGHT << endl <<
	"button_down="  << SDLK_DOWN  << endl <<
	"button_left="  << SDLK_LEFT  << endl <<
	"button_up="    << SDLK_UP << endl <<
	"button_menuOK="    << SDLK_LCTRL << endl <<
	"button_hard_drop=" << SDLK_LALT << endl <<
	"button_hold=" << SDLK_LSHIFT << endl <<
	"button_rotate=" << SDLK_LCTRL << endl <<
	"button_pause=" << SDLK_RETURN << endl <<
	"button_type_right=" << 0 << endl <<
	"button_type_down="  << 0  << endl <<
	"button_type_left="  << 0  << endl <<
	"button_type_up="    << 0 << endl <<
	"button_type_menuOK="    << 0 << endl <<
	"button_type_hard_drop=" << 0 << endl <<
	"button_type_hold=" << 0 << endl <<
	"button_type_rotate=" << 0 << endl <<
	"button_type_pause=" << 0 << endl <<

	"MPbutton_P1_right=" << SDLK_RIGHT << endl <<
	"MPbutton_P1_down="  << SDLK_DOWN  << endl <<
	"MPbutton_P1_left="  << SDLK_LEFT  << endl <<
	"MPbutton_P1_up="    << SDLK_UP << endl <<
	"MPbutton_P1_menuOK="    << SDLK_LCTRL << endl <<
	"MPbutton_P1_hard_drop=" << -1 << endl <<
	"MPbutton_P1_hold=" << -1 << endl <<
	"MPbutton_P1_rotate=" << SDLK_UP << endl <<
	"MPbutton_P1_pause=" << SDLK_RETURN << endl <<
	"MPbutton_P1_type_right=" << 0 << endl <<
	"MPbutton_P1_type_down="  << 0  << endl <<
	"MPbutton_P1_type_left="  << 0  << endl <<
	"MPbutton_P1_type_up="    << 0 << endl <<
	"MPbutton_P1_type_menuOK="    << 0 << endl <<
	"MPbutton_P1_type_hard_drop=" << 0 << endl <<
	"MPbutton_P1_type_hold=" << 0 << endl <<
	"MPbutton_P1_type_rotate=" << 0 << endl <<
	"MPbutton_P1_type_pause=" << 0 << endl <<

	"MPbutton_P2_right=" << SDLK_LCTRL << endl <<
	"MPbutton_P2_down="  << SDLK_LALT  << endl <<
	"MPbutton_P2_left="  << SDLK_LSHIFT  << endl <<
	"MPbutton_P2_up="    << SDLK_SPACE << endl <<
	"MPbutton_P2_menuOK="    << SDLK_LCTRL << endl <<
	"MPbutton_P2_hard_drop=" << -1 << endl <<
	"MPbutton_P2_hold=" << -1 << endl <<
	"MPbutton_P2_rotate=" << SDLK_SPACE << endl <<
	"MPbutton_P2_pause=" << SDLK_RETURN << endl <<
	"MPbutton_P2_type_right=" << 0 << endl <<
	"MPbutton_P2_type_down="  << 0  << endl <<
	"MPbutton_P2_type_left="  << 0  << endl <<
	"MPbutton_P2_type_up="    << 0 << endl <<
	"MPbutton_P2_type_menuOK="    << 0 << endl <<
	"MPbutton_P2_type_hard_drop=" << 0 << endl <<
	"MPbutton_P2_type_hold=" << 0 << endl <<
	"MPbutton_P2_type_rotate=" << 0 << endl <<
	"MPbutton_P2_type_pause=" << 0;


	#elif caanoo
	fout << VERSION << endl <<
	      D_VERSION << endl <<
    "//Note about control binding:" << endl <<
    "//Types:" << endl <<
    "//0: Keyboard. button=ascii id" << endl <<
    "//1: Joystick +axis. button=axis" << endl <<
    "//2: Joystick -axis. button=axis" << endl <<
    "//3: Joystick +dpad. button=x or y" << endl <<
    "//4: Joystick -dpad. button=x or y" << endl <<
    "//5: Joystick button. button=button id" << endl <<
	"scaling_x=" << 1 << endl <<
	"scaling_y=" << 1 << endl <<
	"scaling=" << 1 << endl <<
	"screen_bit=" << 16 << endl <<
	"HWS=" << 0 << endl <<
	"joystick=" << 0 << endl <<
	"joystick_deadzone=" << 1600 << endl <<
	"button_right=" << 0 << endl <<
	"button_down="  << 1  << endl <<
	"button_left="  << 0  << endl <<
	"button_up="    << 1 << endl <<
	"button_menuOK="    << 2 << endl <<
	"button_hard_drop=" << 1 << endl <<
	"button_hold=" << 0 << endl <<
	"button_rotate=" << 2 << endl <<
	"button_pause=" << 8 << endl <<
	"button_type_right=" << 1 << endl <<
	"button_type_down="  << 1  << endl <<
	"button_type_left="  << 2  << endl <<
	"button_type_up="    << 2 << endl <<
	"button_type_menuOK="    << 5 << endl <<
	"button_type_hard_drop=" << 5 << endl <<
	"button_type_hold=" << 5 << endl <<
	"button_type_rotate=" << 5 << endl <<
	"button_type_pause=" << 5;
	#else
	fout << VERSION << endl <<
	      D_VERSION << endl <<
    "//Note about control binding:" << endl <<
    "//Types:" << endl <<
    "//0: Keyboard. button=ascii id" << endl <<
    "//1: Joystick +axis. button=axis" << endl <<
    "//2: Joystick -axis. button=axis" << endl <<
    "//3: Joystick button. button=button id" << endl <<
	"scaling_x=" << 2 << endl <<
	"scaling_y=" << 2 << endl <<
	"scaling=" << 2 << endl <<
	"screen_bit=" << 16 << endl <<
	"HWS=" << 0 << endl <<
	"joystick=" << -1 << endl <<
	"joystick_deadzone=" << 1600 << endl <<
	"button_right=" << SDLK_RIGHT << endl <<
	"button_down="  << SDLK_DOWN  << endl <<
	"button_left="  << SDLK_LEFT  << endl <<
	"button_up="    << SDLK_UP << endl <<
	"button_menuOK="    << SDLK_RETURN << endl <<
	"button_hard_drop=" << SDLK_SPACE << endl <<
	"button_hold=" << (int)'c' << endl <<
	"button_rotate=" << SDLK_UP << endl <<
	"button_pause=" << SDLK_ESCAPE << endl <<
	"button_type_right=" << 0 << endl <<
	"button_type_down="  << 0  << endl <<
	"button_type_left="  << 0  << endl <<
	"button_type_up="    << 0 << endl <<
	"button_type_menuOK="    << 0 << endl <<
	"button_type_hard_drop=" << 0 << endl <<
	"button_type_hold=" << 0 << endl <<
	"button_type_rotate=" << 0 << endl <<
	"button_type_pause=" << 0 << endl <<


	"joystick_P1=" << -1 << endl <<
	"MPbutton_P1_right=" << SDLK_RIGHT << endl <<
	"MPbutton_P1_down="  << SDLK_DOWN  << endl <<
	"MPbutton_P1_left="  << SDLK_LEFT  << endl <<
	"MPbutton_P1_up="    << SDLK_UP << endl <<
	"MPbutton_P1_menuOK="    << SDLK_RETURN << endl <<
	"MPbutton_P1_hard_drop=" << SDLK_SPACE << endl <<
	"MPbutton_P1_hold=" << (int)'c' << endl <<
	"MPbutton_P1_rotate=" << SDLK_UP << endl <<
	"MPbutton_P1_pause=" << SDLK_ESCAPE << endl <<
	"MPbutton_P1_type_right=" << 0 << endl <<
	"MPbutton_P1_type_down="  << 0  << endl <<
	"MPbutton_P1_type_left="  << 0  << endl <<
	"MPbutton_P1_type_up="    << 0 << endl <<
	"MPbutton_P1_type_menuOK="    << 0 << endl <<
	"MPbutton_P1_type_hard_drop=" << 0 << endl <<
	"MPbutton_P1_type_hold=" << 0 << endl <<
	"MPbutton_P1_type_rotate=" << 0 << endl <<
	"MPbutton_P1_type_pause=" << 0 << endl <<

    "joystick_P2=" << 0 << endl <<
	"MPbutton_P2_right=" << 0 << endl <<
	"MPbutton_P2_down="  << 1  << endl <<
	"MPbutton_P2_left="  << 0  << endl <<
	"MPbutton_P2_up="    << 1 << endl <<
	"MPbutton_P2_menuOK="    << 2 << endl <<
	"MPbutton_P2_hard_drop=" << 1 << endl <<
	"MPbutton_P2_hold=" << 0 << endl <<
	"MPbutton_P2_rotate=" << 2 << endl <<
	"MPbutton_P2_pause=" << 8 << endl <<
	"MPbutton_P2_type_right=" << 3 << endl <<
	"MPbutton_P2_type_down="  << 3  << endl <<
	"MPbutton_P2_type_left="  << 4  << endl <<
	"MPbutton_P2_type_up="    << 4 << endl <<
	"MPbutton_P2_type_menuOK="    << 5 << endl <<
	"MPbutton_P2_type_hard_drop=" << 5 << endl <<
	"MPbutton_P2_type_hold=" << 5 << endl <<
	"MPbutton_P2_type_rotate=" << 5 << endl <<
	"MPbutton_P2_type_pause=" << 5;
	#endif
	fout.close();
	return;
}

void load_config()
{
	char versionchar[32];
	char filechar[32];
	char resultchar[32];
	char matchchar[32];
	int num;
	int matchchar_count;
	int copy;
	double versiondouble;

	scale_x=1;
	scale_y=1;
	screen_bit=16;

	ifstream fin;
	fin.open("config.cfg");
	fin >> versionchar;
	fin >> versiondouble;

	while(!fin.eof())
	{
		copy=-1;
		matchchar_count=0;
		filechar[0]=0;
		resultchar[0]=0;
		matchchar[0]=0;
		fin >> filechar;
		for (int i=0; i<31; i++)
		{
			if (filechar[i]==0 or filechar[i]==10) break;

			if (copy>=0) {resultchar[copy]=filechar[i]; copy++;}
			if (filechar[i]=='=') {copy=0; matchchar[i]=0;}
			if (copy==-1) {matchchar[matchchar_count]=filechar[i]; matchchar_count++;}
		}
		matchchar[matchchar_count]=0;
		resultchar[copy]=0;

		//cout << "Filechar: " << filechar << endl << "Resultchar: " << resultchar << endl << "Matchchar: " << matchchar << endl;

		     if (str_match(matchchar,(char*)"scaling_x"))  scale_x=strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"scaling_y"))  scale_y=strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"screen_bit")) screen_bit=strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"scaling"))   {scale_x=strtod(resultchar,0); scale_y=strtod(resultchar,0);}
		else if (str_match(matchchar,(char*)"HWS"))       HWS=strtod(resultchar,0);
		//Controls
		else if (str_match(matchchar,(char*)"joystick")) joystick_sel[0]=strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"joystick_deadzone")) joydead=strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_right"))     but[0][but_right]    =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_down" ))     but[0][but_down]     =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_left" ))     but[0][but_left]     =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_up"   ))     but[0][but_up]       =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_menuOK"))    but[0][but_menuOK]   =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_hard_drop")) but[0][but_hard_drop]=strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_hold"))      but[0][but_hold]     =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_rotate"))    but[0][but_rot]      =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_pause"))     but[0][but_pause]    =strtod(resultchar,0);

		else if (str_match(matchchar,(char*)"button_type_right"))     but_type[0][but_right]    =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_type_down" ))     but_type[0][but_down]     =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_type_left" ))     but_type[0][but_left]     =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_type_up"   ))     but_type[0][but_up]       =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_type_menuOK"))    but_type[0][but_menuOK]   =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_type_hard_drop")) but_type[0][but_hard_drop]=strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_type_hold"))      but_type[0][but_hold]     =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_type_rotate"))    but_type[0][but_rot]      =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"button_type_pause"))     but_type[0][but_pause]    =strtod(resultchar,0);


        else if (str_match(matchchar,(char*)"joystick_P#"))           joystick_sel[str_num(matchchar)]=strtod(resultchar,0);

		else if (str_match(matchchar,(char*)"MPbutton_P#_right"))     but[str_num(matchchar)][but_right]    =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_down" ))     but[str_num(matchchar)][but_down]     =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_left" ))     but[str_num(matchchar)][but_left]     =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_up"   ))     but[str_num(matchchar)][but_up]       =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_menuOK"))    but[str_num(matchchar)][but_menuOK]   =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_hard_drop")) but[str_num(matchchar)][but_hard_drop]=strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_hold"))      but[str_num(matchchar)][but_hold]     =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_rotate"))    but[str_num(matchchar)][but_rot]      =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_pause"))     but[str_num(matchchar)][but_pause]    =strtod(resultchar,0);

		else if (str_match(matchchar,(char*)"MPbutton_P#_type_right"))     but_type[str_num(matchchar)][but_right]    =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_type_down" ))     but_type[str_num(matchchar)][but_down]     =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_type_left" ))     but_type[str_num(matchchar)][but_left]     =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_type_up"   ))     but_type[str_num(matchchar)][but_up]       =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_type_menuOK"))    but_type[str_num(matchchar)][but_menuOK]   =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_type_hard_drop")) but_type[str_num(matchchar)][but_hard_drop]=strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_type_hold"))      but_type[str_num(matchchar)][but_hold]     =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_type_rotate"))    but_type[str_num(matchchar)][but_rot]      =strtod(resultchar,0);
		else if (str_match(matchchar,(char*)"MPbutton_P#_type_pause"))     but_type[str_num(matchchar)][but_pause]    =strtod(resultchar,0);
	}

	screenrect.w=320*scale_x;
	screenrect.h=240*scale_y;

	fin.close();
}

double cfg_getdouble(char str[32])
{
	char temp_char[32];
	int copy=-1;

	for (int i=0; i<32; i++)
	{
		if (str[i]==0 or str[i]==10) break;

		if (copy>=0) {temp_char[copy]=str[i]; copy++;}
		if (str[i]=='=') copy=0;
	}
	temp_char[copy]=0;
	return strtod(temp_char,0);
}

bool str_match(char *str,char *str2)
{
	for (int i=0; i<32 && (str[i]!=0 or str2[i]!=0); i++)
	{
		if (str[i]!=str2[i] && !(str2[i]=='#' && str[i]>'0' && str[i]<='9')) return 0;
	}
	return 1;
}

int str_num(char *str)
{
	for (int i=0; i<32; i++)
	{
		if (str[i]>'0' && str[i]<='9') return str[i]-48;
	}
	return 0;
}

int main_menu(int menu_choice)
{
    strcpy(menustr[0][0].ch,"Play");
    strcpy(menustr[0][1].ch,"Multiplayer");
    strcpy(menustr[0][2].ch,"Hi-scores (WIP)");
    strcpy(menustr[0][3].ch,"Statistic (WIP)");
    strcpy(menustr[0][4].ch,"Options");
    strcpy(menustr[0][5].ch,"Quit");
    strcpy(menustr[0][6].ch,"I am a pen");

    strcpy(menustr[2][0].ch,"Online (WIP)");
    strcpy(menustr[2][1].ch,"Hot-seat");
    strcpy(menustr[2][2].ch,"Back");

    FPStimer=SDL_GetTicks();
    fliptimer=SDL_GetTicks();
    FPS=0;
    while(!quit)
    {
        eventblock();

        for(int i=0; i<menu_num[menu_set]; i++)
        {
            temp_rect.x=(cos((menustr_dir[menu_set]+i*(360/menu_num[menu_set]))*PI/180)*(80*scale_x))-
                font.stringw(menustr[menu_set][i].ch)/2;

			temp_rect.y=screenrect.h/2-font.h+sin((menustr_dir[menu_set]+i*
                (360/menu_num[menu_set]))*PI/180)*(80*scale_y),menustr[menu_set][i].ch;
			temp_rect.w=font.stringw(menustr[menu_set][i].ch);
			temp_rect.h=font.h;
			apply_surface(temp_rect.x,temp_rect.y,menu_back_dark,screen,&temp_rect);

            font.draw((cos((menustr_dir[menu_set]+i*(360/menu_num[menu_set]))*
                PI/180)*(80*scale_x))-font.stringw(menustr[menu_set][i].ch)/2,
            screenrect.h/2-font.h+sin((menustr_dir[menu_set]+i*(360/menu_num[menu_set]))*
                PI/180)*(80*scale_y),menustr[menu_set][i].ch,screen);
        }

        if (keyp==SDLK_DOWN) menu_sel[menu_set]=(menu_sel[menu_set]+1)%menu_num[menu_set];
        if (keyp==SDLK_UP)   menu_sel[menu_set]--;
        if (menu_sel[menu_set]<0) menu_sel[menu_set]+=menu_num[menu_set];
        menustr_tdir[menu_set]=-menu_sel[menu_set]*(360/menu_num[menu_set]);
        menustr_dir[menu_set]+=(angle_difference(menustr_tdir[menu_set],menustr_dir[menu_set])*2)*FPS;
        if (menustr_dir[menu_set]<menustr_tdir[menu_set]+1 && menustr_dir[menu_set]>menustr_tdir[menu_set]-1) menustr_dir[menu_set]=menustr_tdir[menu_set];

        if (input_p(but_menuOK,0)) switch(menu_set)
        {
            case 0:
             switch(menu_sel[menu_set])
             {
                 case 0:
                  game_type=GAMETYPE_SP;
                  level_width=10;
                  players=1;
                  if (game_options()==0) return 0;
                  //do temp[1]=game_loop(); while(temp[1]==2);
                  //if (temp[1]==0) return 0;
                 break;

                 case 1:
                  menustr_dir[menu_set]=menustr_tdir[menu_set];
                  menu_set=2;
                  menu_sel[menu_set]=0;
                  menustr_dir[menu_set]=0;
                  menustr_tdir[menu_set]=0;
                 break;

                 case 4:
                  options_menu();
                 break;

                 case 5:
                  return 0;
                 break;
             }
            break;

            case 2:
             switch(menu_sel[menu_set])
             {
                 case 1:
                  game_type=GAMETYPE_COOP;
                  level_width=20;
                  players=2;
                  if (game_options()==0) return 0;
                  /*do temp[1]=game_loop(); while(temp[1]==2);
                  if (temp[1]==0) return 0;*/
                 break;

                 case 2:
                  menu_set=0;
                 break;
             }
            break;
        }
        if (menu_set>0) for (int i=0; i<menu_num[0]; i++)
        {
            temp_rect.x=screenrect.w-font.stringw(menustr[0][(i+menu_sel[0])%menu_num[0]].ch);
            temp_rect.y=i*font.h;
            temp_rect.w=font.stringw(menustr[0][(i+menu_sel[0])%menu_num[0]].ch);
            temp_rect.h=font.h;
            apply_surface(temp_rect.x,temp_rect.y,menu_back_dark,screen,&temp_rect);
            font.draw(temp_rect.x,temp_rect.y,menustr[0][(i+menu_sel[0])%menu_num[0]].ch,screen);
        }

        if (bd[SDLK_RETURN])
        {
            if (bp[SDLK_UP]) {flip_time++; time_flips=1;}
            if (bp[SDLK_DOWN]) flip_time--;
            if (flip_time<=0) {flip_time=0; time_flips=0;}
        }
        //font.draw(0,0,flip_time,screen);

        if (SDL_GetTicks()-fliptimer>flip_time or !time_flips)
        {
            SDL_Flip(screen);
            fliptimer=SDL_GetTicks();
        }

        FPS=(SDL_GetTicks()-FPStimer)/1000.f;
        FPStimer=SDL_GetTicks();
        apply_surface(0,0,menu_back,screen);
    }

    return 0;

	/*char tip_text[32];
	strcpy(tip_text,"Tip of the day (x/8)");

	jstr tip_otd[32]; //Info of the day
	tip_otd[0 ].ch=(char*)"A elevator can take you both up #$and$# down! :D";
	tip_otd[1 ].ch=(char*)"If you can read this, you can read";
	tip_otd[2 ].ch=(char*)"'Idiot' in Swedish means idiot";
	tip_otd[3 ].ch=(char*)"Be quiet so the deaf guys can't hear you";
	tip_otd[4 ].ch=(char*)"Run away from your legs!";
	tip_otd[5 ].ch=(char*)"Show random tips in your program";
	tip_otd[6 ].ch=(char*)"These tips are full of inside jokes";
	tip_otd[7 ].ch=(char*)"Divide by 0";
	tip_otd[8 ].ch=(char*)"Using stat as variabel name can be a bad idea";
	tip_otd[9 ].ch=(char*)"There are no easter eggs here, go away";
	tip_otd[10].ch=(char*)"Steal easter eggs from GTA";
	tip_otd[11].ch=(char*)"Don't give <censured name> a <censured genre> game";
	tip_otd[12].ch=(char*)"Running out of things to put here...";
	tip_otd[13].ch=(char*)"The square root of 1 is 1";
	tip_otd[14].ch=(char*)"Get minecraft";
	tip_otd[15].ch=(char*)"Haters gonna hate";
	#ifdef dingux
	tip_otd[16].ch=(char*)"Get a Dingoo!";
	#elif caanoo
	tip_otd[16].ch=(char*)"Get a Caanoo!";
	#else
	tip_otd[16].ch=(char*)"Get a computer!";
	#endif

	int tip_sel;

	if (menu_choice==0)
	{
		while(!quit)
		{
			srand(time(NULL)/4);
			tip_sel=rand()%17;
			tip_text[16]=49+(time(NULL)/4)%8;

			eventblock();

			if (mbp) Mix_PlayMusic(mus_drop,0);

			if (input_p(but_menuOK,0))
			{
				switch(menu_sel)
				{
					case 0:
					 if (!main_menu(1)) return 0;
					break;

					case 5:
					 return 0;
					break;
				}
			}

			if (mouse_x!=mouse_oldx or mouse_y!=mouse_oldy or mbp)
			{
				if      (mouse_x>=32  && mouse_x<96  && mouse_y>=64  && mouse_y<128) {menu_drawframe=1; menu_sel=0;
				if (mbp) if (!main_menu(1)) return 0;}

				else if (mouse_x>=128 && mouse_x<192 && mouse_y>=64  && mouse_y<128) {menu_drawframe=1; menu_sel=1;
				if (mbp) main_menu(1);}

				else if (mouse_x>=224 && mouse_x<288 && mouse_y>=64  && mouse_y<128) {menu_drawframe=1; menu_sel=2;
				if (mbp) main_menu(1);}

				else if (mouse_x>=32  && mouse_x<96  && mouse_y>=144 && mouse_y<208) {menu_drawframe=1; menu_sel=3;
				if (mbp) main_menu(1);}

				else if (mouse_x>=128 && mouse_x<192 && mouse_y>=144 && mouse_y<208) {menu_drawframe=1; menu_sel=4;
				if (mbp) main_menu(1);}

				else if (mouse_x>=224 && mouse_x<288 && mouse_y>=144 && mouse_y<208) {menu_drawframe=1; menu_sel=5;
				if (mbp) return 0;}

				else menu_drawframe=0;
			}
			if (input_p(but_right,0)) {menu_drawframe=1; menu_sel++; if (menu_sel==3)  menu_sel=0; else if (menu_sel==6) menu_sel=3;}
			if (input_p(but_left,0))  {menu_drawframe=1; menu_sel--; if (menu_sel==-1) menu_sel=2; else if (menu_sel==2) menu_sel=5;}
			if (input_p(but_down,0))  {menu_drawframe=1; menu_sel+=3; if (menu_sel>5) menu_sel-=6;}
			if (input_p(but_up,0))    {menu_drawframe=1; menu_sel-=3; if (menu_sel<0) menu_sel+=6;}

			for (int ix=0; ix<3; ix++)
				apply_surface((32+(ix*96))*scale_x,64*scale_y,menu_icon,screen,&menu_iconrect[ix]);
			for (int ix=0; ix<3; ix++)
				apply_surface((32+(ix*96))*scale_x,144*scale_y,menu_icon,screen,&menu_iconrect[3+ix]);

			//if (menu_drawframe)
			//{
				if (menu_sel<3) apply_surface((32+(menu_sel*96))*scale_x,64*scale_y,menu_icon,screen,&menu_iconrect[6]);
				else apply_surface((32+((menu_sel-3)*96))*scale_x,144*scale_y,menu_icon,screen,&menu_iconrect[6]);
			//}

			#ifdef dingux
			temp_rect.x=0;
			temp_rect.y=screenrect.h-16*scale_y;
			temp_rect.w=stringw("Dingux Version compiled ")+stringw(__DATE__)+2*scale_x;
			temp_rect.h=16*scale_y;
			apply_surface(0,temp_rect.y,menu_back_dark,screen,&temp_rect);
			draw_text(0,screenrect.h-(16*scale_y),(char*)"Dingux version compiled ",screen);
			draw_text(stringw((char*)"Dingux Version compiled "),screenrect.h-(16*scale_y),(char*)__DATE__,screen);

			#elif caanoo
			temp_rect.x=0;
			temp_rect.y=screenrect.h-16*scale_y;
			temp_rect.w=stringw("Caanoo Version compiled ")+stringw(__DATE__)+2*scale_x;
			temp_rect.h=16*scale_y;
			apply_surface(0,temp_rect.y,menu_back_dark,screen,&temp_rect);
			draw_text(8*scale_x,screenrect.h-(24*scale_y),(char*)"Caanoo version compiled ",screen);
			draw_text((8*scale_x)+stringw((char*)"Caanoo version compiled "),screenrect.h-(24*scale_y),(char*)__DATE__,screen);

			#else
			temp_rect.x=0;
			temp_rect.y=screenrect.h-16*scale_y;
			temp_rect.w=stringw("PC Version compiled ")+stringw(__DATE__)+2*scale_x;
			temp_rect.h=16*scale_y;
			apply_surface(0,temp_rect.y,menu_back_dark,screen,&temp_rect);
			draw_text(0,screenrect.h-(16*scale_y),(char*)"PC version compiled ",screen);
			draw_text(stringw((char*)"PC Version compiled "),screenrect.h-(16*scale_y),(char*)__DATE__,screen);
			#endif

			draw_text(8*scale_x,8*scale_y,tip_text,screen);
			draw_text(8*scale_x,24*scale_y,tip_otd[tip_sel].ch,screen);

            if (bd[SDLK_SPACE]) apply_surface(0,0,menu_back,screen);
			SDL_Flip(screen);
			apply_surface(0,0,menu_back,screen);
			//SDL_FillRect(screen,&screenrect,background_col);
		}
	}
	else if (menu_choice==1)
	{
		Uint32 tempcolor=SDL_MapRGB(screen->format,250,150,0);
		SDL_Rect temprect;
		temprect.h=16*scale_y;
		temprect.x=0;
		int gl_ret=1;
		int game_sel=0;
		jstr menu_str[16];
		int tempvarlol=0;
		menu_str[0].ch=(char*)"Start";
		menu_str[1].ch=(char*)"Load";
		menu_str[2].ch=(char*)"";
		menu_str[3].ch=(char*)"name";
		menu_str[4].ch=(char*)"name2";
		menu_str[5].ch=(char*)"name3";
		menu_str[6].ch=(char*)"";
		menu_str[7].ch=(char*)"Preview: ";
		menu_str[8].ch=(char*)"Hold: ";
		menu_str[9].ch=(char*)"Nudge: ";
		menu_str[10].ch=(char*)"Easyspin: ";
		menu_str[11].ch=(char*)"Shadow: ";
		menu_str[12].ch=(char*)"Dark: ";
		menu_str[13].ch=(char*)"Flashlight: ";
		menu_str[14].ch=(char*)"Thunder: ";

		temprect.x=0;
		temprect.y=0;
		temprect.w=128*scale_x;
		temprect.h=16*scale_y;

		while(!quit)
		{
			eventblock();

			if (input_p(but_down,0))
			{
				game_sel++;
				if (game_sel>=15) game_sel=0;
				if (game_sel==2 or game_sel==6) game_sel++;
			}
			if (input_p(but_up,0))
			{
				game_sel--;
				if (game_sel<0) game_sel=14;
				if (game_sel==2 or game_sel==6) game_sel--;
			}

			if (mouse_x!=mouse_oldx or mouse_y!=mouse_oldy) game_sel=mouse_y/16;

			if (input_p(but_menuOK,0) or input_p(but_right,0) or input_p(but_left,0) or mbp)
			{
				if (game_sel==0)
				{
					do gl_ret=game_loop(); while (gl_ret==2);
					if (gl_ret==0) return 0;
				}

				else if (game_sel==7)
				{
					if (input_p(but_left,0)) opt_preview--;
					else opt_preview++;
					if (opt_preview>5) opt_preview=0;
					if (opt_preview<0) opt_preview=5;
				}

				else if (game_sel>7) opt[game_sel-8]=(!opt[game_sel-8]);
			}

			temprect.y=(game_sel*16)*scale_y;
			SDL_FillRect(screen,&temprect,tempcolor);

			for (int i=0; i<15; i++)
			{
				draw_text(8*scale_x,(i*16)*scale_y,menu_str[i].ch,screen);
				if (i==7) draw_number((8*scale_x)+stringw(menu_str[7].ch),(i*16)*scale_y,opt_preview,screen);
				else if (i>7) {if (opt[i-8]) draw_text((8*scale_x)+stringw(menu_str[i].ch),(i*16)*scale_y,(char*)"On",screen);
					else draw_text((8*scale_x)+stringw(menu_str[i].ch),(i*16)*scale_y,(char*)"Off",screen);}
			}
			if (keyp!=-1) tempvarlol=keyp;
			draw_number(128*scale_x,8*scale_y,tempvarlol,screen);
			draw_number(128*scale_x,24*scale_y,bd[tempvarlol],screen);

			SDL_Flip(screen);
			SDL_FillRect(screen,&screenrect,background_col);
		}

	if (quit) return 0;
	return 1;
	}

	return 0;*/
}

void options_menu()
{
    int sel=0;
    int playersel=0;
    double scroll=0;

    jstr menu_str[20];
    for (int i=0; i<20; i++) menu_str[i].ch=new char[24];
    strcpy(menu_str[0].ch,"Right: ");
    strcpy(menu_str[1].ch,"Down: ");
    strcpy(menu_str[2].ch,"Left: ");
    strcpy(menu_str[3].ch,"Up: ");
    strcpy(menu_str[4].ch,"Menu accept: ");
    strcpy(menu_str[5].ch,"Hard drop: ");
    strcpy(menu_str[6].ch,"Hold: ");
    strcpy(menu_str[7].ch,"Rotate: ");
    strcpy(menu_str[8].ch,"Pause: ");

    strcpy(menu_str[9].ch,"Right type: ");
    strcpy(menu_str[10].ch,"Down type: ");
    strcpy(menu_str[11].ch,"Left type: ");
    strcpy(menu_str[12].ch,"Up type: ");
    strcpy(menu_str[13].ch,"Menu accept type: ");
    strcpy(menu_str[14].ch,"Hard drop type: ");
    strcpy(menu_str[15].ch,"Hold type: ");
    strcpy(menu_str[16].ch,"Rotate type: ");
    strcpy(menu_str[17].ch,"Pause type: ");

    strcpy(menu_str[18].ch,"Player: ");
    strcpy(menu_str[19].ch,"Joystick: ");

    while(!quit)
    {
        eventblock();

        if (input_p(but_down,0) && sel<19)
        {
            sel++;
            if (sel==11) sel++;
            if (scroll<sel-13) scroll=sel-13;
        }
        if (input_p(but_up,0) && sel>0)
        {
            sel--;
            if (sel==11) sel--;
            if (scroll>=sel) scroll=sel;
        }

        if (input_p(but_menuOK,0) && sel>=2 && sel<=10)
        {
            while(!quit)
            {
                eventblock();

                if (input_p(but_pause,0)) break;
                if (keyp>=0) {but[playersel][sel-2]=keyp; break;}

            }
            bd[but[0][but_pause]]=0;
            bp[but[0][but_pause]]=0;
        }

        if (input_p(but_pause,0)) break;


        if (sel-2>0)font.draw(160*scale_x,0,menu_str[sel-2].ch,screen);


        for (int i=1; i<=14; i++)
            apply_surface(0,i*16*scale_y,textbar,screen,&textbar_rect[i%2]);
        apply_surface(0,(sel-scroll+1)*16*scale_y,textbar,screen,&textbar_rect[2]);

        font.draw(0,1*font.h,menu_str[18].ch,screen);
        font.draw(0,2*font.h,menu_str[19].ch,screen);
        font.draw(font.stringw(menu_str[18].ch),1*font.h,playersel,screen);
        if (joystick_sel[playersel]==-1) font.draw(font.stringw(menu_str[19].ch),2*font.h,"None",screen);
        else font.draw(font.stringw(menu_str[19].ch),2*font.h,joystick_sel[playersel],screen);

        for (int i=0; i<9; i++)
        {
            font.draw(0,(i-scroll+3)*font.h,menu_str[i].ch,screen);
            font.draw(font.stringw(menu_str[i].ch),(i-scroll+3)*font.h,get_bind_str(i,playersel),screen);
        }

        for (int i=9; i<18; i++)
        {
            font.draw(0,(i-scroll+4)*font.h,menu_str[i].ch,screen);
            font.draw(font.stringw(menu_str[i].ch),(i-scroll+4)*font.h,get_bindtype_str(i-9,playersel),screen);
        }

        SDL_Flip(screen);
        SDL_FillRect(screen,&screenrect,background_col);
    }


    cout << __LINE__ << endl;
    for (int i=0; i<20; i++) delete [] menu_str[i].ch;
    cout << __LINE__ << endl;
}

char* get_bind_str(int Abut, int Aplayer)
{
    strcpy(parsechar,"unknown");
    if (but_type[Aplayer][Abut]==0)
    {
             if (but[Aplayer][Abut]==SDLK_RIGHT) strcpy(parsechar,"Right");
        else if (but[Aplayer][Abut]==SDLK_DOWN)  strcpy(parsechar,"Down");
        else if (but[Aplayer][Abut]==SDLK_LEFT)  strcpy(parsechar,"Left");
        else if (but[Aplayer][Abut]==SDLK_UP)    strcpy(parsechar,"Up");

        #ifdef dingux
        else if (but[Aplayer][Abut]==SDLK_LCTRL) strcpy(parsechar,"A");
        else if (but[Aplayer][Abut]==SDLK_LALT) strcpy(parsechar,"B");
        else if (but[Aplayer][Abut]==SDLK_LSHIFT) strcpy(parsechar,"Y");
        else if (but[Aplayer][Abut]==SDLK_SPACE) strcpy(parsechar,"X");
        else if (but[Aplayer][Abut]==SDLK_TAB) strcpy(parsechar,"L");
        else if (but[Aplayer][Abut]==SDLK_BACKSPACE) strcpy(parsechar,"R");
        else if (but[Aplayer][Abut]==SDLK_RETURN) strcpy(parsechar,"Start");
        else if (but[Aplayer][Abut]==SDLK_ESCAPE) strcpy(parsechar,"Select");
        #elif caanoo

        #else
        else if (but[Aplayer][Abut]==SDLK_LCTRL) strcpy(parsechar,"Left ctrl");
        else if (but[Aplayer][Abut]==SDLK_LALT) strcpy(parsechar,"Left alt");
        else if (but[Aplayer][Abut]==SDLK_LSHIFT) strcpy(parsechar,"Left shift");
        else if (but[Aplayer][Abut]==SDLK_SPACE) strcpy(parsechar,"Space");
        else if (but[Aplayer][Abut]==SDLK_TAB) strcpy(parsechar,"Tab");
        else if (but[Aplayer][Abut]==SDLK_BACKSPACE) strcpy(parsechar,"Backspace");
        else if (but[Aplayer][Abut]==SDLK_RETURN) strcpy(parsechar,"Return");
        else if (but[Aplayer][Abut]==SDLK_ESCAPE) strcpy(parsechar,"Escape");
        #endif
        else if (but[Aplayer][Abut]>=97 && but[Aplayer][Abut]<=122) {parsechar[0]=but[Aplayer][Abut]; parsechar[1]=0;}
    }

    return parsechar;
}

char* get_bindtype_str(int Abut, int Aplayer)
{
    strcpy(parsechar,"unknown");

         if (but_type[Aplayer][Abut]==0) strcpy(parsechar,"Keyboard");
    else if (but_type[Aplayer][Abut]==1) strcpy(parsechar,"Joystick axis +");
    else if (but_type[Aplayer][Abut]==2) strcpy(parsechar,"Joystick axis -");
    else if (but_type[Aplayer][Abut]==3) strcpy(parsechar,"Dpad -");
    else if (but_type[Aplayer][Abut]==4) strcpy(parsechar,"Dpad -");
    else if (but_type[Aplayer][Abut]==5) strcpy(parsechar,"Joystick button");

    return parsechar;
}

int game_options()
{
    int sel=0;
    jstr menu_str[11];
    for (int i=0; i<11; i++) menu_str[i].ch=new char[16];

    menu_str[0].ch=(char*)"Start";
    menu_str[1].ch=(char*)"Load (WIP)";
    menu_str[2].ch=(char*)"";
	menu_str[3].ch=(char*)"Preview: ";
	menu_str[4].ch=(char*)"Hold: ";
	menu_str[5].ch=(char*)"Nudge: ";
	menu_str[6].ch=(char*)"Easyspin: ";
	menu_str[7].ch=(char*)"Shadow: ";
	menu_str[8].ch=(char*)"Dark: ";
	menu_str[9].ch=(char*)"Flashlight: ";
	menu_str[10].ch=(char*)"Thunder: ";

	Uint32 tempcolor=SDL_MapRGB(screen->format,250,150,0);

    while(!quit)
    {
        eventblock();

        if (input_p(but_down,0)) {sel++; if (sel==2) sel=3;}
        if (input_p(but_up,0)){sel--; if (sel==2) sel=1;}
        if (sel>=11) sel=0;
        if (sel<0) sel=10;

        if (input_p(but_menuOK,0))
        {
            if (sel==0)
            {
                do temp[1]=game_loop(); while(temp[1]==2);
                    if (temp[1]==0) return 0;
            }
            if (sel==1)
            {
                sync_load();
                do temp[1]=game_loop(); while(temp[1]==2);
                    if (temp[1]==0) return 0;
            }
            else if (sel==3)
            {
                opt_preview++;
                if (opt_preview>5) opt_preview=0;
            }
            else if (sel>=4)
            {
                opt[sel-4]=(!opt[sel-4]);
            }
        }

        temp_rect.x=0;
        temp_rect.y=sel*font.h;
        //temp_rect.w=font.stringw(menu_str[sel].ch);
        temp_rect.w=160*scale_x;
        temp_rect.h=font.h;
        SDL_FillRect(screen,&temp_rect,tempcolor);

        for (int i=0; i<11; i++)
            font.draw(0,i*font.h,menu_str[i].ch,screen);
        font.draw(font.stringw(menu_str[3].ch),3*font.h,opt_preview,screen);
        for (int i=4; i<11; i++)
        {
            if (opt[i-4]) font.draw(font.stringw(menu_str[i].ch),i*font.h,"On",screen);
            else font.draw(font.stringw(menu_str[i].ch),i*font.h,"Off",screen);
        }

        switch(sel)
        {
            case 3:
             font.draw(160*scale_x,0,"Shows your next piece",screen,320*scale_x);
            break;
            case 4:
             font.draw(160*scale_x,0,"Allows you to 'hold' a\npiece and use it later",screen,320*scale_x);
            break;
            case 5:
             font.draw(160*scale_x,0,"Makes rotations easier and allows for more\ntwists.\n\nAlso known as\n'wall kick'",screen,320*scale_x);
            break;
            case 6:
             font.draw(160*scale_x,0,"Increase your time \nbefore locking by \nrotating",screen,320*scale_x);
            break;
            case 7:
             font.draw(160*scale_x,0,"Shows a shadow under your piece",screen,320*scale_x);
            break;
            case 8:
             font.draw(160*scale_x,0,"Makes the screen dark,\ngood memory training.\n\nFlashlight, thunder or\nflare recommended\nwhen using this",screen,320*scale_x);
            break;
            case 9:
             font.draw(160*scale_x,0,"Shows the blocks \nunder your piece.\n\nMust be used\nwith 'dark' to have\nany effect",screen,320*scale_x);
            break;
            case 10:
             font.draw(160*scale_x,0,"The map lights up\nsometimes, getting line\nclears increases the\nchance of 'thunder'.\n\nMust be used\nwith 'dark' to have\nany effect",screen,320*scale_x);
            break;
        }

        SDL_Flip(screen);
        SDL_FillRect(screen,&screenrect,background_col);
    }

    for (int i=0; i<11; i++) delete [] menu_str[i].ch;
    return 0;
}


int game_loop()
{
	sync_init();
	SDL_Rect temprect;
	temprect.x=16*scale_x;
	temprect.y=16*scale_y;
	temprect.w=80;
	temprect.h=160;

	Mix_PlayMusic(mus_music,0);

	while(!quit)
	{
		eventblock();
		for (int i=0; i<players; i++)
		{
		    if (blocktype[i]==-1) new_block(i);
		}

		sync_move();
		sync_drop();
		sync_rot();
		sync_hardrop(); //Loose-check
		sync_hold();

		if (bp['a'])
            flare_use=1;

		if (input_p(but_pause,0))
		{
			switch(sync_pause())
			{
				case 0:
				 return 0;
				break;
				case 2:
				 return 2;
				break;
				case 3:
				 return 1;
				break;
			}
		}

		sync_fall(); //loose check here

		if (opt[OPT_FLASHLIGHT])
		{
			for (int i=0; i<level_width; i++) board_flashlight[i]=0;
			for (int i=0; i<players; i++) for (int ix=0; ix<4; ix++) for (int iy=0; iy<4; iy++)
			if (block[i][ix][iy])
			{
				board_flashlight[ix+x[0]]=1;
				if (ix+x[0]-1>=0) board_flashlight[ix+x[0]-1]=1;
				if (ix+x[0]+1<20) board_flashlight[ix+x[0]+1]=1;
			}
		}


		/*if (1) //opt[OPT_FLASHLIGHT])
		{
		    for (int ix=0; ix<level_width; ix++) for (int iy=0; iy<22; iy++) board_dark[ix][iy]=0;
		    for (int ix=0; ix<level_width; ix++) for (int iy=0; iy<22; iy++) if (board_flare[ix][iy])
		    {
		        for (int ix2=ix-1; ix2<=ix+1; ix2++) for (int iy2=iy-1; iy2<=iy+1; iy2++)
                    if (ix2>=0 && ix2<level_width && iy2>=0 && iy2<22)
                    {
                        board_dark[ix2][iy2-2]=1;
                    }
		    }

		    if (flare_use) for (int ix=x[0]; ix<x[0]+4; ix++) for (int iy=y[0]; iy<y[0]+4; iy++) if (block[0][ix-x[0]][iy-y[0]])
		    {
		        for (int ix2=ix-1; ix2<=ix+1; ix2++) for (int iy2=iy-1; iy2<=iy+1; iy2++)
                    if (ix2>=0 && ix2<level_width && iy2>=0 && iy2<22)
                    {
                        board_dark[ix2][iy2-2]=1;
                    }
		    }
		}*/


		if (opt[OPT_THUNDER])
		{
			if ((int)((SDL_GetTicks()-gametimer)/1000)>thunder_time)
			{
				if (thunder_chance) {if (rand()%5) {thunder=1; thunder_chance--;}}
				else if (rand()%8==0) thunder=1;
				thunder_time=(SDL_GetTicks()-gametimer)/1000;
			}
			if ((int)((SDL_GetTicks()-gametimer)/500)>thunder_time*2) thunder=0;
		}

        font.draw(0,0,joy[0].hatold[1],screen);
		sync_draw();

		if (SDL_GetTicks()-fliptimer>flip_time or !time_flips)
        {
            SDL_Flip(screen);
            fliptimer=SDL_GetTicks();
        }
		SDL_FillRect(screen,&screenrect,background_col);

		#ifdef caanoo
		while(joy[0].bd[7] && !quit)
		{
			eventblock();
			SDL_FillRect(screen,&temprect,SDL_MapRGB(screen->format,0,0,0));
			SDL_UpdateRect(screen,temprect.x,temprect.y,temprect.w,temprect.h);
		}
		#elifdef dingux
		while(bd[18] && !quit)
		{
			eventblock();
			SDL_FillRect(screen,&temprect,SDL_MapRGB(screen->format,0,0,0));
			SDL_UpdateRect(screen,temprect.x,temprect.y,temprect.w,temprect.h);
		}
		#endif

		gameticks[0][0]=SDL_GetTicks();
	}
	if (quit) return 0;
	return 1;
}

int new_block(int Aplayer)
{
    if (block_collide(xstart[Aplayer],ystart[Aplayer],0,preview[Aplayer][0],Aplayer)==2)
    {
        blocktype[Aplayer]=-1;
        return 2;
    }

    x[Aplayer]=xstart[Aplayer];
    y[Aplayer]=ystart[Aplayer];

	rot[Aplayer]=0;
	holdused[Aplayer]=0;
	if (opt[OPT_BAG])
	{

	}
	else
	{
		//if (!opt[OPT_TRAINING])
		blocktype[Aplayer]=preview[Aplayer][0];
		for (int ix=0; ix<4; ix++) for (int iy=0; iy<4; iy++)
                block[Aplayer][ix][iy]=blockrot[blocktype[Aplayer]][rot[Aplayer]][ix][iy];
		for (int i=1; i<5; i++) preview[Aplayer][i-1]=preview[Aplayer][i];
		preview[Aplayer][4]=1+rand()%7;
	}

	sync_shadow();
	if (!block_collide(x[Aplayer],y[Aplayer],rot[Aplayer],blocktype[Aplayer],Aplayer)) return 0;
	game_stats[STAT_PIECES]++;

	return 1;
}

bool place_block(int Aplayer)
{
	Mix_PlayChannel(-1,mus_drop,0);
	if (game_type==GAMETYPE_COOP) {for (int ix=0; ix<4; ix++) for (int iy=0; iy<4; iy++)
        if (block[Aplayer][ix][iy]) board[x[Aplayer]+ix][y[Aplayer]+iy]=Aplayer+1;}

	else {for (int ix=0; ix<4; ix++) for (int iy=0; iy<4; iy++)
        if (block[Aplayer][ix][iy]) board[x[Aplayer]+ix][y[Aplayer]+iy]=block[Aplayer][ix][iy];}

    if (flare_use) {for (int ix=0; ix<4; ix++) for (int iy=0; iy<4; iy++)
        if (block[Aplayer][ix][iy]) board_flare[x[Aplayer]+ix][y[Aplayer]+iy]=1; flare_use=0;}

	check_line();
	easyspins[Aplayer]=0;
	return new_block(Aplayer);
}

int block_collide(int Ax, int Ay, int Arot, int Atype, int Aplayer)
{
    if (Atype==-1) return 1;
	int is_free=1;

    for (int iplayer=0; iplayer<players; iplayer++)
    {
        if (iplayer==Aplayer) continue;
        if (blocktype[iplayer]==-1) continue;

        for (int ix=0; ix<4;  ix++) for (int iy=0; iy<4; iy++)
        {
            if (Ax+ix-x[iplayer]>=0 && Ax+ix-x[iplayer]<4 && Ay+iy-y[iplayer]>=0 && Ay+iy-y[iplayer]<4)
            {
                if (blockrot[Atype][Arot][ix][iy] && blockrot[blocktype[iplayer]][rot[iplayer]][Ax+ix-x[iplayer]][Ay+iy-y[iplayer]])
                return 2;
            }
        }
    }

    for (int ix=0; ix<4;  ix++) for (int iy=0; iy<4; iy++)
    {
        if (Ax+ix<0 or Ax+ix>=level_width or Ay+iy<0 or Ay+iy>=22)
        {
            if (blockrot[Atype][Arot][ix][iy]) is_free=0;
            continue;
        }
        if (blockrot[Atype][Arot][ix][iy] && board[Ax+ix][Ay+iy]) is_free=0;
    }
	return is_free;
}

void check_line()
{
	bool is_line;
	int lines=0;

	for (int iy=0; iy<22; iy++)
	{
		is_line=1;
		while(is_line)
		{
			for (int ix=0; ix<level_width; ix++)
			{
				if (board[ix][iy]==0) is_line=0;
			}
			if (is_line)
			{
				lines++;
				for (int i=0; i<players; i++)
				{
				    fall_lines[i]--;
				    if (fall_lines[i]<=0)
				    {
				        level[i]++;
				        fall_lines[i]=level[i]*5;
				        falltime[i]-=30;
				        if (falltime[i]<25) falltime[i]=25;
				    }
				}
				for (int ix2=0; ix2<level_width; ix2++) for (int iy2=iy; iy2>0; iy2--)
				{
                    board[ix2][iy2]=board[ix2][iy2-1];
                    board_flare[ix2][iy2]=board_flare[ix2][iy2-1];
				}
				for (int ix2=0; ix2<level_width; ix2++) board[ix2][0]=0;
			}
		}
	}
	if (lines==1) game_stats[STAT_SCORE]+=100;
	else if (lines==2) game_stats[STAT_SCORE]+=300;
	else if (lines==3) game_stats[STAT_SCORE]+=500;
	else if (lines==4)
	{
	    game_stats[STAT_SCORE]+=800;
        for (int i=0; i<players; i++)
        {
            fall_lines[i]--;
            if (fall_lines[i]<=0)
            {
                level[i]++;
                fall_lines[i]=level[i]*5;
                falltime[i]-=30;
                if (falltime[i]<25) falltime[i]=25;
            }
        }
	}
	if (lines>0) {game_stats[lines-1]++; thunder_chance+=lines;}
}

void block_rot(int Aplayer)
{
	if (blocktype[Aplayer]==4) return;
	int oldrot=rot[Aplayer];
	bool canrot=0;
	int temp_x=0;
	int temp_y=0;

	rot[Aplayer]++;
	if (rot[Aplayer]>3) rot[Aplayer]=0;
	if (opt[OPT_NUDGE])
	{
		for (int i=0; i<9; i++)
		{
			if (i==1) temp_x=1;
			else if (i==2) temp_x=-1;
			else if (i==3) {temp_x=0; temp_y=1;}
			else if (i==4) temp_x=1;
			else if (i==5) temp_x=-1;
			else if (i==6) {temp_y=-1; temp_x=0;}
			else if (i==7) temp_x=1;
			else if (i==8) temp_x=-1;

			if (block_collide(x[Aplayer]+temp_x,y[Aplayer]+temp_y,rot[Aplayer],blocktype[Aplayer],Aplayer)==1) {canrot=1; break;}
		}

		if (!canrot) rot[Aplayer]=oldrot;
		else
		{
			x[Aplayer]+=temp_x;
			y[Aplayer]+=temp_y;

			for (int ix=0; ix<4; ix++) for (int iy=0; iy<4; iy++)
				block[Aplayer][ix][iy]=blockrot[blocktype[Aplayer]][rot[Aplayer]][ix][iy];
			if (opt[OPT_EASYSPIN] && easyspins[Aplayer]<5)
				{place[Aplayer]=((SDL_GetTicks()-gametimer)/falltime[Aplayer])+1; easyspins[Aplayer]++;}
		}
	}
	else
	{
		if (block_collide(x[Aplayer],y[Aplayer],rot[Aplayer],blocktype[Aplayer],Aplayer)==1)
		{
			if (opt[OPT_EASYSPIN] && easyspins[Aplayer]<5)
				{place[Aplayer]=((SDL_GetTicks()-gametimer)/falltime[Aplayer])+1; easyspins[Aplayer]++;}
			for (int ix=0; ix<4; ix++) for (int iy=0; iy<4; iy++)
                block[Aplayer][ix][iy]=blockrot[blocktype[Aplayer]][rot[Aplayer]][ix][iy];
		}
		else rot[Aplayer]=oldrot;
	}
}

void init_block()
{
	for (int i=0; i<8; i++) for (int irot=0; irot<4; irot++) for (int ix=0; ix<4; ix++) for (int iy=0; iy<4; iy++) blockrot[i][irot][ix][iy]=0;

	//I block
	previewx[1]=0;
	previewy[1]=0;

	blockrot[1][0][0][1]=1;
	blockrot[1][0][1][1]=1;
	blockrot[1][0][2][1]=1;
	blockrot[1][0][3][1]=1;

	blockrot[1][1][2][0]=1;
	blockrot[1][1][2][1]=1;
	blockrot[1][1][2][2]=1;
	blockrot[1][1][2][3]=1;

	blockrot[1][2][0][2]=1;
	blockrot[1][2][1][2]=1;
	blockrot[1][2][2][2]=1;
	blockrot[1][2][3][2]=1;

	blockrot[1][3][1][0]=1;
	blockrot[1][3][1][1]=1;
	blockrot[1][3][1][2]=1;
	blockrot[1][3][1][3]=1;


	//J
	previewx[2]=4;
	previewy[2]=-4;

	blockrot[2][0][0][1]=2;
	blockrot[2][0][0][2]=2;
	blockrot[2][0][1][2]=2;
	blockrot[2][0][2][2]=2;

	blockrot[2][1][2][1]=2;
	blockrot[2][1][1][1]=2;
	blockrot[2][1][1][2]=2;
	blockrot[2][1][1][3]=2;

	blockrot[2][2][0][2]=2;
	blockrot[2][2][1][2]=2;
	blockrot[2][2][2][2]=2;
	blockrot[2][2][2][3]=2;

	blockrot[2][3][1][1]=2;
	blockrot[2][3][1][2]=2;
	blockrot[2][3][1][3]=2;
	blockrot[2][3][0][3]=2;


	//L
	previewx[3]=4;
	previewy[3]=-4;

	blockrot[3][0][0][2]=3;
	blockrot[3][0][1][2]=3;
	blockrot[3][0][2][2]=3;
	blockrot[3][0][2][1]=3;

	blockrot[3][1][1][1]=3;
	blockrot[3][1][1][2]=3;
	blockrot[3][1][1][3]=3;
	blockrot[3][1][2][3]=3;

	blockrot[3][2][0][3]=3;
	blockrot[3][2][0][2]=3;
	blockrot[3][2][1][2]=3;
	blockrot[3][2][2][2]=3;

	blockrot[3][3][0][1]=3;
	blockrot[3][3][1][1]=3;
	blockrot[3][3][1][2]=3;
	blockrot[3][3][1][3]=3;


	//O
	previewx[4]=0;
	previewy[4]=-4;

	blockrot[4][0][1][1]=4;
	blockrot[4][0][1][2]=4;
	blockrot[4][0][2][1]=4;
	blockrot[4][0][2][2]=4;

	blockrot[4][1][1][1]=4;
	blockrot[4][1][1][2]=4;
	blockrot[4][1][2][1]=4;
	blockrot[4][1][2][2]=4;

	blockrot[4][2][1][1]=4;
	blockrot[4][2][1][2]=4;
	blockrot[4][2][2][1]=4;
	blockrot[4][2][2][2]=4;

	blockrot[4][3][1][1]=4;
	blockrot[4][3][1][2]=4;
	blockrot[4][3][2][1]=4;
	blockrot[4][3][2][2]=4;


	//S
	previewx[5]=4;
	previewy[5]=-8;

	blockrot[5][0][0][3]=5;
	blockrot[5][0][1][3]=5;
	blockrot[5][0][1][2]=5;
	blockrot[5][0][2][2]=5;

	blockrot[5][1][1][1]=5;
	blockrot[5][1][1][2]=5;
	blockrot[5][1][2][2]=5;
	blockrot[5][1][2][3]=5;

	blockrot[5][2][0][3]=5;
	blockrot[5][2][1][3]=5;
	blockrot[5][2][1][2]=5;
	blockrot[5][2][2][2]=5;

	blockrot[5][3][0][1]=5;
	blockrot[5][3][0][2]=5;
	blockrot[5][3][1][2]=5;
	blockrot[5][3][1][3]=5;


	//T
	previewx[6]=4;
	previewy[6]=-4;

	blockrot[6][0][0][2]=6;
	blockrot[6][0][1][2]=6;
	blockrot[6][0][2][2]=6;
	blockrot[6][0][1][1]=6;

	blockrot[6][1][1][1]=6;
	blockrot[6][1][1][2]=6;
	blockrot[6][1][1][3]=6;
	blockrot[6][1][2][2]=6;

	blockrot[6][2][0][2]=6;
	blockrot[6][2][1][2]=6;
	blockrot[6][2][2][2]=6;
	blockrot[6][2][1][3]=6;

	blockrot[6][3][1][1]=6;
	blockrot[6][3][1][2]=6;
	blockrot[6][3][1][3]=6;
	blockrot[6][3][0][2]=6;


	//Z
	previewx[7]=4;
	previewy[7]=-4;

	blockrot[7][0][0][1]=7;
	blockrot[7][0][1][1]=7;
	blockrot[7][0][1][2]=7;
	blockrot[7][0][2][2]=7;

	blockrot[7][1][2][1]=7;
	blockrot[7][1][2][2]=7;
	blockrot[7][1][1][2]=7;
	blockrot[7][1][1][3]=7;

	blockrot[7][2][0][2]=7;
	blockrot[7][2][1][2]=7;
	blockrot[7][2][1][3]=7;
	blockrot[7][2][2][3]=7;

	blockrot[7][3][1][1]=7;
	blockrot[7][3][1][2]=7;
	blockrot[7][3][0][2]=7;
	blockrot[7][3][0][3]=7;
}

int pause_menu()
{
	SDL_Rect temprect[2];
	Uint32 tempcolor[2];
	tempcolor[0]=SDL_MapRGB(screen->format,0,0,0);
	tempcolor[1]=SDL_MapRGB(screen->format,192,192,192);
	temprect[0].x=16*scale_x;
	temprect[0].y=16*scale_y;
	temprect[0].w=80*scale_x;
	temprect[0].h=160*scale_y;

	temprect[1].x=16*scale_x;
	temprect[1].y=16*scale_y;
	temprect[1].w=80*scale_x;
	temprect[1].h=16*scale_y;
	jstr menu_text[6];
	menu_text[0].ch=(char*)"Resume";
	menu_text[1].ch=(char*)"Reset";
	menu_text[2].ch=(char*)"Options";
	menu_text[3].ch=(char*)"Go to menu";
	menu_text[4].ch=(char*)"Save&quit";
	menu_text[5].ch=(char*)"Quit";

	int pause_sel=0;

	while(!quit)
	{
		eventblock();
		if (input_p(but_pause,0)) return 1;
		if (input_p(but_down,0))
		{
			pause_sel++;
			if (pause_sel>=6) pause_sel=0;
			temprect[1].y=(16+(pause_sel*16))*scale_y;
		}
		else if (input_p(but_up,0))
		{
			pause_sel--;
			if (pause_sel<0) pause_sel=5;
			temprect[1].y=(16+(pause_sel*16))*scale_y;
		}
		else if (input_p(but_menuOK,0)) switch(pause_sel)
		{
			case 0:
			 return 1;
			break;

			case 1:
			 return 2;
			break;

			case 2:
			 //options
			break;

			case 3:
			 return 3;
			break;

			case 4:
			 sync_save();
			 return 0;
			break;

			case 5:
			 return 0;
			break;
		}

		SDL_FillRect(screen,&temprect[1],tempcolor[1]);

		for (int i=0; i<6; i++) font.draw(16*scale_x,(16+i*16)*scale_y,menu_text[i].ch,screen);

        if (SDL_GetTicks()-fliptimer>flip_time or !time_flips)
        {
            SDL_Flip(screen);
            fliptimer=SDL_GetTicks();
        }
		//SDL_UpdateRect(screen,temprect[0].x,temprect[0].y,temprect[0].w,temprect[0].h);
		//SDL_Flip(screen);
		SDL_FillRect(screen,&temprect[0],tempcolor[0]);
	}

	return 1;
}

void stats_screen()
{

	Uint32 tempcolor[2];
	tempcolor[0]=SDL_MapRGB(screen->format,0,0,0);
	tempcolor[1]=SDL_MapRGB(screen->format,192,192,192);

	double stat_show[8];
	jstr stat_str[9];
	stat_str[0].ch=(char*)"Singles: ";
	stat_str[1].ch=(char*)"Doubles: ";
	stat_str[2].ch=(char*)"Triples: ";
	stat_str[3].ch=(char*)"Quads: ";
	stat_str[4].ch=(char*)"Pieces: ";
	stat_str[5].ch=(char*)"Hard drops: ";
	stat_str[6].ch=(char*)"Holds used: ";
	stat_str[7].ch=(char*)"Score: ";
	stat_str[8].ch=(char*)"Time: ";
	int stat_sel=0;

	Uint32 menu_timer[2];
	menu_timer[0]=SDL_GetTicks();
	menu_timer[1]=SDL_GetTicks();
	while(!quit)
	{
		eventblock();

		if (stat_sel<8)
		{
			stat_show[stat_sel]+=((SDL_GetTicks()-menu_timer[0])/100.0)+(game_stats[stat_sel]*((SDL_GetTicks()-menu_timer[0])/5000.0));
			menu_timer[0]=SDL_GetTicks();
		}
		if (input_p(but_menuOK,0) && stat_sel<8) stat_show[stat_sel]=game_stats[stat_sel];
		if (input_p(but_pause,0) && stat_sel<8)
		{
			for (int i=0; i<8; i++) stat_show[i]=game_stats[i];
			stat_sel=8;
		}

		if (input_p(but_menuOK,0) && stat_sel==8) return;

		if (stat_show[stat_sel]>=game_stats[stat_sel] && stat_sel<8)
		{
			stat_show[stat_sel]=game_stats[stat_sel];
			stat_sel++;
		}
		for (int i=0; i<8; i++)
		{
			font.draw(8*scale_x,(8+i*16)*scale_y,stat_str[i].ch,screen);
			font.draw((8*scale_x)+font.stringw(stat_str[i].ch),(8+i*16)*scale_y,(int)stat_show[i],screen);
		}
		font.draw(8*scale_x,(8+8*16)*scale_y,stat_str[8].ch,screen);
		font.draw((8*scale_x)+font.stringw(stat_str[8].ch),(8+8*16)*scale_y,(menu_timer[1]-gametimer)/1000,screen);

		font.draw(8*scale_x,screenrect.h-(56*scale_y),(char*)"Save replay: yes",screen);
		font.draw(8*scale_x,screenrect.h-(40*scale_y),(char*)"Save stats: yes",screen);
		font.draw(8*scale_x,screenrect.h-(24*scale_y),(char*)"Quit to menu",screen);

		if (SDL_GetTicks()-fliptimer>flip_time or !time_flips)
        {
            SDL_Flip(screen);
            fliptimer=SDL_GetTicks();
        }
		SDL_FillRect(screen,&screenrect,tempcolor[0]);
	}
}

double angle_difference(double ang1,double ang2)
{
    double difference=ang1-ang2;
    while (difference<-180)difference+=360;
    while (difference>180) difference-=360;
    return difference;
}
