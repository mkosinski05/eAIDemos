#include <gst/gst.h>
#include <glib.h>
#include "eAI_Stream.hpp"

int main (int   argc, char *argv[])
{

    /* Check input arguments */
    if (argc != 2) {
        g_printerr ("Usage: %s <MP3 filename>\n", argv[0]);
        return -1;
    } 
    
    // Start Stream
    eAIStream ( argc, argv);
    
    return 0;
}
