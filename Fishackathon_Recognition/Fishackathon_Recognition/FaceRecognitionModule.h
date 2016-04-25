#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

#include <Windows.h>
//#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <thread>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/contrib/contrib.hpp>

#include "tinyxml2.h"

using namespace std;
using namespace cv;

class FaceRecognitionModule
{
private:
	void readImage(string fileName, vector<Mat> &images, vector<int> &labels, int id);
	void refreshFileDirectory();
	bool deleteFeatureXMLContent(string name);
	std::string myreplace(std::string &s, std::string toReplace, std::string replaceWith);
	void removeCharsFromString(string &str, char* charsToRemove);
	map<string, int> parseFaceCount(string input);

	string _baseDirectoryFolderName;
	string _FaceDirectoryMappingFileName;
	string _id2NameMappingFileName;
	string _featureFileName;

	string _fn_haar; // harr like file directory
	string _faceImageDBDir; //DB main folder directory
	string _faceImageIndexFileName; //DB index mapping file name (include main folder directory)
	string _id2nameFileName; //DB id2name file name (include main folder directory)
	string _mkdirDirString; //Faces folder name (mkdir format)(include main folder directory)
	string _personFaceDir; //Faces folder name (include main folder directory)
	string _featureXMLDataDir; //output feature file name

	int _predictProbability;
	int _maxFaceCount;
	Ptr<FaceRecognizer>* _faceRecognizer;
	vector<string> _nameMap;

public:
	//constructer
	//need to set harr like path when constructed
	//you can change harr like path with the function setHaarLikePath(string);
	FaceRecognitionModule(string harrLikePath);

	~FaceRecognitionModule();

	//chage harr like path when needed
	void setHaarLikePath(string path);

	//intput a camera frame and this method will return a vector list of faces inside this camera frame
	//Usually you will get only one object in the list, but if the size is not equale to one
	//then just simply change another input frame for better result
	//Make sure if the return list size is greater than one, the return face mat could't be ensure is a face
	vector<Mat> getFaceROI(Mat inputFrame);

	//input a face image(you can get it by calling getFaceROI(Mat))
	//and the name with this face.
	//If something went wrong, this method will return false
	bool addFaceToDatabase(vector<Mat> inputFaces, string nameLabel);
	//bool addFaceToDatabase(std::map<std::string, std::vector<cv::Mat*> >& training_face_images);

	//train the face database, this methid will generate a feature xml file
	//If something went wrong, this method will return false
	bool trainFaceDatabase();//need to setHaarLikePath first

	//calling this method can ensure that if the inner module can execute predict method
	//please call loadFaceDB() first
	bool canPredict(string name = "");

	//load the face database into memory
	//you should call this before calling predict(Mat)
	bool loadFaceDB();

	//input a face image(you can get it by calling getFaceROI(Mat))
	//face image need to be grayscale
	//this methos will return the cloest face name
	//if the feature distance is too far, this method will return unknown
	//
	//call loadFaceDB() before calling this method
	string predict(Mat inputFace);

	//get the predict value for every predict
	int getPredictProb();

	//set the max face number for a single person
	//Notice that this value need to be consistent in the entire Database
	//This value will be set to 8 when the constructer is called
	void setMaxFaceCount(int value);

	//set base directory folder name
	//this folder name will be the face database base folder name
	//Should call this method and set the values immediately after calling class constructer
	//or else this class will use the default folder and file names
	void setBaseDirectoryFolderName(string folderName);

	//set face directory mapping file name
	//File name should include .txt
	//Should call this method and set the values immediately after calling class constructer
	//or else this class will use the default folder and file names
	void setFaceDirectoryMappingFileName(string fileName);

	//set id2name file name
	//File name should include .txt
	//Should call this method and set the values immediately after calling class constructer
	//or else this class will use the default folder and file names
	void setIdToNameMappingFileName(string fileName);

	//set output feature file name
	//File name should include .xml
	//Should call this method and set the values immediately after calling class constructer
	//or else this class will use the default folder and file names
	void setFeatureFileName(string fileName);

	vector<string> getFaceNmaeList();
	bool isNameExistInNameList(string searchName);
	bool deleteFacefromDB(string name);
};

