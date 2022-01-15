#include <iostream>
#include <io.h> //_setmode()
#include <fcntl.h> ///_O_WTEXT
#include <fstream> //file
#include <string> //getline
#include <locale> //truyen cho file
#include <codecvt> 
#include <cstdlib> // rand
#include <ctime> // time
#include <conio.h> // getch
#include <Windows.h> //Sleep + gotoxy
#include <iomanip>
#include <cstdlib>
#include <cctype>

using namespace std;

// Cấu trúc dữ liệu người chơi
struct NameAndScore {
	wstring name;
	int score{};
	double time{};
	NameAndScore() {};
};

// Thiết đặt cửa sổ hiển thị
void set_color(int, int); // c2 https://daynhauhoc.com/t/huong-dan-nhung-cach-don-gian-de-doi-mau-chu-tren-console-windows-trong-ngon-ngu-c-cho-newbie/106319
void SetWindowSize(SHORT, SHORT); // kích thước màn hình
void DisableResizeWindow();	// ngăn không cho thay đổi kích thước
void DisableCtrButton(bool, bool, bool); // tắt các nút điều khiển (tắt, phóng to, thu nhỏ)
void ShowScrollbar(BOOL); // Tắt thanh cuộn
void ShowCur(int);// Tắt con trỏ
void DisableSelection();// Tắt bôi đen
void EnableSelection();

// Hàm vẽ giao diện
void gotoxy(short, short);
void veVien();
void veVienPlayWindows();
void veBox(int, int, int, int);
void veBox2Vien(int, int, int, int);
void presentMouseLocation(int);

// Hàm xoá
void deleteLine(int);
void deleteDapan();
void deleteThanhThongBao();
void deleteMouseLocation(int);

// Xử lí và in câu hỏi
void demTu();
wstring getRandomLine();
wstring getEnglish(wstring);
wstring getVietnamse(wstring);
void inCauHoi(wstring dapAnCauHoi, wstring dapAn[]);

// các cửa sổ
void mainMenu();
void playWindows();
void inHighScore(int);
void thongTin();
void huongDan();
void intro1();
void nameSoftware();

// biến điểm thiết đặt toàn cục để sử dụng trong các hàm
int score, soTu = 0;

int main() {
	// Thiết lập cửa sổ hiển thị
	system("color f0"); // nền trắng chữ đen
	SetWindowSize(101, 29); // kích thước màn hình
	DisableResizeWindow(); // ngắt chỉnh độ rộng window
	DisableCtrButton(0, 0, 1); // ngắt phím phóng to (tắt, phóng, chìm)
	ShowScrollbar(0); // không hiển thị thanh cuộn
	ShowCur(0); //Tắt con trỏ
	DisableSelection(); //Tắt bôi đen
	SetConsoleTitle(L"Từ vựng English");//đặt tên cửa sổ

	// Thiết lập ban đầu
	(void)_setmode(_fileno(stdout), _O_WTEXT); //needed for output tiếng việt
	(void)_setmode(_fileno(stdin), _O_WTEXT); //needed for input tiếng việt
	srand(static_cast<unsigned int>(time(NULL))); // hạt giống random
	demTu(); // đếm xem có bao nhiêu từ trong file

	// Bắt đầu sẽ chạy 2 intro sau đó sẽ ra menu
	intro1();
	nameSoftware();
	mainMenu();

	return 0;
}

// Hàm xử lí đáp án
void demTu() {
	locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	wifstream file(L"lib.canh");
	file.imbue(loc); //thiết đặt đọc tiếng việt cho file
	wstring line;
	while (getline(file, line)) {
		soTu++;
	}
	file.close();
	return;
}
wstring getRandomLine() {
	// File đầu vào
	locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	wifstream file(L"lib.canh");
	file.imbue(loc); //thiết đặt đọc tiếng việt cho file

	// Lấy một số ngẫu nhiên để chọn từ trong file
	int num_line = (rand() % soTu + 1); //(cuoi - dau + 1)
	wstring line;

	// Gán cho line 1 chuỗi ở dòng thứ num_line
	for (int i = 0; i < num_line; i++)
	{
		getline(file, line);
	}

	file.close();
	return line;
}
wstring getEnglish(wstring line) { // tách tiếng anh ra khỏi câu nhờ dấu ':'
	wstring english;

	for (int i = 0; i < line.find(':'); i++)
		english += line[i];

	// viết hoa đầu dòng
	english[0] = toupper(english[0]);
	return english;
}
wstring getVietnamse(wstring line) { // tách tiếng việt ra khỏi câu
	wstring vietnamse;

	for (int i = line.find(':') + 2; i < (unsigned int)line.length(); i++)
		vietnamse += line[i];

	// viết hoa đầu dòng, nhưng chưa bao gồm tiếng việt
	vietnamse[0] = toupper(vietnamse[0]);
	return vietnamse;
}
void inCauHoi(wstring dapAnCauHoi, wstring dapAn[]) {
	// xoá điểm cũ, in điểm mới
	gotoxy(13, 1);
	wcout << "               ";
	gotoxy(13, 1);
	set_color(15, 8);
	wcout << score;

	// xoá câu hỏi, đáp án trước
	gotoxy(2, 3);
	for (int i = 1; i < 100; i++) {
		wcout << ' ';
	}
	deleteDapan();

	// in câu hỏi tiếng anh
	set_color(15, 3);
	wstring english = getEnglish(dapAnCauHoi);
	gotoxy(51 - english.length() / 2, 3); wcout << english;

	// in lần lượt mỗi lần 2 đáp án theo chiều ngang
	set_color(15, 0);
	wstring vietnam;
	for (int i = 0, y = 9; i < 4; i += 2, y += 5) {
		vietnam = getVietnamse(dapAn[i]);
		gotoxy(27 - vietnam.length() / 2, y); wcout << vietnam;

		vietnam = getVietnamse(dapAn[i + 1]);
		gotoxy(76 - vietnam.length() / 2, y); wcout << vietnam;
	}
	
	return;
}

// Hàm vẽ giao diện
void set_color(int a, int b) {
	int x  = a * 16 + b;
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(color, x);
}
void gotoxy(short x, short y) {
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}
void veBox(int x1, int y1, int x2, int y2) {
	// ngang trên
	gotoxy(x1 , y1); wcout << L'┌';
	for (int i = x1+1; i < x2; i++)
		wcout << L"─";
	gotoxy(x2, y1); wcout << L'┐';

	// 2 dọc
	for (int i = y1 + 1; i < y2; i++) {
		gotoxy(x1, i); wcout << L'│';
		gotoxy(x2, i); wcout << L'│';
	}

	//  ngang dưới
	gotoxy(x1, y2); wcout << L'└';
	for (int i = x1+1; i < x2; i++)
		wcout << L"─";
	gotoxy(x2, y2); wcout << L'┘';

	return;
}
void veBox2Vien(int x1, int y1, int x2, int y2) {

	// ngang trên
	gotoxy(x1, y1); wcout << L'╔';
	for (int i = x1 + 1; i < x2; i++) wcout << L"═";
	gotoxy(x2, y1); wcout << L'╗';

	// 2 dọc
	for (int i = y1 + 1; i < y2; i++) {
		gotoxy(x1, i); wcout << L'║';
		gotoxy(x2, i); wcout << L'║';
	}

	//  ngang dưới
	gotoxy(x1, y2); wcout << L'╚';
	for (int i = x1 + 1; i < x2; i++) wcout << L"═";
	gotoxy(x2, y2); wcout << L'╝';

	return;
}
void veVien() {
	veBox(1, 0, 101, 29);
	return;
}
void veVienPlayWindows() {

	// viền câu hỏi, điểm, thời gian
	veBox2Vien(1, 0, 101, 6);

	// in điểm
	set_color(15, 0);
	gotoxy(6, 1); wcout << L"Score: ";
	set_color(15, 8);
	wcout << score;
	
	// In 4 chữ đại diện đáp án
	set_color(15, 2); 
	// A
	gotoxy(3, 8);  wcout << L"╔═╗";
	gotoxy(3, 9);  wcout << L"╠═╣";
	gotoxy(3, 10); wcout << L"╩ ╩";
	// B
	gotoxy(97, 8);  wcout << L"╔╗";
	gotoxy(97, 9);  wcout << L"╠╩╗";
	gotoxy(97, 10); wcout << L"╚═╝";
	// C
	gotoxy(3, 13); wcout << L"╔═╗";
	gotoxy(3, 14); wcout << L"║";
	gotoxy(3, 15); wcout << L"╚═╝";
	// D
	gotoxy(97, 13); wcout << L"╔╦╗";
	gotoxy(97, 14); wcout << L" ║║";
	gotoxy(97, 15); wcout << L"═╩╝";


	// kẻ ô 4 đáp án
	set_color(15, 8);
	veBox(1, 7, 49, 11);
	veBox(51, 7, 101, 11);
	veBox(1, 12, 49, 16);
	veBox(51, 12, 101, 16);
}
void presentMouseLocation(int now) {
	int x = 0, y = 0, s = 0;
	set_color(15, 8);
	switch (now) {
		case 0:
			x = 48;
			y = 15;
			s = 7;
			break;
		case 1:
			x = 45;
			y = 17;
			s = 13;
			break;
		case 2:
			x = 47;
			y = 19;
			s = 9;
			break;
		case 3:
			x = 48;
			y = 21;
			s = 7;
			break;
		case 4: 
			x = 49;
			y = 23;
			s = 5;
			break;
	}

	// vẽ 2 dấu mũi tên menu
	gotoxy(x - 3, y); wcout << char(16);
	gotoxy(x + 2 + s, y); wcout << char(17);

	return;
}

// Hàm xoá giao diện
void deleteThanhThongBao() {
	for (int i = 20; i <= 28; i++) {
		deleteLine(i);
	}
}
void deleteLine(int line) {
	gotoxy(2, line);
	for (int i = 1; i < 100; i++)
		wcout << ' ';
}
void deleteMouseLocation(int i) {

	// vị trí của các mục trong menu theo thứ tự. Mảng 1 : x, mảng 2 y, mảng 3 số kí tự
	int arr[3][5] = { { 48, 45, 47, 48, 49 }, { 15, 17, 19, 21, 23 }, {7, 13, 9, 7, 5} };

	//xoá mũi tên
	gotoxy(arr[0][i] - 3, arr[1][i]); wcout << ' ';
	gotoxy(arr[0][i] + 2 + arr[2][i], arr[1][i]); wcout << ' ';

	return;
} 
void deleteDapan() {
	gotoxy(6, 9); for (int i = 0; i <= 42; i++) wcout << ' ';
	gotoxy(6, 14); for (int i = 0; i <= 42; i++) wcout << ' ';
	gotoxy(53, 9); for (int i = 0; i <= 43; i++) wcout << ' ';
	gotoxy(53, 14); for (int i = 0; i <= 43; i++) wcout << ' ';

	return;
}

// Thiết đặt cửa sổ
void SetWindowSize(SHORT width, SHORT height) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	SMALL_RECT WindowSize{};
	WindowSize.Top = 0;
	WindowSize.Left = 0;
	WindowSize.Right = width;
	WindowSize.Bottom = height;

	SetConsoleWindowInfo(hStdout, 1, &WindowSize);
}
void DisableResizeWindow() {
	HWND hWnd = GetConsoleWindow();
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}
void DisableCtrButton(bool Close, bool Min, bool Max) {
	HWND hWnd = GetConsoleWindow();
	HMENU hMenu = GetSystemMenu(hWnd, false);

	if (Close == 1) {
		DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
	}
	if (Min == 1) {
		DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
	}
	if (Max == 1) {
		DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
	}
}
void ShowScrollbar(BOOL Show) {
	HWND hWnd = GetConsoleWindow();
	ShowScrollBar(hWnd, SB_BOTH, Show);
}
void ShowCur(int i)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConCurInf;

	ConCurInf.dwSize = 10;
	ConCurInf.bVisible = i;

	SetConsoleCursorInfo(handle, &ConCurInf);
}
void DisableSelection()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleMode(hStdin, ~ENABLE_QUICK_EDIT_MODE);
}
void EnableSelection()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleMode(hStdin, ENABLE_QUICK_EDIT_MODE);
}

// Các cửa sổ
void intro1() {
	// xoá, viền, màu nền
	system("cls");
	veVien();
	system("color fB");

	gotoxy(10, 10); wcout << L"████████╗██╗  ██╗ █████╗ ███╗   ██╗██╗  ██╗     ██████╗ █████╗ ███╗   ██╗██╗  ██╗";
	gotoxy(10, 11); wcout << L"╚══██╔══╝██║  ██║██╔══██╗████╗  ██║██║  ██║    ██╔════╝██╔══██╗████╗  ██║██║  ██║";
	gotoxy(10, 12); wcout << L"   ██║   ███████║███████║██╔██╗ ██║███████║    ██║     ███████║██╔██╗ ██║███████║";
	gotoxy(10, 13); wcout << L"   ██║   ██╔══██║██╔══██║██║╚██╗██║██╔══██║    ██║     ██╔══██║██║╚██╗██║██╔══██║";
	gotoxy(10, 14); wcout << L"   ██║   ██║  ██║██║  ██║██║ ╚████║██║  ██║    ╚██████╗██║  ██║██║ ╚████║██║  ██║";
	gotoxy(10, 15); wcout << L"   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝     ╚═════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝";
	gotoxy(85, 17); wcout << L"Đẹp trai";
	gotoxy(0, 0);

	Sleep(800);
	return; 
}
void nameSoftware() {
	// xoá, đặt màu nề màu chữ, vẽ viền
	system("cls");
	system("color fC");
	veVien();

	wstring line;
	int num_line = 6;
	//int col = 12; >= 17 col = 34

	gotoxy(12, 6); wcout << L"▄▄▄█████▓ █    ██     ██▒   █▓ █    ██  ███▄    █   ▄████ ";
	gotoxy(12, 7); wcout << L"▓  ██▒ ▓▒ ██  ▓██▒   ▓██░   █▒ ██  ▓██▒ ██ ▀█   █  ██▒ ▀█▒";
	gotoxy(12, 8); wcout << L"▒ ▓██░ ▒░▓██  ▒██░    ▓██  █▒░▓██  ▒██░▓██  ▀█ ██▒▒██░▄▄▄░";
	gotoxy(12, 9); wcout << L"░ ▓██▓ ░ ▓▓█  ░██░     ▒██ █░░▓▓█  ░██░▓██▒  ▐▌██▒░▓█  ██▓";
	gotoxy(12, 10); wcout << L"  ▒██▒ ░ ▒▒█████▓       ▒▀█░  ▒▒█████▓ ▒██░   ▓██░░▒▓███▀▒";
	gotoxy(12, 11); wcout << L"  ▒ ░░   ░▒▓▒ ▒ ▒       ░ ▐░  ░▒▓▒ ▒ ▒ ░ ▒░   ▒ ▒  ░▒   ▒ ";
	gotoxy(12, 12); wcout << L"    ░    ░░▒░ ░ ░       ░ ░░  ░░▒░ ░ ░ ░ ░░   ░ ▒░  ░   ░ ";
	gotoxy(12, 13); wcout << L"  ░       ░░░ ░ ░         ░░   ░░░ ░ ░    ░   ░ ░ ░ ░   ░ ";
	gotoxy(12, 14); wcout << L"            ░              ░     ░              ░       ░ ";
	gotoxy(12, 15); wcout << L"                          ░                              ";
	gotoxy(34, 17); wcout << L"▓█████  ███▄    █   ▄████  ██▓     ██▓  ██████  ██░ ██ ";
	gotoxy(34, 18); wcout << L"▓█   ▀  ██ ▀█   █  ██▒ ▀█▒▓██▒    ▓██▒▒██    ▒ ▓██░ ██▒";
	gotoxy(34, 19); wcout << L"▒███   ▓██  ▀█ ██▒▒██░▄▄▄░▒██░    ▒██▒░ ▓██▄   ▒██▀▀██░";
	gotoxy(34, 20); wcout << L"▒▓█  ▄ ▓██▒  ▐▌██▒░▓█  ██▓▒██░    ░██░  ▒   ██▒░▓█ ░██ ";
	gotoxy(34, 21); wcout << L"░▒████▒▒██░   ▓██░░▒▓███▀▒░██████▒░██░▒██████▒▒░▓█▒░██▓";
	gotoxy(34, 22); wcout << L"░░ ▒░ ░░ ▒░   ▒ ▒  ░▒   ▒ ░ ▒░▓  ░░▓  ▒ ▒▓▒ ▒ ░ ▒ ░░▒░▒";
	gotoxy(34, 23); wcout << L" ░ ░  ░░ ░░   ░ ▒░  ░   ░ ░ ░ ▒  ░ ▒ ░░ ░▒  ░ ░ ▒ ░▒░ ░";
	gotoxy(34, 24); wcout << L"   ░      ░   ░ ░ ░ ░   ░   ░ ░    ▒ ░░  ░  ░   ░  ░░ ░";
	gotoxy(34, 25); wcout << L"   ░  ░         ░       ░     ░  ░ ░        ░   ░  ░  ░                                                      ";

	veVien();
	Sleep(1000);
	system("cls");
	
	return;
}
void mainMenu() {
	// xoá, in
	DisableSelection();
	system("cls");
	set_color(15, 3);
	veVien();

	//int num_line = 2; 30
	gotoxy(30, 3); wcout << L"   ███╗   ███╗███████╗███╗   ██╗██╗   ██╗";
	gotoxy(30, 4); wcout << L"   ████╗ ████║██╔════╝████╗  ██║██║   ██║";
	gotoxy(30, 5); wcout << L"   ██╔████╔██║█████╗  ██╔██╗ ██║██║   ██║";
	gotoxy(30, 6); wcout << L"   ██║╚██╔╝██║██╔══╝  ██║╚██╗██║██║   ██║";
	gotoxy(30, 7); wcout << L"   ██║ ╚═╝ ██║███████╗██║ ╚████║╚██████╔╝";
	gotoxy(30, 8); wcout << L"   ╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝ ╚═════╝ ";

	// In menu
	gotoxy(48, 15); wcout << L"Bắt đầu";
	gotoxy(45, 17); wcout << L"Bảng xếp hạng";
	gotoxy(47, 19); wcout << L"Hướng dẫn";
	gotoxy(48, 21); wcout << L"Tác giả";
	gotoxy(49, 23); wcout << L"Thoát";

	// Vẽ con trỏ
	static int choose = 0; // thêm static để sau khi thoát khỏi 1 cửa sổ khác thì con trỏ vẫn kh đổi về 0
	presentMouseLocation(choose);

	// Xử lí nhập
	char mouse;
	bool enter = false;
	while (!enter) {
		mouse = _getch();
		
		if (_kbhit) {
			deleteMouseLocation(choose);
			switch (mouse) {
				case 72:
				case 'w':
					choose--;
					break;
				case 80:
				case 's':
					choose++;
					break;
				case 13:
					enter = true;
					switch (choose) {
						case 0:
							playWindows();
							break;
						case 1:
							inHighScore(-1);
							break;
						case 2:
							huongDan();
							break;
						case 3:
							thongTin();
							break;
						case 4:
							system("cls");
							exit(0);
							break;
					}
			}
			
		}

		// giới hạn vùng trỏ
		if (choose > 4) choose -= 5;
		if (choose < 0) choose += 5;
		presentMouseLocation(choose);
	}

	return;
}
void playWindows() {
	// xoá màn hình trước, in viền
	system("cls");
	veVienPlayWindows();

	// Reset score
	score = 0;
	double total_time = 0;

	// sinh mạng
	bool life = true;

	// Còn thở là còn gỡ
	while (life == true) {

		// khởi tạo đáp án
		wstring dapAn[5];
		for (int i = 0; i < 4; i++) {
			// chắc rằng không có cái nào trùng nhau
			bool trung = false;
			do {
				trung = false;
				dapAn[i] = getRandomLine();

				if (dapAn[i].empty()) trung = true;
				for (int j = i; j > 0; j--)
					if (dapAn[i] == dapAn[j - 1]) trung = true;
			} while (trung == true);
		}

		// chọn ngẫu nhiên một đáp án để hỏi
		wstring dapAnCauHoi = dapAn[rand() % 4];

		// in giao diện câu hỏi
		inCauHoi(dapAnCauHoi, dapAn);

		// Nhận đáp án từ người dùng
		gotoxy(40, 19); wcout << L"Đáp án của bạn là :  ";
		char reply = 'o';

		// không nhập sẽ thua
		life = false;

		// in thời gian ban đầu
		set_color(15, 0);
		gotoxy(84, 1); wcout << L"Thời gian : ";
		int time = 10;
		set_color(15, 2);
		gotoxy(96, 1); wcout << time;

		// nhận đáp án từ người dùng mỗi 0.1s, mỗi một giây in lại time
		int count = 1;
		while (time > 0) {
			//dừng 0.1 s
			total_time += 0.1;
			count++;
			Sleep(100);

			if (count% 10 == 0) {
				// vì 10 - 0 + 1 = 11 nên để v cho nó chuẩn
				count -= 8;
				time--;
				if (time >= 7) set_color(15, 2);
				else if (time >= 3) set_color(15, 6);
				else set_color(15, 4);
				gotoxy(96, 1); wcout << "  ";
				gotoxy(96, 1); wcout << time;
				Sleep(100);
			}

			// nếu ng dùng nhập cái gì đó sẽ đem đi xử lí và thoát khỏi đây
			if (_kbhit()) {
				reply = _getch();
				life = true; // đã ấn nên tạm thời cho sống lại
				break;
			}
		}

		// Xử lí kết quả
		int indexReply;
		set_color(15, 0);
		gotoxy(60, 19);

		switch (reply) {
		case '1':
		case 'a':
		case 'A':
			indexReply = 0;
			wcout << 'A';
			break;
		case '2':
		case 'b':
		case 'B':
			indexReply = 1;
			wcout << 'B';
			break;
		case '3':
		case 'c':
		case 'C':
			indexReply = 2;
			wcout << 'C';
			break;
		case '4':
		case 'd':
		case 'D':
			indexReply = 3;
			wcout << 'D';
			break;
		default:
			indexReply = 4;
			deleteLine(19);

			if (reply == 'o' || reply == '\n') {
				set_color(15, 8);
				gotoxy(46, 19); 
				wcout << L"Hết giờ!!"; 
			}
			else {
				set_color(15, 8); 
				gotoxy(27, 18); 
				if (reply != 13) wcout << reply; // loại trừ phím enter
				wcout << L" là đáp án nào ?! Cho game over luôn cho biết ha"; }
			break;
		}

		// kiểm tra đúng sai
		if (getVietnamse(dapAn[indexReply]) == getVietnamse(dapAnCauHoi)) {
			// nếu đúng + điểm chơi tiếp
			set_color(15, 2);
			gotoxy(28, 21); wcout << L"Bạn đã trả lời đúng, bạn được cộng 100 điểm !!";
			
			if (score == 0) gotoxy(15, 1); else gotoxy(17 + score / 1000, 1); // hiển thị cho chính xác
			wcout << L" +  100";
			score += 100;

			// đợi 1 chút r xoá 1 số chỗ
			Sleep(600);
			gotoxy(17 + score / 1000, 1); wcout << "        ";
			deleteThanhThongBao();
		}
		else { // Trả lời sai thì kết thúc
			life = false;
			DisableSelection();
			// in ra đáp án chính xác
			wstring dap_an = getVietnamse(dapAnCauHoi);
			set_color(15, 0);
			gotoxy(50 - (dap_an.length()+22) / 2, 22); 
			wcout << L"Đáp án đúng phải là ";
			set_color(15, 2);
			wcout << "\"" << dap_an << "\"";

			// Thông báo số câu trả lời đúng
			set_color(15, 8);
			if (score / 100 >= 5) {
				gotoxy(27, 23); 
				wcout << L"Chúc mừng bạn đã trả lời đúng " << score / 100 << L" câu liên tiếp !!!";
			}

			// Xem có nằm trong bảng xếp hạng không
			bool inTop = false;
			if (score != 0) // nếu có 1 câu đúng trở lên
			{
				// đọc file điểm
				locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
				wifstream high_score(L"hs.canh");
				high_score.imbue(loc);

				NameAndScore Top[10];
				for (int i = 0; i < 10; i++) high_score >> Top[i].name >> Top[i].score >> Top[i].time;
				high_score.close();
				for (int i = 0; i < 10; i++) {
					// kiểm tra xem top mấy, duyệt từ top 1 tới 10
					bool check = false;
					if (score > Top[i].score) check = true;
					if (score == Top[i].score && total_time < Top[i].time) check = true;

					// nếu đã có vị trí trên bảng xếp hạng
					if (check) {
						inTop = true;
						

						// đẩy bảng xếp hạng xuống để lấy chỗ
						for (int j = 9; j > i; j--) {
							Top[j] = Top[j - 1];
						}

						// in ra thông báo để lấy thông tin người chơi
						deleteLine(24);
						set_color(15, 3);
						if (Top[i].name != L"None") {
							gotoxy(35, 26); wcout << L"Bạn vừa đánh bại kỷ lục của " << Top[i].name << " !!";
						}
						else {
							gotoxy(40, 26); wcout << L"Điểm cũng cao đấy";
						}
						
						gotoxy(35, 27); wcout << L"Xin hãy cho biết quý danh : ";
						wstring player_name =L"";
						wcin >> player_name;
						
						// lưu thông tin người chơi
						Top[i].name = player_name;
						Top[i].score = score;
						Top[i].time = total_time;

						// lưu file highscore mới
						wofstream fout(L"hs.canh");
						fout.imbue(loc);
						for (int i = 0; i < 10; i++) fout << Top[i].name << " " << Top[i].score << " " << setprecision(2) << Top[i].time << endl;
						fout.close();

						// in bảng xếp hạng và thoát lặp
						inHighScore(i);
						break;
					}
				}

			}

			// nếu không trong top
			if (!inTop) {
				set_color(15, 12);
				gotoxy(45, 25); wcout << L"GAME OVER";

				// hỏi ng ta muốn chơi lại không
				set_color(15, 8);
				gotoxy(35, 26); wcout << L"Bạn có muốn thử lại không? (Y/N)";
				reply = _getch();
				if (reply == 'Y' or reply == 'y' or reply == 13) playWindows();
				else mainMenu();
			}
		}
	}
}
void inHighScore(int current = -1) {
	// vẽ viền
	system("cls");
	set_color(15, 3);
	veVien();

	// vẽ chữ high score to
	set_color(15, 0);
	gotoxy(25, 1); wcout << L"  __    __       __ ";
	gotoxy(25, 2); wcout << L" |  |--|__.-----|  |--.  .-----.----.-----.----.-----.";
	gotoxy(25, 3); wcout << L" |     |  |  _  |     |  |__ --|  __|  _  |   _|  -__|";
	gotoxy(25, 4); wcout << L" |__|__|__|___  |__|__|  |_____|____|_____|__| |_____|";
	gotoxy(25, 5); wcout << L"          |_____|";

	// đọc dữ liệu trong file
	locale loc(std::locale(), new std::codecvt_utf8<wchar_t>);
	wifstream highScore("hs.canh");
	highScore.imbue(loc);

	NameAndScore nameAndScore[10];
	for (int i = 0, j = 8; i < 10; i++, j += 2)
	{	
		if (i == 0) set_color(15, 3); else set_color(15, 8); // nếu top 1 đổi thành màu xanh

		highScore >> nameAndScore[i].name >> nameAndScore[i].score >> nameAndScore[i].time; // truyền từ file ra

		//in ra dòng đó
		gotoxy(40, j); wcout << L"Top " << i + 1 << L" ";
		gotoxy(47, j); wcout << nameAndScore[i].name << L" " << nameAndScore[i].score << L" (" << nameAndScore[i].time << L"s)";

		// vẽ 2 cái chim
		if (i == 0) {
			set_color(15, 4); 
			wcout << L" ♥";
			gotoxy(38, j); 
			wcout << L"♥";
		}
	}
	highScore.close();

	set_color(15, 12);
	gotoxy(38, 28); wcout << L"Ấn P để reset bảng xếp hạng"; gotoxy(0, 0);

	if (current != -1)
	{
		while (true)
		{
			int hang = current * 2 + 8; // xác định vị trí tên ng chơi
			
			// thiết đặt màu chữ
			if (current == 0) set_color(15, 3);
			else set_color(15, 6);

			// nháy điểm hiện tại
			Sleep(350);
			deleteLine(hang);
			Sleep(350);
			gotoxy(40, hang); wcout << L"Top " << current + 1 << L"\t";
			gotoxy(47, hang); wcout << nameAndScore[current].name << L" " << nameAndScore[current].score << L" (" << nameAndScore[current].time << L"s)";

			// rời khỏi đó nếu ng chơi ấn phím bất kỳ
			if (_kbhit())
			{
				break;
			}

		}
	}
	
	set_color(15, 0);
	char reply;
	reply = _getch();

	if (reply == 'P' || reply == 'p') {
		// sửa file
		wofstream highScore("hs.canh");
		highScore.imbue(loc);
		// reset all
		for (int i = 0; i < 10; i++) {
			highScore << L"Nobody" << ' ' << 0 << ' ' << 0 << endl;
		}
		
		// in thông báo và bảng xếp hạng sau khi reset
		deleteLine(28);
		highScore.close();
		set_color(15, 12);
		gotoxy(41, 28); wcout << L"Đã xoá bảng xếp hạng!!!"; gotoxy(0, 0);
		Sleep(500);
		inHighScore(-1);
	}
	
	mainMenu(); // trở về menu
	return;
}
void huongDan() {
	system("cls");
	set_color(15, 3);
	veVien();

	set_color(15, 8);
	gotoxy(4, 2); wcout << L"Bạn có thể sử dụng các phím số {1, 2, 3, 4} hoặc các chữ như {a, b, c, d} để nhập vào đáp án.";
	gotoxy(4, 4); wcout << L"Với mỗi câu trả lời đúng bạn sẽ được 100đ.";
	gotoxy(4, 6); wcout << L"Mỗi câu hỏi đều có 1 đáp án đúng, vì vậy bạn hãy đưa ra đáp án đúng trong vòng 5 giây.";
	gotoxy(4, 8); wcout << L"Hãy ghi tên mình lên bảng xếp hạng trong trò chơi này và có được thêm nhiều kiến thức nhé ^^.";
	gotoxy(4, 12); wcout << L"Bạn có thể thay đổi từ vựng bằng cách thêm vào file \"lib.canh\" theo cấu trúc sau: ";
	gotoxy(4, 13); wcout << L"Mỗi dòng là 1 từ vựng khác nhau dưới dạng <tiếng anh> : <tiếng việt>.";
	
	gotoxy(26, 15); wcout << L" Ví dụ:";
	veBox(31, 16, 60, 21);
	gotoxy(35, 17); wcout << L"map : Bản đồ";
	gotoxy(35, 18); wcout << L"government : Chính phủ";
	gotoxy(35, 19); wcout << L"way : Đường";
	gotoxy(35, 20); wcout << L"art : Nghệ thuật";

	(void)_getch();

	// về menu
	mainMenu();
	return;
}
void thongTin()
{
	system("cls");
	set_color(15, 3);
	veVien();

	// in ra
	set_color(15, 8);
	EnableSelection();
	gotoxy(4, 2); wcout << L"Xin chào, Cảm ơn bạn vì đã tải và sử dụng phần mềm của mình"; set_color(15, 4); wcout << L" ♥♥♥"; set_color(15, 8);
	gotoxy(4, 4); wcout << L"Mình là Nguyễn Thanh Cảnh 2k3 từ Gia Lai và là sinh viên K17 IUH.";
	gotoxy(4, 6); wcout << L"Hồi mình học lớp 12, mình cần 1 phần mềm để học các từ vựng trong SGK nhưng tìm không thấy, nên";
	gotoxy(4, 7); wcout << L"mình quyết định làm 1 cái theo ý của mình, qua nhiều phiên bản sửa đổi, bổ sung và với sự giúp";
	gotoxy(4, 8); wcout << L"sức của bạn Nguyễn Đình Nguyên Bắc thì phần mềm đang ngày càng được tốt hơn (nhưng vẫn dởm =))";
	gotoxy(4, 10); wcout << L"Nếu bạn muốn góp ý hoặc trao đổi với mình, hãy làm điều đó qua các phương tiện như:";
	gotoxy(4, 12); wcout << L"	Facebook : https://www.facebook.com/canh.like.programming/";
	gotoxy(4, 13); wcout << L"	Email	 : thanhcanh203c1@gmail.com";
	gotoxy(4, 14); wcout << L"	Hoặc     : 21086671.canh@student.iuh.edu.vn";

	(void)_getch();

	//về lại menu
	DisableSelection();
	mainMenu();
	return;
}