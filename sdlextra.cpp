void clearSDL()
{
    for (int i=0; i<MENU_MAX; i++) for (int i2=0; i2<menu_num[i]; i2++) delete [] menustr[i][i2].ch;
	font.clear();
	SDL_FreeSurface(sur_block);
	SDL_FreeSurface(sur_border);
	SDL_FreeSurface(sur_square);
	SDL_FreeSurface(sur_hold);
	SDL_FreeSurface(textbar);
	SDL_FreeSurface(menu_back);
	SDL_FreeSurface(menu_back_dark);
	for (int i=0; i<joynum; i++) SDL_JoystickClose(joy[i].joy);
	Mix_HaltMusic();
	Mix_FreeChunk(mus_drop);
	Mix_FreeMusic(mus_music);
	Mix_CloseAudio();
	SDL_Quit();
}

SDL_Surface *load_img(string Afilename)
{
	SDL_Surface* loadimg;
	SDL_Surface* optimg;

	loadimg=SDL_LoadBMP(Afilename.c_str());
	optimg=SDL_DisplayFormat(loadimg);
	SDL_FreeSurface(loadimg);
	if (scale_x>1 or scale_y>1)
	{
		SDL_Surface *temp_surface=NULL;
		temp_surface=SDL_CreateRGBSurface(SDL_SWSURFACE,optimg->w*scale_x,optimg->h*scale_y,screen->format->BitsPerPixel,
			screen->format->Rmask,screen->format->Gmask,screen->format->Bmask,screen->format->Amask);

		for(int ix=0; ix<optimg->w; ix++) for(int iy=0; iy<optimg->h; iy++)
		{
			for(int ix2=0; ix2<scale_x; ix2++) for(int iy2=0; iy2<scale_y; iy2++)
			{
				DrawPixel(temp_surface,(ix*scale_x)+ix2,(iy*scale_y)+iy2,
				GetPixel(optimg,ix,iy));
			}
		}

		SDL_FreeSurface(optimg);
		SDL_SetColorKey(temp_surface,SDL_SRCCOLORKEY,alpha_col);
		return temp_surface;
	}

	SDL_SetColorKey(optimg,SDL_SRCCOLORKEY,alpha_col);
	return optimg;
}

void apply_surface(int x,int y,SDL_Surface* source,SDL_Surface* destination, SDL_Rect *clip)
{
	SDL_Rect offset;
	offset.x=x;
	offset.y=y;
	SDL_BlitSurface(source,clip,destination,&offset);
}

void DrawPixel(SDL_Surface *sur,int x, int y, Uint32 pixel)
{
    int bpp = sur->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)sur->pixels + y * sur->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

Uint32 GetPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}
