#include <iostream>
#include <SDL.h>
#include <fmod.h>

#define	WINDOW_WIDTH		1024
#define	WINDOW_HEIGHT		768
#define	RATIO			(WINDOW_HEIGHT / 255.0)
#define	REFRESH_RATE		25
#define	SPECTRUM_SIZE		512
#define GRAPH_STEP		(WINDOW_WIDTH / SPECTRUM_SIZE)


typedef	struct t_data
{
	SDL_Surface	*screen;
	FMOD_SYSTEM	*system;
	FMOD_SOUND	*music;
	FMOD_CHANNEL	*channel;
	FMOD_RESULT	result;
	char		*sound_path;
	float		spectrum[SPECTRUM_SIZE];
	bool		running;
};


void setPixel(SDL_Surface *surface, const int x, const int y, const Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp)
	{
	case 1:
		*p = pixel;
		break;
	case 2:
		*(Uint16 *)p = pixel;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else
		{
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

void handle_events(t_data *data)
{
	SDL_Event event;

	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT)
		data->running = false;
	else if (event.type == SDL_KEYDOWN)
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			data->running = false;
			break;
		default:
			break;
		}
}

void display_point_bars(t_data *data, int x, int y)
{
	setPixel(data->screen, x, y, SDL_MapRGB(data->screen->format, 0, 255, 0));
	setPixel(data->screen, x + 1, y, SDL_MapRGB(data->screen->format, 0, 255, 0));
	setPixel(data->screen, x, y, SDL_MapRGB(data->screen->format, 255 - (y / RATIO), y / RATIO, 0));
	setPixel(data->screen, x + 1, y, SDL_MapRGB(data->screen->format, 255 - (y / WINDOW_HEIGHT), y / WINDOW_HEIGHT, 0));
}

void display_bars(t_data *data)
{
	int value = 0, x = 0;
	int y, i;

	for (i = 0; i < WINDOW_WIDTH && i < SPECTRUM_SIZE; ++i)
	{
		value = data->spectrum[i] * 20 * WINDOW_HEIGHT;
		if (value >= WINDOW_HEIGHT)
			value = WINDOW_HEIGHT - 1;
		if (value <= 0)
			value = 1;

		SDL_LockSurface(data->screen);
		for (y = WINDOW_HEIGHT - value; y < WINDOW_HEIGHT; y++)
			display_point_bars(data, x, y);
		SDL_UnlockSurface(data->screen);

		x += GRAPH_STEP;
	}
}

void display_point_mess(t_data *data, int x, int y, Uint32 color)
{
	setPixel(data->screen, x, y, color);
	setPixel(data->screen, x, WINDOW_HEIGHT - y, color);
	setPixel(data->screen, WINDOW_WIDTH - x - 1, y, color);
	setPixel(data->screen, WINDOW_WIDTH - x - 1, WINDOW_HEIGHT - y, color);
}

void display_mess(t_data *data)
{
	int value = 0, x = 0;
	int y, tmp, i;
	Uint32 color;

	for (i = 0; i < WINDOW_WIDTH && i < SPECTRUM_SIZE; ++i)
	{
		value = data->spectrum[i] * 20 * WINDOW_HEIGHT;
		if (value >= WINDOW_HEIGHT)
			value = WINDOW_HEIGHT - 1;
		if (value <= 0)
			value = 1;

		y = WINDOW_HEIGHT - value;
		if (y > 1 && y < WINDOW_HEIGHT)
		{
			color = SDL_MapRGB(data->screen->format, 255 - (1.5 * y / RATIO), y / RATIO, 0);
			SDL_LockSurface(data->screen);
			display_point_mess(data, x, y, color);
			SDL_UnlockSurface(data->screen);
			tmp = x;
			x = y * WINDOW_HEIGHT / WINDOW_WIDTH;
			y = tmp * WINDOW_WIDTH / WINDOW_HEIGHT;
			SDL_LockSurface(data->screen);
			display_point_mess(data, y, x, color);
			SDL_UnlockSurface(data->screen);
		}

		x += GRAPH_STEP;
	}
}

void display_graph(t_data *data)
{
	//display_bars(data);
	display_mess(data);
}

void clear_screen(t_data *data)
{
	SDL_FillRect(data->screen, NULL, SDL_MapRGB(data->screen->format, 0, 0, 0));
}

void handle_time()
{
	static int prevTime = 0;
	int currTime;

	currTime = SDL_GetTicks();
	if (currTime - prevTime < REFRESH_RATE)
		SDL_Delay(REFRESH_RATE - (currTime - prevTime));
	prevTime = SDL_GetTicks();
}

void process_music(t_data *data)
{
	while (data->running)
	{
		handle_events(data);
		clear_screen(data);
		FMOD_Channel_GetSpectrum(data->channel, data->spectrum, SPECTRUM_SIZE, 0, FMOD_DSP_FFT_WINDOW_RECT);
		SDL_LockSurface(data->screen);
		display_graph(data);
		SDL_UnlockSurface(data->screen);
		SDL_Flip(data->screen);
	}
}

bool fmod_init(t_data *data)
{
	FMOD_System_Create(&data->system);
	FMOD_System_Init(data->system, 1, FMOD_INIT_NORMAL, NULL);
	data->result = FMOD_System_CreateSound(data->system, data->sound_path,
		FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &data->music);
	if (data->result != FMOD_OK)
	{
		std::cerr << "Unable to load " << data->sound_path << std::endl;
		return false;
	}
	FMOD_System_PlaySound(data->system, FMOD_CHANNEL_FREE, data->music, 0, NULL);
	FMOD_System_GetChannel(data->system, 0, &data->channel);
	return true;
}

bool sdl_init(t_data *data, const char* window_name)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}
	
	data->screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption(window_name, NULL);
	return true;
}

bool data_init(t_data *data)
{
	/*if (nb_args == 2 && args[1] != NULL && args[1][0] != '\0')
		data->sound_path = args[1];
	else {
		puts("No file specified.");
		return false;
	}*/
	data->screen = NULL;
	data->running = true;
	return true;
}

bool mainLoop()
{
	t_data data;

	if (data_init(&data) == false)
		return false;
	fmod_init(&data);
	sdl_init(&data, "Frequency spectrum");
	process_music(&data);
	FMOD_Sound_Release(data.music);
	FMOD_System_Close(data.system);
	FMOD_System_Release(data.system);
	SDL_Quit();
	return true;
}

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Quit();

	return 0;
}
