#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <sstream>
#include <ctime>
#include <thread>
#include <sys/stat.h>
#include "FaceRecognitionModule.h"


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/core.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include "opencv2/contrib/contrib.hpp"

using namespace std;
using namespace cv;

void faceCapture()
{
	CvCapture *capture;
	IplImage *frame;

	capture = cvCaptureFromCAM(0);
	cvNamedWindow("Webcam", 0);
	while (true)
	{
		frame = cvQueryFrame(capture);

		/*Point upperMiddle(frame->width / 2, 0);
		Point lowwerMiddle(frame->width / 2, frame->height);
		cvLine(frame, upperMiddle, lowwerMiddle, CV_RGB(0, 255, 0), 1);

		Point chinLineLeft(0, frame->height - (frame->height / 6));
		Point chinLineRight(frame->width, frame->height - (frame->height / 6));
		cvLine(frame, chinLineLeft, chinLineRight, CV_RGB(0, 255, 0), 2);

		Point rightEyePointOne((frame->width / 2) - frame->width / 5, (frame->height / 10) * 3);
		Point rightEyePointTwo(frame->width / 2, (frame->height / 10) * 5);
		cvRectangle(frame, rightEyePointOne, rightEyePointTwo, CV_RGB(0, 255, 0), 2);

		Point leftEyePointOne(frame->width / 2, (frame->height / 10) * 3);
		Point leftEyePointTwo((frame->width / 2) + frame->width / 5, (frame->height / 10) * 5);
		cvRectangle(frame, leftEyePointOne, leftEyePointTwo, CV_RGB(0, 255, 0), 2);*/

		cvShowImage("Webcam", frame);

		if (cvWaitKey(1) >= 0){
			break;
		}
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("Webcam");
}


void CollectFaces()
{
	int NUM = 8;
	int count = 0;
	string createName = "";
	string command;
	char filename[] = "person/face.txt";
	wchar_t* _folderName = L"person";
	int id = 0;
	string path;
	string floderName;

	cout << "input person name: ";
	getline(cin, createName);

	struct stat info;
	if (stat(filename, &info) != 0)
	{
		//CreateDirectory(_folderName, NULL);
		ofstream outFile;
		outFile.open(filename);
	}


	std::fstream fpp;
	fpp.open(filename, std::ios::in);
	if (!fpp)
	{
		std::cout << "Fail to open file: " << filename << std::endl;
		fpp.open(filename, ios::out);
		fpp.open(filename, std::ios::in);
	}

	int tempId = 0;
	string tempPath;

	/*while (!fpp.eof())
	{
		fpp >> tempId;
		fpp >> tempPath;
	}*/

	fpp.close();


	floderName = format("%s", createName.c_str());
	command = "mkdir person\\faces\\" + floderName;
	std::system(command.c_str());


	CascadeClassifier haar_cascade;

	//haar_cascade.load(fn_haar);

	// Get a handle to the Video device:
	VideoCapture cap(0);

	// Check if we can use this device at all:
	if (!cap.isOpened())
	{
		std::cerr << "Capture Device cannot be opened." << std::endl;
		std::system("pause");
		return;
	}

	Mat frame;
	count = 0;

	while (count < NUM) 
	{
		cap >> frame;
		Mat original = frame.clone();
		Mat gray;
		cvtColor(original, gray, CV_BGR2GRAY);
		vector< Rect_<int> > faces;

		//haar_cascade.detectMultiScale(gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(5, 5));

  		CvScalar FontColor = CV_RGB(255, 0, 0);
		CvFont font;
		cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 1, 2, 8);

		//for (int i = 0; i < faces.size(); i++)
		//{
		
			Mat face = cvCreateImage(cvSize(100,100),16,1);
			Mat face_resized;
			Mat face_save;
			cv::resize(face, face_resized, cv::Size(400, 400), 1.0, 1.0, INTER_CUBIC);
			cv::resize(face, face_save, cv::Size(100, 100), 1.0, 1.0, INTER_CUBIC);
			/*IplImage *show = cvCloneImage(&(IplImage)face_resized);
			std::string picName = format("Picture %d", count + 1);
			cvPutText(show, picName.c_str(), cvPoint(0, 20), &font, CV_RGB(0, 255, 0));
			cvPutText(show, "Press Space to save,", cvPoint(0, 360), &font, FontColor);
			cvPutText(show, "the other keys to continue", cvPoint(0, 380), &font, FontColor);
			cv::imshow("Face", cvarrToMat(show));

			cvReleaseImage(&show);*/
			char key = (char)cvWaitKey(0);
			printf("%c\n",key);
			if (key == 0x5e)
			{
				string name = "person/faces/" + floderName + format("/%d.jpg", count++);
				imwrite(name.c_str(), original);
				break;
			}
			//else
			//{
			//	break;
			//}
		//}

		/*char key = (char)waitKey(20);

		if (key == 27)
		{
			break;
		}*/
	}

	cvDestroyWindow("Face");
	std::fstream fp;
	fp.open(filename, std::ios::in);

	if (!fp)
	{
		std::cout << "Fail to open file: " << filename << std::endl;
	}

	while (!fp.eof())
	{
		fp >> id;
		fp >> path;
	}

	fp.close();
	fp.open(filename, std::ios::app);
	id++;
	path = format("\n%d ", id) + "person/faces/" + floderName + "/";
	fp << path;
	fp.close();
	////write ID to Name Mapping File
	char IDfilename[] = "person/id2name.txt";
	std::fstream IDFstream;
	IDFstream.open(IDfilename, std::ios::app);
	string IDPath;
	IDPath = format("\n%d ", id) + createName;
	IDFstream << IDPath;
	IDFstream.close();
}

void readImage(string fileName, vector<Mat> &images, vector<int> &labels, int id)
{
	int count = 0;
	while (count < 40)
	{
		string name = fileName + format("%d.jpg", count++);
		std::ifstream file(name.c_str(), ifstream::in);
		if (!file)
		{
			//cerr << "Error opening file \"" << name << endl;
		}
		else
		{
			images.push_back(imread(name.c_str(), CV_LOAD_IMAGE_GRAYSCALE));
			labels.push_back(id);
		}
	}
}

void training()
{
	vector<Mat> images;
	vector<int> labels;
	int count = 0;
	int id;
	string path;
	char filename[] = "person/face.txt";
	std::fstream fp;
	fp.open(filename, std::ios::in);

	if (!fp)
	{
		std::cout << "Fail to open file: " << filename << std::endl;
	}

	while (!fp.eof())
	{
		fp >> id;
		fp >> path;
		readImage(path, images, labels, id);
	}

	fp.close();
	Ptr<FaceRecognizer> FisherTrainingModel = createLBPHFaceRecognizer();//createFisherFaceRecognizer();// createEigenFaceRecognizer();
	time_t start, end, time;
	start = std::clock();
	std::cout << "Start Fisher Face Training\n";
	FisherTrainingModel->train(images, labels);
	FisherTrainingModel->save("person/fisherface.xml");
	std::cout << "Fisher Face Training is over!!!\n";
	cout << " Face Training is over";
	end = clock();
	time = end - start;
	std::cout << "Used Time : " << time << " ms\n";
}


void FaceRecognition()
{
	Mat img;
	CascadeClassifier haar_cascade;
	//haar_cascade.load(fn_haar);
	int errorCount = 0;

	char filename[] = "person/id2name.txt";
	std::fstream fp;
	fp.open(filename, std::ios::in);
	int id;
	string name;
	vector<string> nameMap;
	if (!fp)
	{
		std::cout << "Fail to open id2name.txt" << std::endl;
	}
	while (!fp.eof())
	{
		fp >> id;
		fp >> name;
		nameMap.push_back(name);
	}

	Ptr<FaceRecognizer> *FisherModel;
	FisherModel = new Ptr<FaceRecognizer>();
	(*FisherModel) = createLBPHFaceRecognizer();// createEigenFaceRecognizer();
	//(*FisherModel) = createLBPHFaceRecognizer();
	(*FisherModel)->set("threshold", 1500.0);
	(*FisherModel)->load("person/fisherface.xml");

	CvCapture *capture;
	while (1)
	{
		//cvWaitKey(30);
		//cv::Mat *cvtImg;
		//cvtImg = new cv::Mat();
		//*cvtImg = cv::Mat(cv::Size(640, 480), CV_8UC2);
		//img = *cvtImg;
		//imshow ("face recogition" , img);
		//cvWaitKey (1);
		//Mat original = img.clone();
		// Convert the current frame to grayscale:
		Mat gray;
		IplImage *frame;
		capture = cvCaptureFromCAM(1);
		frame = cvQueryFrame(capture);
		Mat original(frame, 0);
		cvtColor(original, gray, CV_BGR2GRAY);
		//cout << "correct" << endl;
		vector< Rect_<int> > faces;
		Rect *lastFace = NULL;
		//haar_cascade.detectMultiScale(gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(5, 5));
		int fisherPredicted = -1;
		double fisherConfidence = 0.0;
		int eigenPredicted = -1;
		double eigenConfidence = 0.0;
		//if (faces.size() != 0)
		//{
			for (int i = 0; i < 1; i++)
			{
				//*lastFace = faces[i];
				//Rect face_i = faces[i];
				//Mat face = gray(face_i);
				cv::imshow("Face", original);
				string name = "person/capimg.jpg";
				imwrite(name.c_str(), original);
				Mat face_resized;
				cv::resize(gray, face_resized, cv::Size(100, 100), 1.0, 1.0, INTER_CUBIC);
				(*FisherModel)->predict(face_resized, fisherPredicted, fisherConfidence);
				string predictedName;
				//std::cout << "fisher face predicted = " << fisherPredicted << ", confidence = " << fisherConfidence << std::endl;
				if (fisherPredicted != -1)
				{
					predictedName = nameMap[fisherPredicted - 1];
					std::cout << "fisher face predicted = " << predictedName << ", confidence = " << fisherConfidence << std::endl;
					char fishname[] = "person/fishname.txt";
					std::fstream stream;
					stream.open(fishname, std::ios::out);
					string DPath;
					DPath = predictedName;
					stream << DPath;
					stream.close();
				}
				errorCount++;
				cvWaitKey(5);
			}


		//}
		/*else if (faces.size() == 0 && lastFace != NULL)
		{
			Mat faceHSV;
			cv::Rect mean_shift_window = *lastFace;
			Mat hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
			float hranges[] = { 0, 180 };
			const float* phranges = hranges;
			Rect trackWindow;
			int hsize = 16;


			cvtColor(original, faceHSV, COLOR_BGR2HSV);


			int _vmin = 10, _vmax = 256;

			inRange(faceHSV, Scalar(0, 30, MIN(_vmin, _vmax)),
				Scalar(180, 256, MAX(_vmin, _vmax)), mask);
			int ch[] = { 0, 0 };
			hue.create(faceHSV.size(), faceHSV.depth());
			mixChannels(&faceHSV, 1, &hue, 1, ch, 1);


			Mat roi(hue, mean_shift_window), maskroi(mask, mean_shift_window);
			calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
			normalize(hist, hist, 0, 255, NORM_MINMAX);

			trackWindow = mean_shift_window;

			histimg = Scalar::all(0);
			int binW = histimg.cols / hsize;
			Mat buf(1, hsize, CV_8UC3);
			for (int i = 0; i < hsize; i++)
				buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180. / hsize), 255, 255);
			cvtColor(buf, buf, COLOR_HSV2BGR);

			for (int i = 0; i < hsize; i++)
			{
				int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows / 255);
				rectangle(histimg, Point(i*binW, histimg.rows),
					Point((i + 1)*binW, histimg.rows - val),
					Scalar(buf.at<Vec3b>(i)), -1, 8);
			}

			calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
			backproj &= mask;
			RotatedRect trackBox = CamShift(backproj, trackWindow,
				TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));
			if (trackWindow.area() <= 1)
			{
				int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5) / 6;
				trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
					trackWindow.x + r, trackWindow.y + r) &
					Rect(0, 0, cols, rows);
			}
		}*/

		cvReleaseImage(&frame);
	}
	cvReleaseCapture(&capture);
}

int main()
{
	thread mThread(faceCapture);
	//CollectFaces();
	//training();
	FaceRecognition();
	printf("hello");
	system("pause");
	return 0;
}