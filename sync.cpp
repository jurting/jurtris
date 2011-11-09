void sync_move()
{
	switch (game_type)
	{
		case GAMETYPE_SP:
		case GAMETYPE_COOP:
		 for (int iplayer=1; iplayer<=players; iplayer++)
		 {
            if (blocktype[iplayer-1]==-1) continue;

		 	if (input_p(but_right,iplayer-(game_type==GAMETYPE_SP))) moveticks[iplayer][0]=SDL_GetTicks();
			if (input_p(but_left ,iplayer-(game_type==GAMETYPE_SP))) moveticks[iplayer][2]=SDL_GetTicks();

		 	if (input_p(but_right,iplayer-(game_type==GAMETYPE_SP)) or
		 	(input_h(but_right,iplayer-(game_type==GAMETYPE_SP)) &&
		 	SDL_GetTicks()-moveticks[iplayer][0]>192 && SDL_GetTicks()-moveticks[iplayer][1]>48))
		 	{
		 		moveticks[iplayer][1]=SDL_GetTicks();
		 		if (block_collide(x[iplayer-1]+1,y[iplayer-1],rot[iplayer-1],blocktype[iplayer-1],iplayer-1)==1) x[iplayer-1]++;
		 		sync_shadow();
		 	}

		 	else if (input_p(but_left,iplayer-(game_type==GAMETYPE_SP)) or
		 	(input_h(but_left,iplayer-(game_type==GAMETYPE_SP)) &&
		 	SDL_GetTicks()-moveticks[iplayer][2]>192 && SDL_GetTicks()-moveticks[iplayer][3]>48))
		 	{
		 		moveticks[iplayer][3]=SDL_GetTicks();
		 		if (block_collide(x[iplayer-1]-1,y[iplayer-1],rot[iplayer-1],blocktype[iplayer-1],iplayer-1)==1) x[iplayer-1]--;
		 		sync_shadow();
		 	}
		 }
		break;

	}
}

void sync_drop()
{
	switch(game_type)
	{
		case GAMETYPE_SP:
		case GAMETYPE_COOP:
		 update_shadow=0;
		 for (int iplayer=1; iplayer<=players; iplayer++)
		 {
            if (blocktype[iplayer-1]==-1) continue;

		 	if ((input_h(but_down,iplayer-(game_type==GAMETYPE_SP)) &&
		 	SDL_GetTicks()-gameticks[iplayer][1]>48) or
		 	input_p(but_down,iplayer-(game_type==GAMETYPE_SP)))
		 	{
		 		if (block_collide(x[iplayer-1],y[iplayer-1]+1,rot[iplayer-1],blocktype[iplayer-1],iplayer-1)==1)
		 		{
		 			y[iplayer-1]++;
		 			gameticks[iplayer][1]=SDL_GetTicks();
		 			fall[iplayer-1]=(SDL_GetTicks()-gametimer)/falltime[iplayer-1];
		 			place[iplayer-1]=fall[iplayer-1]+(1*opt[OPT_EASYSPIN]);
		 			game_stats[STAT_SCORE]++;
		 			update_shadow=1;
		 		}
		 	}
		 }

		 if (update_shadow==1) {for (int iplayer=1; iplayer<=players; iplayer++)
		 {
		     if (blocktype[iplayer-1]==-1) continue;
		     sync_shadow();
		 }}
		break;
	}
}

void sync_rot()
{
	switch(game_type)
	{
		case GAMETYPE_SP:
		case GAMETYPE_COOP:
		 for (int iplayer=1; iplayer<=players; iplayer++)
		 {
            if (blocktype[iplayer-1]==-1) continue;
		 	if (input_p(but_up,iplayer-(game_type==GAMETYPE_SP)) or input_p(but_rot,iplayer-(game_type==GAMETYPE_SP)))
		 	{
		 		block_rot(iplayer-1);
		 		sync_shadow();
		 	}
		 }
		break;
	}
}

void sync_hardrop()
{
    int temp_status=0;
    int temp_y=0;
	switch(game_type)
	{
		case GAMETYPE_SP:
		case GAMETYPE_COOP:
		 for (int iplayer=1; iplayer<=players; iplayer++)
		 {
            if (blocktype[iplayer-1]==-1) continue;

		 	if (input_p(but_hard_drop,iplayer-(game_type==GAMETYPE_SP)))
		 	{
		 	    temp_y=y[iplayer-1];
		 	    temp_status=0;

		 		do {temp_status=block_collide(x[iplayer-1],temp_y+1,rot[iplayer-1],blocktype[iplayer-1],iplayer-1);
		 		if (temp_status==1) temp_y++;}
		 		while (temp_status==1);

                if (temp_status==0)
                {
                    game_stats[STAT_DROPS]++;
                    y[iplayer-1]=temp_y;
                    place[iplayer-1]=((SDL_GetTicks()-gametimer)/falltime[iplayer-1]);
                    if (!place_block(iplayer-1)) {stats_screen(); /*loose*/}
                }
		 	}
		 }
		break;
	}
}

void sync_hold()
{
	switch(game_type)
	{
		case GAMETYPE_SP:
		case GAMETYPE_COOP:
		 for (int iplayer=1; iplayer<=players; iplayer++)
		 {
            if (blocktype[iplayer-1]==-1) continue;
		 	if (input_p(but_hold,iplayer-(game_type==GAMETYPE_SP)) && opt[OPT_HOLD])
		 	{
		 		if (opt[OPT_TRAINING])
		 		{
		 			blocktype[iplayer-1]++;
		 			if (blocktype[iplayer-1]>7) blocktype[iplayer-1]=1;
		 			for (int ix=0; ix<4; ix++) for (int iy=0; iy<4; iy++)
		 				block[iplayer-1][ix][iy]=blockrot[blocktype[iplayer-1]][rot[iplayer-1]][ix][iy];
		 			x[iplayer-1]=xstart[iplayer-1];
		 			y[iplayer-1]=ystart[iplayer-1];
		 		}
		 		else if (hold[iplayer-1]==0) {hold[iplayer-1]=blocktype[iplayer-1]; new_block(iplayer-1); holdused[iplayer-1]=1; game_stats[STAT_HOLDS]++;}
		 		else if (!holdused[iplayer-1])
		 		{
		 			holdused[iplayer-1]=1;
		 			temp[0]=blocktype[iplayer-1];
		 			blocktype[iplayer-1]=hold[iplayer-1];
		 			hold[iplayer-1]=temp[0];
		 			x[iplayer-1]=xstart[iplayer-1];
		 			y[iplayer-1]=ystart[iplayer-1];
		 			rot[iplayer-1]=0;
		 			game_stats[STAT_HOLDS]++;
		 			for (int ix=0; ix<4; ix++) for (int iy=0; iy<4; iy++)
		 				block[iplayer-1][ix][iy]=blockrot[blocktype[iplayer-1]][rot[iplayer-1]][ix][iy];
		 		}
		 			sync_shadow();
		 	}
		 }
		break;
	}
}

int sync_pause()
{
	return pause_menu();
	return 1;
}

void sync_fall()
{
	switch(game_type)
	{
		case GAMETYPE_SP:
		case GAMETYPE_COOP:
		update_shadow=1;
		 for (int iplayer=1; iplayer<=players; iplayer++)
		 {
            if (blocktype[iplayer-1]==-1) continue;

		 	if ((int)((SDL_GetTicks()-gametimer)/falltime[iplayer-1])>fall[iplayer-1] && !opt[OPT_TRAINING])
		 	{
		 		if (block_collide(x[iplayer-1],y[iplayer-1]+1,rot[iplayer-1],blocktype[iplayer-1],iplayer-1)==1)
		 		{
		 			y[iplayer-1]++;
		 			place[iplayer-1]=(SDL_GetTicks()-gametimer)/falltime[iplayer-1];
		 			easyspins[iplayer-1]=0;
		 			gameticks[iplayer][1]=SDL_GetTicks();
		 			update_shadow=1;
		 		}
		 		if ((int)((SDL_GetTicks()-gametimer)/falltime[iplayer-1])>place[iplayer-1] &&
		 		!block_collide(x[iplayer-1],y[iplayer-1]+1,rot[iplayer-1],blocktype[iplayer-1],iplayer-1))
		 		{
		 			if (!place_block(iplayer-1)) {stats_screen(); /*loose*/}
		 			gameticks[iplayer][1]=SDL_GetTicks();
		 		}
		 		fall[iplayer-1]=(SDL_GetTicks()-gametimer)/falltime[iplayer-1];
		 	}
		 }

		 if (update_shadow==1) {for (int iplayer=1; iplayer<=players; iplayer++)
		 {
		     if (blocktype[iplayer-1]==-1) continue;
		     sync_shadow();
		 }}
		break;
	}
}

void sync_draw()
{
	switch(game_type)
	{
		case GAMETYPE_SP:
		 for (int ix=0; ix<level_width; ix++) for (int iy=0; iy<20; iy++)
		 {
		 	if (board_dark[ix][iy] or board_flashlight[ix] or thunder or !opt[OPT_DARK])
		 		apply_surface((16+ix*8)*scale_x,(16+iy*8)*scale_y,sur_block,screen,&block_rect[board[ix][iy+2]]);
		 	else apply_surface((16+ix*8)*scale_x,(16+iy*8)*scale_y,sur_block,screen,&block_rect[8]);
		 }


		 for (int iy=0; iy<4; iy++) for (int ix=0; ix<4; ix++)
		 {
		 	if (block[0][ix][iy] && shadow_y[0]+iy>=2 && opt[OPT_SHADOW])
		 		apply_surface((16+(x[0]+ix)*8)*scale_x,((shadow_y[0]+iy)*8)*scale_y,sur_block,screen,&block_rect[8]);
		 	if (block[0][ix][iy] && y[0]+iy>=2) apply_surface((16+(x[0]+ix)*8)*scale_x,((y[0]+iy)*8)*scale_y,sur_block,screen,&block_rect[blocktype[0]]);
		 	if (blockrot[hold[0]][0][ix][iy]) apply_surface((16+previewx[hold[0]]+(ix*8))*scale_x,
		 	(176+previewy[hold[0]]+(iy*8))*scale_y,sur_block,screen,&block_rect[hold[0]]);
		 }


		 for (int i=0; i<opt_preview; i++)
		 {
		 	if (opt[OPT_TRAINING])
		 	{
		 		temp[0]=blocktype[0]+i;
		 		if (temp[0]>=8) temp[0]=temp[0]-7;
		 		for (int ix=0; ix<4; ix++) for (int iy=0; iy<4; iy++)
		 		{
		 			if (blockrot[temp[0]][0][ix][iy])
		 				apply_surface((112+previewx[temp[0]]+ix*8)*scale_x,
		 				               (16+previewy[temp[0]]+(i*24)+iy*8)*scale_y,
		 				sur_block,screen,&block_rect[temp[0]]);
		 		}
		 	}
		 	else
		 	{
		 		for (int ix=0; ix<4; ix++) for (int iy=0; iy<4; iy++)
		 		{
		 			if (blockrot[preview[0][i]][0][ix][iy])
		 				apply_surface((112+previewx[preview[0][i]]+ix*8)*scale_x,
		 				               (16+previewy[preview[0][i]]+(i*24)+iy*8)*scale_y,
		 				sur_block,screen,&block_rect[preview[0][i]]);
		 		}
		 	}
		 }
		 //font.draw(112*scale_x,136*scale_y,game_stats[STAT_PIECES]/(double)((SDL_GetTicks()-gametimer)/60000.0),screen);
		 //font.draw(112*scale_x,152*scale_y,game_stats[STAT_SCORE],screen);
		 //font.draw(112*scale_x,168*scale_y,fall_lines[0],screen);

		 font.draw(112*scale_x,136*scale_y,"Lines: ",screen);
		 font.draw(112*scale_x+font.stringw("Lines: "),136*scale_y,fall_lines[0],screen);
		 font.draw(112*scale_x,152*scale_y,"Level: ",screen);
		 font.draw(112*scale_x+font.stringw("Level: "),152*scale_y,level[0],screen);
		 font.draw(112*scale_x,168*scale_y,"Score: ",screen);
		 font.draw(112*scale_x+font.stringw("Score: "),168*scale_y,game_stats[STAT_SCORE],screen);
		break;


		case GAMETYPE_COOP:
		 for (int ix=0; ix<level_width; ix++) for (int iy=0; iy<20; iy++)
		 {
		 	if (board_dark[ix][iy] or board_flashlight[ix] or thunder or !opt[OPT_DARK])
		 		apply_surface((16+ix*8)*scale_x,(16+iy*8)*scale_y,sur_block,screen,&block_rect[board[ix][iy+2]]);
		 	else apply_surface((16+ix*8)*scale_x,(16+iy*8)*scale_y,sur_block,screen,&block_rect[0]);
		 }

		 for (int iplayer=1; iplayer<=players; iplayer++) for (int iy=0; iy<4; iy++) for (int ix=0; ix<4; ix++)
		 {
            //if (hold[iplayer-1]<=0) continue;
		 	if (blockrot[hold[iplayer-1]][iplayer-1][ix][iy]) apply_surface((16+(iplayer-1)*64+previewx[hold[iplayer-1]]+(ix*8))*scale_x,
		 		(176+previewy[hold[iplayer-1]]+(iy*8))*scale_y,sur_block,screen,&block_rect[iplayer]);
		 }

		 for (int iplayer=1; iplayer<=players; iplayer++) for (int iy=0; iy<4; iy++) for (int ix=0; ix<4; ix++)
		 {
            if (blocktype[iplayer-1]==-1) continue;
		 	if (block[iplayer-1][ix][iy] && shadow_y[iplayer-1]+iy>=2 && opt[OPT_SHADOW])
		 		apply_surface((16+(x[iplayer-1]+ix)*8)*scale_x,((shadow_y[iplayer-1]+iy)*8)*scale_y,sur_block,screen,&block_rect[8]);
		 }

		 for (int iplayer=1; iplayer<=players; iplayer++) for (int iy=0; iy<4; iy++) for (int ix=0; ix<4; ix++)
		 {
            if (blocktype[iplayer-1]==-1) continue;
		 	if (block[iplayer-1][ix][iy] && y[iplayer-1]+iy>=2)
		 		apply_surface((16+(x[iplayer-1]+ix)*8)*scale_x,((y[iplayer-1]+iy)*8)*scale_y,sur_block,screen,&block_rect[iplayer]);
		 }
		break;
	}
}

void sync_init()
{
	srand(SDL_GetTicks());
	for (int iplayer=1; iplayer<=players; iplayer++)
	{
		for (int ix=0; ix<4; ix++) for (int iy=0; iy<4; iy++) block[iplayer-1][ix][iy]=0;
		if (!loaded)
        {
            rot[iplayer-1]=0;
            if (players==1) x[0]=3;
            else x[iplayer-1]=((float)(level_width-3)/players)*(iplayer-1);
            y[iplayer-1]=0;
        }
        xstart[iplayer-1]=x[iplayer-1];
        ystart[iplayer-1]=0;

        if (!loaded)
        {
            hold[iplayer-1]=0;
            blocktype[iplayer-1]=-1;
            fall_lines[iplayer-1]=5;
            level[iplayer-1]=1;
        }
        holdused[iplayer-1]=0;
		thunder_chance=12;
		shadow_y[iplayer-1]=0;
		//blocktype[iplayer-1]=1+rand()%5;
		//if (blocktype[iplayer-1]==5) blocktype[iplayer-1]=6;
		falltime[iplayer-1]=500;

		for (int i=0; i<5; i++) {preview[iplayer-1][i]=1+rand()%5;
		if (preview[iplayer-1][i]==5) preview[iplayer-1][i]=6;}

		for (int ix=0; ix<4; ix++) for (int iy=0; iy<4; iy++)
			block[iplayer-1][ix][iy]=blockrot[blocktype[iplayer-1]][rot[iplayer-1]][ix][iy];
		for (int ix=0; ix<level_width; ix++) for (int iy=0; iy<22; iy++)
		{
			if (!loaded) board[ix][iy]=0;
			board_dark[ix][iy]=0;
			board_flare[ix][iy]=0;
		}
		for (int i=0; i<level_width; i++) board_flashlight[i]=0;

		for (int i=0; i<8; i++) game_stats[i]=0;

		gameticks[iplayer][0]=SDL_GetTicks();
		gameticks[iplayer][1]=SDL_GetTicks();

		gametimer=SDL_GetTicks();
		fall[iplayer-1]=0;
		place[iplayer-1]=0;
		thunder_time=0;

		sync_shadow();
	}
	loaded=0;
}

void sync_shadow()
{
	for (int iplayer=1; iplayer<=players; iplayer++)
	{
		shadow_y[iplayer-1]=y[iplayer-1];
		while(block_collide(x[iplayer-1],shadow_y[iplayer-1]+1,rot[iplayer-1],blocktype[iplayer-1],iplayer-1)==1 && shadow_y[iplayer-1]<21)
		shadow_y[iplayer-1]++;
	}
}

void sync_save()
{
    if (game_type==GAMETYPE_SP)
    {
        ofstream fout("savegame.sav",ios_base::binary);

        fout.write((char*)x[0],sizeof(x[0]));
        fout.write((char*)y[0],sizeof(y[0]));
        fout.write((char*)blocktype[0],sizeof(blocktype[0]));
        fout.write((char*)rot[0],sizeof(rot[0]));
        fout.write((char*)hold[0],sizeof(hold[0]));
        fout.write((char*)level[0],sizeof(level[0]));
        fout.write((char*)fall_lines[0],sizeof(fall_lines[0]));
        fout.write((char*)score,sizeof(score));
        fout.write((char*)level_width,sizeof(level_width));
        for (int ix=0; ix<level_width; ix++) for (int iy=0; iy<22; iy++)
        {
            fout.write((char*)board[ix][iy],sizeof(board[ix][iy]));
        }
    }
}

void sync_load()
{
    if (game_type==GAMETYPE_SP)
    {
        ifstream fin("savegame.sav",ios_base::binary);

        fin.read((char*)&x[0],sizeof(x[0]));
        fin.read((char*)&y[0],sizeof(y[0]));
        fin.read((char*)&blocktype[0],sizeof(blocktype[0]));
        fin.read((char*)&rot[0],sizeof(rot[0]));
        fin.read((char*)&hold[0],sizeof(hold[0]));
        fin.read((char*)&level[0],sizeof(level[0]));
        fin.read((char*)&fall_lines[0],sizeof(fall_lines[0]));
        fin.read((char*)&score,sizeof(score));
        fin.read((char*)&level_width,sizeof(level_width));
        for (int ix=0; ix<level_width; ix++) for (int iy=0; iy<22; iy++)
        {
            fin.read((char*)&board[ix][iy],sizeof(board[ix][iy]));
        }
        loaded=1;
    }
}

/*
First i was gonna call this file netcode.cpp, but i changed my mind since
this code also controls singleplayer and hotseat multiplayer.
Since it fixes so the code syncs in all modes, without directly changing
the game code, i thought sync.cpp was a better name.

When all is done, i might move some code from here to game.cpp, since
game.cpp will basicly just be calling functions when im done here but
my HDD is crashing, so im trying to get this thing done before it
crashes completely ;P
*/
