#include "include.h"


void entity_draw(const Entity* entity, const int flag, const float step)
{  
	Sprite* sprite = NULL;
	vec2 sz, src, dst;
	if (entity->type == ET_DYN) {
		EntityDynamic* ent_dyn = ((EntityDynamic*)entity);

		sprite = &ent_dyn->base.sprite;

		const int8_t animcurrent = sprite->animation.current;
		const uint16_t spritesheet_columns = sprite->spritesheet->columns;
		sz.w = sprite->spritesheet->size.w;
		sz.h = sprite->spritesheet->size.h;
		src.x = sprite->spritesheet->start.x + (animcurrent % spritesheet_columns) * sz.w;
		src.y = sprite->spritesheet->start.y + (animcurrent / spritesheet_columns) * sz.h;
		dst.x = ent_dyn->pos.x;//entity->xposold + (entity->xpos - entity->xposold) * step;
		dst.y = ent_dyn->pos.y;//entity->yposold + (entity->ypos - entity->yposold) * step;
	} else if (entity->type == ET_STA) {
		EntityStatic* ent_sta = ((EntityStatic*)entity);
		sprite = &ent_sta->base.sprite;
		sz.w = sprite->spritesheet->size.w;
		sz.h = sprite->spritesheet->size.h;
		src.x = sprite->spritesheet->start.x;
		src.y = sprite->spritesheet->start.y;
		dst.x = ent_sta->pos.x;
		dst.y = ent_sta->pos.y;
	}
	al_draw_bitmap_region(sprite->spritesheet->bitmap, src.x, src.y, sz.w, sz.h, dst.x, dst.y, flag );
}

void entity_update(Entity *entity, const float dt)
{
	if (entity->type == ET_DYN) {
		EntityDynamic* ent_dyn = ((EntityDynamic*)entity);
		ent_dyn->pos.x += 22 * dt;
		//entity->ypos += 43 * dt;

		if (ent_dyn->pos.x >= 800)
			ent_dyn->pos.x = 1;

		//if (ent_dyn->ypos >= 600)
		//    ent_dyn->ypos = 2;

		ent_dyn->pos_old.x = ent_dyn->pos.x;
	}
	//entity->yposold = entity->ypos;

}

Entity* entity_new( EntityType type, vec2 pos,
                        SpriteSheet* spritesheet,
                        SpriteAnimation* animation,
                        bit_t direction,
                        uint8_t timedelay)
{
	Entity* entity = NULL;
	if(type == ET_DYN) {
		EntityDynamic* ent_dyn = malloc(sizeof(*ent_dyn));
		memset(ent_dyn, 0, sizeof(*ent_dyn));
		ent_dyn->pos_old.x = pos.x;
		ent_dyn->pos_old.y = pos.y;
		ent_dyn->pos.x = pos.x;
		ent_dyn->pos.y = pos.y;
		entity  = &ent_dyn->base;
		entity->sprite.timedelay    = timedelay;
		entity->sprite.animation    = *animation;
		SET_BIT(entity->sprite.status, AS_PLAY | AS_CYCLE);
		SET_BIT(entity->sprite.direction, direction);
	} else if (type == ET_STA) {
		EntityStatic* ent_sta = malloc(sizeof(*ent_sta));
		memset(ent_sta, 0, sizeof(*ent_sta));
		ent_sta->pos.x = pos.x;
		ent_sta->pos.y = pos.y;
		entity  = &ent_sta->base;
	}
	entity->type = type;
	entity->sprite.spritesheet = spritesheet;
	return entity;
}
