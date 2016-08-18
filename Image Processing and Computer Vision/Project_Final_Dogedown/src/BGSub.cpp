

#include "BGSub.h"

BGSub::BGSub()
{
    //DO NOTHING
}

BGSub::~BGSub()
{
    switch(m_method)
    {
        case BGSUB_RUNNING_GAUSSIAN_AVG:
            delete m_rgaPixels;
            break;
        case BGSUB_TEMPORAL_MEDIAN:
            delete m_tmPixels;
            break;
        case BGSUB_JUST_PREVIOUS:
            delete m_jpImgPrev;
            break;
    }
}

void BGSub::init(unsigned char* in, unsigned char* out,
               unsigned int height, unsigned int width, unsigned int channels,
                BGSubMethod method)
{
    //Initialize values (Core)
    m_imgInput      = in;
    m_imgOutput     = out;
    m_imgChannels   = channels;
    m_imgHeight     = height;
    m_imgWidth      = width;
    m_totalPixels   = m_imgHeight * m_imgWidth * m_imgChannels;
    m_method        = method;
    m_framesRun     = 0;


    switch(m_method)
    {
        case BGSUB_RUNNING_GAUSSIAN_AVG:
            //Init RGA Vars
            m_rgaPixels     = new struct rgaPixStruct[m_imgWidth*m_imgHeight*m_imgChannels];
            m_rgaAlpha      = 0.05;     //Whatever - empirically defined!
            m_rgaK          = 3;      //Empirical as well, I guess   - Add get/set
            m_rgaHasCalibrated = false;
            break;

        case BGSUB_IMPROVED_RGA:
            m_irgaPixels    = new struct rgaPixStruct[m_imgWidth*m_imgHeight*m_imgChannels];
            m_irgaHistogram = new unsigned char[256];
            m_irgaAlpha1    = 0.05;
            m_irgaAlpha2    = 0.1;
            m_irgaTCounter  = 0;
            m_irgaThreshold = new float[10*10*m_imgChannels]; //100 regions (10x10)

        case BGSUB_TEMPORAL_MEDIAN:
            //Init TM Vars
            m_tmPixels      = new struct tmPixStruct[m_imgWidth*m_imgHeight*m_imgChannels];
            m_tmBufferCount = 11;       //10 should be fine - add get/set
            m_tmBufferFull  = false;
            m_tmThreshold   = 50;
            break;

        case BGSUB_JUST_PREVIOUS:
            m_jpImgPrev     = new unsigned char[m_imgWidth*m_imgHeight*m_imgChannels];
            m_jpThreshold   = 50;
            break;
    }
}

void BGSub::update()
{
    switch(m_method)
    {
        case BGSUB_RUNNING_GAUSSIAN_AVG:
            RGA();
            break;

        case BGSUB_IMPROVED_RGA:
            IRGA();
            break;

        case BGSUB_TEMPORAL_MEDIAN:
            TM();
            break;

        case BGSUB_JUST_PREVIOUS:
            JP();
    }

    m_framesRun++;
}

void BGSub::updatePointer(unsigned char* new_in)
{
    //Point to the NEW image!
    m_imgInput = new_in;
}

//Running Gaussian Avg
void BGSub::RGA()
{
    //Check if first run
    if(m_framesRun == 0)
    {
       //Yes - don't calculate, just get values
       for(unsigned int y = 0; y < m_imgHeight; y++)
       {
           for(unsigned int x = 0; x < m_imgWidth; x++)
           {
               for(unsigned int c = 0; c < m_imgChannels; c++)
               {
                    unsigned int i = (y*m_imgWidth+x)*m_imgChannels+c;
                    m_rgaPixels[i].average      = m_imgInput[i];
                    m_rgaPixels[i].average_prev = m_imgInput[i];
                    m_rgaPixels[i].std_deviation    = 0;
               }
           }
        }
    }
    else
    {
        //Not first run
       for(unsigned int y = 0; y < m_imgHeight; y++)
       {
           for(unsigned int x = 0; x < m_imgWidth; x++)
           {
               for(unsigned int c = 0; c < m_imgChannels; c++)
               {
                    unsigned int i = (y*m_imgWidth+x)*m_imgChannels+c;
                    unsigned int M = 0;

                    //Is difference larger than deviation?
                    if(std::abs(m_imgInput[i]-m_rgaPixels[i].average) > m_rgaK * m_rgaPixels[i].std_deviation)
                    {
                        //Foreground object!
                        m_imgOutput[(y*m_imgWidth+x)*m_imgChannels+0] = 255;
                        m_imgOutput[(y*m_imgWidth+x)*m_imgChannels+1] = 255;
                        m_imgOutput[(y*m_imgWidth+x)*m_imgChannels+2] = 255;
                        M = 1;
                    }
                    else
                    {
                        //Background object!
                        m_imgOutput[(y*m_imgWidth+x)*m_imgChannels+0] = 0;
                        m_imgOutput[(y*m_imgWidth+x)*m_imgChannels+1] = 0;
                        m_imgOutput[(y*m_imgWidth+x)*m_imgChannels+2] = 0;
                        M = 0;
                    }

                    //Save avg
                    m_rgaPixels[i].average_prev = m_rgaPixels[i].average;


                    if(!m_rgaHasCalibrated)
                        M = 0;
                    //Calculate new
                    //m_rgaPixels[i].average = m_rgaAlpha * m_imgInput[i]
                    //                            + (1-m_rgaAlpha)*m_rgaPixels[i].average_prev;
                    m_rgaPixels[i].average = M * m_rgaPixels[i].average_prev + (1-M)*(m_rgaAlpha*m_imgInput[i]+(1-m_rgaAlpha)*m_rgaPixels[i].average_prev);

                    //Calculate Std. Deviation avg
                    m_rgaPixels[i].std_deviation_prev = m_rgaPixels[i].std_deviation;
                    m_rgaPixels[i].std_deviation = M * m_rgaPixels[i].std_deviation_prev + (1-M) * (m_rgaAlpha * std::abs((m_imgInput[i]-m_rgaPixels[i].average))
                                                    + (1-m_rgaAlpha)*m_rgaPixels[i].std_deviation_prev);

               }
           }
        }
    }
}

//Improved Running Gaussian Average
void BGSub::IRGA()
{/*
    if(m_irgaTCounter==0)
        IRGA_updateThreshold();
    m_irgaTCounter = (m_irgaTCounter+1) % 5;*/

    //Check if first run
    if(m_framesRun == 0)
    {
       //Yes - don't calculate, just get values
       for(unsigned int y = 0; y < m_imgHeight; y++)
       {
           for(unsigned int x = 0; x < m_imgWidth; x++)
           {
               for(unsigned int c = 0; c < m_imgChannels; c++)
               {
                    unsigned int i = (y*m_imgWidth+x)*m_imgChannels+c;
                    m_irgaPixels[i].average      = m_imgInput[i];
                    m_irgaPixels[i].average_prev = m_imgInput[i];
                    m_irgaPixels[i].std_deviation    = 0;
               }
           }
        }
        IRGA_updateThreshold();
    }
    else
    {
        //Not first run
       for(unsigned int y = 0; y < m_imgHeight; y++)
       {
           for(unsigned int x = 0; x < m_imgWidth; x++)
           {
               for(unsigned int c = 0; c < m_imgChannels; c++)
               {
                    unsigned int i = (y*m_imgWidth+x)*m_imgChannels+c;
                    float correctAlpha;
                    float correctThreshold;

                    //Threshold in 10x10 grid
                    int ty = floor(y/(m_imgHeight/10));
                    int tx = floor(x/(m_imgWidth/10));
                    correctThreshold = m_irgaThreshold[(ty * 10 + tx)*m_imgChannels+c];

                    //Is difference larger than deviation?
                    if(std::abs(m_imgInput[i]-m_irgaPixels[i].average) > correctThreshold)
                    {
                        //Foreground object!
                        m_imgOutput[i] = 255;
                        correctAlpha = m_irgaAlpha2;
                    }
                    else
                    {
                        //Background object!
                        m_imgOutput[i] = 0;
                        correctAlpha = m_irgaAlpha1;
                    }

                    //Save avg
                    m_irgaPixels[i].average_prev = m_irgaPixels[i].average;


                    if(!m_rgaHasCalibrated)
                        correctAlpha = m_irgaAlpha1;

                    //Calculate new
                    m_irgaPixels[i].average = correctAlpha * m_imgInput[i]
                                               + (1-correctAlpha)*m_irgaPixels[i].average_prev;

                    //Calculate Std. Deviation avg
                    m_irgaPixels[i].std_deviation_prev = m_irgaPixels[i].std_deviation;
                    m_irgaPixels[i].std_deviation = (1-correctAlpha) * m_irgaPixels[i].std_deviation_prev + correctAlpha * std::abs((m_imgInput[i]-m_irgaPixels[i].average));

               }
           }
        }
    }
}

//For above
void BGSub::IRGA_updateThreshold()
{
    //Loop through 10x10 grid for each channel
    for(unsigned int ty = 0; ty < 10; ty++)
    {
        for(unsigned int tx = 0; tx < 10; tx++)
        {
            for(unsigned int c = 0; c < m_imgChannels; c++)
            {
                //Clear histogram
                memset(m_irgaHistogram, 0, 256);

                //Get histogram for this block
                for(unsigned int y = ty * floor((m_imgHeight/10)); y < (ty+1)*(m_imgHeight/10); y++)
                {
                    for(unsigned int x = tx * floor((m_imgWidth/10)); x < (tx+1)*(m_imgWidth/10); x++)
                    {
                        m_irgaHistogram[ m_imgInput[ (y*m_imgWidth+x)*m_imgChannels+c ] ]++;
                    }
                }

                //Find location of max value in histogram and pre/post minimum for lower/upper threshold
                float h_max, h_min1, h_min2;
                unsigned int h_max_loc, h_min1_loc, h_min2_loc;
                h_max  = 0;
                h_min1 = 255;
                h_min2 = 255;

                //Max
                for(unsigned int i = 0; i < 256; i++)
                {
                    if(m_irgaHistogram[i] > h_max)
                    {
                        h_max = m_irgaHistogram[i];
                        h_max_loc = i;
                    }
                }

                //Lower min
                for(unsigned int i=0; i < h_max_loc; i++)
                {
                    if(m_irgaHistogram[i] < h_min1)
                    {
                        h_min1 = m_irgaHistogram[i];
                        h_min1_loc = i;
                    }
                }

                //Upper min
               /* for(unsigned int i=255; i > h_max_loc; i--)
                {
                    if(m_irgaHistogram[i] < h_min2)
                    {
                        h_min2 = m_irgaHistogram[i];
                        h_min2_loc = i;
                    }
                }*/

                //Find max "d" for lower!
                //Sorry for messy variables -  did trigonometry on a paper..
                float d_max = 0;
                unsigned int d_max_loc; //Threshold

                float angle_A = acos(h_max_loc / 255);
                for(unsigned int i=0; i < h_max_loc; i++)
                {
                    float q = sqrt(pow(i,2) + pow(m_irgaHistogram[i],2));
                    float b = acos(i/255);
                    float c = angle_A - b;
                    float d = q * sin(c);

                    if(d > d_max)
                    {
                        d_max = d;
                        d_max_loc = i;
                    }
                }

                //Set (lower) threshold to value at d_max_loc
                m_irgaThreshold[(ty*10+tx)*m_imgChannels+c] = m_irgaHistogram[d_max_loc];
            }
        }
    }
}

//Temporal Median
void BGSub::TM()
{
    //For checking if the buffer is full yet
    bool bufferFullYet = false;

    for(unsigned int y = 0; y < m_imgHeight; y++)
    {
        for(unsigned int x = 0; x < m_imgWidth; x++)
        {
            for(unsigned int c = 0; c < m_imgChannels; c++)
            {
                unsigned int i = (y*m_imgWidth+x)*m_imgChannels+c;

                //Insert new image into buffer
                m_tmPixels[i].pixList.insert(m_tmPixels[i].pixList.begin(),m_imgInput[i]);

                //Is buffer full and ready to be used?
                if(m_tmBufferFull)
                {
                    //Remove oldest element
                    m_tmPixels[i].pixList.pop_back();

                    //Sort list
                    std::sort(m_tmPixels[i].pixList.begin(), m_tmPixels[i].pixList.end());

                    //Output the result of current-median!
                    unsigned int middle = std::floor(m_tmPixels[i].pixList.size()/2);
                    m_imgOutput[i] = abs(m_imgInput[i] - m_tmPixels[i].pixList[middle]);
                }
                else
                {
                    //Else check if buffer is finally full!
                    if(m_tmPixels[i].pixList.size()==m_tmBufferCount)
                        bufferFullYet = true;
                }
            }
        }
    }

    //Remember to set flag, if buffer is finally full
    if(!m_tmBufferFull && bufferFullYet)
    {
        m_tmBufferFull = true;
    }

}

void BGSub::JP()
{
    for(unsigned int y=0; y < m_imgHeight; y++)
    {
        for(unsigned int x=0; x < m_imgWidth; x++)
        {
            for(unsigned int c=0; c < m_imgChannels; c++)
            {
                unsigned int i = (y*m_imgWidth+x)*m_imgChannels+c;
                if(m_framesRun>0)
                    m_imgOutput[i] = abs(m_imgInput[i] - m_jpImgPrev[i]) > m_jpThreshold ? 255 : 0;
                m_jpImgPrev[i] = m_imgInput[i];
            }
        }
    }
}

void BGSub::fgaDoneCalibrating()
{
    m_rgaHasCalibrated = true;
}

void BGSub::reCalibrate()
{
    m_rgaHasCalibrated = false;
}
