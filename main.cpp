#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void main() {


	std::vector<std::string> path_list;
	std::vector<cv::Mat> images;
	vector<std::vector<cv::Point2f>> corners;

	path_list.emplace_back("D:/imags/calibration/images001/IMG_0191.jpg");
	path_list.emplace_back("D:/imags/calibration/images001/IMG_0192.jpg");
	path_list.emplace_back("D:/imags/calibration/images001/IMG_0193.jpg");
	path_list.emplace_back("D:/imags/calibration/images001/IMG_0194.jpg");
	path_list.emplace_back("D:/imags/calibration/images001/IMG_0195.jpg");
	path_list.emplace_back("D:/imags/calibration/images001/IMG_0196.jpg");

	int board_w = 12;
	int board_h = 12;
	cv::Size borad_sz = cv::Size(board_w, board_h);
	
	//获取棋盘格图片
	for (int  i =0; i < path_list.size(); i++)
	{
		Mat temp = imread(path_list[i]);
		images.emplace_back(temp);

	}

	/*------------提取角点-------------*/
	for (int i=0;i<images.size();i++)
	{
		cv::Mat image_gray;
		vector<cv::Point2f> corner;
		//将图像灰度化
		cv::cvtColor(images[i],image_gray,cv::COLOR_BGR2GRAY);
		//寻找角点
		if (findChessboardCorners(image_gray, borad_sz, corner) == 0) {
			cout << "未寻找到角点！！！" << endl;
		}
		else {	
			//亚像素角点检测
			cv::find4QuadCornerSubpix(image_gray, corner, borad_sz);
			
			//指定亚像素计算迭代标注
			//cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 40, 0.1);
			//亚像素检测
			//cv::cornerSubPix(image_gray, corners, cv::Size(6, 6), cv::Size(-1, -1), criteria);
			
			drawChessboardCorners(images[i], borad_sz, corner, false);

			//保存亚像素角点信息
			corners.emplace_back(corner);
		}

	}

	/*------------相机标定-------------*/

	/*棋盘三维信息*/
	Size square_size = Size(10, 10);  /* 实际测量得到的标定板上每个棋盘格的大小 */
	vector<vector<Point3f>> object_points; /* 保存标定板上角点的三维坐标 */
										   /*内外参数*/
	/* 摄像机内参数矩阵 */
	Mat cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); 
	vector<int> point_counts;  // 每幅图像中角点的数量
	/* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */
	Mat distCoeffs = Mat(1, 5, CV_32FC1, Scalar::all(0)); 

	/* 每幅图像的旋转向量 */
	vector<Mat> tvecsMat;  
	/* 每幅图像的平移向量 */
	vector<Mat> rvecsMat; 
    /* 初始化标定板上角点的三维坐标 */
	int i, j, t;
	for (t = 0; t < images.size(); t++)
	{
		vector<Point3f> tempPointSet;
		for (i = 0; i < borad_sz.height; i++)
		{
			for (j = 0; j < borad_sz.width; j++)
			{
				Point3f realPoint;
				/* 假设标定板放在世界坐标系中z=0的平面上 */
				realPoint.x = i*square_size.width;
				realPoint.y = j*square_size.height;
				realPoint.z = 0;
				tempPointSet.push_back(realPoint);
			}
		}
		object_points.push_back(tempPointSet);
	}
	/* 开始标定 */
	calibrateCamera(object_points, corners, borad_sz, cameraMatrix, distCoeffs, rvecsMat, tvecsMat, 0);
	cout << "标定完成！\n";
	cout <<"相机内参：\n"<< cameraMatrix << endl;
	cout << "畸变系数：\n" << distCoeffs << endl;

	for (int i = 0; i < rvecsMat.size();i++) {
		cout << "第" << i << "张旋转矩阵：" << rvecsMat[i];
		cout << "第" << i << "张平移向量：" << tvecsMat[i];
	}
	


	

}
