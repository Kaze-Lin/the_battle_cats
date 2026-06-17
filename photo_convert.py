from pathlib import Path
from PIL import Image

def batch_convert_webp_to_png_and_delete(folder_path):
    # 將字串路徑轉換為 Path 物件
    folder = Path(folder_path)
    
    # 檢查資料夾是否存在
    if not folder.is_dir():
        print(f"錯誤：找不到資料夾 '{folder_path}'")
        return
        
    # 尋找當前資料夾及「所有子資料夾」中所有的 .webp 檔案（忽略大小寫差異）
    # 使用 rglob (recursive glob) 代替 glob 來達成包含子資料夾的掃描
    webp_files = list(folder.rglob("*.webp")) + list(folder.rglob("*.WEBP"))
    
    if not webp_files:
        print("當前資料夾及子資料夾中沒有找到任何 WebP 圖片。")
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
            
            # 這裡移除 .name，直接印出包含資料夾的相對路徑，方便辨識檔案位置
            print(f"✅ 成功轉換並已刪除原檔: {webp_file} -> {png_file}")
            success_count += 1
            
        except Exception as e:
            print(f"❌ 處理失敗 (已保留原檔): {webp_file} (原因: {e})")
            
    print("-" * 50)
    print(f"🎉 任務完成！共成功轉換並清理 {success_count} / {len(webp_files)} 張圖片。")

# ==========================================
# "." 代表執行這個 Python 程式的當前資料夾
# 程式將會自動往下掃描所有的照片與子資料夾
# ==========================================
target_folder = "." 

batch_convert_webp_to_png_and_delete(target_folder)