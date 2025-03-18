# 🚀 Hướng dẫn Build Chương Trình (Dành cho Developers)

---

## ⚠️ Lưu ý trước khi Build
Trước khi tiến hành build, hãy đảm bảo bạn đã cài đặt các công cụ sau:
- ✅ **[Git](https://git-scm.com/downloads)**: Clone repository.
- ✅ **[vcpkg](https://github.com/microsoft/vcpkg)**: Quản lý package.

---

## 🛠 Bước 1: Clone Repository
📥 Tải mã nguồn từ GitHub:
```sh
git clone https://github.com/thnhmai06/the-floor-is-rhythm.git --recursive
cd the-floor-is-rhythm
```
📌 *Lưu ý: Thêm `--recursive` để tải cả submodules!*

---

## 📦 Bước 2: Cài Đặt Dependencies
📌 Sử dụng **vcpkg** để cài đặt các thư viện cần thiết:
```sh
vcpkg install
```

---


🎉 **Hoàn tất!** Giờ bạn có thể tiếp tục build và chạy chương trình!

