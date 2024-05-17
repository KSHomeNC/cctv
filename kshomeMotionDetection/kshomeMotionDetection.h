#ifndef __kshomeMD_H__
#define  __kshomeMD_H__

// Pin Definition
#define GREEN_LED LED_BUILTIN
#define BUZZER_PIN 8


void mdTask(void );
void osdTask (void);
void commandTask(void);


#endif // __kshomeMD_H__
