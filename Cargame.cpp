#include<iostream>
#include<conio.h>
#include<dos.h> 
#include <windows.h>
#include <time.h>

// Định nghĩa chiều rộng và chiều cao màn hình console
#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
// Định nghĩa chiều rộng của khung trò chơi
#define WIN_WIDTH 70

using namespace std; 

// Xử lý giao diện điều khiển
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
// Tọa độ con trỏ trên console
COORD CursorPosition;

// Vị trí tọa độ Y của enemy
int enemyY[3];
// Vị trí tọa độ X của enemy
int enemyX[3];
// Biến xác định xem enemy có đang hiển thị hay không
int enemyFlag[3];
// Mảng chứa hình dạng xe của người chơi
char car[4][4] = { ' ','O','O',' ', 
					'O','O','O','O', 
					' ','0','0',' ',
					'O','O','O','O' }; 
					
// Vị trí xe của người chơi
int carPos = (WIN_WIDTH/2);
// Điểm số người chơi
int score = 0; 

// Hàm set màu cho các đối tượng, trong thư viện <windows.h>
void SET_COLOR(int color)
{
 WORD wColor;  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
 CONSOLE_SCREEN_BUFFER_INFO csbi;
 if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
 {
 wColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
 SetConsoleTextAttribute(hStdOut, wColor);
 }
}

// Hàm di chuyển con trỏ đến vị trí (x, y) trên console
void gotoxy(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

// Hàm thay đổi trạng thái và kích thước con trỏ trên console
void setcursor(bool visible, DWORD size) {
	if(size == 0)
		size = 20;	
	
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}

// Vẽ đường viền trò chơi 
void drawBorder(){  
	// Vòng lặp chạy qua tất cả chiều cao màn hình 
	for(int i=0; i<SCREEN_HEIGHT; i++){
		// Vòng lặp chạy qua 5 điểm bên trái và bên phải màn hình
		for(int j=0; j<5; j++){
			// Dịch chuyển đến tọa độ (0 + j, i) và in ký tự "±"
			gotoxy(0+j,i); cout<<"±";
			// Dịch chuyển đến tọa độ (WIN_WIDTH - j, i) và in ký tự "±"
			gotoxy(WIN_WIDTH-j,i); cout<<"±";
		}
	} 
	// Vòng lặp chạy qua tất cả chiều cao màn hình 
	for(int i=0; i<SCREEN_HEIGHT; i++){
		// Dịch chuyển đến tọa độ (SCREEN_WIDTH, i) và in ký tự "±"
		gotoxy(SCREEN_WIDTH,i); cout<<"±";
	} 
}

// Tạo xe ngược chiều
void genEnemy(int ind){
	// Tạo tọa độ X của xe tại vị trí (6 + một số ngẫu nhiên trong khoảng từ 6 đến 55)
	enemyX[ind] = 6 + rand()%(56);  
}

// Vẽ xe ngược chiều
void drawEnemy(int ind) {
  // Nếu enemyFlag của xe đó đang được hiển thị
  if (enemyFlag[ind] == true) {
    // Tạo xe ngược chiều bằng ký tự "*"
    gotoxy(enemyX[ind], enemyY[ind]);   cout << "****";  
    gotoxy(enemyX[ind], enemyY[ind]+1); cout << " 00 "; 
    gotoxy(enemyX[ind], enemyY[ind]+2); cout << "****"; 
    gotoxy(enemyX[ind], enemyY[ind]+3); cout << " ** ";  
  } 
}

void eraseEnemy(int ind) {
  // Nếu enemyFlag của xe đó đang được hiển thị
  if (enemyFlag[ind] == true) {
    // Xóa xe bằng các ký tự trắng
    gotoxy(enemyX[ind], enemyY[ind]); cout << "    ";  
    gotoxy(enemyX[ind], enemyY[ind]+1); cout << "    "; 
    gotoxy(enemyX[ind], enemyY[ind]+2); cout << "    "; 
    gotoxy(enemyX[ind], enemyY[ind]+3); cout << "    "; 
  } 
}

void resetEnemy(int ind) {
  // Xóa xe ngược chiều hiện tại
  eraseEnemy(ind);
  // Đặt lại vị trí ban đầu cho xe ngược chiều
  enemyY[ind] = 0;
  // Tạo lại xe ngược chiều mới
  genEnemy(ind);
}

void drawCar() {
  // Vẽ xe tải
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      gotoxy(j + carPos, i + 22); cout << car[i][j];
    }
  }
}

void eraseCar() {
  // Xóa xe tải
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      gotoxy(j + carPos, i + 22); cout << " ";
    }
  }
}


// Hàm xác định trạng thái va chạm
int collision() {
    // Nếu vị trí Y của kẻ địch + 4 >= 23 (vị trí X của xe của người chơi)
    if (enemyY[0] + 4 >= 23) {
        // Nếu vị trí X của kẻ địch + 4 - vị trí X của xe của người chơi >= 0 
        // và vị trí X của kẻ địch + 4 - vị trí X của xe của người chơi < 9 
        if (enemyX[0] + 4 - carPos >= 0 && enemyX[0] + 4 - carPos < 9) {
            // Trả về 1 nếu va chạm
            return 1;
        }
    }
    // Trả về 0 nếu không va chạm
    return 0;
}

// Hàm hiển thị màn hình kết thúc trò chơi
void gameover() {
    // Xóa màn hình console
    system("cls");
    cout << endl;
    cout << "\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t--------------------------" << endl;
    cout << "\t\t\t\t\t\t-------- Game Over -------" << endl;
    cout << "\t\t\t\t\t\t--------------------------" << endl << endl;
	cout << "\t\t\t\t\t\t       Your Score: " << score << endl;
    cout << "\n\t\t\t\t\t     Press any key to go back to menu.";
    // Chờ nhấn phím
    getch();
}

// Hàm cập nhật điểm
void updateScore() {
    // Di chuyển con trỏ tới vị trí hiển thị điểm
    gotoxy(WIN_WIDTH + 7, 5+3);
    cout << "Score: " << score << endl;
}

// Hàm hiển thị hướng dẫn                       
void instructions() {
    // Xóa màn hình console
    system("cls");
    cout << "\n\n\n\n\n\n\n\n		                                       Huong dan";
    cout << "\n		                                   -----------------";
    cout << "\n\n 	                                + Tranh oto bang cach di chuyen trai hoac phai. ";
    cout << "\n 	                                + Nhan 'z' de di chuyen sang trai.";
    cout << "\n 	                                + Nhan 'c' de di chuyen sang phai.";
    cout << "\n 	                                + Nhan 'escape' de thoat.";
    cout << "\n 	                                + Nhan phim bat ki de quay lai menu.";
    // Chờ nhấn phím
    getch();
}

void play(){
	// Đặt vị trí xe tại giữa màn hình
	carPos = -1 + WIN_WIDTH/2;
	// Khởi tạo điểm là 0
	score = 0;
	// Đặt xe ngược chiều xuất hiện đầu tiên (flag = 1) và xe ngược chiều xuất hiện thứ hai (flag = 0)
	enemyFlag[0] = 1;
	enemyFlag[1] = 0;
	// Đặt tọa độ y của cả hai xe ngược chiều là 1
	enemyY[0] = enemyY[1] = 1;
	  
	// Xóa màn hình
	system("cls"); 
	SET_COLOR(3);
	// Vẽ đường viền
	drawBorder(); 
	// Cập nhật hiển thị điểm
	updateScore();
	SET_COLOR(5);
	// Sinh ra xe ngược chiều đầu tiên
	genEnemy(0);
	SET_COLOR(6);
	// Sinh ra xe ngược chiều thứ hai
	genEnemy(1);

	// Hiển thị tiêu đề và các điều khiển
	gotoxy(WIN_WIDTH + 7, 2+3); cout << "Car Game";
	gotoxy(WIN_WIDTH + 6, 4+3); cout << "----------";
	gotoxy(WIN_WIDTH + 6, 6+3); cout << "----------";
	gotoxy(WIN_WIDTH + 8, 12+3); cout << "Control ";
	gotoxy(WIN_WIDTH + 7, 13+3); cout << "-	--------";
	gotoxy(WIN_WIDTH + 4.5, 14+3); cout << " A Key - Left";
	gotoxy(WIN_WIDTH + 4.5, 15+3); cout << " D Key - Right";
	
	// Hiển thị thông báo nhấn bất kỳ phím nào để bắt đầu
	gotoxy(25, 12); cout << "Press any key to start";
	// Chờ người dùng nhấn phím
	getch();
	// Xóa thông báo
	gotoxy(25, 12); cout << "                      ";

	// Bắt đầu vòng lặp trò chơi
	while (1) {
		// Kiểm tra xem có phím nào được nhấn hay không
		if (kbhit()) {
			// Lấy ký tự của phím đã nhấn
			char ch = getch();
			// Nếu phím 'a' hoặc 'A' được nhấn, di chuyển xe sang trái nếu nó chưa đến biên
			if (ch == 'a' || ch == 'A') {
				if (carPos > 6)
			    carPos -= 4;
			}
			// Nếu phím 'd' hoặc 'D' được nhấn, di chuyển xe sang phải nếu nó chưa đến biên
			if (ch == 'd' || ch == 'D') {
				if (carPos < 62)
			    carPos += 4;
			}
			// Nếu phím 'esc' được nhấn, rời khỏi vòng lặp trò chơi
			if (ch == 27) {
				break;
			}
		}


		srand(time(0));
		SET_COLOR((rand() % 14)+2);
		// Vẽ xe
		drawCar();
		SET_COLOR((rand() % 14)+2);
		// Vẽ xe ngược chiều thứ nhất
		drawEnemy(0);
		SET_COLOR((rand() % 14)+2);
		// Vẽ xe ngược chiều thứ hai
		drawEnemy(1);
		SET_COLOR((rand() % 14)+2);


		// Kiểm tra xem có va chạm giữa xe và đối tượng khác không
        if( collision() == 1 ){
        	// Nếu có va chạm, gọi hàm gameover
        	gameover();
        	// Thoát khỏi hàm play
        	return;
        }
		// Tạm dừng 50ms
		Sleep(30);
		// Xóa vẽ xe cũ
		eraseCar();
		// Xóa vẽ đối tượng 0 cũ
		eraseEnemy(0);
		// Xóa vẽ đối tượng 1 cũ
		eraseEnemy(1);
		// Nếu đối tượng 0 đạt đến vị trí y = 10, cho đối tượng 1 bắt đầu di chuyển
		if( enemyY[0] == 10 )
			if( enemyFlag[1] == 0 )
				enemyFlag[1] = 1;

		// Nếu đối tượng 0 có cho phép di chuyển, tăng vị trí y của nó lên 1
		if( enemyFlag[0] == 1 )
			enemyY[0] += 1;

		// Nếu đối tượng 1 có cho phép di chuyển, tăng vị trí y của nó lên 1
		if( enemyFlag[1] == 1 )
			enemyY[1] += 1;

		// Nếu đối tượng 0 đạt đến cuối màn hình, gọi hàm resetEnemy để reset vị trí của nó và tăng điểm lên 1
		if( enemyY[0] > SCREEN_HEIGHT-4 ){
			resetEnemy(0);
			score++;
			updateScore();
		}
		// Nếu đối tượng 1 đạt đến cuối màn hình, gọi hàm resetEnemy để reset vị trí của nó và tăng điểm lên 1
		if( enemyY[1] > SCREEN_HEIGHT-4 ){
			resetEnemy(1);
			score++;
			updateScore();
		}
	}
}

int main()
{
	// Thiết lập vị trí con trỏ dòng và cột ban đầu là (0, 0)
	setcursor(0,0); 
	
	// Khởi tạo hàm random
	srand( (unsigned)time(NULL)); 
	 
	// Vòng lặp chạy vô hạn, hiển thị menu chính
	do{
		// Xóa màn hình console
		system("cls");
		
		// Hiển thị tiêu đề
		gotoxy(45,9); cout<<" -------------------------- "; 
		gotoxy(45,10); cout<<" |        Car Game        | "; 
		gotoxy(45,11); cout<<" --------------------------";
		
		// Hiển thị các tùy chọn trong menu
		gotoxy(46,13); cout<<"1. Start Game";
		gotoxy(46,14); cout<<"2. Instructions";	 
		gotoxy(46,15); cout<<"3. Quit";
		
		// Hiển thị yêu cầu người chơi nhập tùy chọn
		gotoxy(46,17); cout<<"Select option: ";
		
		// Lấy tùy chọn của người chơi
		char op = getche();
		
		// Xử lý tùy chọn của người chơi
		if( op=='1') play();
		else if( op=='2') instructions();
		else if( op=='3') exit(0);
		
	}while(1);
	
	// Trả về giá trị 0 khi chương trình kết thúc
	return 0;
}






