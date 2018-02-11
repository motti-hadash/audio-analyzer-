/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_init                       2       /* control type: command, callback function: INIT_callback */
#define  PANEL_GRAPH                      3       /* control type: graph, callback function: graph_callback */
#define  PANEL_QUITBUTTON_2               4       /* control type: command, callback function: QuitCallback */
#define  PANEL_BINARYSWITCH               5       /* control type: binary, callback function: START_STOP_button */
#define  PANEL_STRING                     6       /* control type: string, callback function: (none) */
#define  PANEL_delta_t                    7       /* control type: numeric, callback function: (none) */
#define  PANEL_delta_v                    8       /* control type: numeric, callback function: (none) */
#define  PANEL_curr2                      9       /* control type: numeric, callback function: (none) */
#define  PANEL_curr1                      10      /* control type: numeric, callback function: (none) */
#define  PANEL_mag                        11      /* control type: numeric, callback function: (none) */
#define  PANEL_freq                       12      /* control type: numeric, callback function: (none) */
#define  PANEL_GRAPH_2                    13      /* control type: graph, callback function: graph_2_callback */
#define  PANEL_ch_buff_btn                14      /* control type: command, callback function: change_buffer_size_callback */
#define  PANEL_x_log                      15      /* control type: binary, callback function: x_log_callback */
#define  PANEL_buffer                     16      /* control type: ring, callback function: buffer_size_val */
#define  PANEL_NUMERICMETER               17      /* control type: scale, callback function: (none) */
#define  PANEL_plot_style                 18      /* control type: binary, callback function: plot_style_callback */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK buffer_size_val(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK change_buffer_size_callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK graph_2_callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK graph_callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK INIT_callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK plot_style_callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK START_STOP_button(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK x_log_callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
