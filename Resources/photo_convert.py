from pathlib import Path
from PIL import Image

def batch_convert_webp_to_png_and_delete(folder_path):
    # 將字串路徑轉換為 Path 物件
    folder = Path(folder_path)
    
    # 檢查資料夾是否存在
    if not folder.is_dir():
        print(f"錯誤：找不到資料夾 '{folder_path}'")
        return
        
    # 尋找資料夾中所有的 .webp 檔案（忽略大小寫差異）
    webp_files = list(folder.glob("*.webp")) + list(folder.glob("*.WEBP"))
    
    if not webp_files:
        print("資料夾中沒有找到任何 WebP 圖片。")
        return

    print(f"總共找到 {len(webp_files)} 個 WebP 檔案，開始轉換並刪除原檔...\n")
    print("-" * 50)
    
    success_count = 0
    
    for webp_file in webp_files:
        # 建立新的檔案路徑，將副檔名替換為 .png
        png_file = webp_file.with_suffix('.png')
        
        try:
            # 1. 打開 WebP 圖片並另存為 PNG
            with Image.open(webp_file) as img:
                img.save(png_file, "PNG")
                
            # 2. 轉檔成功後，刪除原本的 WebP 檔案
            webp_file.unlink()
            
            print(f"✅ 成功轉換並已刪除原檔: {webp_file.name} -> {png_file.name}")
            success_count += 1
            
        except Exception as e:
            print(f"❌ 處理失敗 (已保留原檔): {webp_file.name} (原因: {e})")
            
    print("-" * 50)
    print(f"🎉 任務完成！共成功轉換並清理 {success_count} / {len(webp_files)} 張圖片。")

# ==========================================
# 請在這裡填入你的資料夾路徑
# 預設 "./" 代表與這個 Python 程式同一個資料夾
# ==========================================
target_folder = "./towel" 

batch_convert_webp_to_png_and_delete(target_folder)