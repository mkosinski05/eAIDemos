
/*DRPAI Driver Header*/
#include <linux/drpai.h>
/*Definition of Macros & other variables*/
#include "define.h"

class YoloInference
{
    public:
        YoloInference();
        ~YoloInference();
        
                // Methods
        int8_t init( void );
        int8_t start ( void );
        int8_t wait ( void );
        uint8_t*  getInputBuf( void );
        uint8_t*  getOutputBuf( void );
        
    private:
        // Inference Input Image
        uint16_t height;
        uint16_t width;
        uint16_t channel;
        uint32_t size;
        
        // DRP-AI Parameters
        drpai_data_t proc[DRPAI_INDEX_NUM];
        
        // CDMA and DRP-AI File descripturs
        int32_t udmabuf_fd;
        int32_t drpai_fd;
        
       // Buffers
       uint8_t *inBuffer;
       uint8_t *outBuffer;
};
