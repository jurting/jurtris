class jstr
{
	public:
	char *ch;
};

class Jurt_Joystick
{
	public:
	SDL_Joystick *joy;
	int axis[8];
	int axis_old[8];
	int hat[2];
	int hatold[2];
	bool bd[32];
	int keyp;
};
