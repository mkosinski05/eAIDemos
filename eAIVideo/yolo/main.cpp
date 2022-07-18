/*DRPAI Driver Header*/
#include <linux/drpai.h>
#include "define.h"
#include "yolo.hpp"

YoloInference eAI;

uint8_t read_bmp(std::string filename);
uint8_t * img_buffer;

uint8_t header_size = FILEHEADERSIZE+INFOHEADERSIZE_W_V3;
uint8_t bmp_header[FILEHEADERSIZE+INFOHEADERSIZE_W_V3];
        
int32_t main(int32_t argc, char * argv[])
{
    img_buffer = eAI.getInputBuf();
    
    eAI.init();
    read_bmp(input_img);
    eAI.start();
    eAI.wait();
    
    return 0; 
}

/*****************************************
* Function Name : read_bmp
* Description   : Function to load BMP file into img_buffer
* NOTE          : This is just the simplest example to read Windows Bitmap v3 file.
*                 This function does not have header check.
* Arguments     : filename = name of BMP file to be read
* Return value  : 0 if succeeded
*                 not 0 otherwise
******************************************/
uint8_t read_bmp(std::string filename)
{
    uint32_t width = DRPAI_IN_WIDTH;
    uint32_t height = DRPAI_IN_HEIGHT;
    uint32_t channel = DRPAI_IN_CHANNEL_BGR;
    int32_t i = 0;
    FILE *fp = NULL;
    size_t ret = 0;
    uint8_t * bmp_line_data;
    /* Number of byte in single row */
    /* NOTE: Number of byte in single row of Windows Bitmap image must be aligned to 4 bytes. */
    int32_t line_width = width * channel + width % 4;

    /*  Read header for Windows Bitmap v3 file. */
    fp = fopen(filename.c_str(), "rb");
    if (NULL == fp)
    {
        return -1;
    }

    /* Read all header */
    ret = fread(bmp_header, sizeof(uint8_t), header_size, fp);
    if (!ret)
    {
        fclose(fp);
        return -1;
    }
    /* Single row image data */
    bmp_line_data = (uint8_t *) malloc(sizeof(uint8_t) * line_width);
    if (NULL == bmp_line_data)
    {
        free(bmp_line_data);
        fclose(fp);
        return -1;
    }

    for (i = height-1; i >= 0; i--)
    {
        ret = fread(bmp_line_data, sizeof(uint8_t), line_width, fp);
        if (!ret)
        {
            free(bmp_line_data);
            fclose(fp);
            return -1;
        }
        memcpy(img_buffer+i*width*channel, bmp_line_data, sizeof(uint8_t)*width*channel);
    }

    free(bmp_line_data);
    fclose(fp);
    return 0;
}
