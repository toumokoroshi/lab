import cv2
import numpy as np

FLOW_W = 10
FLOW_H = 10

# 画像を読み込む
img_pre = cv2.imread('C:\\lab\\foo\\forOpenCV\\240508\\3.png', 1)
img_now = cv2.imread('C:\\lab\\foo\\forOpenCV\\240508\\4.png', 1)

# 画像が正しく読み込まれたか確認
if img_pre is None or img_now is None:
    print("Error: 画像が見つかりません。パスを確認してください。")
    exit()

# グレースケールに変換
img_pre_g = cv2.cvtColor(img_pre, cv2.COLOR_BGR2GRAY)
img_now_g = cv2.cvtColor(img_now, cv2.COLOR_BGR2GRAY)

# 画像の形状を取得
rows, cols, ch = img_now.shape

# 特徴点を格納する配列を初期化
ps = np.empty((0, 2), np.float32)

# 特徴点を設定
for y in range(0, rows, FLOW_H):  # cols -> rows
    for x in range(0, cols, FLOW_W):  # rows -> cols
        pp = np.array([[x, y]], np.float32)
        ps = np.vstack((ps, pp))

# 光フローを計算
pe, status, error = cv2.calcOpticalFlowPyrLK(img_pre_g, img_now_g, ps, None)

# 光フローを描画
for i in range(len(ps)):
    if status[i]:
        cv2.line(img_now, (int(ps[i][0]), int(ps[i][1])),
                 (int(pe[i][0]), int(pe[i][1])), (0, 0, 255), 2)

# 結果を表示
cv2.imwrite('C:\\lab\\foo\\forOpenCV\\240508\\output.jpg', img_now)
cv2.imshow('opticalflow', img_now)
cv2.waitKey(0)
cv2.destroyAllWindows()
