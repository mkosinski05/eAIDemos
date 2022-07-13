#include <gst/gst.h>
#include <glib.h>
#include "gstutils.hpp"

typedef struct _CustomData {
     guint sourceid;        /* To control the GSource */
}CustomData;

static CustomData data;
/* The appsink has received a buffer */
static GstFlowReturn new_sample (GstElement *sink, CustomData *data) {

  GstSample *sample;
  GstMapInfo map;
  GstBuffer* buffer;
  
  static int size = 0;
  
  /* Retrieve the buffer */
  g_signal_emit_by_name (sink, "pull-sample", &sample);
  if (sample) {
    /* The only thing we do in this example is print a * to indicate a received buffer */
    // Actual compressed image is stored inside GstSample.
    buffer = gst_sample_get_buffer (sample);
	
    gst_buffer_map (buffer, &map, GST_MAP_READ);
    
    if ( size == 0 )
    {
        /* Do this once for debug  */
        size = map.size;
        g_print("\n%d\n", size);
        print_pad_capabilities (sink, "sink");
    }
    g_print("*");
    
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
