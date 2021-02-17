#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include <time.h>
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include "ImageData.h"
#include "EPD_2in13.h"

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:Goto Sleep mode\r\n");
    EPD_Sleep();
    DEV_ModuleExit();

    exit(0);
}

int main(void)
{
    printf("2.13inch e-Paper demo\r\n");
    DEV_ModuleInit();

    // Exception handling:ctrl + c
    signal(SIGINT, Handler);

    if(EPD_Init(FULL_UPDATE) != 0) {
        printf("e-Paper init failed\r\n");
    }
    EPD_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1)) * EPD_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for memory...\r\n");
        exit(0);
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);
    Paint_SelectImage(BlackImage);
    Paint_SetMirroring(MIRROR_HORIZONTAL); //
    Paint_Clear(WHITE);
    // show bmp    
    printf("show bmp\r\n");
    Paint_SelectImage(BlackImage);
    
    Paint_Clear(WHITE);
    GUI_ReadBmp("./pic/100x100.bmp", 0, 0);
    Paint_DrawString_EN(120, 20,"Temperature:", &Font16, WHITE, BLACK);
    EPD_Display(BlackImage);
    DEV_Delay_ms(500);
    //Partial refresh, example shows time    
    if(EPD_Init(PART_UPDATE) != 0) {
        printf("e-Paper init failed\r\n");
    }

    Paint_SelectImage(BlackImage);
    PAINT_TIME sPaint_time;
    struct tm *t;
    time_t tt;
    for (;;) {
        time(&tt);
        t = localtime(&tt);
        sPaint_time.Hour = t->tm_hour;
        sPaint_time.Min = t->tm_min;
        sPaint_time.Sec = t->tm_sec;
        Paint_ClearWindows(140, 90, 140 + Font20.Width * 7, 90 + Font20.Height, WHITE);
        Paint_DrawTime(140, 90, &sPaint_time, &Font20, WHITE, BLACK);
        EPD_DisplayPart(BlackImage);
        char line[255];
        if((t->tm_sec>30) && (t->tm_sec <=32))
        {
            FILE * fpointer = fopen("/home/pi/Desktop/ePaper-Project/bcm2835/obj/data.txt","r");
            fgets(line,255,fpointer);
            fclose(fpointer);
            Paint_SelectImage(BlackImage);
            Paint_ClearWindows(140, 40, 140 + Font16.Width * 7, 40 + Font16.Height, WHITE);
            Paint_DrawString_EN(140, 40,line, &Font16, WHITE, BLACK);
            EPD_Display(BlackImage);
        }
        if(t->tm_sec>29 && (t->tm_sec <=30))
        {
            Paint_ClearWindows(140, 40, 140 + Font16.Width * 7, 40 + Font16.Height, WHITE);
        }
    }
    printf("Goto Sleep mode...\r\n");
    EPD_Sleep();
    
    if(NULL != BlackImage){
        free(BlackImage);
        BlackImage = NULL;        
    }
    return 0;
}
