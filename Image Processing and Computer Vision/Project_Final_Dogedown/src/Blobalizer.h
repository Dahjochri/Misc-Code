

#ifndef BLOBALIZER_H
#define BLOBALIZER_H

//Includes
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>  //system
#include <algorithm>

//Structures
struct MyPoint
{
    MyPoint(unsigned int a=0, unsigned int b=0) : x(a), y(b) {}
    unsigned int x,y;
};

struct MyRect
{
    MyPoint     top_left, bottom_right;
};

struct Blob
{
    unsigned int            id;
    unsigned int            area;

    MyRect                  bounding_rect;

    std::vector<MyPoint>    convex_points;
    std::vector<MyPoint>    pixels;
};

class Blobalizer
{
    public:
        Blobalizer();
        ~Blobalizer();

        void init(unsigned char* img, unsigned int height, unsigned int width);

        void detectBlobs();

        //void update();
        void updatePointer(unsigned char* n);

        std::vector<Blob>   getBlobs();


    private:
        //Image stuff
        unsigned char*      m_imgInput;
        unsigned int*       m_imgAnalysis;
        unsigned int        m_imgWidth;
        unsigned int        m_imgHeight;
        unsigned int        m_blobAreaThreshold;
        float               m_blobConvexMinDist;
        size_t              m_frameCounter;


        std::vector<Blob>    m_blobs;
        unsigned int   m_gID;

        std::vector<Blob>   _merge_sort(std::vector<Blob> m);
        std::vector<Blob>   _merge(std::vector<Blob> left, std::vector<Blob> right);

};


#endif // BLOBALIZER_H

