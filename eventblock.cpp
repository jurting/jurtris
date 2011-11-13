void eventblock()
{
	keyp=-1;
	for (int i=0; i<512; i++) bp[i]=0;
	mbp=0;
	mbr=0;
	mouse_oldx=mouse_x;
	mouse_oldy=mouse_y;
	for (int i=0; i<joynum; i++)
	{
		joy[i].keyp=-1;
		for (int i2=0; i2<8; i2++)
		{
			joy[i].axis_old[i2]=joy[i].axis[i2];
		}
		joy[i].hatold[0]=joy[i].hat[0];
		joy[i].hatold[1]=joy[i].hat[1];
	}
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_MOUSEMOTION:
			 mouse_x=event.motion.x/scale_x;
			 mouse_y=event.motion.y/scale_y;
			break;

			case SDL_MOUSEBUTTONDOWN:
			 mb=1;
			 mbp=1;
			break;

			case SDL_MOUSEBUTTONUP:
			 mb=0;
			 mbr=1;
			break;

			case SDL_KEYDOWN:
			 bd[event.key.keysym.sym]=1;
			 bp[event.key.keysym.sym]=1;
			 keyp=event.key.keysym.sym;
			break;

			case SDL_KEYUP:
			 bd[event.key.keysym.sym]=0;
			break;

			case SDL_JOYAXISMOTION:
			 if (event.jaxis.which<joynum) joy[event.jaxis.which].axis[event.jaxis.axis]=event.jaxis.value;
			break;

			case SDL_JOYBUTTONDOWN:
			 if (event.button.which<joynum) joy[event.button.which].bd[event.button.button]=1;
			 if (event.button.which<joynum) joy[event.button.which].keyp=event.button.button;
			break;

			case SDL_JOYBUTTONUP:
			 if (event.button.which<joynum) joy[event.button.which].bd[event.button.button]=0;
			break;

			case SDL_JOYHATMOTION:
			 if (event.jhat.value==SDL_HAT_RIGHT || event.jhat.value==SDL_HAT_RIGHTUP || event.jhat.value==SDL_HAT_RIGHTDOWN){
			     joy[event.jhat.which].hat[0]=1;
                if (event.jhat.value==SDL_HAT_RIGHT)
			     joy[event.jhat.which].hat[1]=0;}

             if (event.jhat.value==SDL_HAT_DOWN || event.jhat.value==SDL_HAT_RIGHTDOWN || event.jhat.value==SDL_HAT_LEFTDOWN){
			     joy[event.jhat.which].hat[1]=1;
                if (event.jhat.value==SDL_HAT_DOWN)
			     joy[event.jhat.which].hat[0]=0;}

             if (event.jhat.value==SDL_HAT_LEFT || event.jhat.value==SDL_HAT_LEFTUP || event.jhat.value==SDL_HAT_LEFTDOWN){
			     joy[event.jhat.which].hat[0]=-1;
                if (event.jhat.value==SDL_HAT_LEFT)
			     joy[event.jhat.which].hat[1]=0;}

			 if (event.jhat.value==SDL_HAT_UP || event.jhat.value==SDL_HAT_RIGHTUP || event.jhat.value==SDL_HAT_LEFTUP){
			     joy[event.jhat.which].hat[1]=-1;
                if (event.jhat.value==SDL_HAT_UP)
			     joy[event.jhat.which].hat[0]=0;}

             if (event.jhat.value==SDL_HAT_CENTERED)
             {
                joy[event.jhat.which].hat[1]=0;
                joy[event.jhat.which].hat[0]=0;
             }
			break;

			case SDL_QUIT:
			 quit=1;
			break;
		}
	}
}

bool input_p(int Abut, int Aplayer)
{
    if (Abut==-1) return 0;

	switch(but_type[Aplayer][Abut])
	{
		case 0:
		 if (bp[but[Aplayer][Abut]]) return 1;
		 else return 0;
		break;

		case 1:
		 if (joystick_sel[Aplayer]<joynum && joynum>0)
		 {
		 	if (joy[joystick_sel[Aplayer]].axis[but[Aplayer][Abut]]>=joydead && joy[joystick_sel[Aplayer]].axis_old[but[Aplayer][Abut]]<joydead) return 1;
		 	else return 0;
		 }
		break;

		case 2:
		 if (joystick_sel[Aplayer]<joynum && joynum>0)
		 {
		 	if (joy[joystick_sel[Aplayer]].axis[but[Aplayer][Abut]]<=-joydead && joy[joystick_sel[Aplayer]].axis_old[but[Aplayer][Abut]]>-joydead) return 1;
		 	else return 0;
		 }
		break;

		case 3:
		 if (joystick_sel[Aplayer]<joynum && joynum>0)
		 {
		 	if (joy[joystick_sel[Aplayer]].hat[but[Aplayer][Abut]]==1 && joy[joystick_sel[Aplayer]].hatold[but[Aplayer][Abut]]<=0) return 1;
		 	else return 0;
		 }
		break;

		case 4:
		 if (joystick_sel[Aplayer]<joynum && joynum>0)
		 {
            cout << joy[joystick_sel[Aplayer]].hat[but[Aplayer][Abut]] << " " << joy[joystick_sel[Aplayer]].hatold[but[Aplayer][Abut]] << endl;
		 	if (joy[joystick_sel[Aplayer]].hat[but[Aplayer][Abut]]==-1 && joy[joystick_sel[Aplayer]].hatold[but[Aplayer][Abut]]>=0) return 1;
		 	else return 0;
		 }
		break;

		case 5:
		 if (joystick_sel[Aplayer]<joynum && joynum>0)
		 {
		 	if (joy[joystick_sel[Aplayer]].keyp==but[Aplayer][Abut]) return 1;
		 	else return 0;
		 }
		break;
	}
	return 0;
}

bool input_h(int Abut, int Aplayer)
{
    if (Abut==-1) return 0;

	switch(but_type[Aplayer][Abut])
	{
		case 0:
		 if (bd[but[Aplayer][Abut]]) return 1;
		 else return 0;
		break;

		case 1:
		 if (joystick_sel[Aplayer]<joynum && joynum>0)
		 {
		 	if (joy[joystick_sel[Aplayer]].axis[but[Aplayer][Abut]]>=joydead) return 1;
		 	else return 0;
		 }
		break;

		case 2:
		 if (joystick_sel[Aplayer]<joynum && joynum>0)
		 {
		 	if (joy[joystick_sel[Aplayer]].axis[but[Aplayer][Abut]]<=-joydead) return 1;
		 	else return 0;
		 }
		break;

		case 3:
		 if (joystick_sel[Aplayer]<joynum && joynum>0)
		 {
		 	if (joy[joystick_sel[Aplayer]].hat[but[Aplayer][Abut]]>0) return 1;
		 	else return 0;
		 }
		break;

		case 4:
		 if (joystick_sel[Aplayer]<joynum && joynum>0)
		 {
		 	if (joy[joystick_sel[Aplayer]].hat[but[Aplayer][Abut]]<0) return 1;
		 	else return 0;
		 }
		break;

		case 5:
		 if (joystick_sel[Aplayer]<joynum && joynum>0)
		 {
		 	if (joy[joystick_sel[Aplayer]].bd[but[Aplayer][Abut]]) return 1;
		 	else return 0;
		 }
		break;
	}
	return 0;
}
