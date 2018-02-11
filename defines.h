#include <stdio.h>
#include <stdlib.h>
#include "portaudio.h"

#define SAMPLE_RATE  (44100)
#define NUM_SECONDS     (10)
#define NUM_CHANNELS    (1)     
/* #define DITHER_FLAG     (paDitherOff) */
#define DITHER_FLAG     (0) /**/

/* Select sample format. */
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;		 ///was float
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif

typedef struct
{
    int          frameIndex;  /* Index into sample array. */
    int          maxFrameIndex;
    SAMPLE      *recordedSamples;
}
paTestData;
////////////////////////////////////////////////////////////////// my parameters
#define low_limit 0.00000000000000000001
static int 					panelHandle;
static int 					start1;
static float 				*x_axis ;

static int					init_flag;
static int					plot_style;
static double 				*fft_re,*time_series;
static int 					my_err;
static int 					tf_handler;
static int					quit;
/////////////////////////////////////////////////////////////////// portaudio parameters

static  PaStreamParameters  *inputParameters ;//*outputParameters //used whan output sound
static PaStream*           stream;
static PaError             err = paNoError;
static paTestData          data;
static int                 i;
static int                 totalFrames;
static int                 numSamples;
static int                 numBytes;
static int				   SAMPLES_PER_BUFFER=1024;

/////////////////////////////////////////////////////////////////// my functions
int CVICALLBACK ThredFunction (void *functionData);

void close_stream(PaStream* my_stream) ;
static int audio_in_Callback( const void *inputBuffer,
						   void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo *timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData );


