void fontinit()
{
	fontw=font->w/16;
	fonth=font->h/16;
	int tempvar=0;
	for (unsigned int i2 = 0; i2 < 16; i2 += 1)
	{
		for (unsigned int i = 0; i < 16; i += 1)
		{
			rfont[tempvar].x=i*fontw;
			rfont[tempvar].y=i2*fonth;
			rfont[tempvar].w=fontw;
			rfont[tempvar].h=fonth;
			for (int ix = fontw-1; ix >= 0; ix -= 1)
			{
				for (int iy = 0; iy < fonth; iy += 1)
				{
					if(GetPixel(font,ix+rfont[tempvar].x,iy+rfont[tempvar].y)!=alpha_col)
					{
						rfont[tempvar].w=ix+1;
						ix=0;
						iy=fonth;
					}
				}
			}
			tempvar++;
		}
	}
	rfont[32].w=7*scale_x;
}

void draw_text(int x, int y, char str[256], SDL_Surface *starget, int max) //Draw text
{
	int tx=x;
	int ty=y;
	bool drawbold=0;
	bool drawline=0;
	for (unsigned int i = 0; i < 256; i += 1)
	{
		if(str[i]==10)
		{
			tx=x;
			ty=ty+fonth;
		}
		else if(str[i]==0) break;
		else if (str[i]=='$') drawbold=(!drawbold);
		else if (str[i]=='#') drawline=(!drawline);
		else
		{
			if(tx+rfont[(int)str[i]].w>max*scale_x) {tx=x; ty=ty+fonth;}
			apply_surface(tx,ty,font,starget,&rfont[(int)str[i]]);
			if (drawbold) apply_surface(tx,ty+1,font,starget,&rfont[(int)str[i]]);
			if (drawline) for (int ix=0; ix<rfont[(int)str[i]].w; ix++)
			{
				DrawPixel(starget,tx+ix,ty+fonth,SDL_MapRGB(screen->format,0,0,0));
			}
			tx=tx+rfont[(int)str[i]].w;
		}
	}
}

void draw_number(int x, int y, double val, SDL_Surface *starget)
{
	char chtemp[32];
	snprintf(chtemp, sizeof(chtemp), "%g", val);
	draw_text(x,y,chtemp,starget,320);
}

int stringw(char *cchar)
{
	int width=0;
	int lwidth=0;
	for (int i = 0; i < 64; i += 1)
	{
		if (cchar[i]==10)
		{
			if (width>lwidth) lwidth=width;
			width=0;
		}
		else if (cchar[i]==0) break;
		else width+=rfont[(int)cchar[i]].w;
	}
	if (width>lwidth) lwidth=width;
	return lwidth;
}
