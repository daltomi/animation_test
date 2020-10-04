
#include "include.h"


bool key[4] = {false, false, false, false};


int main(int arc, char * argv[])
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE * event_queue = NULL;
	ALLEGRO_TIMER *timer_fps = NULL;
	ALLEGRO_TIMER *timer_sprite = NULL;

	ALLEGRO_EVENT event;
	bool redraw = true;
	bool run = true;

	puts("Game on...");

	CriticalFunctionCall(al_init());
	CriticalFunctionCall(al_init_image_addon());
	CriticalFunctionCall(al_install_keyboard());
	CriticalFunctionCall(al_init_image_addon());

	const float FPS		= 60.0f;
	const float DELTA	= 1.0f / FPS;

	timer_fps	= al_create_timer(DELTA);
	timer_sprite	= al_create_timer(1.0 / 15.0f);
	display		= al_create_display(800, 600);
	event_queue	= al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer_fps));
	al_register_event_source(event_queue, al_get_timer_event_source(timer_sprite));

	SpriteSheet spritesheet_zombie = {
	.bitmap = al_load_bitmap("img/zombie.png"),
	.size.w = 45,
	.size.h = 36,
	.columns = 3,
	.total = 3,
	.start.x = 0,
	.start.y = 0
	};

	SpriteSheet spritesheet_flying = {
	.bitmap = al_load_bitmap("img/flying.png"),
	.size.w = 48,
	.size.h = 55,
	.columns = 16,
	.total = 16,
	.start.x = 0,
	.start.y = 0
	};

	SpriteSheet spr_smw = {
	.bitmap = al_load_bitmap("img/smw_foreground.png"),
	.size.w = 16,
	.size.h = 16,
	.columns = 8,
	.total = 4,
	.start.x = 0,
	.start.y = 0,
	};

	SpriteAnimation anim_zombie_walk_left	= {.current=3, .start=3};
	SpriteAnimation anim_zombie_walk_front	= {0,0};
	SpriteAnimation anim_flying		= anim_zombie_walk_front;
	SpriteAnimation anim_coin		= {56, 56};

	Entity* entities[] = {
	entity_new(ET_DYN, (vec2){.x=200, 100}, &spritesheet_zombie, &anim_zombie_walk_left, AD_RIGHT, 1),
	entity_new(ET_DYN, (vec2){.x=200, 200}, &spritesheet_zombie, &anim_zombie_walk_front, AD_RIGHT, 8),
	entity_new(ET_DYN, (vec2){.x=200, 300}, &spritesheet_flying, &anim_flying, AD_RIGHT, 0),
	entity_new(ET_DYN, (vec2){.x=200, 356}, &spritesheet_flying, &anim_flying, AD_RIGHT, 1),
	entity_new(ET_DYN, (vec2){.x=200, 412}, &spritesheet_flying, &anim_flying, AD_RIGHT, 2),
	entity_new(ET_STA, (vec2){.x=200, 467}, &spritesheet_flying, NULL, 0, 0),
	entity_new(ET_DYN, (vec2){.x=100, 467}, &spr_smw, &anim_coin, AD_RIGHT, 4),
	NULL
	};

	al_flip_display();
	al_start_timer(timer_fps);
	al_start_timer(timer_sprite);

	float old_time = 0, cur_time = 0;
	float offset_time = al_get_time();

	while (run) {
		al_get_next_event(event_queue, &event);
		//al_wait_for_event(event_queue, &event);
		switch (event.type) {
		case ALLEGRO_EVENT_TIMER:
			if (event.timer.source == timer_fps) {
				//~ old_time = cur_time;
				//~ cur_time = event.timer.count * DELTA;
				if (key[RIGHT]) {
					entities[0]->sprite.animation = anim_zombie_walk_front;
					//TGL_BIT(entities[0]->sprite.status, AS_PLAY);
				}else if (key[LEFT]) {
					//TGL_BIT(entities[0]->sprite.status, AS_PLAY);
					entities[0]->sprite.animation = anim_zombie_walk_left;
				}
				for (Entity** entity = entities; *entity != NULL; entity++)
					entity_update(*entity, DELTA);

				redraw = true;

			} else if (event.timer.source == timer_sprite) {
				for (Entity** entity = entities; *entity != NULL; entity++)
					sprite_update(&(*entity)->sprite);
			}
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			switch(event.keyboard.keycode) {
			case ALLEGRO_KEY_UP     : key[UP]   = true; break;
			case ALLEGRO_KEY_DOWN   : key[DOWN] = true; break;
			case ALLEGRO_KEY_LEFT   : key[LEFT] = true; break;
			case ALLEGRO_KEY_RIGHT  : key[RIGHT] = true; break;
			case ALLEGRO_KEY_ESCAPE : run = false; break;
			}
			break;
		case ALLEGRO_EVENT_KEY_UP:
			switch(event.keyboard.keycode) {
			case ALLEGRO_KEY_UP     : key[UP]   = false; break;
			case ALLEGRO_KEY_DOWN   : key[DOWN] = false; break;
			case ALLEGRO_KEY_LEFT   : key[LEFT] = false; break;
			case ALLEGRO_KEY_RIGHT  : key[RIGHT]= false; break;
			}
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			run = false;
			break;
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			//~ float int_t =  al_get_time() - offset_time;
			//~ float step = (int_t - DELTA - old_time) / (cur_time - old_time);
			redraw = false;
			al_clear_to_color(al_map_rgb(0,0,0));
			al_hold_bitmap_drawing(true);
			for(Entity** entity= entities; *entity != NULL; entity++)
				entity_draw(*entity, 0, 2);
			al_hold_bitmap_drawing(false);
			al_wait_for_vsync();
			al_flip_display();
		}
	} /* end while */

	puts("Game shutdown...");

	al_destroy_bitmap(spritesheet_zombie.bitmap);
	al_destroy_bitmap(spritesheet_flying.bitmap);

	for(Entity** entity = entities; *entity != NULL; entity++)
		free(*entity);

	al_destroy_timer(timer_fps);
	al_destroy_timer(timer_sprite);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);

	exit(EXIT_SUCCESS);
}
