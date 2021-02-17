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
#if 1   // show bmp    
    printf("show bmp\r\n");
    Paint_SelectImage(BlackImage);
    
    Paint_Clear(WHITE);
    GUI_ReadBmp("./pic/100x100.bmp", 0, 0);
    EPD_Display(BlackImage);
    DEV_Delay_ms(500);

    Paint_Clear(WHITE);    
    GUI_ReadBmp("./pic/bbb.bmp", 0, 0);
    EPD_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif

//show image for array    
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_2in13);
    EPD_Display(BlackImage);
    DEV_Delay_ms(2000);
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    // 2.Drawing on the image 
    Paint_DrawString_EN(140, 15, "Hellow World", &Font20, BLACK, WHITE);
    EPD_Display(BlackImage);
    printf("Goto Sleep mode...\r\n");
    EPD_Sleep();
    if(NULL != BlackImage){
        free(BlackImage);
        BlackImage = NULL;        
    }
    return 0;
}
