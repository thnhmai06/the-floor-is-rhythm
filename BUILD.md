# 🚀 Hướng dẫn Build Chương Trình (Dành cho Developers)

---

## ⚠️ Lưu ý trước khi Build
Trước khi tiến hành build, hãy đảm bảo bạn đã cài đặt các công cụ sau:

- ✅ **[Git](https://git-scm.com/downloads)**: 📥 Clone repository.
- ✅ **[vcpkg](https://github.com/microsoft/vcpkg)**: 📦 Quản lý package.
- ✅ **[MSBuild](https://github.com/dotnet/msbuild)**: 🔨 Build Project.

Nếu bạn sử dụng **[Visual Studio 2022](https://visualstudio.microsoft.com/)**, bạn không cần phải cài đặt **vcpkg** và **MSBuild**, vì thường Visual Studio đã tích hợp sẵn. 🏗️

Ngoài ra:
- ✅ Cần đảm bảo đã tích hợp **vcpkg** vào **MSBuild**  (chỉ cần làm một lần): 
  ```sh
  vcpkg integrate install
  ```

---

## 🛠 Bước 1: Clone Repository
👝 Tải mã nguồn từ GitHub:
```sh
git clone https://github.com/thnhmai06/the-floor-is-rhythm.git --recursive
cd the-floor-is-rhythm
```
📌 *Lưu ý: Thêm `--recursive` để tải cả submodules!* 🔗

---

## 🛠️ Bước 2: Build Chương Trình

### 🔹 Trường hợp 1: Có Visual Studio 2022
1. 🎯 Mở **Visual Studio 2022**.
2. 📂 Chọn **File** → **Open** → **Project/Solution...**.
3. 📄 Mở file `.sln` trong thư mục dự án.
4. ⚙️ Chọn cấu hình **Release** hoặc **Debug**.
5. 🔨 Chọn **Build -> Build Solution (Ctrl + Shift + B)**.
6. ✅ Sau khi build xong, file chương trình sẽ ở 📁 `bin`.

---

### 🔸 Trường hợp 2: Không có Visual Studio (Build thủ công)
1. 🔧 Dùng **MSBuild** để tự cài đặt các package và build:
   ```sh
   msbuild
   ```
2. ✅ Sau khi build xong, file chương trình sẽ ở 📁 `bin`.

---

🎉 **Giờ bạn có thể chạy chương trình! 🚀**

