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
	
	//��ȡ���̸�ͼƬ
	for (int  i =0; i < path_list.size(); i++)
	{
		Mat temp = imread(path_list[i]);
		images.emplace_back(temp);

	}

	/*------------��ȡ�ǵ�-------------*/
	for (int i=0;i<images.size();i++)
	{
		cv::Mat image_gray;
		vector<cv::Point2f> corner;
		//��ͼ��ҶȻ�
		cv::cvtColor(images[i],image_gray,cv::COLOR_BGR2GRAY);
		//Ѱ�ҽǵ�
		if (findChessboardCorners(image_gray, borad_sz, corner) == 0) {
			cout << "δѰ�ҵ��ǵ㣡����" << endl;
		}
		else {	
			//�����ؽǵ���
			cv::find4QuadCornerSubpix(image_gray, corner, borad_sz);
			
			//ָ�������ؼ��������ע
			//cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 40, 0.1);
			//�����ؼ��
			//cv::cornerSubPix(image_gray, corners, cv::Size(6, 6), cv::Size(-1, -1), criteria);
			
			drawChessboardCorners(images[i], borad_sz, corner, false);

			//���������ؽǵ���Ϣ
			corners.emplace_back(corner);
		}

	}

	/*------------����궨-------------*/

	/*������ά��Ϣ*/
	Size square_size = Size(10, 10);  /* ʵ�ʲ����õ��ı궨����ÿ�����̸�Ĵ�С */
	vector<vector<Point3f>> object_points; /* ����궨���Ͻǵ����ά���� */
										   /*�������*/
	/* ������ڲ������� */
	Mat cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); 
	vector<int> point_counts;  // ÿ��ͼ���нǵ������
	/* �������5������ϵ����k1,k2,p1,p2,k3 */
	Mat distCoeffs = Mat(1, 5, CV_32FC1, Scalar::all(0)); 

	/* ÿ��ͼ�����ת���� */
	vector<Mat> tvecsMat;  
	/* ÿ��ͼ���ƽ������ */
	vector<Mat> rvecsMat; 
    /* ��ʼ���궨���Ͻǵ����ά���� */
	int i, j, t;
	for (t = 0; t < images.size(); t++)
	{
		vector<Point3f> tempPointSet;
		for (i = 0; i < borad_sz.height; i++)
		{
			for (j = 0; j < borad_sz.width; j++)
			{
				Point3f realPoint;
				/* ����궨�������������ϵ��z=0��ƽ���� */
				realPoint.x = i*square_size.width;
				realPoint.y = j*square_size.height;
				realPoint.z = 0;
				tempPointSet.push_back(realPoint);
			}
		}
		object_points.push_back(tempPointSet);
	}
	/* ��ʼ�궨 */
	calibrateCamera(object_points, corners, borad_sz, cameraMatrix, distCoeffs, rvecsMat, tvecsMat, 0);
	cout << "�궨��ɣ�\n";
	cout <<"����ڲΣ�\n"<< cameraMatrix << endl;
	cout << "����ϵ����\n" << distCoeffs << endl;

	for (int i = 0; i < rvecsMat.size();i++) {
		cout << "��" << i << "����ת����" << rvecsMat[i];
		cout << "��" << i << "��ƽ��������" << tvecsMat[i];
	}
	


	

}
