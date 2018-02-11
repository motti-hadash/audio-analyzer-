#include <utility.h>
#include <analysis.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "io1.h"

#include "defines.h"

////////////////////////////////////////////////////////////////////
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "io1.uir", PANEL)) < 0)
		return -1;
	
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle); 
   
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
int CVICALLBACK QuitCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			quit=1;
			Delay(0.5);		//make shure thred if finished and its safe to free memory
			close_stream(stream);
			QuitUserInterface (0);

			break;
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
int CVICALLBACK INIT_callback (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	int k;
		
	switch (event)
	{
		case EVENT_COMMIT:
			init_flag=1;
			x_axis = (float*)malloc(sizeof(float)*SAMPLES_PER_BUFFER);
			time_series =(double*)malloc(sizeof(double)*SAMPLES_PER_BUFFER);
			fft_re = (double*)malloc(sizeof(double)*SAMPLES_PER_BUFFER);
			
			if(x_axis != NULL && time_series != NULL && fft_re != NULL)
			{
				for( k=0;k < SAMPLES_PER_BUFFER-1 ;k++)
				{ 
					x_axis[k] = k*(SAMPLE_RATE/SAMPLES_PER_BUFFER); 
				}
			}
			else
			{
			    my_err=-10;
				goto done;
			}
			data.maxFrameIndex = totalFrames = SAMPLES_PER_BUFFER;       
    		data.frameIndex = 0;
    		numSamples = totalFrames * NUM_CHANNELS;
    		numBytes = numSamples * sizeof(SAMPLE);
    		
			data.recordedSamples = (SAMPLE *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */
			if( data.recordedSamples == NULL )
			{
				my_err=-10;
				goto done;
			}
			else
			{
				for( i=0; i<numSamples; i++ ) 
				data.recordedSamples[i] = 0;
			}
					
			err = Pa_Initialize();
			if( err != paNoError ) 
				goto done;
					
			inputParameters = (PaStreamParameters*)malloc(sizeof(PaStreamParameters));
			inputParameters->device = Pa_GetDefaultInputDevice(); /* default input device */
			if (inputParameters->device == paNoDevice) 
				goto done;
				    
			inputParameters->channelCount = 2;                    /* as if we record stereo input */
			inputParameters->sampleFormat = PA_SAMPLE_TYPE;
			inputParameters->suggestedLatency = Pa_GetDeviceInfo( inputParameters->device )->defaultLowInputLatency;
			inputParameters->hostApiSpecificStreamInfo = NULL;
						
			/* Record audio. */
   
			err = Pa_OpenStream(
					  &stream,
					  inputParameters,
					  NULL,SAMPLE_RATE,
				      SAMPLES_PER_BUFFER,
				      paClipOff,      /* we won't output out of range samples so don't bother clipping them */
				      audio_in_Callback,
				      &data );
			if( err != paNoError )
				goto done;
					
			err = Pa_StartStream( stream );
			if( err != paNoError ) 
				goto done;
						
			SetCtrlAttribute (panelHandle, PANEL_BINARYSWITCH, ATTR_DIMMED, 0);
			SetCtrlAttribute (panelHandle, PANEL_init, ATTR_DIMMED, 1);
			SetCtrlVal (panelHandle, PANEL_STRING,"please press the RUN button" );
						
done:
			if(err<0)
			close_stream(stream);
			break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
int CVICALLBACK START_STOP_button (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			 start1 = !start1;
			 
			 if(start1)
			 {
				SetCtrlVal (panelHandle, PANEL_STRING,"you can PAUSE the plot" );  
				CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, ThredFunction, NULL, &tf_handler);
				
				SetCtrlAttribute (panelHandle, PANEL_init, ATTR_DIMMED, 1);
				SetCtrlAttribute (panelHandle, PANEL_buffer, ATTR_DIMMED, 1);
				SetCtrlAttribute (panelHandle, PANEL_ch_buff_btn, ATTR_DIMMED, 1);
			 }
			else
			{	
				 CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,tf_handler, OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
				 CmtReleaseThreadPoolFunctionID (DEFAULT_THREAD_POOL_HANDLE, tf_handler);
				 SetCtrlAttribute (panelHandle, PANEL_buffer, ATTR_DIMMED, 0);
				 SetCtrlAttribute (panelHandle, PANEL_ch_buff_btn, ATTR_DIMMED, 0);
				 SetCtrlVal (panelHandle, PANEL_STRING,"you can CHOOSE BUFFER SIZE or RUN" );
				 
			}	 
			break;
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
int CVICALLBACK graph_callback (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{   static double X,Y;
	switch (event)
	{
		case EVENT_COMMIT:
			GetGraphCursor (panelHandle, PANEL_GRAPH, 1,&X ,&Y );
			SetCtrlVal (panelHandle, PANEL_mag, Y);
			SetCtrlVal (panelHandle, PANEL_freq, X);

			break;
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
int CVICALLBACK graph_2_callback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{   static double X1,Y1,X2,Y2;
	switch (event)
	{
		case EVENT_COMMIT:
			GetGraphCursor (panelHandle, PANEL_GRAPH_2, 1, &X1, &Y1);
			GetGraphCursor (panelHandle, PANEL_GRAPH_2, 2, &X2, &Y2);
			SetCtrlVal (panelHandle, PANEL_curr1, Y1);
			SetCtrlVal (panelHandle, PANEL_curr2, Y2);
			Y2=Y1-Y2;
			X2=(X1-X2)/SAMPLE_RATE;
			SetCtrlVal (panelHandle, PANEL_delta_v, Y2);
			SetCtrlVal (panelHandle, PANEL_delta_t, X2);
			break;
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
int CVICALLBACK ThredFunction (void *functionData)


{
	int j;									   
	static double rms_val;
		
		while(start1)
		{   
			if(quit)
				CmtExitThreadPoolThread (0);
			
			if ( ( err = Pa_IsStreamActive( stream ) ) == 1)
   			{
				for(j=0;j<SAMPLES_PER_BUFFER;j++)
				{
					fft_re[j] = (double)data.recordedSamples[j];	
				}
								
				memcpy ( time_series, fft_re, sizeof(double)*SAMPLES_PER_BUFFER);
								
				HanWin (fft_re, SAMPLES_PER_BUFFER);//pre processing to avoid the rect window effects
				Spectrum (fft_re,SAMPLES_PER_BUFFER ); //magnutude spectrum calculation
				
				RMS (time_series,SAMPLES_PER_BUFFER ,&rms_val );
				SetCtrlVal (panelHandle, PANEL_NUMERICMETER, rms_val);
				
				DeleteGraphPlot (panelHandle, PANEL_GRAPH, -1, VAL_DELAYED_DRAW);
				DeleteGraphPlot (panelHandle, PANEL_GRAPH_2, -1, VAL_DELAYED_DRAW);
				for(j=0;j<SAMPLES_PER_BUFFER;j++)
				{
					if(fft_re[j]<low_limit)
					{
						fft_re[j]=-200;
					}
				else
					{
						fft_re[j] = 10*log10 (fft_re[j]);
					}
				}
									
				PlotY (panelHandle, PANEL_GRAPH_2, time_series, SAMPLES_PER_BUFFER, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_GREEN);
				if(plot_style)
				{
					PlotXY (panelHandle, PANEL_GRAPH, x_axis, fft_re, SAMPLES_PER_BUFFER/2, VAL_FLOAT, VAL_DOUBLE, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1,
										VAL_MAGENTA);
				}
				else
				{
					PlotXY (panelHandle, PANEL_GRAPH, x_axis, fft_re, SAMPLES_PER_BUFFER/2, VAL_FLOAT, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_MAGENTA);
				}
								
					//SetAxisRange (panelHandle, PANEL_GRAPH, VAL_AUTOSCALE, 0.0, x_axis[SAMPLES_PER_BUFFER/2], VAL_NO_CHANGE, 0.0, 1.0);
				data.frameIndex = 0;  
							
			}
			/*else
			{
				close_stream(stream);
			}*/
			Delay(SAMPLES_PER_BUFFER/SAMPLE_RATE);
				
		}
	if(quit)
		CmtExitThreadPoolThread (0);
		
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
int CVICALLBACK change_buffer_size_callback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_buffer, &SAMPLES_PER_BUFFER);
			if(init_flag == 1)
			{
				close_stream(stream);
			}
			SetCtrlAttribute (panelHandle, PANEL_init, ATTR_DIMMED, 0);
			SetCtrlAttribute (panelHandle, PANEL_BINARYSWITCH, ATTR_DIMMED, 1);
			SetCtrlVal (panelHandle, PANEL_STRING,"please press the INIT button" );
			break;
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
int CVICALLBACK plot_style_callback (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			plot_style = !plot_style;
			break;
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
int CVICALLBACK x_log_callback (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{   int val;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_x_log, &val);
			if(val)
			{
				SetCtrlAttribute (panelHandle, PANEL_GRAPH,ATTR_XMAP_MODE, VAL_LOG);
			}
			else
			{
			  	SetCtrlAttribute (panelHandle, PANEL_GRAPH, ATTR_XMAP_MODE, VAL_LINEAR);
			}
			break;
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
int CVICALLBACK buffer_size_val (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlVal (panelHandle, PANEL_STRING,"please press the CHOOSE BUFFER SIZE button" );
			break;
		case EVENT_RIGHT_CLICK:

			break;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
void close_stream(PaStream* my_stream)
{
		
		if(fft_re!=NULL)
		{
			free(fft_re);
			fft_re=NULL;
		}
							   
		if(time_series!=NULL)
		{
			free(time_series);
			time_series=NULL;
		}
									
		if(x_axis!=NULL)
		{
			free(x_axis);
			x_axis=NULL;
		}
							   
		if( data.recordedSamples!=NULL )
		{       /* Sure it is valid pointer. */
			free( data.recordedSamples );
			data.recordedSamples = NULL;
		}
			
		if( err != paNoError )
		{
			SetCtrlVal (panelHandle, PANEL_STRING,"An error occured while using the portaudio stream" );
		}
			
		if(my_err == -10)
		{
		 	SetCtrlVal (panelHandle, PANEL_STRING,"error allocating memory" );
		}
			
		err = Pa_CloseStream( my_stream );
	  	Pa_Terminate();
}
////////////////////////////////////////////////////////////////////////////////
static int audio_in_Callback( const void *inputBuffer,
						   void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo *timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    paTestData *data = (paTestData*)userData;
    const SAMPLE *read_ptr = (const SAMPLE*)inputBuffer;
    SAMPLE *write_ptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
	//(void) outputBuffer; /* Prevent unused variable warnings. */
    //(void) timeInfo;
    //(void) statusFlags;
    //(void) userData;    
	//long i;
    
    if( inputBuffer == NULL ) //in case of mised buffer
    {
        for( i=0; i<data->maxFrameIndex; i++ )
        {
            *write_ptr++ = SAMPLE_SILENCE;  
          
        }
    }
    else
    {
		memcpy(write_ptr , read_ptr , sizeof(SAMPLE)*framesPerBuffer);    
    }
    
	return paContinue;
   
}


