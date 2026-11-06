#pragma once
#include "giaodien.h"

class NgayThang
{
    int ngay, thang, nam;
public:
    NgayThang() : ngay(1), thang(1), nam(2000) {}
    NgayThang(int d, int m, int y) : ngay(d), thang(m), nam(y)
    {
        YeuCau(HopLe(d, m, y), "Ngày tháng năm không hợp lệ!");
    }
    NgayThang(const NgayThang &n) : ngay(n.ngay), thang(n.thang), nam(n.nam) {}
    NgayThang& operator=(const NgayThang &n)
    {
        ngay = n.ngay; thang = n.thang; nam = n.nam;
        return *this;
    }

    int GetNgay()  { return ngay; }
    int GetThang() { return thang; }
    int GetNam()   { return nam; }
    void SetNgay(int v)
    {
        YeuCau(HopLe(v, thang, nam), "Ngày không hợp lệ với tháng và năm hiện có!");
        ngay = v;
    }
    void SetThang(int v)
    {
        YeuCau(HopLe(ngay, v, nam), "Tháng phải từ 1 đến 12 và phù hợp với ngày hiện có!");
        thang = v;
    }
    void SetNam(int v)
    {
        YeuCau(HopLe(ngay, thang, v), "Năm không hợp lệ (từ " + to_string(NAM_NHO_NHAT) + " đến năm hiện tại)!");
        nam = v;
    }

    static bool NamNhuan(int y) { return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0; }
    static int SoNgayTrongThang(int m, int y)
    {
        static int d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        return (m == 2 && NamNhuan(y)) ? 29 : d[m - 1];
    }
    static bool HopLe(int d, int m, int y)
    {
        return y >= NAM_NHO_NHAT && y <= NamHienTai() && m >= 1 && m <= 12 && d >= 1 && d <= SoNgayTrongThang(m, y);
    }

    int Thu()
    {
        int d = ngay, m = thang, y = nam;
        if (m < 3) { m += 12; y--; }
        int k = y % 100, j = y / 100;
        return (d + 13 * (m + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
    }
    bool LaCuoiTuan() { return Thu() == 0 || Thu() == 1; }
    string TenThu()
    {
        static const char *t[] = {"Thứ bảy", "Chủ nhật", "Thứ hai", "Thứ ba", "Thứ tư", "Thứ năm", "Thứ sáu"};
        return t[Thu()];
    }
    string ToString() { ostringstream o; o << *this; return o.str(); }

    friend ostream& operator<<(ostream &os, const NgayThang &n)
    {
        os << setfill('0') << setw(2) << n.ngay << '/' << setw(2) << n.thang << '/' << setw(4) << n.nam << setfill(' ');
        return os;
    }
    friend istream& operator>>(istream &is, NgayThang &n)
    {
        int d, m, y;
        char c1, c2;
        is >> d >> c1 >> m >> c2 >> y;
        if (!is.fail() && c1 == '/' && c2 == '/' && HopLe(d, m, y)) { n.ngay = d; n.thang = m; n.nam = y; }
        else is.setstate(ios::failbit);
        return is;
    }

    static NgayThang NhapTuBanPhim(string nhan)
    {
        InGiua(MAU_XAM, "(Định dạng ngày/tháng/năm, ví dụ 17/10/2026)");
        while (true) {
            istringstream ss(NhapChuoi(nhan));
            NgayThang n;
            ss >> n;
            if (!ss.fail()) return n;
            LoiNhap("Ngày không hợp lệ! Định dạng đúng là ngày/tháng/năm, ví dụ 17/10/2026; năm không được lớn hơn năm hiện tại.");
        }
    }
};

class TaiLieu
{
    // Toàn bộ dữ liệu và hằng số đều private
    string maTL, tenTL, nhaXB;
    int namXB;
    static const int MA_TOI_DA = 9;
protected:
    virtual void NhapNamXuatBan()
    {
        SetNamXB(NhapSo("Năm xuất bản", NAM_NHO_NHAT, NamHienTai()));
    }
public:
    static int MaToiDa() { return MA_TOI_DA; }      // getter static cho hằng private

    static bool HopLeMa(string ma)                  
    {
        if (ma.empty() || (int)ma.size() > MA_TOI_DA) return false;
        for (size_t i = 0; i < ma.size(); i++)
            if (!isalnum((unsigned char)ma[i])) return false;
        return true;
    }

    // 3 hàm thiết lập: mặc định, có tham số, sao chép 
    TaiLieu() : maTL("TAILIEU00"), tenTL("Chưa đặt tên"), nhaXB("Chưa rõ"), namXB(2000) {}
    
    // gọi các hàm Set để kiểm tra
    TaiLieu(string ma, string ten, string nxb, int nam) : namXB(2000)
    {
        SetMaTL(ma);
        SetTenTL(ten);
        SetNhaXB(nxb);
        SetNamXB(nam);
    }
    TaiLieu(const TaiLieu &t) : maTL(t.maTL), tenTL(t.tenTL), nhaXB(t.nhaXB), namXB(t.namXB) {}
    virtual ~TaiLieu() {}

    string GetMaTL()  { return maTL; }
    string GetTenTL() { return tenTL; }
    string GetNhaXB() { return nhaXB; }
    int GetNamXB()    { return namXB; }

    // Get / Set (Set ném invalid_argument nếu giá trị sai) 
    void SetMaTL(string v)
    {
        v = Trim(v);
        YeuCau(HopLeMa(v), "Mã tài liệu chỉ gồm chữ cái không dấu và chữ số, dài từ 1 đến " + to_string(MA_TOI_DA) + " ký tự!");
        maTL = ChuHoaAscii(v);                  // mã luôn lưu chữ hoa: sach001 -> SACH001
    }
    void SetTenTL(string v)
    {
        v = Trim(v);
        YeuCau(!v.empty(), "Tên tài liệu không được để trống!");
        tenTL = v;
    }
    void SetNhaXB(string v)
    {
        v = Trim(v);
        YeuCau(!v.empty(), "Nhà xuất bản không được để trống!");
        nhaXB = v;
    }
    virtual void SetNamXB(int v)
    {
        YeuCau(v >= NAM_NHO_NHAT && v <= NamHienTai(),
               "Năm xuất bản phải từ " + to_string(NAM_NHO_NHAT) + " đến " + to_string(NamHienTai()) + "!");
        namXB = v;
    }
    // Phương thức ảo 
    virtual void Nhap()
    {
        SetTenTL(NhapChuoi("Tên tài liệu"));
        SetNhaXB(NhapChuoi("Nhà xuất bản"));
        NhapNamXuatBan();
    }
    virtual void HienThiThongTin()
    {
        DongThe("Mã tài liệu", maTL);
        DongThe("Tên tài liệu", tenTL);
        DongThe("Nhà xuất bản", nhaXB);
        DongThe("Năm xuất bản", to_string(namXB));
    }

    // Phương thức thuần ảo -> TaiLieu là lớp trừu tượng 
    virtual double TinhTien() = 0;
    virtual string CachTinh() = 0;
    virtual string GetLoai() = 0;
    virtual string ThongTinRieng() = 0;

    // Quá tải toán tử nhập / xuất 
    friend istream& operator>>(istream &is, TaiLieu &t) { t.Nhap(); return is; }
    friend ostream& operator<<(ostream &os, TaiLieu &t)
    {
        os << "[" << t.GetLoai() << "] " << t.maTL << " - " << t.tenTL
           << " (" << t.nhaXB << ", " << t.namXB << ") - " << DinhDangTien(t.TinhTien());
        return os;
    }
};

class Sach : public TaiLieu
{
    string tacGia;
    int soTrang;
    int giaBia;                     // giá bìa do nhà xuất bản niêm yết (đồng)
    // hằng private, chỉ dùng trong Sach
    static const int SO_TRANG_TOI_DA = 10000;                         
    static const int GIA_TOI_THIEU = 1000, GIA_TOI_DA = 10000000;
public:

    // 3 hàm thiết lập 
    Sach() : TaiLieu("SACH000", "Chưa đặt tên", "Chưa rõ", 2000), tacGia("Khuyết danh"), soTrang(100), giaBia(50000) {}
    Sach(string ma, string ten, string nxb, int nam, string tg, int trang, int gia)
        : TaiLieu(ma, ten, nxb, nam), soTrang(1), giaBia(GIA_TOI_THIEU)
    {
        SetTacGia(tg);
        SetSoTrang(trang);
        SetGiaBia(gia);
    }
    Sach(const Sach &s) : TaiLieu(s), tacGia(s.tacGia), soTrang(s.soTrang), giaBia(s.giaBia) {}

    string GetTacGia() { return tacGia; }
    int GetSoTrang()   { return soTrang; }
    int GetGiaBia()    { return giaBia; }
    void SetTacGia(string v)
    {
        v = Trim(v);
        YeuCau(!v.empty(), "Tác giả không được để trống!");
        tacGia = v;
    }
    void SetSoTrang(int v)
    {
        YeuCau(v >= 1 && v <= SO_TRANG_TOI_DA, "Số trang phải từ 1 đến " + to_string(SO_TRANG_TOI_DA) + "!");
        soTrang = v;
    }
    void SetGiaBia(int v)
    {
        YeuCau(v >= GIA_TOI_THIEU && v <= GIA_TOI_DA,
               "Giá bìa phải từ " + to_string(GIA_TOI_THIEU) + " đến " + to_string(GIA_TOI_DA) + " đồng!");
        giaBia = v;
    }

    // TaiLieu:: Nhap() rồi hỏi tác giả, số trang, giá bìa
    void Nhap()
    {
        TaiLieu::Nhap();
        SetTacGia(NhapChuoi("Tác giả"));
        SetSoTrang(NhapSo("Số trang", 1, SO_TRANG_TOI_DA));
        SetGiaBia(NhapSo("Giá bìa (đồng)", GIA_TOI_THIEU, GIA_TOI_DA));
    }
    void HienThiThongTin()          // in thẻ "SÁCH"
    {
        MoThe("✿ SÁCH ✿");
        TaiLieu::HienThiThongTin();
        DongThe("Tác giả", tacGia);
        DongThe("Số trang", to_string(soTrang));
        DongThe("Cách tính", CachTinh());
        DongThe("Giá", DinhDangTien(TinhTien()), MAU_XANHLA);
        DayThe();
    }
    double TinhTien() { return giaBia; }
    string CachTinh() { return "Giá bìa do nhà xuất bản niêm yết trên sách"; }
    string GetLoai() { return "Sách"; }
    string ThongTinRieng() { return tacGia; }
};

class TapChi : public TaiLieu
{
    int soPhatHanh, thangPhatHanh;
    bool dacBiet;                       // true: số đặc biệt (dày hơn, giá cao hơn)
public:
    TapChi() : TaiLieu("TAPCHI000", "Chưa đặt tên", "Chưa rõ", 2000), soPhatHanh(1), thangPhatHanh(1), dacBiet(false) {}
    TapChi(string ma, string ten, string nxb, int nam, int so, int thang, bool db)
        : TaiLieu(ma, ten, nxb, nam), soPhatHanh(1), thangPhatHanh(1), dacBiet(db)
    {
        SetSoPhatHanh(so);
        SetThangPhatHanh(thang);
    }
    TapChi(const TapChi &t) : TaiLieu(t), soPhatHanh(t.soPhatHanh), thangPhatHanh(t.thangPhatHanh), dacBiet(t.dacBiet) {}

    int GetSoPhatHanh()    { return soPhatHanh; }
    int GetThangPhatHanh() { return thangPhatHanh; }
    bool LaSoDacBiet()     { return dacBiet; }
    void SetDacBiet(bool v)      { dacBiet = v; }
    void SetSoPhatHanh(int v)
    {
        YeuCau(v >= 1 && v <= 9999, "Số phát hành phải từ 1 đến 9999!");
        soPhatHanh = v;
    }
    void SetThangPhatHanh(int v)
    {
        YeuCau(v >= 1 && v <= 12, "Tháng phát hành phải từ 1 đến 12!");
        thangPhatHanh = v;
    }

    void Nhap()
    {
        TaiLieu::Nhap();
        SetSoPhatHanh(NhapSo("Số phát hành", 1, 9999));
        SetThangPhatHanh(NhapSo("Tháng phát hành", 1, 12));
        SetDacBiet(NhapSo("Loại số, 1 là số thường, 2 là số đặc biệt", 1, 2) == 2);
    }
    void HienThiThongTin()
    {
        MoThe("✿ TẠP CHÍ ✿");
        TaiLieu::HienThiThongTin();
        DongThe("Số phát hành", to_string(soPhatHanh));
        DongThe("Tháng phát hành", to_string(thangPhatHanh));
        DongThe("Loại số", dacBiet ? "Số đặc biệt" : "Số thường");
        DongThe("Cách tính", CachTinh());
        DongThe("Giá", DinhDangTien(TinhTien()), MAU_XANHLA);
        DayThe();
    }
    double TinhTien() { return dacBiet ? 46000 : 27000; }
    string CachTinh() { return "Số thường 27.000đ; số đặc biệt (dày hơn) 46.000đ, theo bảng giá Tạp chí Tòa án nhân dân 2026"; }
    string GetLoai() { return "Tạp chí"; }
    string ThongTinRieng() { return "Số " + to_string(soPhatHanh) + ", tháng " + to_string(thangPhatHanh); }
};

class Bao : public TaiLieu
{
    NgayThang ngayPhatHanh;
    // true: ấn bản cuối tuần (nhiều trang, giá cao hơn)
    bool cuoiTuan;
protected:
    void NhapNamXuatBan() {}
public:
    Bao() : TaiLieu("BAO000", "Chưa đặt tên", "Chưa rõ", 2000), ngayPhatHanh(), cuoiTuan(false) {}
    Bao(string ma, string ten, string nxb, NgayThang ngay, bool ct)
        : TaiLieu(ma, ten, nxb, ngay.GetNam()), ngayPhatHanh(ngay), cuoiTuan(ct) {}
    Bao(const Bao &b) : TaiLieu(b), ngayPhatHanh(b.ngayPhatHanh), cuoiTuan(b.cuoiTuan) {}

    NgayThang GetNgayPhatHanh() { return ngayPhatHanh; }
    bool LaCuoiTuan()           { return cuoiTuan; }
    void SetCuoiTuan(bool v)          { cuoiTuan = v; }
    void SetNgayPhatHanh(NgayThang n)
    {
        ngayPhatHanh = n;
        TaiLieu::SetNamXB(n.GetNam());
    }
    void SetNamXB(int v)
    {
        NgayThang n = ngayPhatHanh;
        n.SetNam(v);
        SetNgayPhatHanh(n);
    }

    void Nhap()
    {
        TaiLieu::Nhap();
        SetCuoiTuan(NhapSo("Loại báo, 1 là báo hằng ngày, 2 là báo cuối tuần", 1, 2) == 2);
        SetNgayPhatHanh(NgayThang::NhapTuBanPhim("Ngày phát hành"));
    }
    void HienThiThongTin()
    {
        MoThe("✿ BÁO ✿");
        TaiLieu::HienThiThongTin();
        DongThe("Ngày phát hành", ngayPhatHanh.ToString() + " (" + ngayPhatHanh.TenThu() + ")");
        DongThe("Loại báo", cuoiTuan ? "Báo cuối tuần" : "Báo hằng ngày");
        DongThe("Cách tính", CachTinh());
        DongThe("Giá", DinhDangTien(TinhTien()), MAU_XANHLA);
        DayThe();
    }
    double TinhTien() { return cuoiTuan ? 4800 : 4200; }
    string CachTinh() { return "Báo hằng ngày 4.200đ; báo cuối tuần (nhiều trang hơn) 4.800đ, theo giá lẻ báo Nhân Dân"; }
    string GetLoai() { return "Báo"; }
    string ThongTinRieng() { return "Ngày " + ngayPhatHanh.ToString(); }
};

const int MAX = 100;

class ThuVien
{
    // mảng con trỏ lớp cơ sở -> đa hình
    TaiLieu *ds[MAX];
    int n;

    // cấm sao chép: tránh hai đối tượng
    ThuVien(const ThuVien &);
    ThuVien& operator=(const ThuVien &);        // cùng delete một vùng nhớ

    int TimViTri(string ma)
    {
        for (int i = 0; i < n; i++)
            if (ChuThuong(ds[i]->GetMaTL()) == ChuThuong(ma)) return i;
        return -1;
    }
    static TaiLieu* TaoMoi(string loai)
    {
        if (loai == "Sách") return new Sach;
        if (loai == "Tạp chí") return new TapChi;
        return new Bao;
    }

    static void Vien(int w[], int sl, string trai, string giua, string phai)
    {
        cout << LeBang();
        string s = trai;
        for (int i = 0; i < sl; i++) s += Lap("─", w[i] + 2) + (i + 1 < sl ? giua : phai);
        InMau(MAU_XANHNGOC, s);
        cout << endl;
    }
    static void Hang(string c[], int w[], int mau[], char can[], int sl)
    {
        cout << LeBang(); InMau(MAU_XANHNGOC, "│");
        for (int i = 0; i < sl; i++) {
            string o = Cat(c[i], w[i]);
            if (can[i] == 'P') o = CanPhai(o, w[i]);
            else if (can[i] == 'G') o = CanGiua(o, w[i]);
            else o = CanTrai(o, w[i]);
            cout << " "; InMau(mau[i], o); cout << " "; InMau(MAU_XANHNGOC, "│");
        }
        cout << endl;
    }
    static void Bang(TaiLieu *v[], int sl, string tieuDe)
    {
        int w[8] = {3, 7, 9, 26, 14, 8, 16, 10};
        char canDau[8] = {'G', 'G', 'G', 'G', 'G', 'G', 'G', 'G'};
        char can[8]    = {'P', 'T', 'T', 'T', 'T', 'G', 'T', 'P'};
        string phu = "  (" + to_string(sl) + " mục)";
        cout << "\n" << LeGiua(DoDai(tieuDe) + DoDai(phu)); InMau(MAU_HONG, tieuDe); InMau(MAU_XAM, phu); cout << endl;
        if (sl == 0) { InGiua(MAU_XAM, "Chưa có tài liệu nào."); return; }
        Vien(w, 8, "╭", "┬", "╮");
        string dau1[8] = {"", "", "", "", "Nhà", "Năm", "Thông tin", ""};
        string dau2[8] = {"#", "Loại", "Mã", "Tên tài liệu", "xuất bản", "xuất bản", "riêng", "Giá"};
        int mauDau[8] = {MAU_HONG, MAU_HONG, MAU_HONG, MAU_HONG, MAU_HONG, MAU_HONG, MAU_HONG, MAU_HONG};
        Hang(dau1, w, mauDau, canDau, 8);
        Hang(dau2, w, mauDau, canDau, 8);
        Vien(w, 8, "├", "┼", "┤");
        for (int i = 0; i < sl; i++) {
            string c[8] = {to_string(i + 1), v[i]->GetLoai(), v[i]->GetMaTL(), v[i]->GetTenTL(), v[i]->GetNhaXB(),
                           to_string(v[i]->GetNamXB()), v[i]->ThongTinRieng(), DinhDangTien(v[i]->TinhTien())};
            int mau[8] = {MAU_XAM, MauLoai(v[i]->GetLoai()), MAU_VANG, MAU_MACDINH, MAU_MACDINH,
                          MAU_MACDINH, MAU_MACDINH, MAU_XANHLA};
            Hang(c, w, mau, can, 8);
        }
        Vien(w, 8, "╰", "┴", "╯");
        InGiua(MAU_XAM, "Thông tin riêng: sách là tác giả; tạp chí là số và tháng phát hành; báo là ngày phát hành.");
    }

public:
    ThuVien() : n(0) {}
    // giải phóng bộ nhớ động
    ~ThuVien() { for (int i = 0; i < n; i++) delete ds[i]; }

    // đầy hoặc trùng mã thì delete t, trả false
    bool Them(TaiLieu *t)
    {
        if (n >= MAX) { ThongBaoLoi("Thư viện đã đầy!"); delete t; return false; }
        if (TimViTri(t->GetMaTL()) != -1) {
            ThongBaoLoi("Mã '" + t->GetMaTL() + "' đã tồn tại, không thể thêm!");
            delete t;
            return false;
        }
        ds[n++] = t;
        return true;
    }
    bool ThemSach(string ma, string ten, string nxb, int nam, string tacGia, int soTrang, int giaBia)
    {
        try { return Them(new Sach(ma, ten, nxb, nam, tacGia, soTrang, giaBia)); }
        catch (invalid_argument &e) { ThongBaoLoi("Không thêm được sách '" + ma + "': " + e.what()); return false; }
    }
    bool ThemTapChi(string ma, string ten, string nxb, int nam, int soPhatHanh, int thangPhatHanh, bool dacBiet)
    {
        try { return Them(new TapChi(ma, ten, nxb, nam, soPhatHanh, thangPhatHanh, dacBiet)); }
        catch (invalid_argument &e) { ThongBaoLoi("Không thêm được tạp chí '" + ma + "': " + e.what()); return false; }
    }
    bool ThemBao(string ma, string ten, string nxb, int ngay, int thang, int nam, bool cuoiTuan)
    {
        try { return Them(new Bao(ma, ten, nxb, NgayThang(ngay, thang, nam), cuoiTuan)); }
        catch (invalid_argument &e) { ThongBaoLoi("Không thêm được báo '" + ma + "': " + e.what()); return false; }
    }

    // nạp 7 tài liệu mẫy, trả về số tài liệu lỗi
    int NapMau()
    {
        int loi = 0;
        if (!ThemSach("SACH001", "Dế Mèn phiêu lưu ký (2023)", "Kim Đồng", 2023, "Tô Hoài", 48, 60000)) loi++;
        if (!ThemSach("SACH002", "Dế Mèn phiêu lưu ký - Diary of a Cricket (2023)", "Kim Đồng", 2023, "Tô Hoài", 296, 65000)) loi++;
        if (!ThemTapChi("TAPCHI001", "Tạp chí Tòa án nhân dân", "Tòa án nhân dân tối cao", 2026, 19, 10, false)) loi++;
        if (!ThemTapChi("TAPCHI002", "Tạp chí Tòa án nhân dân", "Tòa án nhân dân tối cao", 2026, 20, 10, false)) loi++;
        if (!ThemBao("BAO001", "Nhân Dân cuối tuần", "Báo Nhân Dân", 2, 10, 2026, true)) loi++;
        if (!ThemBao("BAO002", "Nhân Dân cuối tuần", "Báo Nhân Dân", 9, 10, 2026, true)) loi++;
        if (!ThemBao("BAO003", "Nhân Dân cuối tuần", "Báo Nhân Dân", 23, 10, 2026, true)) loi++;
        return loi;
    }

    string NhapMa(string nhan)
    {
        HienThiDanhSach();
        InGiua(MAU_XAM, "(Gõ mã ở cột 'Mã' trong bảng trên, ví dụ: SACH001)");
        return NhapChuoi(nhan);
    }
    string NhapMaMoi(int boQua)
    {
        InGiua(MAU_XAM, "(Chữ cái không dấu và chữ số, tối đa " + to_string(TaiLieu::MaToiDa()) + " ký tự, ví dụ SACH004)");
        while (true) {
            string ma = NhapChuoi("Mã tài liệu");
            if (!TaiLieu::HopLeMa(ma)) {
                LoiNhap("Mã chỉ gồm chữ cái không dấu và chữ số, dài từ 1 đến " + to_string(TaiLieu::MaToiDa()) + " ký tự!");
                continue;
            }
            int j = TimViTri(ma);
            if (j != -1 && j != boQua) {
                LoiNhap("Mã '" + ChuHoaAscii(ma) + "' đã tồn tại, hãy nhập mã khác!");
                continue;
            }
            return ma;
        }
    }

    void MenuThem()
    {
        cout << "\n" << LeGiua(50); InMau(MAU_HONG, "[1]"); cout << " Sách    ";
        InMau(MAU_XANHDUONG, "[2]"); cout << " Tạp chí    ";
        InMau(MAU_VANG, "[3]"); cout << " Báo    ";
        InMau(MAU_XAM, "[0]"); cout << " Quay lại\n";
        int c = NhapSo("Chọn loại tài liệu", 0, 3);
        if (c == 0) return;
        TaiLieu *t = TaoMoi(c == 1 ? "Sách" : (c == 2 ? "Tạp chí" : "Báo"));
        cout << "\n"; InGiua(MAU_HONG, "✿ NHẬP THÔNG TIN ✿");
        t->SetMaTL(NhapMaMoi(-1));
        cin >> *t;
        ostringstream o;
        o << *t;
        if (Them(t)) ThongBaoOk("Đã thêm: " + o.str());
    }
    void HienThiDanhSach() { Bang(ds, n, "✿ DANH SÁCH TÀI LIỆU ✿"); }
    void HienThiChiTiet()
    {
        string ma = NhapMa("Nhập mã cần xem");
        int i = TimViTri(ma);
        if (i < 0) { ThongBaoLoi("Không tìm thấy mã '" + ma + "'!"); return; }
        // ĐA HÌNH: tự gọi đúng phiên bản của lớp con
        ds[i]->HienThiThongTin();
    }
    void TimKiem()
    {
        string k = ChuThuong(NhapChuoi("Từ khóa (mã, tên hoặc nhà xuất bản)"));
        TaiLieu *kq[MAX];
        int sl = 0;
        for (int i = 0; i < n; i++) {
            string g = ChuThuong(ds[i]->GetMaTL() + " " + ds[i]->GetTenTL() + " " + ds[i]->GetNhaXB());
            if (g.find(k) != string::npos) kq[sl++] = ds[i];
        }
        Bang(kq, sl, "✿ KẾT QUẢ TÌM KIẾM ✿");
    }
    void Sua()
    {
        string ma = NhapMa("Nhập mã cần sửa");
        int i = TimViTri(ma);
        if (i < 0) { ThongBaoLoi("Không tìm thấy mã '" + ma + "'!"); return; }
        ds[i]->HienThiThongTin();
        cout << "\n"; InGiua(MAU_HONG, "✿ NHẬP LẠI THÔNG TIN MỚI ✿");
        InGiua(MAU_XAM, "(Muốn giữ nguyên mã cũ thì nhập lại đúng mã cũ)");
        TaiLieu *moi = TaoMoi(ds[i]->GetLoai());
        moi->SetMaTL(NhapMaMoi(i));
        cin >> *moi;
        delete ds[i];
        ds[i] = moi;
        ThongBaoOk("Đã cập nhật thông tin");
    }
    void Xoa()
    {
        string ma = NhapMa("Nhập mã cần xóa");
        int i = TimViTri(ma);
        if (i < 0) { ThongBaoLoi("Không tìm thấy mã '" + ma + "'!"); return; }
        ds[i]->HienThiThongTin();
        if (ChuThuong(NhapChuoi("Chắc chắn xóa? (gõ y để xóa, gõ n để hủy)")) == "y") {
            delete ds[i];
            for (int k = i; k < n - 1; k++) ds[k] = ds[k + 1];
            n--;
            ThongBaoOk("Đã xóa tài liệu");
        }
        else { cout << "\n"; InGiua(MAU_XAM, "Đã hủy xóa."); }
    }
    void ThongKe()
    {
        MoThe("✿ THỐNG KÊ ✿");
        string loai[3] = {"Sách", "Tạp chí", "Báo"};
        double tong = 0;
        for (int k = 0; k < 3; k++) {
            int sl = 0;
            double tien = 0;
            for (int i = 0; i < n; i++)
                if (ds[i]->GetLoai() == loai[k]) { sl++; tien += ds[i]->TinhTien(); }
            tong += tien;
            DongThe(loai[k], to_string(sl) + " " + Lap("█", min(sl, 15)) + "  " + DinhDangTien(tien), MauLoai(loai[k]));
        }
        DongThe("Tổng số", to_string(n) + " tài liệu");
        DongThe("Tổng giá trị", DinhDangTien(tong), MAU_XANHLA);
        DayThe();
    }
};
