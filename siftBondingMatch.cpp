#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;
RNG rng(12345);

void readme();
/// Function header
Mat findBonding(int thresh, Mat src_gray);


/** @function main */
int main( int argc, char** argv )
{
  if( argc != 3 )
  { readme(); return -1; }

  Mat img_1 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
  Mat img_2 = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE );

  if( !img_1.data || !img_2.data )
  { std::cout<< " --(!) Error reading images " << std::endl; return -1; }

  //-- Step 1: Detect the keypoints using SURF Detector
  int minHessian = 400;

  SurfFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_1, keypoints_2;

  detector.detect( img_1, keypoints_1 );
  detector.detect( img_2, keypoints_2 );

  //-- Draw keypoints
  Mat img_keypoints_1; Mat img_keypoints_2;

  drawKeypoints( img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
  drawKeypoints( img_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

  //-- Show detected (drawn) keypoints
  //imshow("Keypoints 1", img_keypoints_1 );
  //imshow("Keypoints 2", img_keypoints_2 );
  //imwrite("/Users/matcha/fish_shadow/fishall-pt.jpg", img_keypoints_1);
  //imwrite("/Users/matcha/Desktop/fish_shadow/onefish-pt.jpg", img_keypoints_2);
  //waitKey(0);

  //////////////// find the bounding box//////////////////
  Mat bonding;
  int thresh = 100;
  int max_thresh = 255;

  /// Convert image to gray and blur it
  //cvtColor( img_keypoints_1, src_gray, CV_BGR2GRAY );
  blur( img_1, bonding, Size(3,3) );

  bonding = findBonding( thresh, bonding);
  Mat test;
  drawKeypoints( bonding, keypoints_1, test, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
  //imwrite("/Users/matcha/Desktop/fish_shadow/fishall-pt-bonding.jpg", bonding);
  // /// Create Window
  namedWindow( "bounding", CV_WINDOW_AUTOSIZE );
  imshow( "bounding", test);
  waitKey(0);



  return 0;
  }

  /** @function readme */
  void readme()
  { std::cout << " Usage: ./SURF_detector <img1> <img2>" << std::endl; }

  /** @function thresh_callback */
  Mat findBonding(int thresh, Mat src_gray)
  {
    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using Threshold
    threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );

    //Canny( src_gray, threshold_output, thresh, thresh*2, 3 );

    /// Find contours
    findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Find the rotated rectangles and ellipses for each contour
    vector<RotatedRect> minRect( contours.size() );
    vector<RotatedRect> minEllipse( contours.size() );

    for( int i = 0; i < contours.size(); i++ )
       { minRect[i] = minAreaRect( Mat(contours[i]) );
         if( contours[i].size() > 5 )
            continue;
           //{ minEllipse[i] = fitEllipse( Mat(contours[i]) ); }

       }

    /// Draw contours + rotated rects + ellipses
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );

    int count = 0;
    for( int i = 0; i< contours.size(); i++ )
       {
         Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
         // contour
         drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
         // ellipse
         //ellipse( drawing, minEllipse[i], color, 2, 8 );
         // rotated rectangle
         Point2f rect_points[4];
         minRect[i].points( rect_points );
         float area = contourArea(contours[i]);
         printf(" area = %f", area);
         if (area < 100 || area >= src_gray.rows * src_gray.cols - 100000)
              printf("pass\n");
         else{
           for( int j = 0; j < 4; j++ ){
              printf("%d = (%f, %f)\n", i, rect_points[j].x , rect_points[j].y);
              line( drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );

              }
                count++;

            }


          // namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
          // imshow( "Contours", drawing );
          // waitKey(0);
       }

    printf("count=%d\n", count);
    printf("size=%lu\n", contours.size());
    // /// Show in a window
    // namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    // imshow( "Contours", drawing );
    return drawing;
  }
