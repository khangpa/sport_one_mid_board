

#ifndef __DFPLAYER_H_
#define __DFPLAYER_H_


void Send_cmd (uint8_t cmd, uint8_t Parameter1, uint8_t Parameter2);
void DF_PlayFromStart(void);
void DF_Init (uint8_t volume);
void DF_Next (void);
void DF_Pause (void);
void DF_Previous (void);
void DF_Playback (void);
void DF_Play(uint32_t fileName);

#endif /* __DFPLAYER_H_ */
