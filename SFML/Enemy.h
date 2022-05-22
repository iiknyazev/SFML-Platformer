#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"


class ENEMY : public Entity
{
public:

	ENEMY(AnimationManager& a, Level& lev, int x, int y) :Entity(a, x, y)
	{
		option("Enemy", 0.01, 15, "move");
	}

	void update(float time, int numbLvl = 0)
	{
		x += dx * time;
		timer += time;
		if (timer > 3200) { dx *= -1; timer = 0; }

		if (Health <= 0) {
			anim.set("dead"); dx = 0;
			timer_end += time;
			if (timer_end > 4000) life = false;
		}

		anim.tick(time);
	}
};

#endif ENEMY_H
