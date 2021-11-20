

#ifndef __ANDROID_H_
#define __ANDROID_H_
#define ANDROID_CMD_MAX_SIZE    10
typedef struct android_cmd
{
    char cmd[ANDROID_CMD_MAX_SIZE];
} android_cmd_t;

uint32_t ANDROID_Init ();
void ANDROID_DeInit();
uint32_t ANDROID_SendCmd(char *cmd, uint32_t cmdSize);
#endif /* __ANDROIDPLAYER_H_ */
