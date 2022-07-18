#include <gst/gst.h>
#include <glib.h>
#include "gstutils.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <atomic>
#include <string.h>
#include "yolo.hpp"

#define DISPLAY_WIDTH       (640)
#define DISPLAY_HEIGHT      (480)
#define DISPLAY_CHANNEL     (4)
#define TMP_BUF_SIZE        (DISPLAY_WIDTH*DISPLAY_HEIGHT*DISPLAY_CHANNEL)

typedef struct _CustomData {
     guint sourceid;        /* To control the GSource */
}CustomData;

static FILE *fptr;

static std::atomic<uint32_t> flag_display (0);

static YoloInference eAI;

static CustomData data;
/* The appsink has received a buffer */
static GstFlowReturn new_sample (GstElement *sink, CustomData *data) {

  GstSample *sample;
  GstMapInfo map;
  GstBuffer* buffer;
  
  static int size = 0;
  static bool caponce = true;
  
  /* Retrieve the buffer */
  g_signal_emit_by_name (sink, "pull-sample", &sample);
  if (sample) {
    /* The only thing we do in this example is print a * to indicate a received buffer */
    // Actual compressed image is stored inside GstSample.
    buffer = gst_sample_get_buffer (sample);
	
    gst_buffer_map (buffer, &map, GST_MAP_READ);
    
    if ( caponce )
    {
        /* Do this once for debug  */
        size = map.size;
        g_print("\n%d\n", size);
        print_pad_capabilities (sink, "sink");
        caponce = false;
    }
    
    if ( flag_display.load() == 0 ) {
        
        // eAI Inference ready 
        // Send buffer to eAI
        if ( eAI.getInputBuf() != NULL ) 
        {
            memcpy(eAI.getInputBuf(), map.data, map.size);
        }
        flag_display.store(size);
        
    } else {
        // Missed frame
        g_print("*");
    }
    
    gst_buffer_unmap (buffer, &map);
    gst_sample_unref (sample);
    
    return GST_FLOW_OK;
  }
  return GST_FLOW_ERROR;
}

/* Configure appsink */
void eAI_AppSinkConfigure ( GstElement*  app_sink) {
    GstCaps * caps = gst_caps_from_string("video/x-raw, width=640, height=480, format=BGR");
    g_object_set (app_sink, "emit-signals", TRUE, "caps", caps, NULL);
    g_signal_connect (app_sink, "new-sample", G_CALLBACK (new_sample), &data);
}

void *R_eAIInference_thread(void *threadid)
{
    uint32_t sz = 0;
    
    // Initialize Inference
    if ( eAI.init() != 0 )
    {
        printf("ERROR: Failed Initialize eAI Inference \n");
    }
    
    printf("Started Ai Inference Thread\n");
    while (1) {
        sz = flag_display.load();
        if (sz != 0 )
        {

            //printf("Processing AI Inference %d\n", sz);
            if ( eAI.start() != 0)
                break;
            if ( eAI.wait() != 0 )
                break;
            //printf("\nFinished AI Inference\n");
  
            flag_display.store(0);

        }
    }
    printf("eAI Ending Thread\n");
    return NULL;
}
