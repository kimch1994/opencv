# opencv
OpenCV를 통한 
만화 만들기
보고서 (자세한 내용은 word파일 )

-목차-
I.	서론 ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙

1.	프로젝트 배경
2.	프로젝트 효율성

II.	본론∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙

1.	시스템 구조도
1)	구조도
2)	유저 시나리오 흐름도
2.	필터 Method
1)	Mat sketch(Mat mt)
2)	Mat painting(Mat mt)
3)	Mat devil(Mat mt)
4)	Mat alien(const Mat& image)
3.	기능 Method
1)	void imageWrite(Mat &mt, string filename)
2)	string currentDateTime()
4.	Main( )

III.	결론∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙
1.	기대 효과
2.	보완해야 할 점

IV.	참고문헌 ∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙∙


	프로젝트 배경
최근 학생들 사이에는 웹툰이라는 웹에서 보는 만화가 유행이다. 만화를 방식은 장면을 하나하나를 직접 생각하고 그려서 작업을 한다고 한다. 이러한 복잡한 방식을 “OpenCV에서 제공하는 여러 필터와 함수들을 이용해 어떻게 편하게 바꿀 수 있을 까?” 라는 생각에 프로젝트 배경으로 잡았다.

	프로젝트 효율성
이번 프로젝트의 효율성으로는 가장 큰 효과로는 직접 만화를 그리지 않고 영상에서 캡쳐를 통해 바로 원하는 장면을 만화로 제작 할 수 있다는 점이다. 또한 만화를 전문적으로 그리는 작가와 배운적이 없는 비전문가 모두에게 자신이 생각하는 장면을 손 쉽게 만들 수 있게 도움을 주며 만화 제작 시간을 많이 감소 시킬수 있을 것이다.
	시스템 구조도
1)	구조도
 
2)	유저 시나리오 흐름도
 
	필터 Method
1.	Mat sketch(Mat mt)
 
>> 해당 이미지를 받아서 그레이 이미지로 변환 후 Laplacian 필터를 사용하여 edge를 찾은후 해당 영상을 이진화 시킴
>> Laplacian 필터를 사용한 이유는 소벨,샤르,캐니 에지 검출기 중에 직접 스케치한 느낌이 강한 Laplacian 필터가 가장 적합하다는 판단에 사용

2.	Mat painting(Mat mt)
  
>> bilateralFilter를 사용하여 에지를 보존하고 평탄 영역을 부드럽게 만든 후(bilateralFilter의 단점인 속도부분에 대한 문제를 해결하기 위해 해당 이미지를 작게 만든 후 처리) 원본 크기로 되돌린 후 에 에지가 아닌 곳에 페인팅 화소를 적용

3.	Mat devil(Mat mt)
  
>>해당 이미지를 받아서 그레이 이미지로 변환 후 샤르 필터를 사용하여 무서운 느낌의 이미지를 생성 (샤르 필터)

4.	Mat alien(const Mat& image)
 
>>해당 이미지 크기만큼 마스크를 만들어 반복문을 통해 조건에 따른 화소 값을 흰색(255)로 만든 후 add를 통하여 초록색(0,70,0)을 삽입하여 최종 이미지에 에일리언 처럼 나오게 한다.
<< 피부색 범위값 >>
 
	기능 Method

1.	Void imageWirte(Mat &img, string filename) 
 >>캡처된 영상을 .jpg 확장자로 저장하는 기능을 수행해주는 Method이며 해당 프로젝트 파일/save에 시간대별로 저장이된다.
2.	String currentDataTime()
 >>저장된 영상의 제목에 캡처한 시간대를 표현해주는 Method이다.
 
	Main( )

>> 처음 선택으로 동영상에서 캡쳐를 진행할건지 이미지 자체를 변환 시킬건지 선택을 하게한다. 각각의 키 이벤트 (Q,W,E,R필터 변환)가 눌릴때마다 waitKey(0)를 통하여 대기를 시키고 변환된 프레임의 이미지를 보여준다. 또한 마지막에 ‘S’ 버튼을 통하여 캡쳐된 이미지를 저장 할 수있다.

	기대효과
기대효과로는 만화 제작을 좋아하지만 직접 제작을 못하는 사람들에게 간단한 방법으로 자신만의 만화를 제작 할수 있으며 전문가들에게는 만화 제작시 일의 효율을 극대화 시킬 수 있을 것이며 크게 보면 웹툰 시장에 많은 영향을 줘서 국내 웹툰 시장의 성장에 크게 기여 할 것이다.


	보완해야 할 점
1.	에일리언 필터에서 피부색 범위안에 들어오는 배경의 변화에 대한 처리가 미흡하다.
2.	여러가지 필터를 추가해서 콘텐츠를 풍부하게 만들어 사용자가 만화를 제작할 때 다양한 방법을 사용 할 수있게해야한다.



	참고문헌
	COMPUTER VISION / 오일석 지음 / 한빛 아카데미  (바이래터럴 사용법)
	Opencv 컴퓨터 비전 / 세르빈 이마미 외 6명 지음 / 이문호 옮김 / 에이콘아카데미(필터 사용법 참고)
	https://m.blog.naver.com/PostView.nhn?blogId=pckbj123&logNo=100203340815&proxyReferer=https%3A%2F%2Fwww.google.co.kr%2F    (RGB 공간 피부색 검출 참고)
	http://blog.naver.com/shop2930/220298412171   (.JPG 확장자로 저장하는 법 참고)
	http://cafe.naver.com/opencv Opencv 커뮤니티 

