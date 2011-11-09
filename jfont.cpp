 void Jfont::load(char *filepath, int spacing, int spacew, SDL_Surface *screen, int R, int G, int B)
{
	int tempvar=0;
    SDL_Surface* loadimg;

	loadimg=load_img(filepath);
	FontSurface=SDL_DisplayFormat(loadimg);
	SDL_FreeSurface(loadimg);
	SDL_SetColorKey(FontSurface,SDL_SRCCOLORKEY,SDL_MapRGB(screen->format,R,G,B));

	w=FontSurface->w/16;
	h=FontSurface->h/16;

	for (unsigned int i2 = 0; i2 < 16; i2 += 1)
	{
		for (unsigned int i = 0; i < 16; i += 1)
		{
			FontRect[tempvar].x=i*w;
			FontRect[tempvar].y=i2*h;
			FontRect[tempvar].w=w;
			FontRect[tempvar].h=h;
			for (int ix = w-3; ix >= 0; ix -= 1)
			{
				for (int iy = 0; iy < h; iy += 1)
				{
					if(JfontGetPixel(FontSurface,ix+FontRect[tempvar].x,iy+FontRect[tempvar].y)!=SDL_MapRGB(screen->format,R,G,B))
					{
						FontRect[tempvar].w=ix+spacing;
						ix=0;
						iy=h;
					}
				}
			}
			tempvar++;
		}
	}
	FontRect[32].w=spacew;
}


void Jfont::draw(int x, int y, char *str, SDL_Surface *target, int max)
{
    SDL_Rect coords;
	coords.x=x;
	coords.y=y;

	int tx=x;
	int ty=y;
	bool drawbold=0;
	bool drawline=0;
	for (unsigned int i = 0; i < 256; i += 1)
	{
        if(str[i]==0) break;
		else if(str[i]==10)
		{
			tx=x;
			ty=ty+h;
		}
		else if (str[i]=='$') drawbold=(!drawbold);
		else if (str[i]=='#') drawline=(!drawline);
		else
		{
		    if (tx+FontRect[(int)str[i]].w-1>max && max>0)
            {
                tx=x;
                ty=ty+h;
            }

			coords.x=tx; coords.y=ty;
			SDL_BlitSurface(FontSurface,&FontRect[(int)str[i]],target,&coords);
			coords.x=tx; coords.y=ty+1;
			if (drawbold) SDL_BlitSurface(FontSurface,&FontRect[(int)str[i]],target,&coords);
			/*if (drawline) for (int ix=0; ix<rfont[(int)str[i]].w; ix++)
			{
				DrawPixel(starget,tx+ix,ty+fonth,SDL_MapRGB(screen->format,0,0,0));
			}*/
			tx=tx+FontRect[(int)str[i]].w-1;
		}
	}
}

void Jfont::draw(int x, int y, double val, SDL_Surface *target, int max)
{
    char chtemp[32];
	snprintf(chtemp, sizeof(chtemp), "%g", val);
	draw(x,y,chtemp,target,max);
}

int Jfont::stringw(char *cchar, int x, int max)
{
	int width=0;
	int lwidth=0;
	int height=0;
	for (int i = 0; i < 64; i += 1)
	{
        if(cchar[i]==0) break;
	    if(cchar[i]==10)
		{
		    if (width>lwidth) lwidth=width;
			width=0;
			height=height+h;
		}
		else if (cchar[i]=='$') continue;
		else if (cchar[i]=='#') continue;
		else
		{
            if (width+FontRect[(int)cchar[i]].w-1>max-x && max>0)
            {
                if (width>lwidth) lwidth=width;
                width=0;
                height=height+h;
            }

			width=width+FontRect[(int)cchar[i]].w-1;
		}
	}
	if (width>lwidth) lwidth=width;
	return lwidth;
}

int Jfont::stringh(char *cchar, int x, int max)
{
	int width=0;
	int height=h;
	bool newline=1;
	for (int i = 0; i < 64; i += 1)
	{
        if(cchar[i]==0) break;
	    if(cchar[i]==10)
		{
			width=0;
			height=height+h;
			newline=0;
		}
		else if (cchar[i]=='$') continue;
		else if (cchar[i]=='#') continue;
		else
		{
            if (width+FontRect[(int)cchar[i]].w-1>max-x && max>0)
            {
                width=0;
                height=height+h;
            }

			width=width+FontRect[(int)cchar[i]].w-1;
			newline=1;
		}
	}
	if (!newline) height-=h;
	return height;
}

int Jfont::stringw(double val, int x, int max)
{
    char chtemp[32];
	snprintf(chtemp, sizeof(chtemp), "%g", val);
	stringw(chtemp, x, max);
}

void Jfont::clear()
{
    SDL_FreeSurface(FontSurface);
}

Uint32 JfontGetPixel(SDL_Surface *surface, int x, int y)
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
