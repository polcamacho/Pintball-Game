#ifndef __P2ANIMATION_H__
#define __P2ANIMATION_H__
#include <assert.h>
#include "SDL/include/SDL_rect.h"

#define MAX_FRAMES 50000

class Animation
{
public:
	bool loop = true;
	SDL_Rect frames[MAX_FRAMES];
	int pivotx[MAX_FRAMES];
	int pivoty[MAX_FRAMES];
	double speed[MAX_FRAMES];
	int last_frame = 0;

private:

	float current_frame;
	int speeddelay = 0;
	int loops = 0;

public:

	void PushBack(const SDL_Rect& rect, double delay, int px, int py)
	{
		pivotx[last_frame] = px;
		pivoty[last_frame] = py;
		frames[last_frame++] = rect;
		speed[speeddelay++] = delay;
	}

	

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed[(int)current_frame];
		

		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0;
	}

	float GetCurrentFrameinFloat()
	{
		return current_frame;
	}

	int GetLastFrameinInt()
	{
		return last_frame;
	}

	void ChangeCurrentFrame(float frame)
	{
		current_frame = frame;
	}
};

#endif