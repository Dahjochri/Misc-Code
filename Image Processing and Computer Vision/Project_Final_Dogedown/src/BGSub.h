

#ifndef BGSUB_H
#define BGSUB_H
//Includes
#include <cstddef>      //size_t
#include <cmath>
#include <vector>
#include <algorithm>    //sort
#include <cstring>      //memset

//Enums
enum BGSubMethod {BGSUB_RUNNING_GAUSSIAN_AVG, BGSUB_TEMPORAL_MEDIAN, BGSUB_JUST_PREVIOUS, BGSUB_IMPROVED_RGA};

//Structs
struct rgaPixStruct
{
    float       average;
    float       average_prev;
    float       std_deviation;
    float       std_deviation_prev;
};


struct tmPixStruct
{
    std::vector<unsigned char> pixList;
};

class BGSub
{
    public:
        BGSub();
        ~BGSub();

        void init(unsigned char* in, unsigned char* out,
               unsigned int height, unsigned int width,
               unsigned int channels, BGSubMethod method);
        void update();
        void updatePointer(unsigned char* new_in);


        void fgaDoneCalibrating();
        void reCalibrate();
        //Getters and Setters here

    private:
        //Running Gaussian Avg
        void RGA();

        //Improved Running Gaussian Average
        void IRGA();
        void IRGA_updateThreshold();

        //Temporal Median
        void TM();

        //Just Previous
        void JP();


        //Core Variables
        unsigned char*          m_imgInput;
        unsigned char*          m_imgOutput;
        unsigned int            m_imgChannels;
        unsigned int            m_imgHeight;
        unsigned int            m_imgWidth;
        unsigned int            m_totalPixels;
        size_t                  m_framesRun;

        BGSubMethod             m_method;

        //Running Gaussian Avg (RGA)
        struct rgaPixStruct*    m_rgaPixels;
        float                   m_rgaAlpha;
        float                   m_rgaK;
        bool                    m_rgaHasCalibrated;

        //Improved Running Gaussian Average
        struct rgaPixStruct*     m_irgaPixels;
        float                   m_irgaAlpha1;        //a1 < a2
        float                   m_irgaAlpha2;
        float*                  m_irgaThreshold;
        unsigned char*          m_irgaHistogram;
        unsigned int            m_irgaTCounter;

        //Temporal Median
        struct tmPixStruct*     m_tmPixels;
        unsigned int            m_tmBufferCount;
        bool                    m_tmBufferFull;
        unsigned int            m_tmThreshold;

        //Just Previous
        unsigned char*          m_jpImgPrev;
        unsigned int            m_jpThreshold;

};

#endif // BGSUB_H

