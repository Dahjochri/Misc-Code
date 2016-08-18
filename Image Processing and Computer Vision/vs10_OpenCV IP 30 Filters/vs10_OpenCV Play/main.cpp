/*


YOUTUBE LINKS (2 PARTS BECAUSE FAIL :/ )

Part 1: http://youtu.be/r6dPw1uhvj4
Part 2: http://youtu.be/JUzllnOFtqY


*/



/*
 *	Standard headers
 */
#include <iostream>
#include <vector>
#include <cmath>


/*
 *	OpenCV headers
 */
#include <opencv2/opencv.hpp>;
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/gpu/gpu.hpp>

using namespace cv;

int main(int argc, char** argv)
{
	//Video Capture
	VideoCapture capture;
	capture.open("Funny_Video_NO_SOUNDS_small.flv");
	if(!capture.isOpened())
		return 1;
	//Mats
	Mat src_img, display_img, edit_img;
	capture >> src_img;
	Mat resized_img(src_img.rows/2, src_img.cols/2, CV_8UC3);
	resize(src_img, resized_img, resized_img.size(), 0.5, 0.5);
	gpu::GpuMat src_gpu,tmp_gpu, prev_gpu, edit_gpu, mono_gpu, final_gpu(resized_img.rows*5+10, resized_img.cols*6, CV_8UC3);

	int w = resized_img.cols;
	int h = resized_img.rows;
	//Input
	int c = 0;

	namedWindow("play", CV_WINDOW_AUTOSIZE);
	while(true)
	{
		//Get image from video
		capture >> src_img;
		if (src_img.empty()) 
		{
			capture.open("Funny_Video_NO_SOUNDS_small.flv");
			continue;
		}  

		resize(src_img, resized_img, resized_img.size(), 0.5, 0.5);

		//Upload to gpu
		src_gpu.upload(resized_img);
		
		if(prev_gpu.empty())
			src_gpu.copyTo(prev_gpu);

		gpu::cvtColor(src_gpu, mono_gpu, CV_BGR2GRAY);

		int im = 0;

		for(int fy = 0; fy < 5; fy++)
		{
			for(int fx = 0; fx < 6; fx++)
			{
				edit_gpu = final_gpu(Rect(fx * w, fy*h, w,h));
				src_gpu.copyTo(edit_gpu);


				im++;
				gpu::GpuMat tmp;
				gpu::GpuMat mono2;

				switch(im)
				{
					case 1:
						//invert
						gpu::bitwise_not(edit_gpu, edit_gpu);
						break;

					case 2:
						//Brightness
						edit_gpu.convertTo(edit_gpu, -1, 1.0, 100.0);
						break;

					case 3:
						//Contrast
						edit_gpu.convertTo(edit_gpu, -1, 5, 0);
						break;

					case 4:
						//Gamma
						edit_gpu.convertTo(edit_gpu, -1, 0.5, 50.0);
						break;

					case 5:
						//Alpha blend
						{
							//Get rotated
							Mat rot_mat;
							gpu::cvtColor(mono_gpu, tmp, CV_GRAY2BGR);
							rot_mat = getRotationMatrix2D(Point(edit_gpu.cols/2, edit_gpu.rows/2), 120, 1);
							gpu::warpAffine(edit_gpu, tmp_gpu, rot_mat, edit_gpu.size());
							gpu::addWeighted(tmp, 0.5,tmp_gpu,0.5, 0.0, edit_gpu);
							rot_mat.release();
						}
						break;

					case 6:
						//Eq. Histogram
						{
							gpu::equalizeHist(mono_gpu, tmp);
							gpu::cvtColor(tmp, edit_gpu, CV_GRAY2BGR);
						}
						break;

					case 7:
						//Gray
						gpu::cvtColor(mono_gpu, edit_gpu, CV_GRAY2BGR);
						break;

					case 8:
						//Binary
						{
							gpu::threshold(mono_gpu, mono2, 100, 255,THRESH_BINARY);
							gpu::cvtColor(mono2, edit_gpu, CV_GRAY2BGR);
						}
						break;

					case 9:
						//Sobel
						gpu::Sobel(edit_gpu, edit_gpu,edit_gpu.depth(), 1, 1);
						break;

					case 10:
						//Gaussian
						gpu::GaussianBlur(edit_gpu, edit_gpu, Size(7,7), 0);
						break;

					case 11:
						//Crop
						{
							gpu::GpuMat cropped;
							cropped = final_gpu(Rect(100,100,edit_gpu.cols, edit_gpu.rows));
							cropped.copyTo(edit_gpu);
							cropped.release();
						}
						break;

					case 12:
						//Rotate
						{
							Mat rot_mat;
							rot_mat = getRotationMatrix2D(Point(edit_gpu.cols/2, edit_gpu.rows/2), 120, 1);
							gpu::warpAffine(edit_gpu, tmp_gpu, rot_mat, edit_gpu.size());
							tmp_gpu.copyTo(edit_gpu);
							rot_mat.release();
						}
						break;

					case 13:
						//Eroding
						{
							gpu::GpuMat wat(edit_gpu.rows, edit_gpu.cols, CV_8UC1);
							gpu::GpuMat wat2(edit_gpu.rows, edit_gpu.cols, CV_8UC1);
							gpu::threshold(mono_gpu, mono2, 200, 255,THRESH_BINARY);
							wat = mono2;
							gpu::erode(wat, wat2, Mat());
							gpu::cvtColor(wat2, edit_gpu, CV_GRAY2BGR);
							wat.release();
							wat2.release();
						}
						break;
						
					case 14:
						//Dilation
						{
							gpu::GpuMat wat(edit_gpu.rows, edit_gpu.cols, CV_8UC1);
							gpu::GpuMat wat2(edit_gpu.rows, edit_gpu.cols, CV_8UC1);
							gpu::threshold(mono_gpu, mono2, 200, 255,THRESH_BINARY);
							wat = mono2;
							gpu::dilate(wat, wat2, Mat());
							gpu::cvtColor(wat2, edit_gpu, CV_GRAY2BGR);
							wat.release();
							wat2.release();
						}
						break;
						
					case 15:
						//Opening
						{
							gpu::GpuMat wat(edit_gpu.rows, edit_gpu.cols, CV_8UC1);
							gpu::GpuMat wat2(edit_gpu.rows, edit_gpu.cols, CV_8UC1);
							gpu::threshold(mono_gpu, mono2, 200, 255,THRESH_BINARY);
							wat = mono2;
							gpu::erode(wat, wat2, Mat());
							gpu::dilate(wat2, wat, Mat());
							gpu::cvtColor(wat, edit_gpu, CV_GRAY2BGR);
							wat.release();
							wat2.release();
						}
						break;

					case 16:
						//Closing
						{
							gpu::GpuMat wat(edit_gpu.rows, edit_gpu.cols, CV_8UC1);
							gpu::GpuMat wat2(edit_gpu.rows, edit_gpu.cols, CV_8UC1);
							gpu::threshold(mono_gpu, mono2, 200, 255,THRESH_BINARY);
							wat = mono2;
							gpu::dilate(wat, wat2, Mat());
							gpu::erode(wat2, wat, Mat());
							gpu::cvtColor(wat, edit_gpu, CV_GRAY2BGR);
							wat.release();
							wat2.release();
						}
						break;

					case 17:
						//Mean
						{
							gpu::blur(mono_gpu, mono2, Size(7,7));
							gpu::cvtColor(mono2, edit_gpu, CV_GRAY2BGR);
						}
						break;

					case 18:
						//Mirror x
						{
							gpu::flip(edit_gpu, tmp, 1);
							tmp.copyTo(edit_gpu);
						}
						break;

					case 19:
						//Movement
						{
							gpu::GpuMat tmp2;
							gpu::cvtColor(prev_gpu, tmp2, CV_BGR2GRAY);
							gpu::subtract(mono_gpu, tmp2, tmp2);
							gpu::cvtColor(tmp2, edit_gpu, CV_GRAY2BGR);
							tmp2.release();
						}
						break;

					case 20:
						//Canny lines
						{
							gpu::Canny( mono_gpu, tmp, 50, 200, 3 );
							gpu::cvtColor(tmp, edit_gpu, CV_GRAY2BGR);
						}
						break;

					case 21:
						//Mirror y
						{
							gpu::flip(edit_gpu, tmp, -1);
							tmp.copyTo(edit_gpu);
						}
						break;

					case 22:
						//Laplacian
						{
							Mat kern(3,3,CV_8SC1,Scalar(-1));
							kern.at<signed char>(Point(1,1)) = 8;

							gpu::filter2D(mono_gpu, tmp, -1, kern, Point(-1,-1), BORDER_DEFAULT);
							gpu::cvtColor(tmp,edit_gpu, CV_GRAY2BGR);
							kern.release();
						}
						break;

					case 23:
						//Shift Colors
						{
							gpu::GpuMat channels[3];
							gpu::split(edit_gpu, channels);
							channels[1].copyTo(tmp);
							channels[2].copyTo(channels[1]);
							tmp.copyTo(channels[2]);
							gpu::merge(channels, 3, edit_gpu);
							channels[0].release();
							channels[1].release();
							channels[2].release();
						}
						break;

					case 24:
						//Remove Color
						{
							gpu::GpuMat channels[3];
							gpu::split(edit_gpu, channels);

							channels[2] = gpu::GpuMat(edit_gpu.rows, edit_gpu.cols, CV_8UC1,Scalar(0));

							gpu::merge(channels, 3, edit_gpu);
							channels[0].release();
							channels[1].release();
							channels[2].release();
						}
						break;

					case 25:
						//Rotate color
						{
							gpu::GpuMat channels[3];
							Mat t_mat;
							gpu::split(edit_gpu, channels);

							t_mat = getRotationMatrix2D(Point(edit_gpu.cols/2, edit_gpu.rows/2), 120, 1);

							gpu::warpAffine(channels[2], tmp, t_mat, channels[2].size());
							tmp.copyTo(channels[2]);
							gpu::merge(channels, 3, edit_gpu);

							t_mat.release();
							channels[0].release();
							channels[1].release();
							channels[2].release();
						}
						break;
						
					case 26:
						//Oldschool 3D
						{
							gpu::GpuMat channels[3];
							Mat t;
							Point2f a[3];
							Point2f b[3];
							a[0] = Point2f(0,0);
							a[1] = Point2f(10,0);
							a[2] = Point2f(0,10);
							b[0] = Point2f(-10,0);
							b[1] = Point2f(0,0);
							b[2] = Point2f(-10,10);
							t = getAffineTransform(a, b);

							gpu::split(edit_gpu, channels);
							
							gpu::warpAffine(channels[2], tmp, t, channels[2].size());
							tmp.copyTo(channels[2]);
							gpu::merge(channels, 3, edit_gpu);

							t.release();
							channels[0].release();
							channels[1].release();
							channels[2].release();
						}
						break;
					
					case 27:
						//Hori. Prewitt.
						{
							Mat kern(3,3,CV_8SC1,Scalar(1));
							kern.at<signed char>(Point(0,0)) = -1;
							kern.at<signed char>(Point(1,0)) = -1;
							kern.at<signed char>(Point(2,0)) = -1;
							kern.at<signed char>(Point(0,1)) = 0;
							kern.at<signed char>(Point(1,1)) = 0;
							kern.at<signed char>(Point(2,1)) = 0;

							gpu::filter2D(mono_gpu, tmp, -1, kern, Point(-1,-1), BORDER_DEFAULT);
							gpu::cvtColor(tmp,edit_gpu, CV_GRAY2BGR);
							kern.release();
						}
						break;
					
					case 28:
						//Verti. Prewitt.
						{
							Mat kern(3,3,CV_8SC1,Scalar(1));
							kern.at<signed char>(Point(0,0)) = -1;
							kern.at<signed char>(Point(0,1)) = -1;
							kern.at<signed char>(Point(0,2)) = -1;
							kern.at<signed char>(Point(1,0)) = 0;
							kern.at<signed char>(Point(1,1)) = 0;
							kern.at<signed char>(Point(1,2)) = 0;

							gpu::filter2D(mono_gpu, tmp, -1, kern, Point(-1,-1), BORDER_DEFAULT);
							gpu::cvtColor(tmp,edit_gpu, CV_GRAY2BGR);
							kern.release();
						}
						break;

					case 29:
						//Sharpen
						{
							Mat kern(3,3,CV_8SC1,Scalar(0));
							kern.at<signed char>(Point(1,0)) = -1;
							kern.at<signed char>(Point(0,1)) = -1;
							kern.at<signed char>(Point(2,1)) = -1;
							kern.at<signed char>(Point(1,2)) = -1;
							kern.at<signed char>(Point(1,1)) = 5;

							gpu::filter2D(mono_gpu, tmp, -1, kern, Point(-1,-1), BORDER_DEFAULT);
							gpu::cvtColor(tmp,edit_gpu, CV_GRAY2BGR);
							kern.release();
						}
						break;

					case 30:
						//Emboss
						{
							Mat kern(3,3,CV_8SC1,Scalar(0));
							kern.at<signed char>(Point(0,0)) = -2;
							kern.at<signed char>(Point(1,0)) = -1;
							kern.at<signed char>(Point(0,1)) = -1;
							kern.at<signed char>(Point(1,1)) = 1;
							kern.at<signed char>(Point(2,1)) = 1;
							kern.at<signed char>(Point(1,2)) = 1;
							kern.at<signed char>(Point(2,2)) = 2;

							gpu::filter2D(mono_gpu, tmp, -1, kern, Point(-1,-1), BORDER_DEFAULT);
							gpu::cvtColor(tmp,edit_gpu, CV_GRAY2BGR);
							kern.release();
						}
						break;
				}
				tmp.release();
				mono2.release();
			}
		}
		src_gpu.copyTo(prev_gpu);
		

		final_gpu.download(display_img);

		//Add text
		im=0;
		for(int fy = 0; fy < 5; fy++)
		{
			for(int fx = 0; fx < 6; fx++)
			{
				im++;
				string text;
				switch(im)
				{
					case 1:
						//invert
						text = "Invert";
						break;

					case 2:
						//Brightness
						text = "Brightness";
						break;

					case 3:
						//Contrast
						text = "Contrast";
						break;

					case 4:
						//Gamma
						text = "Gamma";
						break;

					case 5:
						//Alpha blend
						text = "Alpha blend";
						break;

					case 6:
						//Eq. Histogram
						text = "Eq. Histogram";
						break;

					case 7:
						//Gray
						text = "Gray";
						break;

					case 8:
						//Binary
						text = "Binary";
						break;

					case 9:
						//Sobel
						text = "Sobel";
						break;

					case 10:
						//Gaussian
						text = "Gaussian";
						break;

					case 11:
						//Crop
						text = "Crop";
						break;

					case 12:
						//Rotate
						text = "Rotate";
						break;

					case 13:
						//Eroding
						text = "Eroding";
						break;
						
					case 14:
						//Dilation
						text = "Dilation";
						break;
						
					case 15:
						//Opening
						text = "Opening";
						break;

					case 16:
						//Closing
						text = "Closing";
						break;

					case 17:
						//Mean
						text = "Mean";
						break;

					case 18:
						//Mirror x
						text = "Mirror x";
						break;

					case 19:
						//Movement
						text = "Movement";
						break;

					case 20:
						//Canny lines
						text = "Canny lines";
						break;

					case 21:
						//Mirror y
						text = "Mirror y";
						break;

					case 22:
						//Laplacian
						text = "Laplacian";
						break;

					case 23:
						//Shift Colors
						text = "Shift Colors";
						break;

					case 24:
						//Remove Color
						text = "Remove Color";
						break;

					case 25:
						//Rotate color
						text = "Rotate color";
						break;

					case 26:
						//Oldschool 3D
						text = "Oldschool 3D";
						break;

					case 27:
						//Hori. Prewitt.
						text = "Hori. Prewitt";
						break;
					
					case 28:
						//Verti. Prewitt.
						text = "Verti. Prewitt";
						break;

					case 29:
						//Sharpen
						text = "Sharpen";
						break;

					case 30:
						//Emboss
						text = "Emboss";
						break;
				}
				putText(display_img, text, Point((fx) * w,((fy+1)*h)-1),FONT_HERSHEY_COMPLEX_SMALL, 0.8,Scalar(0,0,255),1,CV_AA);
			}
		}
		imshow("play", display_img);

		//Check for 'ESC'
		c = waitKey(1);
		if(c==27)break;
	}
}

