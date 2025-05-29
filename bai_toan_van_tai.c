#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define MAX 100
#define INTMAX 2147483647

#define Vang     14
#define DoNhat   12
#define XanhNhat 11
#define DoDam    4

int m, n, Cung[MAX], Cau[MAX], ChiPhi[MAX][MAX], PhanPhoi[MAX][MAX], KhongCanBang = 0;
int Cha[MAX * MAX] = {0}, sz[MAX * MAX] = {0}, ODaChon[MAX][MAX];

// định hướng cải tiến: thêm lựa chọn nhập vào file hoặc nhập = tay
// nhập bằng tay sẽ chọn = cách có dấu [ ] để nhập vào 

// thuật toán;
void XuLyCanBang();
void PhuongPhapGocTayBac();
void PhuongPhapChiPhiNhoNhat();
void PhuongPhapVogel();
void KhoiTao();
int Tim(int i); // tìm chu trình
int KtraNoi(int x, int y);
void XuLySuyBien(int SoOChon);
void TimDuongDi(int start, int end, int MangKq[], int SoLuong, int DaTham[], int *SoLuongThat, int *DaTimThay);
void BienDoi(int MangKq[], int SoLuong);
void PhuongPhapTheVi();

// đồ họa
void ToaDoxy(int x, int y); // di chuyển con trỏ tới vị trí (x, y)
void Mau(int color); // đặt màu kí tự
void AnCursor(); // ẩn con trỏ nhấp nháy
void VeKhung(int x, int y, int width, int height); // vẽ khung bằng ký tự ASCII
void NhapNhay(int x, int y, const char* text, int blinkCount) ; // nhấp nháy dòng được chọn
void CachNhap();
void Nhap();
void NhapTay();
int Menu();
int LuaChon();
void Xuat(int CongY);

int main() {
    AnCursor();
    CachNhap();
    while(1) {
        if (Menu()) return 0;
        Xuat(0);
        PhuongPhapTheVi();
        Mau(DoNhat);
        ToaDoxy(43, 22); printf("PHƯƠNG ÁN ĐÃ TỐI ƯU");
        Xuat(14);
        if (LuaChon()) return 0;
    }
    return 0;
}

void XuLyCanBang() {
    int tongCung = 0, tongCau = 0;
    for (int i = 0; i < m; i++) {tongCung += Cung[i];}
    for (int j = 0; j < n; j++) {tongCau += Cau[j];}
    if (tongCung == tongCau) return;
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    Mau(DoNhat);
    printf("\nTổng cung (%d) ≠ Tổng cầu (%d). Tự động cân bằng...\n", tongCung, tongCau);
    if (tongCung > tongCau) {
        Cau[n] = tongCung - tongCau;
        printf("Đã thêm điểm cầu giả %d với lượng cầu %d\n\n", n+1, Cau[n]);
        for (int i = 0; i < m; i++) {
            ChiPhi[i][n] = 0;
        }
        n++;
        KhongCanBang = 1;
    }
    else {
        Cung[m] = tongCau - tongCung;
        printf("Đã thêm nguồn cung giả %d với lượng cung %d\n\n", m+1, Cung[m]);
        for (int j = 0; j < n; j++) {
            ChiPhi[m][j] = 0;
        }
        m++;
        KhongCanBang = 2;
    }
}

void Nhap() {
    SetConsoleOutputCP(65001);
    scanf("%d", &m);
    scanf("%d", &n);
    for (int i = 0; i < m; i++) {
        scanf("%d", &Cung[i]);
    }
    for (int j = 0; j < n; j++) {
        scanf("%d", &Cau[j]);
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &ChiPhi[i][j]);
        }
    }
    system("cls");
    XuLyCanBang();
}

void NhapTay() {
    Mau(14); // đặt cho chữ
    ToaDoxy(19 + 13, 1); printf("Nhập vào số lượng CUNG và CẦU: ");
    ToaDoxy(31, 4); printf("CUNG");
    ToaDoxy(57, 4); printf("CẦU");
    VeKhung(30, 5, 7, 3);
    VeKhung(55, 5, 7, 3);
    int currentPos = 0; // 0 là ô m, 1 là ô n;
    char input[10] = {0}; // Bộ đệm nhập liệu
    int inputIndex = 0;
    Mau(DoNhat); // đặt cho chữ  
    ToaDoxy(31, 6); printf("[   ]");

    while (1) {
        if (m != 0 && n != 0) break;
        char key = _getch();

        // Xóa dấu [ ] hiện tại
        ToaDoxy(31 + currentPos * 25, 6); // 25 là khoảng cách giữa 2 ô
        printf("     "); // Xóa [ ] 
        // Cập nhật lại số (nếu có)
        ToaDoxy(31 + currentPos * 25 + 1, 6);
        if (currentPos == 0 && m != 0) {printf("%d", m);}
        if (currentPos == 1 && n != 0) {printf("%d", n);}

        // Xử lý phím
        if (key == 0 || key == -32) { // Phím đặc biệt (mũi tên)
            key = _getch();
            if (key == 75 && currentPos > 0) {currentPos--;} // Trái
            if (key == 77 && currentPos < 1) {currentPos++;} // Phải
            inputIndex = 0;
            input[0] = '\0';
        }
        PLACE1:
        ToaDoxy(31 + currentPos * 25, 6);
        Mau(4); // Màu đỏ cho dấu [ ]
        printf("[   ]");
        Mau(7); // Trở lại màu trắng

        // Nhập số
        if (key >= '0' && key <= '9' && inputIndex <= 3) {
            if (inputIndex == 3) {
                inputIndex = 0;
                input[0] = '\0';
            }
            input[inputIndex++] = key;
            input[inputIndex] = '\0';
            ToaDoxy(31 + currentPos * 25 + 1, 6);
            printf("%s", input);
        }
        // Nhấn Enter để xác nhận
        else if (key == 13 && inputIndex > 0) {
            if (currentPos == 0) m = atoi(input); // Lưu giá trị m
            else if (currentPos == 1) n = atoi(input); // Lưu giá trị n
            inputIndex = 0;
            input[0] = '\0';
            ToaDoxy(31 + currentPos * 25 + 1, 6);
            printf("%d", currentPos == 0 ? m : n);
            if (currentPos < 1) {
                ToaDoxy(31 + currentPos * 25, 6); 
                printf("     "); 
                ToaDoxy(31 + currentPos * 25 + 1, 6);
                if (currentPos == 0 && m != 0) printf("%d", m);
                if (currentPos == 1 && n != 0) printf("%d", n);
                currentPos++;
                goto PLACE1;
            }
        }
        else if (key == 8 && inputIndex > 0) { // Nếu là dấu backspace thì sẽ xóa 1 kí tự
            input[--inputIndex] = '\0';
            ToaDoxy(31 + currentPos * 25 + 1, 6);
            printf("%s", input);
        }
    }
    system("cls");
    //----------------------------------------------------------------------------------------------
    
    // Nhập vào số hàng trong cung
    Mau(Vang); ToaDoxy(19 + 13, 1); printf("Nhập vào số lượng hàng hóa trong CUNG: ");
    for (int i = 0; i < m; i++) {
        Mau(Vang); ToaDoxy(26 + i * 10, 4); printf("Cung %d", i + 1);
        VeKhung(25 + i * 10, 5, 7, 3);
    }
    currentPos = 0; // đây là chỉ số của mảng cung
    for (int i = 0; i < 10; i++) {input[i] = 0;} // Bộ đệm nhập liệu
    
    inputIndex = 0;
    Mau(DoDam); // đặt cho chữ 
    ToaDoxy(26, 6); printf("[   ]");

    while (1) {
        int Full = 1;
        for (int i = 0; i < m; i++) {
            if (Cung[i] == 0) {Full = 0;}
        }
        if (Full) break;

        char key = _getch();
        // Xóa dấu [ ] hiện tại
        ToaDoxy(26 + currentPos * 10, 6); // 10 là khoảng cách giữa 2 ô
        printf("     "); // Xóa [ ] 
        // Cập nhật lại số (nếu có)
        for (int i = 0; i < m; i++) {
            if (Cung[i] != 0) {
                ToaDoxy(27 + i * 10, 6);
                printf("%d", Cung[i]);
            }
        }
        
        // Xử lý phím
        if (key == 0 || key == -32) { // Phím đặc biệt (mũi tên)
            key = _getch();
            if (key == 75 && currentPos > 0) {currentPos--;} // Trái
            if (key == 77 && currentPos < m - 1) {currentPos++;} // Phải
            inputIndex = 0;
            input[0] = '\0';
        }
        PLACE2:
        ToaDoxy(26 + currentPos * 10, 6);
        Mau(4); // Màu đỏ cho dấu [ ]
        printf("[   ]");
        Mau(7); // Trở lại màu trắng

        // Nhập số
        if (key >= '0' && key <= '9' && inputIndex <= 3) {
            if (inputIndex == 3) {
                inputIndex = 0;
                input[0] = '\0';
            }
            input[inputIndex++] = key;
            input[inputIndex] = '\0';
            ToaDoxy(27 + currentPos * 10, 6);
            printf("%s", input);
        }
        // Nhấn Enter để xác nhận
        else if (key == 13 && inputIndex > 0) {
            int i = 0;
            while (i != currentPos) {i++;}
            Cung[i] = atoi(input);
            
            inputIndex = 0;
            input[0] = '\0';
            ToaDoxy(27 + currentPos * 10, 6);
            printf("%d", Cung[i]);
            if (currentPos < m - 1) {
                ToaDoxy(26 + currentPos * 10, 6); // 10 là khoảng cách giữa 2 ô
                printf("     "); // Xóa [ ] 
                // Cập nhật lại số (nếu có)
                for (int i = 0; i < m; i++) {
                    if (Cung[i] != 0) {
                        ToaDoxy(27 + i * 10, 6);
                        printf("%d", Cung[i]);
                    }
                }
                currentPos++;
                goto PLACE2;    
            }
        }
        else if (key == 8 && inputIndex > 0) { // Nếu là dấu backspace thì sẽ xóa 1 kí tự
            input[--inputIndex] = '\0';
            ToaDoxy(27 + currentPos * 10, 6);
            printf("%s", input);
        }
    }
    system("cls");
    //----------------------------------------------------------------------------------------------

    // Nhập số lượng hàng hóa trong cầu
    Mau(Vang); ToaDoxy(19 + 13, 1); printf("Nhập vào số lượng hàng hóa trong CAU: ");
    for (int i = 0; i < n; i++) {
        Mau(Vang); ToaDoxy(26 + i * 10, 4); printf("Cau %d", i + 1);
        VeKhung(25 + i * 10, 5, 7, 3);
    }
    currentPos = 0; // đây là chỉ số của mảng Cau
    for (int i = 0; i < 10; i++) {input[i] = 0;} // Bộ đệm nhập liệu
    inputIndex = 0;
    Mau(DoDam); // đặt cho chữ 
    ToaDoxy(26, 6); printf("[   ]");

    while (1) {
        int Full = 1;
        for(int i = 0; i < n; i++) {
            if (Cau[i] == 0) {Full = 0;}
        }
        if (Full) break;

        char key = _getch();
        // Xóa dấu [ ] hiện tại
        ToaDoxy(26 + currentPos * 10, 6); // 10 là khoảng cách giữa 2 ô
        printf("     "); // Xóa [ ] 
        // Cập nhật lại số (nếu có)
        for (int i = 0; i < n; i++) {
            if (Cau[i] != 0) {
                ToaDoxy(27 + i * 10, 6);
                printf("%d", Cau[i]);
            }
        }
        
        // Xử lý phím
        if (key == 0 || key == -32) { // Phím đặc biệt (mũi tên)
            key = _getch();
            if (key == 75 && currentPos > 0) {currentPos--;} // Trái
            if (key == 77 && currentPos < n - 1) {currentPos++;} // Phải
            inputIndex = 0;
            input[0] = '\0';
        }
        PLACE3:
        ToaDoxy(26 + currentPos * 10, 6);
        Mau(4); // Màu đỏ cho dấu [ ]
        printf("[   ]");
        Mau(7); // Trở lại màu trắng

        // Nhập số
        if (key >= '0' && key <= '9' && inputIndex <= 3) {
            if (inputIndex == 3) {
                inputIndex = 0;
                input[0] = '\0';
            }
            input[inputIndex++] = key;
            input[inputIndex] = '\0';
            ToaDoxy(27 + currentPos * 10, 6);
            printf("%s", input);
        }
        // Nhấn Enter để xác nhận
        else if (key == 13 && inputIndex > 0) {
            int i = 0;
            while (i != currentPos) {i++;}
            Cau[i] = atoi(input);
            
            inputIndex = 0;
            input[0] = '\0';
            ToaDoxy(27 + currentPos * 10, 6);
            printf("%d", Cau[i]);
            if (currentPos < n - 1) {
                ToaDoxy(26 + currentPos * 10, 6); // 10 là khoảng cách giữa 2 ô
                printf("     "); // Xóa [ ] 
                // Cập nhật lại số (nếu có)
                for (int i = 0; i < n; i++) {
                    if (Cau[i] != 0) {
                        ToaDoxy(27 + i * 10, 6);
                        printf("%d", Cau[i]);
                    }
                }
                currentPos++;
                goto PLACE3;    
            }
        }
        else if (key == 8 && inputIndex > 0) { // Nếu là dấu backspace thì sẽ xóa 1 kí tự
            input[--inputIndex] = '\0';
            ToaDoxy(27 + currentPos * 10, 6);
            printf("%s", input);
        }
    }
    system("cls");
    //----------------------------------------------------------------------------------------------

    // Nhập số chi phí
    Mau(Vang); ToaDoxy(35, 1); printf("Nhap ma tran chi phi : ");
    for (int i = 0; i < m; i++) {
        Mau(Vang); ToaDoxy(18, 6 + i * 3); printf("Cung %d", i + 1);
        for (int j = 0; j < n; j++) {
            Mau(Vang); ToaDoxy(25 + j * 10, 4); printf("Cầu %d", j + 1);
            VeKhung(25 + j * 10, 5 + i * 3, 7, 3);
            ChiPhi[i][j] = 0;
        }
    }
    int currentPosY = 0; // đây là chỉ số của mảng Cau
    int currentPosX = 0;
    for (int i = 0; i < 10; i++) {input[i] = 0;} // Bộ đệm nhập liệu
    inputIndex = 0;
    Mau(DoDam); // đặt cho chữ 
    ToaDoxy(26, 6); printf("[   ]");

    while (1) {
        int Full = 1;
        for (int i = 0; i < m && Full; i++) {
            for (int j = 0; j < n; j++) {
                if (ChiPhi[i][j] == 0) {
                    Full = 0;
                    break;
                }
            }
        }
        if (Full) break;

        char key = _getch();
        // Xóa dấu [ ] hiện tại
        ToaDoxy(26 + currentPosX * 10, 6 + currentPosY * 3); // 10 là khoảng cách giữa 2 ô
        printf("     "); // Xóa [ ] 
        // Cập nhật lại số (nếu có)
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (ChiPhi[i][j] != 0) {
                    ToaDoxy(27 + j * 10, 6 + i * 3);
                    printf("%d", ChiPhi[i][j]);
                }
            }
        }
        
        // Xử lý phím
        if (key == 0 || key == -32) { // Phím đặc biệt (mũi tên)
            key = _getch();
            if (key == 75 && currentPosX > 0) {currentPosX--;} // Trái
            if (key == 77 && currentPosX < n - 1) {currentPosX++;} // Phải
            if (key == 72 && currentPosY > 0) {currentPosY--;} // Lên
            if (key == 80 && currentPosY < m - 1) {currentPosY++;} // Xuống
            inputIndex = 0;
            input[0] = '\0';
        }
        PLACE4:
        ToaDoxy(26 + currentPosX * 10, 6 + currentPosY * 3);
        Mau(DoDam); printf("[   ]");
        Mau(7);

        // Nhập số
        if (key >= '0' && key <= '9' && inputIndex <= 3) {
            if (inputIndex == 3) {
                inputIndex = 0;
                input[0] = '\0';
            }
            input[inputIndex++] = key;
            input[inputIndex] = '\0';
            ToaDoxy(27 + currentPosX * 10, 6 + currentPosY * 3);
            printf("%s", input);
        }
        // Nhấn Enter để xác nhận
        else if (key == 13 && inputIndex > 0) {
            int i = 0, j = 0;
            while (i != currentPosY) {i++;}
            while (j != currentPosX) {j++;}
            ChiPhi[i][j] = atoi(input);
            
            inputIndex = 0;
            input[0] = '\0';
            ToaDoxy(27 + currentPosX * 10, 6 + currentPosY * 3);
            printf("%d", ChiPhi[i][j]);
            if (currentPosX < n - 1 && currentPosY <= m - 1) {
                ToaDoxy(26 + currentPosX * 10, 6 + currentPosY * 3); 
                printf("     "); // Xóa [ ] 
                // Cập nhật lại số (nếu có)
                for (int i = 0; i < m; i++) {
                    for (int j = 0; j < n; j++) {
                        if (ChiPhi[i][j] != 0) {
                            ToaDoxy(27 + j * 10, 6 + i * 3);
                            printf("%d", ChiPhi[i][j]);
                        }
                    }
                }
                currentPosX++;
                goto PLACE4;    
            }
            else if (currentPosX == n - 1 && currentPosY < m - 1) {
                ToaDoxy(26 + currentPosX * 10, 6 + currentPosY * 3); 
                printf("     "); // Xóa [ ] 
                // Cập nhật lại số (nếu có)
                for (int i = 0; i < m; i++) {
                    for (int j = 0; j < n; j++) {
                        if (ChiPhi[i][j] != 0) {
                            ToaDoxy(27 + j * 10, 6 + i * 3);
                            printf("%d", ChiPhi[i][j]);
                        }
                    }
                }
                currentPosX = 0;
                currentPosY++;
                goto PLACE4;    
            }
        }
        else if (key == 8 && inputIndex > 0) { // Nếu là dấu backspace thì sẽ xóa 1 kí tự
            input[--inputIndex] = '\0';
            ToaDoxy(27 + currentPosX * 10, 6 + currentPosY * 3);
            printf("%s", input);
        }
    }
    system("cls");
    XuLyCanBang();
    printf("\n");
}


void Xuat(int CongY) {
    VeKhung(22, 8, 60 + n, 10 + m + CongY);
    Mau(XanhNhat);
    ToaDoxy(43, 10 + CongY);
    printf("Kết quả phân phối:");
    ToaDoxy(39, 9 + 3 + CongY);
    for (int j = 0; j < n; j++) {
        if (KhongCanBang == 1 && j == n - 1) {
            printf("Cầu giả\t");
        }
        else {
            printf("Cầu %d\t", j+1);
        }
    }
    printf("Cung");
    int tongChiPhi = 0;
    for (int i = 0; i < m; i++) {
        ToaDoxy(25, 12 + i + CongY + 1);
        // Kiểm tra nếu là nguồn giả
        if (KhongCanBang == 2 && i == m - 1) {
            printf("    Cung giả  ");
        }
        else {
            printf("    Cung %d    ", i+1);
        }
        
        for (int j = 0; j < n; j++) {
            if (PhanPhoi[i][j] == 0) {printf("-\t");}
            else {
                printf("%d(%d)\t", PhanPhoi[i][j], ChiPhi[i][j]);
                // Không tính chi phí cho các ô giả
                if (!(KhongCanBang == 1 && j == n-1) && !(KhongCanBang == 2 && i == m-1)) {
                    tongChiPhi += PhanPhoi[i][j] * ChiPhi[i][j];
                }
            }
        }
        printf("%d", Cung[i]);
    }
    ToaDoxy(25, 12 + m + CongY + 1);
    printf("    Cầu        ");

    for (int j = 0; j < n; j++) {
        printf("%d\t", Cau[j]);
    }
    ToaDoxy(25, 12 + m + 2 + CongY + 1);
    printf("Tổng chi phí vận tải thực tế: ");
    Mau(DoNhat);
    printf("%d", tongChiPhi);
    Mau(XanhNhat);
    ToaDoxy(25, 12 + m + 4 + CongY);

    if (KhongCanBang == 1) {
        printf("Lượng dư thừa: %d (được phân vào cầu giả)\n\n", Cau[n-1]);
    }
    else if (KhongCanBang == 2) {
        printf("Lượng thiếu hụt: %d (được lấy từ nguồn giả)\n\n", Cung[m-1]);
    }
}

void PhuongPhapGocTayBac() {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            PhanPhoi[i][j] = 0;
        }
    }
    int CungConLai[MAX], CauConLai[MAX];
    for (int i = 0; i < m; i++) {CungConLai[i] = Cung[i];}
    for (int j = 0; j < n; j++) {CauConLai[j] = Cau[j];}
    int i = 0, j = 0;
    while (i < m && j < n) {
        int SoLuong = (CungConLai[i] < CauConLai[j]) ? CungConLai[i] : CauConLai[j];
        PhanPhoi[i][j] = SoLuong; 
        CungConLai[i] -= SoLuong;
        CauConLai[j] -= SoLuong;
        if (CungConLai[i] == 0) {i++;}
        else {j++;}
    }
}

void PhuongPhapChiPhiNhoNhat() {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            PhanPhoi[i][j] = 0;
        }
    }
    int CungConLai[MAX], CauConLai[MAX];
    for (int i = 0; i < m; i++) {CungConLai[i] = Cung[i];}
    for (int j = 0; j < n; j++) {CauConLai[j] = Cau[j];}

    if (KhongCanBang == 1) {n--;}
    else if (KhongCanBang == 2) {m--;}
    while (1) {
        int minChiPhi = INTMAX;
        int hang = -1, cot = -1;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (CungConLai[i] > 0 && CauConLai[j] > 0 && ChiPhi[i][j] < minChiPhi) {
                    minChiPhi = ChiPhi[i][j];
                    hang = i;
                    cot = j;
                }
            }
        }
        if (hang == -1 || cot == -1) break;
        int SoLuong = (CungConLai[hang] < CauConLai[cot]) ? CungConLai[hang] : CauConLai[cot];
        PhanPhoi[hang][cot] = SoLuong;
        CungConLai[hang] -= SoLuong;
        CauConLai[cot] -= SoLuong;
    }
    if (KhongCanBang != 0) {
        if (KhongCanBang == 1) {
            for (int i = 0; i < m; i++) {
                int SoLuong = (CungConLai[i] < CauConLai[n]) ? CungConLai[i] : CauConLai[n];
                PhanPhoi[i][n] = SoLuong;
                CungConLai[i] -= SoLuong;
                CauConLai[n] -= SoLuong;
            }
            n++;
        }
        else {
            for (int j = 0; j < n; j++) {
                int SoLuong = (CungConLai[m] < CauConLai[j]) ? CungConLai[m] : CauConLai[j];
                PhanPhoi[m][j] = SoLuong;
                CungConLai[m] -= SoLuong;
                CauConLai[j] -= SoLuong;
            }
            m++;
        }
    }
}

void PhuongPhapVogel() {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            PhanPhoi[i][j] = 0;
        }
    }

    int CungConLai[MAX], CauConLai[MAX];
    for (int i = 0; i < m; i++) CungConLai[i] = Cung[i];
    for (int j = 0; j < n; j++) CauConLai[j] = Cau[j];

    while (1) {
        int MaxPenalty = -1;
        int HangMaxPenalty = -1, CotMaxPenalty = -1;
        char Loai; // 'H' (Hàng) hoặc 'C'  (Cột)

        // Tính penalty cho từng hàng
        for (int i = 0; i < m; i++) {
            if (CungConLai[i] == 0) continue; // Bỏ qua hàng đã hết cung

            int Min1 = INTMAX, Min2 = INTMAX;
            for (int j = 0; j < n; j++) {
                if (CauConLai[j] == 0) continue; // Bỏ qua cột đã hết cầu

                if (ChiPhi[i][j] < Min1) {
                    Min2 = Min1;
                    Min1 = ChiPhi[i][j];
                }
                else if (ChiPhi[i][j] < Min2) {
                    Min2 = ChiPhi[i][j];
                }
            }

            int Penalty = (Min2 == INTMAX) ? Min1 : (Min2 - Min1);
            if (Penalty > MaxPenalty) {
                MaxPenalty = Penalty;
                HangMaxPenalty = i;
                Loai = 'H';
            }
        }

        // Tính penalty cho từng cột
        for (int j = 0; j < n; j++) {
            if (CauConLai[j] == 0) continue; // Bỏ qua cột đã hết cầu

            int Min1 = INTMAX, Min2 = INTMAX;
            for (int i = 0; i < m; i++) {
                if (CungConLai[i] == 0) continue; // Bỏ qua hàng đã hết cung

                if (ChiPhi[i][j] < Min1) {
                    Min2 = Min1;
                    Min1 = ChiPhi[i][j];
                }
                else if (ChiPhi[i][j] < Min2) {
                    Min2 = ChiPhi[i][j];
                }
            }

            int Penalty = (Min2 == INTMAX) ? Min1 : (Min2 - Min1);
            if (Penalty > MaxPenalty) {
                MaxPenalty = Penalty;
                CotMaxPenalty = j;
                Loai = 'C';
            }
        }

        // Nếu không còn penalty (đã phân phối xong)
        if (MaxPenalty == -1) break;

        // Tìm ô có chi phí nhỏ nhất trong hàng/cột có penalty lớn nhất
        int Hang = -1, Cot = -1;
        int MinChiPhi = INTMAX;

        if (Loai == 'H') {
            for (int j = 0; j < n; j++) {
                if (CauConLai[j] == 0) continue;
                if (ChiPhi[HangMaxPenalty][j] < MinChiPhi) {
                    MinChiPhi = ChiPhi[HangMaxPenalty][j];
                    Hang = HangMaxPenalty;
                    Cot = j;
                }
            }
        }
        else { // 'C'
            for (int i = 0; i < m; i++) {
                if (CungConLai[i] == 0) continue;
                if (ChiPhi[i][CotMaxPenalty] < MinChiPhi) {
                    MinChiPhi = ChiPhi[i][CotMaxPenalty];
                    Hang = i;
                    Cot = CotMaxPenalty;
                }
            }
        }

        // Phân phối vào ô (Hang, Cot)
        int SoLuong = (CauConLai[Cot] < CungConLai[Hang]) ? CauConLai[Cot] : CungConLai[Hang];
        PhanPhoi[Hang][Cot] = SoLuong;

        // Cập nhật lại cung và cầu
        CungConLai[Hang] -= SoLuong;
        CauConLai[Cot] -= SoLuong;
    }
}

void KhoiTao() {
    for (int i = 0; i < n + m; i++) {
        Cha[i] = i;
        sz[i] = 1;
    }
}

int Tim(int i) {
    if (i == Cha[i]) return i;
    return Cha[i] = Tim(Cha[i]);
}

int KtraNoi(int x, int y) {
    x = Tim(x);
    y = Tim(y);
    if (x == y) return 0;
    if (sz[x] > sz[y]) {
        sz[x] += sz[y];
        Cha[y] = x;
    }
    else{
        sz[y] += sz[x];
        Cha[x] = y;
    }
    return 1;
}

void XuLySuyBien(int SoOChon) {
    KhoiTao();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {   
            if (ODaChon[i][j] > 0) {KtraNoi(i, j + m);}
        }
    }           
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (ODaChon[i][j] == 0 && KtraNoi(i, j + m)) { //dọc(cung) thì đánh số từ 0 đến m - 1, còn ngang(cầu) thì đánh số từ 0 đến m + n - 1
                ODaChon[i][j] = 1; // 1 kí hiệu cho ô chọn
                SoOChon++;
                if (SoOChon == m + n - 1) return; // nếu như = thì không còn suy biến nữa
            }
        }
    }
}

void TimDuongDi(int start, int end, int MangKq[], int SoLuong, int DaTham[], int *SoLuongThat, int *DaTimThay) {
    if (*DaTimThay) return;
    MangKq[SoLuong] = start;    

    if (start == end) {
        *DaTimThay = 1;
        SoLuong++;
        MangKq[SoLuong] = MangKq[0];
        *SoLuongThat = SoLuong + 1;
        return;
    }
    DaTham[start] = 1;
    if (start < m) {
        for (int j = 0; j < n; j++) {
            if (m + j == end && SoLuong < 2) continue;
            if (ODaChon[start][j] > 0 && !DaTham[m + j]) {
                // printf("st = %d, j = %d, %d\n",start, j, ODaChon[start][j]);
                TimDuongDi(m + j, end, MangKq, SoLuong + 1, DaTham, SoLuongThat, DaTimThay);
            }
        }
    }
    else if (start < m + n) {
        for (int i = 0; i < m; i++) {
            if (i == end && SoLuong < 1) continue;
            if (ODaChon[i][start - m] > 0 && !DaTham[i] && !(i == end && SoLuong < 4)) {
                TimDuongDi(i, end, MangKq, SoLuong + 1, DaTham, SoLuongThat, DaTimThay);
            }
        }
    }
}

void BienDoi(int MangKq[], int SoLuong) {
    int MIN = INTMAX, x, y;
    int xGia, yGia;
    // tìm min trong các ô trừ 
    for (int i = 0; i < SoLuong - 1; i++) {
        if (MangKq[i] < MangKq[i + 1]) {x = MangKq[i]; y = MangKq[i + 1] - m;} // xét các ô trong chu trình có ô suy biến không     
        else {x = MangKq[i + 1]; y = MangKq[i] - m;   }         
        if (i % 2 == 0 && PhanPhoi[x][y] < MIN) { // xét các ô trừ, tìm min của các ô trừ đó.
            xGia = x;
            yGia = y;
            MIN = PhanPhoi[x][y];
        }
    }       

    int HeSo;
    for (int i = 0; i < SoLuong - 1; i++) {
        if (i % 2 == 0) HeSo = -MIN;
        else HeSo = MIN;
        if (MangKq[i] < MangKq[i + 1]) {x = MangKq[i]; y = MangKq[i + 1] - m;}
        else  {x = MangKq[i + 1]; y = MangKq[i] - m;}
        PhanPhoi[x][y] += HeSo;
    }
    ODaChon[xGia][yGia] = 0;
}   

void PhuongPhapTheVi() {
    int SoOChon = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            ODaChon[i][j] = 0;
            if (PhanPhoi[i][j] > 0) {
                ODaChon[i][j] = 1;
                SoOChon++;
            }
        }   
    }
    if (SoOChon < m + n - 1) {XuLySuyBien(SoOChon);}
    int Ktra = 1;
    while (Ktra) {
        Ktra = 0;
        int u[MAX], v[MAX];
        for (int i = 0; i <= MAX; i++) {
            u[i] = v[i] = INTMAX;
        }
        u[0] = 0;
        int DemU = 1, DemV = 0;
        while (DemU != m || DemV != n) {
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    if (ODaChon[i][j] > 0) {
                        if (u[i] != INTMAX && v[j] == INTMAX) {
                            v[j] = ChiPhi[i][j] - u[i];
                            DemV++;
                        }
                        else if (u[i] == INTMAX && v[j] != INTMAX) {
                            u[i] = ChiPhi[i][j] - v[j];
                            DemU++;
                        }
                    }
                }
            }
        }

        int denta[MAX][MAX];
        int dentaCauMin, dentaCungMin, dentaMin = INTMAX;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (PhanPhoi[i][j] == 0) {
                    denta[i][j] = ChiPhi[i][j] - u[i] - v[j];
                    if (denta[i][j] < 0) { // chưa tối ưu 
                        Ktra = 1; 
                        if (denta[i][j] < dentaMin) { // lưu lại ô chọn tìm năng để có thể tra cứu nhanh trong trường hợp không tối ưu
                            dentaMin = denta[i][j];
                            dentaCungMin = i;
                            dentaCauMin = j;
                        }
                    }
                }
            }
        }

        if (Ktra == 0) {
            // printf("Phương án đã tối ưu!\n\n");
            break;
        }
        ODaChon[dentaCungMin][dentaCauMin] = 1;
        
        int MangKq[MAX] = {0}, DaTham[MAX] = {0};   
        int SoLuongThat = 0;
        int DaTimThay = 0;
        TimDuongDi(dentaCungMin, dentaCauMin + m, MangKq, 0, DaTham, &SoLuongThat, &DaTimThay);
        
        BienDoi(MangKq, SoLuongThat);  
    }
}

//------------------------------------------------------------------------------------------------

void ToaDoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Mau(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void AnCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void VeKhung(int x, int y, int width, int height) {
    SetConsoleOutputCP(437);
    Mau(9); // Màu vàng cho khung

    // Ký tự ASCII cho khung
    char topLeft = 201;     // ┌
    char topRight = 187;    // ┐
    char bottomLeft = 200;  // └
    char bottomRight = 188; // ┘
    char horizontal = 205;  // ─
    char vertical = 186;    // │

    // Vẽ góc
    ToaDoxy(x, y);
    printf("%c", topLeft);
    ToaDoxy(x + width - 1, y);
    printf("%c", topRight);
    ToaDoxy(x, y + height - 1);
    printf("%c", bottomLeft);
    ToaDoxy(x + width - 1, y + height - 1);
    printf("%c", bottomRight);

    // Vẽ cạnh trên và dưới
    for (int i = 1; i < width - 1; i++) {
        ToaDoxy(x + i, y);
        printf("%c", horizontal);
        ToaDoxy(x + i, y + height - 1);
        printf("%c", horizontal);
    }

    // Vẽ cạnh trái và phải
    for (int i = 1; i < height - 1; i++) {
        ToaDoxy(x, y + i);
        printf("%c", vertical);
        ToaDoxy(x + width - 1, y + i);
        printf("%c", vertical);
    }
    SetConsoleOutputCP(65001);
}

void NhapNhay(int x, int y, const char* text, int blinkCount) {
    for (int i = 0; i < blinkCount; i++) {
        // Hiển thị chữ với màu đỏ
        ToaDoxy(x, y);
        Mau(DoNhat); // Màu đỏ
        printf(">> %s", text);
        
        Sleep(200); // Chờ 200ms

        // Đổi màu sang vàng
        ToaDoxy(x, y);
        Mau(Vang); // Màu vàng
        printf(">> %s", text);
        Sleep(200); // Chờ 200ms
    }
    // Đặt lại màu mặc định sau khi nhấp nháy
    ToaDoxy(x, y);
    Mau(DoNhat);
    printf(">> %s", text);
}

int Menu() {
    SetConsoleOutputCP(65001); // set viết được tiếng việt
    int selectedOption = 0; // được chọn
    char key; // từ vừa được nhập vào
    Mau(Vang); // đặt cho chữ
    const char* Menu[] = {
            "Phương pháp góc tây bắc    ",
            "Phương pháp chi phí nhỏ nhất    ",
            "Phương pháp vogel    ",
            "Thoát     "
    };

    const char* TieuDe[] = {
    "                                                            __                  _____    ",
    "         __                           __                   /  \\                /__  / ",
    "         \\_\\                         /_/                  /_/\\_\\                 /_/   ",
    " ____     _     ___   _____  ___     _     _   _  __     __ _     _   _   _____   _     ___ ",
    "| __ )   / \\   |_ _| |_   _|/ _ \\   / \\   | \\ | | \\ \\   / // \\   | \\ | | |_   _| / \\   |_ _|",
    "|  _ \\  / _ \\   | |    | | | | | | / _ \\  |  \\| |  \\ \\ / // _ \\  |  \\| |   | |  / _ \\   | | ",
    "| |_) |/ ___ \\  | |    | | | |_| |/ ___ \\ | |\\  |   \\ V // ___ \\ | |\\  |   | | / ___ \\  | | ",
    "|____//_/   \\_\\|___|   |_|  \\___//_/   \\_\\|_| \\_|    \\_//_/   \\_\\|_| \\_|   |_|/_/   \\_\\|___|",
    "                                                            _   ",
    "                                                           |_|   "
    };

    for (int i = 0; i <= 9; i++) {
        ToaDoxy(3, i);
        printf("%s",TieuDe[i]);
        Sleep(20);
    }

    while (1) {
        VeKhung(5 + 20, 1 + 7 + 2, 50, 11);
        Mau(Vang); // đặt cho chữ
        ToaDoxy(19 + 17, 3 + 7+ 1 + 2); printf("== CHỌN PHƯƠNG ÁN CƠ BẢN == \n");
        // in menu
        for (int i = 0; i < 4; i++) {
            ToaDoxy(10 + 20 + 3, 6 + i + 7 + 2);
            if (i == selectedOption) {
                Mau(DoNhat); // Màu đỏ cho mục được chọn
                printf("  >> %s", Menu[i]);
            }
            else {
                Mau(Vang); // Màu vàng cho các mục khác
                printf("  %s", Menu[i]);
            }
        }
        
        key = _getch();
        int Enter = FALSE;
        if (key == -32) { // xác định có phải phím mũi tên không
            key = _getch();
            switch (key) {
                case 72: // mũi tên đi lên
                    if (selectedOption > 0) {selectedOption--;}
                    break;
                case 80: // mũi tên đi xuống
                    if (selectedOption < 3) {selectedOption++;}
                    break;
            }
        }
        else if (key == 13) { //  Enter
            Beep(2000, 200);
            NhapNhay(10 + 20 + 5, 6 + selectedOption + 7 + 2, Menu[selectedOption], 2);

            if (selectedOption == 3) { // Thoát
                system("cls");
                ToaDoxy(47, 6 + 7);
                printf("Tạm biệt!");
                Sleep(1000);
                return 1;
            }
            else {
                Enter = TRUE;
                system("cls");
                switch(selectedOption) {
                    case 0:
                        PhuongPhapGocTayBac();
                        break;
                    case 1:
                        PhuongPhapChiPhiNhoNhat();
                        break;
                    case 2:
                        PhuongPhapVogel();
                        break;
                }
            }
        }
        if (Enter) break;
    }
    return 0;
}

int LuaChon() {
    SetConsoleOutputCP(65001); // set viết được tiếng việt
    int selectedOption = 0; // được chọn
    char key; // từ vừa được nhập vào
    Mau(Vang); // đặt cho chữ
    const char* MyChoise[] = {
            "Quay về menu   ",
            "Dừng lại   ",
    };
    while (1) {
        VeKhung(5 + 20 + 8, 1, 40, 7);
        Mau(Vang); // đặt cho chữ
        ToaDoxy(10 + 20 + 5 + 10, 3); printf("Tối ưu hay thoát: ");
        // in menu
        for (int i = 0; i < 2; i++) {
            ToaDoxy(10 + 20 + 5 + 10 + 3, 4 + i);
            if (i == selectedOption) {
                Mau(DoNhat); // Màu đỏ cho mục được chọn
                printf(">> %s", MyChoise[i]);
            }
            else {
                Mau(Vang); // Màu vàng cho các mục khác
                printf("%s", MyChoise[i]);
            }
        }
        
        key = _getch();
        int Enter = FALSE;
        if (key == -32) { // xác định có phải phím mũi tên không
            key = _getch();
            switch (key) {
                case 72: // mũi tên đi lên
                    if (selectedOption > 0) {selectedOption--;}
                    break;
                case 80: // mũi tên đi xuống
                    if (selectedOption < 1) {selectedOption++;}
                    break;
            }
        }

        else if (key == 13) { //  Enter
            Beep(2000, 200);
            NhapNhay(10 + 20 + 5 + 10 + 3, 4 + selectedOption, MyChoise[selectedOption], 2);

            if (selectedOption == 1) { // Thoát
                system("cls");
                ToaDoxy(47, 6);
                printf("Tạm biệt!");
                Sleep(1000);
                return 1;
            }
            else {
                Enter = TRUE;
                system("cls");
                PhuongPhapTheVi();
            }
        }
        if (Enter) break;
    }
    return 0;
}

void CachNhap() {
    SetConsoleOutputCP(65001); // set viết được tiếng việt
    int selectedOption = 0; // được chọn
    char key; // từ vừa được nhập vào
    Mau(Vang); // đặt cho chữ
    const char* TieuDe[] = {
    "                                                            __                  _____    ",
    "         __                           __                   /  \\                /__  / ",
    "         \\_\\                         /_/                  /_/\\_\\                 /_/   ",
    " ____     _     ___   _____  ___     _     _   _  __     __ _     _   _   _____   _     ___ ",
    "| __ )   / \\   |_ _| |_   _|/ _ \\   / \\   | \\ | | \\ \\   / // \\   | \\ | | |_   _| / \\   |_ _|",
    "|  _ \\  / _ \\   | |    | | | | | | / _ \\  |  \\| |  \\ \\ / // _ \\  |  \\| |   | |  / _ \\   | | ",
    "| |_) |/ ___ \\  | |    | | | |_| |/ ___ \\ | |\\  |   \\ V // ___ \\ | |\\  |   | | / ___ \\  | | ",
    "|____//_/   \\_\\|___|   |_|  \\___//_/   \\_\\|_| \\_|    \\_//_/   \\_\\|_| \\_|   |_|/_/   \\_\\|___|",
    "                                                            _   ",
    "                                                           |_|   "
    };

    for (int i = 0; i <= 9; i++) {
        ToaDoxy(3, i);
        printf("%s",TieuDe[i]);
        Sleep(20);
    }

    const char* LuaChonNhap[] = {
            "Nhập từ file 0input.txt    ",
            "Nhập tay    "
    };

    while (1) {
        VeKhung(5 + 20, 1 + 7 + 2, 50, 11);
        Mau(Vang); // đặt cho chữ
        ToaDoxy(19 + 21, 3 + 7 + 1 + 2); printf("== CHỌN CÁCH NHẬP == \n");
        // in LuaChonNhap
        for (int i = 0; i < 2; i++) {
            ToaDoxy(10 + 20 + 3, 6 + i + 7 + 2);
            if (i == selectedOption) {
                Mau(DoNhat); // Màu đỏ cho mục được chọn
                printf("  >> %s", LuaChonNhap[i]);
            }
            else {
                Mau(Vang); // Màu vàng cho các mục khác
                printf("  %s", LuaChonNhap[i]);
            }
        }
        
        key = _getch();
        int Enter = FALSE;
        if (key == -32) { // xác định có phải phím mũi tên không
            key = _getch();
            switch (key) {
                case 72: // mũi tên đi lên
                    if (selectedOption > 0) {selectedOption--;}
                    break;
                case 80: // mũi tên đi xuống
                    if (selectedOption < 1) {selectedOption++;}
                    break;
            }
        }

        else if (key == 13) { //  Enter
            Enter = TRUE;
            Beep(2000, 200);
            NhapNhay(10 + 20 + 5, 6 + selectedOption + 7 + 2, LuaChonNhap[selectedOption], 2);
            if (selectedOption == 0) {
                freopen("0input.txt", "r", stdin);
                Nhap();
                freopen("CON", "r", stdin);
            }
            else {
                system("cls");
                NhapTay();
            } 
        }
        if (Enter) break;
    }
}