#include "FaceRecognitionModule.h"


FaceRecognitionModule::FaceRecognitionModule(string harrLikePath)
{
	_fn_haar = harrLikePath;
	_predictProbability = 999;
	_faceRecognizer = new Ptr<FaceRecognizer>();
	(*_faceRecognizer) = createLBPHFaceRecognizer();
	_maxFaceCount = 8;

	//
	_faceImageDBDir = "person";
	_faceImageIndexFileName = "person/face.txt";
	_id2nameFileName = "person/id2name.txt";
	_mkdirDirString = "mkdir person/faces/";
	_personFaceDir = "person/faces/";
	_featureXMLDataDir = "person/fisherface.xml";

	loadFaceDB();
}


FaceRecognitionModule::~FaceRecognitionModule()
{
}

void FaceRecognitionModule::setMaxFaceCount(int value)
{
	_maxFaceCount = value;
}

void FaceRecognitionModule::readImage(string fileName, vector<Mat> &images, vector<int> &labels, int id)
{
	int count = 0;

	while (count < _maxFaceCount)
	{
		string name = fileName + format("%d.jpg", count++);
		std::ifstream file(name.c_str(), ifstream::in);

		if (file)
		{
			images.push_back(imread(name.c_str(), CV_LOAD_IMAGE_GRAYSCALE));
			labels.push_back(id);
		}
	}
}

void FaceRecognitionModule::setHaarLikePath(string path)
{
	_fn_haar = path;
}

vector<Mat> FaceRecognitionModule::getFaceROI(Mat inputFrame)
{
	vector<Mat> returnROIs;
	CascadeClassifier haar_cascade;
	if (!_fn_haar.length() == 0)
	{
		haar_cascade.load(_fn_haar);
	}
	Mat gray;
	cvtColor(inputFrame, gray, CV_BGR2GRAY);
	vector< Rect_<int> > faces;
	haar_cascade.detectMultiScale(gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(5, 5));

	for (int i = 0; i < faces.size(); i++)
	{
		Rect face_i = faces[i];
		Mat face = inputFrame(face_i);
		Mat face_resized;
		Mat face_save;
		cv::resize(face, face_resized, cv::Size(400, 400), 1.0, 1.0, INTER_CUBIC);
		cv::resize(face, face_save, cv::Size(100, 100), 1.0, 1.0, INTER_CUBIC);
		returnROIs.push_back(face_resized);
	}
	return returnROIs;
}

bool FaceRecognitionModule::addFaceToDatabase(vector<Mat> inputFaces, string nameLabel)
//bool FaceRecognitionModule::addFaceToDatabase(std::map<std::string, std::vector<cv::Mat*> >& training_face_images)
{
	string command;

	int id = 0;
	string path;
	string floderName;

	struct stat info;
	if (stat(_faceImageIndexFileName.c_str(), &info) != 0)
	{
		//mkdir(_faceImageDBDir.c_str(), 0700);
		ofstream outFile;
		outFile.open(_faceImageIndexFileName.c_str());
	}

	std::fstream fpp;
	fpp.open(_faceImageIndexFileName.c_str(), std::ios::in);
	if (!fpp)
	{
		//std::cout << "Fail to open file: " << _faceImageIndexFileName.c_str() << std::endl;
		fpp.open(_faceImageIndexFileName.c_str(), ios::out);
		fpp.open(_faceImageIndexFileName.c_str(), std::ios::in);
	}

	int tempId = 0;
	string tempPath;

	while (!fpp.eof())
	{
		fpp >> tempId;
		fpp >> tempPath;
	}

	fpp.close();


	floderName = format("%s", nameLabel.c_str());
	command = _mkdirDirString + floderName;
	std::system(command.c_str());

	int count = 0;
	for (int index = 0; index < inputFaces.size(); index++)
	{
		string name = "person/faces/" + floderName + format("/%d.jpg", count++);
		imwrite(name.c_str(), inputFaces.at(index));
	}


	std::fstream fp;
	fp.open(_faceImageIndexFileName.c_str(), std::ios::in);

	if (!fp)
	{
		//std::cout << "Fail to open file: " << _faceImageIndexFileName.c_str() << std::endl;
		return false;
	}

	while (!fp.eof())
	{
		fp >> id;
		fp >> path;
	}

	fp.close();
	fp.open(_faceImageIndexFileName.c_str(), std::ios::app);
	id++;
	path = format("\n%d ", id) + _personFaceDir + floderName + "/";
	fp << path;
	fp.close();
	////write ID to Name Mapping File
	std::fstream IDFstream;
	IDFstream.open(_id2nameFileName.c_str(), std::ios::app);
	string IDPath;
	IDPath = format("\n%d ", id) + nameLabel;
	IDFstream << IDPath;
	IDFstream.close();
	return true;
}

bool FaceRecognitionModule::trainFaceDatabase()
{
	vector<Mat> images;
	vector<int> labels;
	int id;
	string path;
	std::fstream fp;
	fp.open(_faceImageIndexFileName.c_str(), std::ios::in);

	if (!fp)
	{
		//std::cout << "Fail to open file: " << _faceImageIndexFileName.c_str() << std::endl;
		return false;
	}

	while (!fp.eof())
	{
		fp >> id;
		fp >> path;
		readImage(path, images, labels, id);
	}

	fp.close();
	(*_faceRecognizer)->train(images, labels);
	(*_faceRecognizer)->save(_featureXMLDataDir.c_str());
	return true;
}

bool FaceRecognitionModule::canPredict(string name)
{
	//   int peopleCounter = 0;
	//std::fstream fp;
	//   fp.open(_faceImageIndexFileName.c_str(), std::ios::in);

	//if (!fp)
	//{
	//       //std::cout << "Fail to open file: " << _faceImageIndexFileName.c_str() << std::endl;
	//	return false;
	//}
	//int id;
	//string name;
	//while (!fp.eof())
	//{
	//	fp >> id;
	//	fp >> name;
	//	peopleCounter++;
	//}
	//if (peopleCounter < 1)
	//{
	//	return false;
	//}
	//return true;
	if (_nameMap.size() < 1)
	{
		return false;
	}
	if (name.length() != 0)
	{
		for (int index = 0; index < _nameMap.size(); index++)
		{
			if (_nameMap.at(index) == "name")
			{
				return false;
			}
		}
		return true;
	}
	return true;
}

bool FaceRecognitionModule::loadFaceDB()
{
	(*_faceRecognizer)->set("threshold", 1500.0);
	(*_faceRecognizer)->load(_featureXMLDataDir.c_str());

	std::fstream fp;
	fp.open(_id2nameFileName.c_str(), std::ios::in);
	int id;
	string name;
	if (!fp)
	{
		//std::cout << "Fail to open id2name.txt" << std::endl;
		return false;
	}
	while (!fp.eof())
	{
		fp >> id;
		fp >> name;
		_nameMap.push_back(name);
	}
}

string FaceRecognitionModule::predict(Mat inputFace)
{
	if (!canPredict())
	{
		return "";
	}
	int predicted = -1;
	double confidence = 0.0;
	(*_faceRecognizer)->predict(inputFace, predicted, confidence);
	string predictedName;
	if (predicted != -1)
	{
		predictedName = _nameMap[predicted - 1];
		_predictProbability = (int)confidence;
	}
	return predictedName;
}

int FaceRecognitionModule::getPredictProb()
{
	if (_predictProbability != 999)
	{
		return _predictProbability;
	}
	else
	{
		return 999;
	}
}

void FaceRecognitionModule::setBaseDirectoryFolderName(String folderName)
{
	_baseDirectoryFolderName = folderName;
	refreshFileDirectory();
}

void FaceRecognitionModule::setFaceDirectoryMappingFileName(String fileName)
{
	_FaceDirectoryMappingFileName = fileName;
	refreshFileDirectory();
}

void FaceRecognitionModule::setIdToNameMappingFileName(string fileName)
{
	_id2NameMappingFileName = fileName;
	refreshFileDirectory();
}

void FaceRecognitionModule::setFeatureFileName(string fileName)
{
	_featureFileName = fileName;
	refreshFileDirectory();
}

void FaceRecognitionModule::refreshFileDirectory()
{
	_faceImageDBDir = _baseDirectoryFolderName;
	_faceImageIndexFileName = _baseDirectoryFolderName + "/" + _FaceDirectoryMappingFileName;
	_id2nameFileName = _baseDirectoryFolderName + "/" + _id2NameMappingFileName;
	_mkdirDirString = "mkdir " + _baseDirectoryFolderName + "\\faces\\";
	_personFaceDir = _baseDirectoryFolderName + "/faces/";
	_featureXMLDataDir = _baseDirectoryFolderName + "/" + _featureFileName;
}

vector<string> FaceRecognitionModule::getFaceNmaeList()
{
	if (_nameMap.size() == 0)
	{
		loadFaceDB();
	}
	return _nameMap;
}

bool FaceRecognitionModule::isNameExistInNameList(string searchName)
{
	if (_nameMap.size() == 0)
	{
		return false;
	}
	for (int index = 0; index < _nameMap.size(); index++)
	{
		if (_nameMap.at(index) == searchName)
		{
			return true;
		}
	}
	return false;
}

bool FaceRecognitionModule::deleteFacefromDB(string name)
{
	if (_nameMap.size() > 0)
	{
		deleteFeatureXMLContent(name);


	}
	return false;
}

bool FaceRecognitionModule::deleteFeatureXMLContent(string name)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("person/fisherface.xml");
	if (doc.LoadFile("person/fisherface.xml") == 0)
	{

		tinyxml2::XMLElement* titleElement = doc.FirstChildElement("opencv_storage")->FirstChildElement("labels")->FirstChildElement("data");
		cout << titleElement->GetText() << endl;

		map<string, int> faceCount = parseFaceCount(titleElement->GetText());
		int targetIndex = 0;

		for (int index = 0; index < _nameMap.size(); index++)
		{
			string namestring = _nameMap.at(index);
			if (_nameMap.at(index) == name)
			{
				targetIndex = index;
			}
		}

		int moveCount = 0;
		bool isFirst = true;
		std::string tempString = std::to_string(targetIndex);

		map<string, int>::iterator it;
		for (it = faceCount.begin(); it != faceCount.end(); it++)
		{
			if (it->first != tempString && isFirst)
			{
				moveCount += it->second;
				isFirst = false;
			}
		}
		cout << "moveCount: " << moveCount << endl;

		int deleteCount = faceCount[tempString];

		tinyxml2::XMLDocument deleteDoc;
		deleteDoc.LoadFile("person/test.xml");
		tinyxml2::XMLElement* deletionTitleElement = deleteDoc.FirstChildElement("opencv_storage")->FirstChildElement("histograms");
		tinyxml2::XMLNode *removeNode = deletionTitleElement->FirstChildElement("_");

		for (int index = 0; index < deleteCount; index++)
		{

			for (int index = 0; index < moveCount; index++)
			{
				removeNode = removeNode->NextSiblingElement("_");
			}
			deletionTitleElement->DeleteChild(removeNode);
			//deletionTitleElement->RemoveChild(removeNode);
			removeNode = deletionTitleElement->FirstChildElement("_");
		}

		tinyxml2::XMLElement* modifiedTitleElement = deleteDoc.FirstChildElement("opencv_storage")->FirstChildElement("labels")->FirstChildElement("data");

		string bal = titleElement->GetText();
		removeCharsFromString(bal, "\n");
		int numb;
		istringstream(tempString) >> numb;
		numb++;
		std::string s = std::to_string(numb);

		//string removeString = s + " ";
		//char *cstr = &removeString[0u];
		//removeCharsFromString(bal, cstr);

		for (int i = 0; i < deleteCount; i++)
		{
			myreplace(bal, s + " ", "");
		}


		modifiedTitleElement->SetText(bal.c_str());

		deleteDoc.SaveFile("person/testSave.xml");
		return true;
	}
}

std::string FaceRecognitionModule::myreplace(std::string &s, std::string toReplace, std::string replaceWith)
{
	return(s.replace(s.find(toReplace), toReplace.length(), replaceWith));
}


void FaceRecognitionModule::removeCharsFromString(string &str, char* charsToRemove) {
	for (unsigned int i = 0; i < strlen(charsToRemove); ++i) {
		str.erase(remove(str.begin(), str.end(), charsToRemove[i]), str.end());
	}
}

map<string, int> FaceRecognitionModule::parseFaceCount(string input)
{
	map<string, int> returnMap;
	std::map<string, int>::iterator it;

	istringstream iss(input);
	string cutted;

	int count = 0;
	while (getline(iss, cutted, ' '))
	{
		if (cutted != "" && cutted != "\n")
		{
			if (returnMap.find(cutted) == returnMap.end())
			{
				returnMap.insert(pair<string, int>(cutted, 1));
				count = 1;
			}
			else
			{
				count += 1;
				returnMap.at(cutted) = count;
			}
		}

	}
	return returnMap;
}