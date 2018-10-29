#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
using namespace cv;
using namespace std;

int menu(int check)
{
	cout << "--opencv�� ���� ��ȭ�����--" << endl;
	cout << "����������������  ������������" << endl;
	cout << "��1. q ����ġ ����            ��" << endl;
	cout << "��2. w ��ȭ ����              ��" << endl;
	cout << "��3. e �Ǹ� ����              ��" << endl;
	cout << "��3. r ���ϸ��� ����          ��" << endl;
	cout << "��4. s ĸ�ı��               ��" << endl;
	cout << "��5. ESC ����                 ��" << endl;
	cout << "��������������������������������" << endl;
	cout << "1. ������" << endl;
	cout << "2. �̹���" << endl;
	cout << "3. ����" << endl;
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
	strftime(buf, sizeof(buf), "%Y��%m��%d�� %H�� %M�� %S��", &tstruct);

	return buf;
}

void onKeypress(int key, Mat mt)
{
	switch (key) {
	case 115:
		cout << "ĸó�ð� >> " << currentDateTime() << endl;
		imageWrite(mt, currentDateTime());
		break;
	}
}

int main()
{
	int check = 0;
	check = menu(check);
	cout << "������ ��ȣ�� : [" << check << "] �Դϴ�." << endl;
	VideoCapture cap(0);
	Mat image;
	while (true)
	{
		if (check == 1)
		{
			while (1)
			{
				cap >> image; // ���� ����
				if (image.empty())
				{
					cout << "ī�޶� ã�� �� �����ϴ�. ���� �� �ٽ� �õ��ϼ���." << endl;
					exit(1);
				}
				imshow("����", image);
				int ch = waitKey(20);
				if (ch == 27)
				{
					cout << "�Կ��� �����մϴ�." << endl;
					exit(1);
				}
				if (ch == 113)
				{
					imshow("����ġ����", sketch(image));
					waitKey(0);
				}
				if (ch == 119)
				{
					imshow("��ȭ����", painting(image));
					waitKey(0);
				}
				if (ch == 101)
				{
					imshow("�Ǹ�����", devil(image));
					waitKey(0);
				}
				if (ch == 114)
				{
					Mat img = painting(image);
					imshow("���ϸ�������", alien(img));
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
		cout << "1.����ġ 2.��ȭ 3.�Ǹ� 4.���ϸ��� ���͸� �����ϼ���." << endl;
		cin >> check;
		if (check == 1)
		{
			cout << "���ϸ��� �Է����ּ���.(Ȯ���ڸ� ���� �Է����ּ���.)" << endl;
			Mat image;
			char filename[30];
			cin >> filename;
			image = imread(filename, IMREAD_COLOR);
			imshow("����", image);
			char keypress = waitKey(0);
			if (keypress > 0) {
				onKeypress(keypress, sketch(image));
				imshow("����ġ", sketch(image));
				waitKey(0);
			}
			if (keypress == 27)
			{
				cout << "�����մϴ�." << endl;
				exit(1);
			}
		}
		else if (check == 2)
		{
			cout << "���ϸ��� �Է����ּ���.(Ȯ���ڸ� ���� �Է����ּ���.)" << endl;
			Mat image;
			char filename[30];
			cin >> filename;
			image = imread(filename, IMREAD_COLOR);
			imshow("����", image);
			char keypress = waitKey(0);
			if (keypress > 0) {
				onKeypress(keypress, painting(image));
				imshow("��ȭ", painting(image));
				waitKey(0);
			}
			if (keypress == 27)
			{
				cout << "�����մϴ�." << endl;
				exit(1);
			}
		}
		else if (check == 3)
		{
			cout << "���ϸ��� �Է����ּ���.(Ȯ���ڸ� ���� �Է����ּ���.)" << endl;
			Mat image;
			char filename[30];
			cin >> filename;
			image = imread(filename, IMREAD_COLOR);
			imshow("����", image);
			char keypress = waitKey(0);
			if (keypress > 0) {
				onKeypress(keypress, devil(image));
				imshow("�Ǹ�", devil(image));
				waitKey(0);
			}
			if (keypress == 27)
			{
				cout << "�����մϴ�." << endl;
				exit(1);
			}
		}
		else if (check == 4)
		{
			cout << "���ϸ��� �Է����ּ���.(Ȯ���ڸ� ���� �Է����ּ���.)" << endl;
			Mat image;
			char filename[30];
			cin >> filename;
			image = imread(filename, IMREAD_COLOR);
			imshow("����", image);
			char keypress = waitKey(0);
			if (keypress > 0) {
				Mat img = painting(image);
				onKeypress(keypress, alien(img));
				imshow("���ϸ���", alien(img));
				waitKey(0);
			}
			if (keypress == 27)
			{
				cout << "�����մϴ�." << endl;
				exit(1);
			}
		}
	}
	else if (check == 3)
	{
		cout << "���α׷��� �����մϴ�." << endl;
		exit(0);
	}
	else
	{
		cout << "��ȣ�� �߸� �Է��߽��ϴ� �ٽ� �Է����ּ���" << endl;
	}
	return 0;
}
