#include <simple2d.h> 
#include <wiringPi.h>
#include <stdio.h>
#include <time.h>

/*
Game    Room                Device                  WiringPi    Pi Pin  I/O

Kirkland Chambre            puzzle           	    0	        11	    In
					
*/

/*
 * Constant definition
 */

#define PUZZLE_SENSOR           0  
#define BYPASS_PUZZLE           1

/*
 * Variable definition
 */
int puzzleDone = 0;
int kirklandStarted = 0;
 
time_t noTimer = -1;
char bypassPuzzle[20] = {"BYPASS_PUZZLE"};
char stopKirkland[20] = {"STOP_KIRKLAND"};

S2D_Music *mus;
S2D_Music *win;
S2D_Window *window; 



/*
 * Function :   checkBypass
 * Description: Check if the corresponding bypass file is present in the current directory
 *              if it is it means that we need to bypass the correcponding switch
 * 
 * Parameters:  file        Name of the file to check
 *              pin         Pin to drive for the bypass
 *              state       State of the pin to set
 *              startTime   Needed when a 10 second delay is required before restoring initial value
 *
 * Return       No return value
 * 
 */
void checkBypass(char *file, int pin, int state, time_t *startTime)
{
	FILE *file1;
    

	file1 = fopen(file, "rb");
	if (file1)
	{
        if (*startTime == noTimer)
        {
            if (pin == BYPASS_PUZZLE)
            {
                if (puzzleDone == 0) 
                    { 
                        puzzleDone = 1; 
                        S2D_StopMusic();
                        S2D_PlayMusic(win, true);  // play on a loop 
         
                    } 
            }
            else
            {
                digitalWrite(pin, state);
            }
        }
        else if (*startTime == 0)
        {
            *startTime = time(NULL);
            digitalWrite(pin, state);
        }
        else
        {
            time_t curTime = time(NULL);
            if ((curTime - *startTime) > 10)
                {
                    if (state == LOW)
                        digitalWrite(pin, HIGH);
                    else
                        digitalWrite(pin, LOW);

                }
        }
        fclose(file1);
	}
}


/*
 * Function :   checkEndRequest
 * Description: Check if the end file is present
 *              if it is it means that we need to stop the program
 * 
 * Parameters:  file        Name of the file to check
 *
 * Return       No return value
 * 
 */
int checkEndRequest(char *file)
{
	FILE *file1;
    

	file1 = fopen(file, "rb");
	if (file1)
	{
        fclose(file1);
        return 1;
	}
    else
    {
        return 0;
    }
}

/*
 * Function :   TimedActivate
 * Description: Activate n IO pin for 10 seconds1
 * 
 * Parameters:  pin         Pin to drive for the bypass
 *              state       State of the pin to set
 *              startTime   Needed when a 10 second delay is required before restoring initial value
 *
 * Return       No return value
 * 
 */
void TimedActivate(int pin, int state, time_t *startTime)
{

    if (*startTime == 0)
    {
        *startTime = time(NULL);
        digitalWrite(pin, state);
    }
    else
    {
        time_t curTime = time(NULL);
        if ((curTime - *startTime) > 10)
            {
                if (state == LOW)
                    digitalWrite(pin, HIGH);
                else
                    digitalWrite(pin, LOW);
            }
    }
}

/* 
 * Function :   CheckAllBypasss 
 * Description: Verify all game inputs and bypass 
 *  
 * Parameters:  None 
 * 
 * Return       No return value 
 *  
 */ 
void CheckAllBypass() 
{ 
    // Bypass management     
    // Check if any of the bypass file exist 
    checkBypass(bypassPuzzle, BYPASS_PUZZLE, HIGH, &noTimer); 
} 

/* 
 * Function :   CheckControls 
 * Description: Verify all game inputs and bypass 
 *  
 * Parameters:  None 
 * 
 * Return       No return value 
 *  
 */ 
void CheckControls() 
{ 
    // Puzzle sensor management 
    // Check if Puzzle sensor was completed (set to ground) 
    // If turnes then play music 
    if ((digitalRead(PUZZLE_SENSOR) == 0) && (puzzleDone == 0)) 
    { 
        puzzleDone = 1; 
        S2D_StopMusic();
        S2D_PlayMusic(win, true);  // play on a loop 
         
    } 
}

/* 
 * Function :   Monitor keyboard 
 *  
 * Parameters:  S2D_Event       Keyboard event 
 *               
 * 
 * Return       No return value 
 *  
 */ 
void on_key(S2D_Event e)  
{ 
    char myKey;

    switch (e.type)  
    { 
    case S2D_KEY_DOWN:

        
        myKey = e.key[0];
        //printf("Key down: %s, %c\n", e.key, e.key[0]); 
        if (strcmp(e.key, "Escape") == 0) S2D_Close(window); 
        //if (strcmp(e.key, "Return") == 0)  
        //{ 
        //    MaxStage++; 
        //    S2D_PlaySound(succesSnd); 
        //    if (MaxStage == MAX_STAGES) 
        //    { 
        //        missionCompleted = true; 
        //        S2D_SetText(txtTop, "7 4 1 9 5"); 
        //        S2D_SetText(txtTop2, "Mission Complete!"); 
        //        S2D_SetText(txtBot, "Mission Completed!"); 
        //        S2D_SetText(txtBot2, "Mission Completed!"); 
        //        S2D_PlaySound(endSnd); 
        //    } 


        //} 
        break; 


    case S2D_KEY_HELD: 
         break; 


    case S2D_KEY_UP: 
         break; 
    } 
} 

/* 
 * Function :   ClearScreen 
 * Description: Set the screen to black 
 *  
 * Parameters:  None 
 *               
 * 
 * Return       No return value 
 *  
 */ 
void ClearScreen() 
{ 
    S2D_DrawTriangle( 
        320,  90, 0, 0, 0, 1, 
        150, 165, 0, 0, 0, 1, 
        150, 315, 0, 0, 0, 1); 


    S2D_DrawTriangle( 
        320,  90, 0, 0, 0, 1, 
        150, 315, 0, 0, 0, 1, 
        320, 390, 0, 0, 0, 1); 


    S2D_DrawTriangle( 
        320,  90, 0, 0, 0, 1, 
        320, 390, 0, 0, 0, 1, 
        490, 315, 0, 0, 0, 1); 


    S2D_DrawTriangle( 
        320,  90, 0, 0, 0, 1, 
        490, 315, 0, 0, 0, 1, 
        490, 165, 0, 0, 0, 1); 
} 


/* 
 * Function :   Render 
 * Description: Draw the black screen
 *  
 * Parameters:  None 
 *               
 * 
 * Return       No return value 
 *  
 */ 
void Render()  
{
    if (kirklandStarted == 0)
    {
        kirklandStarted = 1;
        S2D_PlayMusic(mus, true);  // play on a loop 
    }
    
    CheckControls(); 
    CheckAllBypass();
    ClearScreen(); 
    return; 
} 

void Update() 
{ 
    if (checkEndRequest(stopKirkland) == 1) 
        { 
            S2D_FreeWindow(window); 
        } 
} 

void cleanUpFiles()
{
    remove("BYPASS_PUZZLE");  
    remove("STOP_ASILE");
}
 
/*
 * Function :   main
 * Description: Program for the Asile
 * 
 * Parameters:  None
 * 
 * Return       int but it never returns
 * 
 */  
int main() 
{        
    cleanUpFiles();
    
    int puzzleDone = 0;
    
    wiringPiSetup() ;
  
    pinMode(PUZZLE_SENSOR, INPUT);
    
    pullUpDnControl(PUZZLE_SENSOR, PUD_UP);
    
    window = S2D_CreateWindow("Kirkland Puzzle", 640, 480, Update, Render, 0); 
     
    mus = S2D_CreateMusic("laserSound.mp3"); 
    win = S2D_CreateMusic("laserSound.mp3"); 

    window->on_key = on_key; 

    S2D_ShowCursor(false); 
    S2D_Show(window); 

    S2D_FreeMusic(win); 
    S2D_FreeMusic(mus); 
     
    //S2D_FreeWindow(window); 

    S2D_Close(window); 
    
    return 0;
}
