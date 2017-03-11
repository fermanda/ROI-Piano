#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "SFML/Audio.hpp"
#include <cv.h>
#include <iostream>

using namespace cv;
using namespace std;
using namespace sf;
	
int main()
{
	VideoCapture myCam(0);
	Mat frame, gframe;
	Mat iopen, bnorm, bgray;
	Mat out, out1, out2, out3, out4, out5;
	Mat bw, bw1, bw2, bw3, bw4, bw5;

	int flags1 = 0,
	    flags2 = 0,
	    flags3 = 0,
	    flags4 = 0,
	    flags5 = 0;

	int thresh = 65;
	int count = 20;

	SoundBuffer buffer1, buffer2, buffer3, buffer4, buffer5;
	Sound audio1, audio2, audio3, audio4, audio5;
	Time t = seconds(1);

	buffer1.loadFromFile("c.wav");
	audio1.setBuffer(buffer1);
	buffer2.loadFromFile("d.wav");
	audio2.setBuffer(buffer2);
	buffer3.loadFromFile("e.wav");
	audio3.setBuffer(buffer3);
	buffer4.loadFromFile("f.wav");
	audio4.setBuffer(buffer4);
	buffer5.loadFromFile("g.wav");
	audio5.setBuffer(buffer5);


	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);

	vector<Mat> channels;

	string imagename;
	CvScalar green = CV_RGB(0,255,0);
	CvScalar white = CV_RGB(255,255,255);

	/*
	Region Of Interest
	membuat static region of interest berbentuk segi-4
	Code => Rect rect(x,y,width,height)
	*/
	Rect roi1(40,40,80,400); 
	Rect roi2(160,40,80,400);
	Rect roi3(280,40,80,400);
	Rect roi4(400,40,80,400);
	Rect roi5(520,40,80,400);

	imagename = "/home/brocolly/background.jpg"; //Menaruh link background disimpan
	iopen = imread(imagename, CV_LOAD_IMAGE_COLOR); //Membuka gambar
	cv::normalize(iopen, bnorm, 0, 255, NORM_MINMAX, -1, noArray());
	cv::cvtColor(bnorm, bgray, CV_BGR2GRAY);
	//equalizeHist( src, dst );
	equalizeHist(bgray, bgray);

	std::cout << "Press c to capture background image" << std::endl;
	
	while (true)
	{
		//Buffering gambar webcam
		myCam >> frame;
		cv::cvtColor(frame, gframe, CV_BGR2GRAY);
		equalizeHist(gframe, gframe);

		if (cvWaitKey(10) == 99)
		{
			imagename = "/home/brocolly/background.jpg";
			imwrite(imagename, frame, compression_params);
			iopen = imread(imagename, CV_LOAD_IMAGE_COLOR);
			cv::normalize(iopen, bnorm, 0, 255, NORM_MINMAX, -1, noArray());
			cv::cvtColor(bnorm, bgray, CV_BGR2GRAY);
			equalizeHist(bgray, bgray);			
			imshow("Background", iopen);
			flags1 = 0, flags2 = 0, flags3 = 0, flags4 = 0, flags5 = 0;
		}

		//Memasang ROI pada frame gambar yang sedang ditangkap
		rectangle(frame, roi1, green, 1, 8, 0);
		rectangle(frame, roi2, green, 1, 8, 0);
		rectangle(frame, roi3, green, 1, 8, 0);
		rectangle(frame, roi4, green, 1, 8, 0);
		rectangle(frame, roi5, green, 1, 8, 0);

		rectangle(gframe, roi1, green, 1, 8, 0);
		rectangle(gframe, roi2, green, 1, 8, 0);
		rectangle(gframe, roi3, green, 1, 8, 0);
		rectangle(gframe, roi4, green, 1, 8, 0);
		rectangle(gframe, roi5, green, 1, 8, 0);

		rectangle(bgray, roi1, green, 1, 8, 0);
		rectangle(bgray, roi2, green, 1, 8, 0);
		rectangle(bgray, roi3, green, 1, 8, 0);
		rectangle(bgray, roi4, green, 1, 8, 0);
		rectangle(bgray, roi5, green, 1, 8, 0);

		//Mencari absolute differnce pada video dan background
		cv::absdiff(gframe(roi1), bgray(roi1), out1);
		cv::absdiff(gframe(roi2), bgray(roi2), out2);
		cv::absdiff(gframe(roi3), bgray(roi3), out3);
		cv::absdiff(gframe(roi4), bgray(roi4), out4);
		cv::absdiff(gframe(roi5), bgray(roi5), out5);
		cv::absdiff(gframe, bgray, out);

		//Mencari binary value dari absolute difference
		cv::threshold(out1, bw1, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
		cv::threshold(out2, bw2, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
		cv::threshold(out3, bw3, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
		cv::threshold(out4, bw4, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
		cv::threshold(out5, bw5, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
		cv::threshold(out, bw, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

		imshow("VideoCam", frame);
		imshow("AbsDif", bw);

		split(bw1, channels);
		Scalar sbw1 = mean(bw1);
		split(bw2, channels);
		Scalar sbw2 = mean(bw2);
		split(bw3, channels);
		Scalar sbw3 = mean(bw3);
		split(bw4, channels);
		Scalar sbw4 = mean(bw4);
		split(bw5, channels);
		Scalar sbw5 = mean(bw5);

		cout << "rata-rata 1:" << sbw1[0] << "	flags1: " << flags1 << endl;
		cout << "rata-rata 2:" << sbw2[0] << "	flags2: " << flags2 << endl;
		cout << "rata-rata 3:" << sbw3[0] << "	flags3: " << flags3 << endl;
		cout << "rata-rata 4:" << sbw4[0] << "	flags4: " << flags4 << endl;
		cout << "rata-rata 5:" << sbw5[0] << "	flags5: " << flags5 << endl;
		cout << "================================" << endl;

		if (sbw1[0] > thresh) {
			if (flags1 == 0) { audio1.play(); flags1++; }
			if (flags1 > 0) flags1++;
			if (flags1 == count) flags1 = 0;
		}
		if (sbw2[0] > thresh) {
			if (flags2 == 0) { audio2.play(); flags2++; }
			if (flags2 > 0) flags2++;
			if (flags2 == count) flags2 = 0;
		}
		if (sbw3[0] > thresh) {
			if (flags3 == 0) { audio3.play(); flags3++; }
			if (flags3 > 0) flags3++;
			if (flags3 == count) flags3 = 0;
		}
		if (sbw4[0] > thresh) {
			if (flags4 == 0) { audio4.play(); flags4++; }
			if (flags4 > 0) flags4++;
			if (flags4 == count) flags4 = 0;
		}
		if (sbw5[0] > thresh) {
			if (flags5 == 0) { audio5.play(); flags5++; }
			if (flags5 > 0) flags5++;
			if (flags5 == count) flags5 = 0;
		}

		if (cvWaitKey(5) == 27) break;
	}
	return 0;
}

//export LD_LIBRARY_PATH=/home/brocolly/SFML-2.2/lib/ && ./testaudio
//g++ gambar.cpp `pkg-config --cflags --libs opencv` -I /home/brocolly/SFML-2.2/include/ -L /home/brocolly/SFML-2.2/lib/ -lsfml-audio -lsfml-system -o video