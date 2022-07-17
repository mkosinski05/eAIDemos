

class YoloInference
{
    public:
        YoloInference();
        ~YoloInference();
    private:
        // Inference Input Image
        uint16_t height;
        uint16_t width;
        uint16_t channel;
        
        // Methods
        int8_t init( void );
        int8_t start ( void );
        int8_t wait ( void );
        uint8_t*  getInputBuf( void );
        uint8_t*  getOutputBuf( void );

        // CDMA and DRP-AI File descripturs
        int32_t udmabuf_fd;
        int32_t drpai_fd;
        
       
}
