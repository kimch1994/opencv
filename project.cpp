#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
using namespace cv;
using namespace std;

int menu(int check)
{
	cout << "--opencv를 통한 만화만들기--" << endl;
	cout << "┌────간단한  사용법───┐" << endl;
	cout << "│1. q 스케치 필터            │" << endl;
	cout << "│2. w 만화 필터              │" << endl;
	cout << "│3. e 악마 필터              │" << endl;
	cout << "│3. r 에일리언 필터          │" << endl;
	cout << "│4. s 캡쳐기능               │" << endl;
	cout << "│5. ESC 종료                 │" << endl;
	cout << "└──────────────┘" << endl;
	cout << "1. 동영상" << endl;
	cout << "2. 이미지" << endl;
	cout << "3. 종료" << endl;
	cin >> check;

	return check;
}
Mat sketch(Mat mt)
{
	Mat gray;
	cvtColor(mt, gray, CV_BGR2GRAY);
	medianBlur(gray, gray, 7);
	Mat edges;
	Laplacian(gray, edges, CV_8U, 5);

	Mat mask;
	threshold(edges, mask, 80, 255, THRESH_BINARY_INV);

	return mask;
}

Mat painting(Mat mt)
{
	Mat gray;
	cvtColor(mt, gray, CV_BGR2GRAY);
	medianBlur(gray, gray, 7);
	Mat edges;
	Laplacian(gray, edges, CV_8U, 5);

	Mat mask;
	threshold(edges, mask, 80, 255, THRESH_BINARY_INV);


	Size size = mt.size();
	Size smallSize;

	smallSize.width = size.width / 2;
	smallSize.height = size.height / 2;
	Mat smallimg = Mat(smallSize, CV_8UC3);
	resize(mt, smallimg, smallSize, 0, 0, INTER_LINEAR);

	Mat tmp = Mat(smallSize, CV_8UC3);
	int repetitions = 7;

	for (int i = 0; i < repetitions; i++)
	{
		int ksize = 9;
		double sigmaColor = 9;
		double sigmaSpace = 7;
		bilateralFilter(smallimg, tmp, ksize, sigmaColor, sigmaSpace);
		bilateralFilter(tmp, smallimg, ksize, sigmaColor, sigmaSpace);
	}

	Mat bigimg;
	resize(smallimg, bigimg, size, 0, 0, INTER_LINEAR);
	tmp.setTo(0);
	bigimg.copyTo(tmp, mask);
	return tmp;
}

Mat devil(Mat mt)
{
	Mat srcGray;
	cvtColor(mt, srcGray, CV_BGR2GRAY);


	medianBlur(srcGray, srcGray, 7);

	Size size = mt.size();
	Mat mask = Mat(size, CV_8U);
	Mat edges = Mat(size, CV_8U);

	Mat edges2;
	Scharr(srcGray, edges, CV_8U, 1, 0);
	Scharr(srcGray, edges2, CV_8U, 1, 0, -1);
	edges += edges2;
	threshold(edges, mask, 12, 255, THRESH_BINARY_INV);
	medianBlur(mask, mask, 3);

	Size smallSize;
	smallSize.width = size.width / 2;
	smallSize.height = size.height / 2;
	Mat smallImg = Mat(smallSize, CV_8UC3);
	resize(mt, smallImg, smallSize, 0, 0, INTER_LINEAR);

	Mat tmp = Mat(smallSize, CV_8UC3);
	int repetitions = 7;
	for (int i = 0; i<repetitions; i++) {
		int size = 9;
		double sigmaColor = 9;
		double sigmaSpace = 7;
		bilateralFilter(smallImg, tmp, size, sigmaColor, sigmaSpace);
		bilateralFilter(tmp, smallImg, size, sigmaColor, sigmaSpace);
	}

	Mat dst;
	resize(smallImg, mt, size, 0, 0, INTER_LINEAR);
	memset((char*)dst.data, 0, dst.step * dst.rows);
	mt.copyTo(dst, mask);
	return dst;
}


Mat alien(const Mat& image) {

	Mat mask(image.size(), CV_8U, Scalar(0));
	const int nr = image.rows;
	const int nc = image.cols;
	for (int i = 0; i<nr; i++) {
		const Vec3b* data = image.ptr<Vec3b>(i);
		for (int j = 0; j<nc; j++) {
			uchar R = data[j][2];
			uchar G = data[j][1];
			uchar B = data[j][0];
			if (R>95 && G>40 && B>20 && max(R, max(G, B)) - min(R, min(G, B))>15 && abs(R - G)>15 && R>G && R>B)
				mask.at<uchar>(i, j) = 255;
		}
	}
	add(image, Scalar(0, 70, 0), image, mask);
	return image;
}



void imageWrite(Mat &mt, string filename)
{
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);

	string path = "./save/" + filename + ".jpg";
	cout << filename << endl;
	imwrite(path, mt, compression_params);
}

string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y년%m월%d일 %H시 %M분 %S초", &tstruct);

	return buf;
}

void onKeypress(int key, Mat mt)
{
	switch (key) {
	case 115:
		cout << "캡처시간 >> " << currentDateTime() << endl;
		imageWrite(mt, currentDateTime());
		break;
	}
}

int main()
{
	int check = 0;
	check = menu(check);
	cout << "선택한 번호는 : [" << check << "] 입니다." << endl;
	VideoCapture cap(0);
	Mat image;
	while (true)
	{
		if (check == 1)
		{
			while (1)
			{
				cap >> image; // 영상 대입
				if (image.empty())
				{
					cout << "카메라를 찾을 수 없습니다. 연결 후 다시 시도하세요." << endl;
					exit(1);
				}
				imshow("원본", image);
				int ch = waitKey(20);
				if (ch == 27)
				{
					cout << "촬영을 종료합니다." << endl;
					exit(1);
				}
				if (ch == 113)
				{
					imshow("스케치필터", sketch(image));
					waitKey(0);
				}
				if (ch == 119)
				{
					imshow("만화필터", painting(image));
					waitKey(0);
				}
				if (ch == 101)
				{
					imshow("악마필터", devil(image));
					waitKey(0);
				}
				if (ch == 114)
				{
					Mat img = painting(image);
					imshow("에일리언필터", alien(img));
					waitKey(0);
				}
				if (ch == 115)
				{
					onKeypress(ch, image);
				}

				cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
				cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
			}
		}
		else
		{
			break;
		}
	}
	if (check == 2)
	{
		int check = 0;
		cout << "1.스케치 2.만화 3.악마 4.에일리언 필터를 선택하세요." << endl;
		cin >> check;
		if (check == 1)
		{
			cout << "파일명을 입력해주세요.(확장자명 까지 입력해주세요.)" << endl;
			Mat image;
			char filename[30];
			cin >> filename;
			image = imread(filename, IMREAD_COLOR);
			imshow("원본", image);
			char keypress = waitKey(0);
			if (keypress > 0) {
				onKeypress(keypress, sketch(image));
				imshow("스케치", sketch(image));
				waitKey(0);
			}
			if (keypress == 27)
			{
				cout << "종료합니다." << endl;
				exit(1);
			}
		}
		else if (check == 2)
		{
			cout << "파일명을 입력해주세요.(확장자명 까지 입력해주세요.)" << endl;
			Mat image;
			char filename[30];
			cin >> filename;
			image = imread(filename, IMREAD_COLOR);
			imshow("원본", image);
			char keypress = waitKey(0);
			if (keypress > 0) {
				onKeypress(keypress, painting(image));
				imshow("만화", painting(image));
				waitKey(0);
			}
			if (keypress == 27)
			{
				cout << "종료합니다." << endl;
				exit(1);
			}
		}
		else if (check == 3)
		{
			cout << "파일명을 입력해주세요.(확장자명 까지 입력해주세요.)" << endl;
			Mat image;
			char filename[30];
			cin >> filename;
			image = imread(filename, IMREAD_COLOR);
			imshow("원본", image);
			char keypress = waitKey(0);
			if (keypress > 0) {
				onKeypress(keypress, devil(image));
				imshow("악마", devil(image));
				waitKey(0);
			}
			if (keypress == 27)
			{
				cout << "종료합니다." << endl;
				exit(1);
			}
		}
		else if (check == 4)
		{
			cout << "파일명을 입력해주세요.(확장자명 까지 입력해주세요.)" << endl;
			Mat image;
			char filename[30];
			cin >> filename;
			image = imread(filename, IMREAD_COLOR);
			imshow("원본", image);
			char keypress = waitKey(0);
			if (keypress > 0) {
				Mat img = painting(image);
				onKeypress(keypress, alien(img));
				imshow("에일리언", alien(img));
				waitKey(0);
			}
			if (keypress == 27)
			{
				cout << "종료합니다." << endl;
				exit(1);
			}
		}
	}
	else if (check == 3)
	{
		cout << "프로그램을 종료합니다." << endl;
		exit(0);
	}
	else
	{
		cout << "번호를 잘못 입력했습니다 다시 입력해주세요" << endl;
	}
	return 0;
}
