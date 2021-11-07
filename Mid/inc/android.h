

#ifndef __ANDROID_H_
#define __ANDROID_H_

typedef struct android_cmd
{
    uint8_t speed;
    uint8_t garbageChar;
    uint8_t incline;
} android_cmd_t;

uint32_t ANDROID_Init ();

#endif /* __ANDROIDPLAYER_H_ */
