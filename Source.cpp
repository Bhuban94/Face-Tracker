#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include "Tserial.h"
#include"Tserial.cpp"

using namespace std;
using namespace cv;

void detectAndDisplay(Mat frame);

String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Capture";

int arduino_command;
Tserial *arduino_com;
short MSBLSB = 0;
unsigned char MSB = 0;
unsigned char LSB = 0;

int main(int argc, const char** argv)
{
	CvCapture* capture;
	Mat frame;

	arduino_com = new Tserial();
	if (arduino_com != 0) {
		arduino_com->connect("\\\\.\\COM37", 57600, spNONE);
	}

	if (!face_cascade.load(face_cascade_name)){ printf("Error loading\n"); return -1; };
	if (!eyes_cascade.load(eyes_cascade_name)){ printf("Error loading\n"); return -1; };
	capture = cvCaptureFromCAM(0);
	if (capture)
	{
		while (true)
		{
			frame = cvQueryFrame(capture);
			if (!frame.empty())
			{
				detectAndDisplay(frame);
			}
			else
			{
				printf("No captured frame"); break;
			}

			int c = waitKey(10);
			if ((char)c == 'c') { break; }
		}
	}
	arduino_com->disconnect();
	delete arduino_com;
	arduino_com = 0;
	return 0;
}
void detectAndDisplay(Mat frame)
{

	std::vector<Rect> faces;
	Mat frame_gray;
	Mat frame_resized;
	resize(frame, frame_resized, Size(320, 240), 1.0, 1.0, INTER_CUBIC);
	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	resize(frame_gray, frame_gray, Size(320, 240), 1.0, 1.0, INTER_CUBIC);
	equalizeHist(frame_gray, frame_gray);
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(70, 70));

	for (int i = 0; i < faces.size(); i++)
	{
		cv::rectangle(frame_resized, faces[i], CV_RGB(0, 255, 0), 1);
		  cout << "x:" << faces[i].x  <<  "  y:" << faces[i].y  << endl; 
		LSB = faces[i].x & 0xff;
		MSB = (faces[i].x >> 8) & 0xff;
		arduino_com->sendChar(MSB);
		arduino_com->sendChar(LSB);

		LSB = faces[i].y & 0xff;
		MSB = (faces[i].y >> 8) & 0xff;
		arduino_com->sendChar(MSB);
		arduino_com->sendChar(LSB);

		Mat faceROI = frame_gray(faces[i]);
		std::vector<Rect> eyes;
	}

	imshow(window_name, frame_resized);

}