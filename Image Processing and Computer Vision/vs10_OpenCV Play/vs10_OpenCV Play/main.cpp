/*
 *	Standard headers
 */
#include <iostream>
#include <vector>

using namespace std;

/*
 *	OpenCV headers
 */
#include <opencv2\opencv.hpp>;
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\objdetect\objdetect.hpp>
#include <opencv2\gpu\gpu.hpp>

using namespace cv;

int main(int argc, char** argv)
{
	//Video Capture
	VideoCapture capture;
	capture.open("./Pedestrians_compilation_My_favourite_clips_of_2013_medium.mp4");

	//Mats
	Mat src_img, display_img, display_img2;
	gpu::GpuMat src_gpu, mono_gpu, cmono_gpu, c_gpu;

	//HOG
	gpu::HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	vector<Rect> found;

	//Input
	int c = 0;

	namedWindow("play", CV_WINDOW_AUTOSIZE);
	namedWindow("play2", CV_WINDOW_AUTOSIZE);

	while(true)
	{
		//Get image from video
		capture >> src_img;

		display_img = src_img.clone();

		//Upload to gpu & convert to gray
		src_gpu.upload(src_img);
		gpu::cvtColor(src_gpu, mono_gpu, CV_BGR2GRAY);
		gpu::equalizeHist(mono_gpu, mono_gpu);
		gpu::Canny(mono_gpu, cmono_gpu, 50, 200, 3);
		
		//Hough line
		gpu::GpuMat d_lines;
		gpu::HoughLinesBuf d_buf;

		gpu::HoughLinesP(cmono_gpu, d_lines, d_buf, 1.0f, (float) (CV_PI/180.0f), 50, 2);

		//Draw hough lines
		vector<Vec4i> lines;
		if(!d_lines.empty())
		{
			lines.resize(d_lines.cols);
			Mat h_lines(1, d_lines.cols, CV_32SC4, &lines[0]);
			d_lines.download(h_lines);
		}

		for(size_t i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			line(display_img2, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 2, CV_AA);
		}

		//HOG detect and draw foundRects
		hog.detectMultiScale(mono_gpu, found);
		for(int i = 0; i < found.size(); i++)
		{
			Rect r = found[i];
			rectangle(display_img, r.tl(), r.br(), Scalar(0,255,0), 2);
		}


		cmono_gpu.download(display_img2);
		imshow("play", display_img);
		imshow("play2", display_img2);
		

		//Check for 'ESC'
		c = waitKey(1);
		if(c==27)break;
	}
}

