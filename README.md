﻿# The Floor is Rhythm (The "Floor" Project)

**The Floor is Rhythm** là game hành động nhịp điệu, trong đó người chơi sẽ tương tác với những vật thể đang di chuyển vào chính giữa tương ứng với vị trí (trái, phải) mà nó đi vào.

## Video Demo:
https://streamable.com/chce9o

## Screenshot:
<img src="https://i.ibb.co/Kcyz1Fx5/image.png">
<img src="https://i.ibb.co/mVK1kQQp/image.png">

## Cách chơi:
Tương ứng với mỗi note từ bên trái/phải sang, bạn sẽ ấn phím tương ứng ở phía bên trái (`D`, `F`) hoặc bên phải (`J`, `K`) vào đúng lúc note ở chính giữa màn hình.
Tùy vào độ chính xác mà sẽ ảnh hưởng đến điểm tương ứng note đó (`300`, `100`, `50`, `Miss`).

## Các thành phần Game
| Ảnh | Tên | Mô tả |
|-------|-------|-------|
| <img src="assets/floor.png" height="80px" width="80px"/> | Floor | Object này yêu cầu bạn bấm phím tương ứng đúng lúc. |
| <img src="assets/healthbar-colour.png" width="400px"/> | HP | Lượng máu hiện tại, bạn sẽ luôn bị mất máu, nhưng thao tác object đúng sẽ giúp bạn bù lại lượng máu đã mất. |
| <img src="assets/score-0.png" height="60px" width="35px"/><img src="assets/score-0.png" height="60px" width="35px"/><img src="assets/score-6.png" height="60px" width="35px"/><img src="assets/score-9.png" height="60px" width="35px"/><img src="assets/score-4.png" height="60px" width="35px"/><img src="assets/score-2.png" height="60px" width="35px"/><img src="assets/score-0.png" height="60px" width="35px"/> | Score | Điểm hiện tại của bạn, phụ thuộc vào Combo tối đa và Accuracy hiện tại. Tối đa 1,000,000 điểm. |
| <img src="assets/score-8.png" height="60px" width="35px"/><img src="assets/score-2.png" height="60px" width="35px"/><img src="assets/score-dot.png" height="60px" width="35px"/><img src="assets/score-1.png" height="60px" width="35px"/><img src="assets/score-5.png" height="60px" width="35px"/><img src="assets/score-percent.png" height="60px" width="35px"/> | Accuracy | Độ chính xác của bạn, accuracy càng cao, điểm càng cao. |
| <img src="assets/score-7.png" height="60px" width="35px"/><img src="assets/score-2.png" height="60px" width="35px"/><img src="assets/score-7.png" height="60px" width="35px"/><img src="assets/score-x.png" height="60px" width="35px"/> | Combo | Combo hiện tại của bạn, combo tối đa càng cao, điểm càng cao. |

## Build chương trình
Hướng dẫn build có ở trong file [BUILD.md](BUILD.md).

## Cấu trúc mã nguồn
Project gồm có 2 thư mục chính - `include` (chứa file header) và `src` (chứa mã nguồn). Gồm:

- `format` - Chứa những quy tắc đặt tên, quy tắc format
- `logging` - Hệ thống Logging
- `structures` - Các cấu trúc dữ liệu sử dụng
- `work` - Các công việc chính của chương trình

Trong đó `structures` gồm:

- `audio` - Hệ thống Quản lý Âm thanh/Soundtrack (gồm mixer, bus và memory)
- `events` - Hệ thống Quản lý sự kiện (gồm action, condition, event và time)
- `game` - Cấu trúc chứa dữ liệu game (mapset và event)
- `render` - Engine Render (gồm các cấp độ Layer, Collection, Object, Texture)
- `screen` - Các phân cảnh trong trò chơi

Tổ chức theo hướng module hóa, mô hình dựa trên namespace. Lập trình theo hướng đối tượng hóa.

## Tiến độ
- Đã hoàn thành các hệ thống chính/engine, Gameplay, Logic tính điểm, Nhập dữ liệu từ osu!, Storyboard (phần lớn hoàn thiện)
- Chưa có phần Pause, phần Điểm cuối cùng.
- Hiện tại `main.cpp` vẫn đang ở trạng thái phát triển.
 
## Tham khảo
- Project có sử dụng tài nguyên skin [JesusOmega『Planets』](https://skins.osuck.net/skins/1489)
- Game được lấy cảm hứng từ [osu!](https://osu.ppy.sh/)