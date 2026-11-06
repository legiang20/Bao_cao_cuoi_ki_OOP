#pragma once
#define NOMINMAX
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <ctime>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif
using namespace std;

enum {
    MAU_MACDINH = 7, MAU_XAM = 8, MAU_XANHDUONG = 9, MAU_XANHLA = 10,
    MAU_XANHNGOC = 11, MAU_DO = 12, MAU_HONG = 13, MAU_VANG = 14
};

// Các hằng độ rộng giao diện: const để không ai sửa được ngoài ý muốn
const int RONG_NHAP = 46;
const int RONG_BANNER = 42;
const int RONG_NHAN = 16, RONG_GIATRI = 58;
const int RONG_BANG = 118;
const int NAM_NHO_NHAT = 1800;

int RongManHinh();
string LeGiua(int rongKhoi);
string LeNhap();
string LeBanner();
string LeBang();
string LeThe();
string LeCho(string s);
void InGiua(int m, string s);
void DatMau(int m);
void InMau(int m, string s);
void KhoiTaoConsole();
void XoaManHinh();
int NamHienTai();
void YeuCau(bool dieuKien, string thongBao);
int DoDai(string s);
string Trim(string s);
string ChuThuong(string s);
string ChuHoaAscii(string s);
string DinhDangTien(double v);
string Lap(string s, int n);
string Cat(string s, int w);
string CanTrai(string s, int w);
string CanPhai(string s, int w);
string CanGiua(string s, int w);
vector<string> NgatDong(string s, int w);
void ThongBaoOk(string m);
void ThongBaoLoi(string m);
void TamDung();
string NhapChuoi(string nhan);
int NhapSo(string nhan, int lo, int hi);
bool LaSoNguyen(string s);
void MoThe(string tieuDe);
void DongThe(string nhan, string giaTri, int mau = MAU_MACDINH);
void DayThe();
int MauLoai(string loai);
void LoiNhap(string m);

int RongManHinh()
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) return ws.ws_col;
#endif
    return 120;
}
string LeGiua(int rongKhoi)
{
    int le = (RongManHinh() - rongKhoi) / 2;
    return string(le < 1 ? 1 : le, ' ');
}
string LeNhap()   { return LeGiua(RONG_NHAP); }
string LeBanner() { return LeGiua(RONG_BANNER); }
string LeBang()   { return LeGiua(RONG_BANG); }
string LeThe()    { return LeGiua(RONG_NHAN + RONG_GIATRI + 7); }
string LeCho(string s) { return LeGiua(DoDai(s)); }
void InGiua(int m, string s) { cout << LeCho(s); InMau(m, s); cout << endl; }

void DatMau(int m)
{
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m);
#else
    if (m == MAU_MACDINH) { cout << "\033[0m"; return; }
    int ansi = ((m & 4) ? 1 : 0) + ((m & 2) ? 2 : 0) + ((m & 1) ? 4 : 0);
    cout << "\033[" << ((m & 8) ? 90 : 30) + ansi << "m";
#endif
}
void InMau(int m, string s) { DatMau(m); cout << s; DatMau(MAU_MACDINH); }

void KhoiTaoConsole()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}
void XoaManHinh()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int NamHienTai()
{
    time_t t = time(NULL);
    tm *lt = localtime(&t);
    return lt ? lt->tm_year + 1900 : 2100;
}
void YeuCau(bool dieuKien, string thongBao)
{
    if (!dieuKien) throw invalid_argument(thongBao);
}

int DoDai(string s)
{
    int n = 0;
    for (size_t i = 0; i < s.size(); i++)
        if ((s[i] & 0xC0) != 0x80) n++;
    return n;
}
string Cat(string s, int w)
{
    if (DoDai(s) <= w) return s;
    string r;
    int dem = 0;
    for (size_t i = 0; i < s.size() && dem < w - 1; ) {
        unsigned char c = s[i];
        int len = (c < 0x80) ? 1 : (c >> 5) == 6 ? 2 : (c >> 4) == 14 ? 3 : 4;
        r += s.substr(i, len);
        i += len;
        dem++;
    }
    return r + "…";
}
string CanTrai(string s, int w)  { int d = DoDai(s); return d >= w ? s : s + string(w - d, ' '); }
string CanPhai(string s, int w)  { int d = DoDai(s); return d >= w ? s : string(w - d, ' ') + s; }
string CanGiua(string s, int w)
{
    int d = DoDai(s);
    if (d >= w) return s;
    int trai = (w - d) / 2;
    return string(trai, ' ') + s + string(w - d - trai, ' ');
}
string Lap(string s, int n) { string r; for (int i = 0; i < n; i++) r += s; return r; }

vector<string> NgatDong(string s, int w)
{
    vector<string> kq;
    istringstream ss(s);
    string tu, dong;
    while (ss >> tu) {
        if (DoDai(tu) > w) tu = Cat(tu, w);
        if (dong.empty()) dong = tu;
        else if (DoDai(dong) + 1 + DoDai(tu) <= w) dong += " " + tu;
        else { kq.push_back(dong); dong = tu; }
    }
    if (!dong.empty() || kq.empty()) kq.push_back(dong);
    return kq;
}

string Trim(string s)
{
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}
string ChuThuong(string s)
{
    string r;
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = s[i];
        if (c < 0x80) { r += (char)tolower(c); i++; continue; }
        int len = (c >> 5) == 6 ? 2 : (c >> 4) == 14 ? 3 : (c >> 3) == 30 ? 4 : 1;
        if (len == 1 || i + len > s.size()) { r += s[i]; i++; continue; }
        if (len == 4) { r += s.substr(i, len); i += len; continue; }
        unsigned cp = (len == 2) ? (((c & 0x1F) << 6) | (s[i + 1] & 0x3F))
                                 : (((c & 0x0F) << 12) | ((s[i + 1] & 0x3F) << 6) | (s[i + 2] & 0x3F));
        if (cp >= 0xC0 && cp <= 0xDE && cp != 0xD7) cp += 0x20;
        else if (cp == 0x102 || cp == 0x110 || cp == 0x128 || cp == 0x168 || cp == 0x1A0) cp += 1;
        else if (cp == 0x1AF) cp = 0x1B0;
        else if (cp >= 0x1EA0 && cp <= 0x1EF8 && cp % 2 == 0) cp += 1;
        if (cp < 0x800) {
            r += (char)(0xC0 | (cp >> 6));
            r += (char)(0x80 | (cp & 0x3F));
        } else {
            r += (char)(0xE0 | (cp >> 12));
            r += (char)(0x80 | ((cp >> 6) & 0x3F));
            r += (char)(0x80 | (cp & 0x3F));
        }
        i += len;
    }
    return r;
}
string ChuHoaAscii(string s)
{
    for (size_t i = 0; i < s.size(); i++)
        if ((unsigned char)s[i] < 128) s[i] = (char)toupper((unsigned char)s[i]);
    return s;
}
string DinhDangTien(double v)
{
    string s = to_string((long long)(v + 0.5)), r;
    int dem = 0;
    for (int i = (int)s.size() - 1; i >= 0; i--) {
        r.insert(r.begin(), s[i]);
        if (++dem % 3 == 0 && i > 0) r.insert(r.begin(), '.');
    }
    return r + "đ";
}
bool LaSoNguyen(string s)
{
    if (s.empty() || s.size() > 9) return false;
    for (size_t i = 0; i < s.size(); i++)
        if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

void LoiNhap(string m)     { InGiua(MAU_DO, "✗ " + m); }
void ThongBaoOk(string m)  { cout << "\n"; InGiua(MAU_XANHLA, "✔ " + m + " ♡"); }
void ThongBaoLoi(string m) { cout << "\n"; InGiua(MAU_DO, "✗ " + m); }
void TamDung()
{
    cout << "\n"; InGiua(MAU_XAM, "(Nhấn Enter để tiếp tục ✿)");
    string s; getline(cin, s);
}

string NhapChuoi(string nhan)
{
    string s;
    while (true) {
        cout << LeNhap(); InMau(MAU_XANHNGOC, nhan); cout << endl;
        cout << LeNhap(); InMau(MAU_HONG, "♡ ");
        DatMau(MAU_XANHLA);
        if (!getline(cin, s)) { DatMau(MAU_MACDINH); exit(0); }
        DatMau(MAU_MACDINH);
        s = Trim(s);
        if (s.empty())
            LoiNhap("Không được để trống!");
        else
            return s;
    }
}
int NhapSo(string nhan, int lo, int hi)
{
    while (true) {
        string s = NhapChuoi(nhan + " (" + to_string(lo) + "-" + to_string(hi) + ")");
        if (!LaSoNguyen(s)) { LoiNhap("Vui lòng nhập một số nguyên không âm!"); continue; }
        int v = atoi(s.c_str());
        if (v < lo || v > hi) { LoiNhap("Giá trị phải từ " + to_string(lo) + " đến " + to_string(hi) + "!"); continue; }
        return v;
    }
}

void MoThe(string tieuDe)
{
    cout << "\n" << LeThe();
    InMau(MAU_XANHNGOC, "╭─ "); InMau(MAU_HONG, tieuDe);
    InMau(MAU_XANHNGOC, " " + Lap("─", max(0, RONG_NHAN - 1 - DoDai(tieuDe))) + "┬" + Lap("─", RONG_GIATRI + 2) + "╮");
    cout << endl;
}
void DongThe(string nhan, string giaTri, int mau)
{
    vector<string> dong = NgatDong(giaTri, RONG_GIATRI);
    for (size_t i = 0; i < dong.size(); i++) {
        cout << LeThe(); InMau(MAU_XANHNGOC, "│");
        cout << " "; InMau(MAU_XAM, CanTrai(i == 0 ? nhan : "", RONG_NHAN)); cout << " ";
        InMau(MAU_HONG, "│");
        cout << " "; InMau(mau, CanTrai(dong[i], RONG_GIATRI)); cout << " ";
        InMau(MAU_XANHNGOC, "│"); cout << endl;
    }
}
void DayThe()
{
    cout << LeThe(); InMau(MAU_XANHNGOC, "╰" + Lap("─", RONG_NHAN + 2) + "┴" + Lap("─", RONG_GIATRI + 2) + "╯"); cout << endl;
}

int MauLoai(string loai)
{
    if (loai == "Sách") return MAU_HONG;
    if (loai == "Tạp chí") return MAU_XANHDUONG;
    return MAU_VANG;
}

void Banner()
{
    XoaManHinh();
    int W = 40;
    cout << "\n" << LeBanner(); InMau(MAU_XAM, CanGiua("~ ✿ ♡ ✿ ~", W + 2)); cout << endl;
    cout << LeBanner(); InMau(MAU_HONG, "╭" + Lap("─", W) + "╮"); cout << endl;
    cout << LeBanner(); InMau(MAU_HONG, "│"); InMau(MAU_HONG, CanGiua("✿  THƯ VIỆN CẦU VỒNG  ✿", W)); InMau(MAU_HONG, "│"); cout << endl;
    cout << LeBanner(); InMau(MAU_HONG, "│"); InMau(MAU_XANHNGOC, CanGiua("Quản lý sách, tạp chí và báo", W)); InMau(MAU_HONG, "│"); cout << endl;
    cout << LeBanner(); InMau(MAU_HONG, "╰" + Lap("─", W) + "╯"); cout << "\n\n";
}
void MucMenu(string so, string ten, int mau = MAU_HONG)
{
    cout << LeNhap(); InMau(mau, so); cout << " " << ten << endl;
}
void Menu()
{
    MucMenu("[1]", "Thêm tài liệu mới");
    MucMenu("[2]", "Xem danh sách tất cả tài liệu");
    MucMenu("[3]", "Xem chi tiết một tài liệu (chọn theo mã)");
    MucMenu("[4]", "Tìm kiếm (theo mã, tên hoặc nhà xuất bản)");
    MucMenu("[5]", "Sửa thông tin một tài liệu");
    MucMenu("[6]", "Xóa một tài liệu");
    MucMenu("[7]", "Thống kê (số lượng, tổng tiền)");
    MucMenu("[0]", "Thoát", MAU_XAM);
    cout << endl;
}
