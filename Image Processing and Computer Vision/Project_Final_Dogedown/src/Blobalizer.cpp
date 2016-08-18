

#include "Blobalizer.h"

Blobalizer::Blobalizer()
{

}

Blobalizer::~Blobalizer()
{
    delete m_imgAnalysis;
}

void Blobalizer::init(unsigned char* img, unsigned int height, unsigned int width)
{
    m_imgInput      = img;
    m_imgHeight     = height;
    m_imgWidth      = width;
    m_imgAnalysis   = new unsigned int[height*width];
    m_blobAreaThreshold = 300;
    m_blobConvexMinDist = 3.0;
    m_frameCounter = 0;

    //Grassfire
    m_gID = 1;  //[1;254]
}

void Blobalizer::detectBlobs()
{
    m_frameCounter++;

    //Remove old blobs
    m_blobs.clear();
    m_gID = 1;

    //Copy image
    unsigned int whites = 0;
    for(unsigned int y(0); y < m_imgHeight; y++)
    {
        for(unsigned int x=0; x < m_imgWidth; x++)
        {
            m_imgAnalysis[y*m_imgWidth+x] = m_imgInput[y*m_imgWidth+x];
            if(m_imgInput[y*m_imgWidth+x] == 255)
                whites++;
        }
    }

   // if(whites > round((m_imgHeight*m_imgWidth)*0.75))
     //  return;

    //Go go grassfire
    for(unsigned int y=0; y < m_imgHeight; y++)
    {
        for(unsigned int x=0; x < m_imgWidth; x++)
        {
            unsigned int i = (y*m_imgWidth+x);
            if(m_imgAnalysis[i]==255)
            {
                //Label (burn) it
                m_imgAnalysis[i] = m_gID;

                std::vector<MyPoint> toCheck;
                toCheck.insert(toCheck.begin(), MyPoint(x,y));

                Blob b;
                b.area = 0;
                b.id = m_gID;

                do
                {
                    MyPoint check = toCheck.back();
                    toCheck.pop_back();

                    b.area++;
                    b.pixels.push_back(check);

                    if(check.x>0 && m_imgAnalysis[check.y*m_imgWidth+check.x-1] == 255)
                    {
                        m_imgAnalysis[check.y*m_imgWidth+check.x-1] = m_gID;
                        toCheck.push_back(MyPoint(check.x-1,check.y));
                    }

                    if(check.x<m_imgWidth-1 && m_imgAnalysis[check.y*m_imgWidth+check.x+1] == 255)
                    {
                        m_imgAnalysis[check.y*m_imgWidth+check.x+1] = m_gID;
                        toCheck.push_back(MyPoint(check.x+1,check.y));
                    }

                    if(check.y>0 && m_imgAnalysis[(check.y-1)*m_imgWidth+check.x] == 255)
                    {
                        m_imgAnalysis[(check.y-1)*m_imgWidth+check.x] = m_gID;
                        toCheck.push_back(MyPoint(check.x,check.y-1));
                    }

                    if(check.y<m_imgHeight-1 && m_imgAnalysis[(check.y+1)*m_imgWidth+check.x] == 255)
                    {
                        m_imgAnalysis[(check.y+1)*m_imgWidth+check.x] = m_gID;
                        toCheck.push_back(MyPoint(check.x,check.y+1));
                    }
                } while(toCheck.size()>0);

                if(b.area > m_blobAreaThreshold)
                {
                    m_blobs.push_back(b);
                }

                m_gID++;
                if(m_gID == 255)
                    m_gID++;
            }


        }
    }

    if(m_blobs.size()>10)
    {
        _merge_sort(m_blobs);
        m_blobs.erase(m_blobs.begin()+4, m_blobs.end());
    }

    //Get infooooo

    if(m_blobs.size() == 0)
        return;

    //Bounding rectangles && Compactness && Center of Mass
    MyRect r;
    float angle;
    float len;
    unsigned int rounds;
    int x;
    int y;


    for(unsigned int i=0; i < m_blobs.size(); i++)
    {
        //BR
        r.top_left.x = m_imgWidth;
        r.top_left.y = m_imgHeight;
        r.bottom_right.x = 0;
        r.bottom_right.y = 0;

        for(unsigned int p=0; p < m_blobs[i].pixels.size(); p++)
        {
            //BR
            if(m_blobs[i].pixels[p].x < r.top_left.x)
            {
                r.top_left.x = m_blobs[i].pixels[p].x;
            }
            if(m_blobs[i].pixels[p].y < r.top_left.y)
            {
                r.top_left.y = m_blobs[i].pixels[p].y;
            }
            if(m_blobs[i].pixels[p].x > r.bottom_right.x)
            {
                r.bottom_right.x = m_blobs[i].pixels[p].x;
            }
            if(m_blobs[i].pixels[p].y > r.bottom_right.y)
            {
                r.bottom_right.y = m_blobs[i].pixels[p].y;
            }
        }

        //BR
        m_blobs[i].bounding_rect = r;

        //Convex Hull
        angle = 0.0;
        len = 0.0;
        rounds = 0;

        //Find top pixel
        MyPoint top(m_blobs[i].pixels.front().x, m_blobs[i].pixels.front().y);

        m_blobs[i].convex_points.push_back(top);

        do
        {
            rounds++;
            len+=0.8;

            x = (int) m_blobs[i].convex_points.back().x+ ceil(cos(((3.1415926535)/180) * angle) * len);
            y = (int) m_blobs[i].convex_points.back().y- ceil(sin(((3.1415926535)/180) * angle) * len);

            if(x < m_blobs[i].bounding_rect.top_left.x
               || y < m_blobs[i].bounding_rect.top_left.y
               || x > m_blobs[i].bounding_rect.bottom_right.x
               || y > m_blobs[i].bounding_rect.bottom_right.y)
            {
                //Increase angle, reset len and go again!
                //std::cout << "Blob " << m_blobs[i].id << " reached end of bounding box (" << x << "," << y << ")" << std::endl;

                angle-=4.5;
                len = 0.0;
                continue;
            }

            if(m_imgAnalysis[y*m_imgWidth+x] == m_blobs[i].id
               && (x != m_blobs[i].convex_points.back().x
                    &&  y != m_blobs[i].convex_points.back().y ))
            {
                //Found new corner!
                MyPoint tmp(x,y);
                m_blobs[i].convex_points.push_back(tmp);

            }


            //End condition
            if((m_blobs[i].convex_points.back().x == m_blobs[i].convex_points.front().x
                && m_blobs[i].convex_points.back().y == m_blobs[i].convex_points.front().y
                && m_blobs[i].convex_points.size() != 1))
            {
                m_blobs[i].convex_points.pop_back();
                break;
            }

        } while( m_blobs[i].convex_points.size() == 1
                || angle >= -360.0);

        if(m_blobs[i].convex_points.back().x > m_blobs[i].convex_points.front().x)
            m_blobs[i].convex_points.pop_back();

        //Remove convex points close to eachother
        for(unsigned int n=0; n < m_blobs[i].convex_points.size(); n++)
        {
            unsigned int n2;
            if(n == m_blobs[i].convex_points.size()-1)
            {
                n2 = 0;
            }
            else
            {
                n2 = n+1;
            }

            if(abs(
                   sqrt( pow(m_blobs[i].convex_points[n].x,2)+pow(m_blobs[i].convex_points[n].y,2))-
                   sqrt( pow(m_blobs[i].convex_points[n2].x,2)+pow(m_blobs[i].convex_points[n2].y,2)))
                    > 5)
            {

                m_blobs[i].convex_points.erase(m_blobs[i].convex_points.begin()+n2);
            }
        }
    }

}

void Blobalizer::updatePointer(unsigned char* n)
{
    m_imgInput = n;
}

std::vector<Blob> Blobalizer::getBlobs()
{
    return m_blobs;
}

std::vector<Blob> Blobalizer::_merge_sort(std::vector<Blob> m)
{
    if(m.size() <= 1)
        return m;

    std::vector<Blob> left, right;
    unsigned int middle = m.size() / 2;

    for(unsigned int i = 0; i < middle; i++)
    {
        left.push_back(m[i]);
    }
    for(unsigned int i = middle; i < m.size(); i++)
    {
        right.push_back(m[i]);
    }

    left = _merge_sort(left);
    right = _merge_sort(right);

    return _merge(left,right);
}

std::vector<Blob> Blobalizer::_merge(std::vector<Blob> left, std::vector<Blob> right)
{
    std::vector<Blob> result;

    while(left.size() > 0 || right.size() > 0)
    {
        if(left.size()>0 && right.size()>0)
        {
            if(left.front().area >= right.front().area)
            {
                result.push_back(left.front());
                left.erase(left.begin());
            }
            else
            {
                result.push_back(right.front());
                right.erase(right.begin());
            }
        }
        else if(left.size() > 0)
        {
            result.push_back(left.front());
            left.erase(left.begin());
        }
        else if(right.size() > 0)
        {
            result.push_back(right.front());
            right.erase(right.begin());
        }
    }

    return result;
}
