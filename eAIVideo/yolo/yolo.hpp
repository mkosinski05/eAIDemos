
/*DRPAI Driver Header*/
#include <linux/drpai.h>
/*Definition of Macros & other variables*/
#include "define.h"

using namespace std;

#define EAI_ERROR   (-1)

class YoloInference
{
    public:
        YoloInference();
        ~YoloInference();
        
        // Inference Methods
        int8_t init( void );
        int8_t start ( void );
        int8_t wait ( void );
        uint8_t*  getInputBuf( void );
        uint8_t*  getOutputBuf( void );
        
        // Post Processing Methods
        int8_t get_result( void );
        
    private:
        // Inference Input Image
        uint16_t height;
        uint16_t width;
        uint16_t channel;
        uint32_t size;
        
        // DRP-AI Parameters
        drpai_data_t proc[DRPAI_INDEX_NUM];
        st_addr_t drpai_address;
        
        // CDMA and DRP-AI File descripturs
        int32_t udmabuf_fd;
        int32_t drpai_fd;
        
       // Buffers
       uint8_t *inBuffer;
       uint8_t *outBuffer;
       
       float drpai_output_buf[num_inf_out];
       
       int8_t load_drpai_data(int8_t drpai_fd);
       int8_t read_addrmap_txt(string addr_file);
};
