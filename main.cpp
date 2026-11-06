#include "thuvien.h"

int main()
{
    KhoiTaoConsole();
    ThuVien tv;
    if (tv.NapMau() > 0) TamDung();
    while (true) {
        Banner();
        Menu();
        int c = NhapSo("Chọn chức năng", 0, 7);
        switch (c) {
            case 1: tv.MenuThem(); break;
            case 2: tv.HienThiDanhSach(); break;
            case 3: tv.HienThiChiTiet(); break;
            case 4: tv.TimKiem(); break;
            case 5: tv.Sua(); break;
            case 6: tv.Xoa(); break;
            case 7: tv.ThongKe(); break;
            case 0:
                cout << "\n"; InGiua(MAU_HONG, "Cảm ơn bạn đã ghé thư viện! Hẹn gặp lại nhé ♡ ✿"); cout << "\n";
                return 0;
        }
        TamDung();
    }
}
