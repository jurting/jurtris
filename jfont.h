/*
Jfont
Requires SDL to work.
*/

class Jfont
{
    private:
    SDL_Surface *FontSurface;
    SDL_Rect FontRect[256];
    Uint32 Font_GetPixel(int x, int y);
    public:
    int w;
    int h;
    void load(char *filepath, int spacing, int spacew, SDL_Surface *screen, int R, int G, int B);
    void draw(int x, int y, char  *str, SDL_Surface *target, int max=0);
    void draw(int x, int y, double val, SDL_Surface *target, int max=0);
    int stringw(char *cchar, int x=0, int max=0);
    int stringh(char *cchar, int x=0, int max=0);
    int stringw(double val, int x=0, int max=0);
    void clear();
};

Uint32 JfontGetPixel(SDL_Surface *surface, int x, int y);
