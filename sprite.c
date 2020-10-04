#include "include.h"


void sprite_update(Sprite* sprite)
{
	const bit_t status = sprite->status;
	if (CHK_BIT(status, AS_PLAY)) {
		if (++sprite->timecount > sprite->timedelay) {
			sprite->timecount = 0;
			if (CHK_BIT(sprite->direction, AD_RIGHT)) {
				if (++sprite->animation.current  > (sprite->animation.start + sprite->spritesheet->total - 1))
					goto finish;
			} else {
				/* AD_LEFT */
				if(--sprite->animation.current < (sprite->animation.start - sprite->spritesheet->total + 1))
					goto finish;
			}
			return;
finish:
			if (NOT_BIT(status, AS_CYCLE))
				TGL_BIT(sprite->status, AS_PLAY);
			sprite->animation.current = sprite->animation.start;
			if(sprite->finish) sprite->finish();
		}
	}
}
