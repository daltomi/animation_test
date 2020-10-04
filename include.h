#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "ccVector.h"

#define PI 3.141592f


typedef uint8_t bit_t;

#define BIT(b)		(1U << (b))
#define SET_BIT(v, b)	((v) |= (b))
#define CLR_BIT(v, b)	((v) &= ~(b))
#define TGL_BIT(v, b)	((v) ^= (b))
#define CHK_BIT(v, b)	((v) & (b))
#define NOT_BIT(v, b)	(!CHK_BIT((v),(b)))

/* Animation Status */
#define AS_PLAY		BIT(0U)
#define AS_CYCLE	BIT(1U)

/* Animation Direction */
#define AD_NONE		BIT(0U)
#define AD_RIGHT	BIT(1U)
#define AD_LEFT		BIT(2U)

#define X 0
#define Y 1
#define W 2
#define H 3

#define CriticalFunctionCall(function)			\
{							\
	if(!function) {					\
		fprintf(stderr,"%s\n", #function);	\
		exit(EXIT_FAILURE);			\
	}						\
}


typedef struct {
	ALLEGRO_BITMAP*	bitmap;
	uint16_t	columns,
			total;
	vec2		start,
			size;
} SpriteSheet;


typedef void (*SpriteFunc)();


typedef struct  {
	int8_t		current,
			start;
} SpriteAnimation;



typedef struct  {
	SpriteSheet*	spritesheet;
	SpriteAnimation	animation;
	SpriteFunc	finish;
	bit_t		direction,
			status;
	uint8_t		timedelay,
			timecount;
} Sprite;

typedef enum  {
	ET_DYN,
	ET_STA
} EntityType;

typedef struct  {
	EntityType	type;
	Sprite		sprite;
} Entity;


typedef struct  {
	Entity		base;
	bool		alive;
	vec2		pos,
			pos_old,
			delay,
			speed,
			count;
} EntityDynamic;


typedef struct  {
	Entity		base;
	vec2		pos;
} EntityStatic;


enum {UP, DOWN, LEFT, RIGHT};

extern bool key[4];


void sprite_update(Sprite* sprite);

void entity_draw(const Entity* entity, const int flag, const float step);

void entity_update(Entity *entity, const float dt);

Entity* entity_new(EntityType type, vec2 pos,
		   SpriteSheet* spritesheet,
		   SpriteAnimation* animation,
		   bit_t direction,
		   uint8_t timedelay);
