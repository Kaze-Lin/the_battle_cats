import os
import subprocess
from pathlib import Path

def convert_ogg_to_mp3_and_delete(root_dir: str):
    # 將路徑字串轉換為 Path 物件
    target_dir = Path(root_dir)
    
    if not target_dir.exists() or not target_dir.is_dir():
        print(f"錯誤：找不到資料夾 {root_dir}")
        return

    # rglob("*.ogg") 會遞迴搜尋所有子資料夾內的 ogg 檔
    ogg_files = list(target_dir.rglob("*.ogg"))
    
    if not ogg_files:
        print("在指定的資料夾中找不到任何 .ogg 檔案。")
        return

    print(f"🔍 找到 {len(ogg_files)} 個 .ogg 檔案，準備開始轉換...\n")

    for ogg_path in ogg_files:
        # 建立輸出的 mp3 檔案路徑 (副檔名直接替換為 .mp3)
        mp3_path = ogg_path.with_suffix(".mp3")
        
        print(f"🔄 轉換中: {ogg_path.name} -> {mp3_path.name}")
        
        # 組合 ffmpeg 指令
        # -i: 來源檔案
        # -q:a 0: 使用 LAME VBR 最高品質 (約 220-260 kbps)
        # -y: 若遇到同名檔案直接覆寫
        command = [
            "ffmpeg",
            "-i", str(ogg_path),
            "-q:a", "0",
            "-y",
            str(mp3_path)
        ]
        
        try:
            # 執行轉檔指令，stdout 和 stderr 導向 DEVNULL 讓終端機畫面保持乾淨
            result = subprocess.run(command, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            
            # 檢查 returncode，0 代表正常結束 (成功)
            if result.returncode == 0:
                # 確認 mp3 真的有生成出來，再執行刪除
                if mp3_path.exists():
                    ogg_path.unlink()  # 刪除原始 ogg 檔
                    print(f"✅ 成功：已轉檔並刪除原檔 ({ogg_path.name})")
                else:
                    print(f"❌ 錯誤：轉檔似乎成功，但找不到輸出的 mp3 檔案 ({mp3_path.name})")
            else:
                print(f"❌ 失敗：FFmpeg 回報轉換錯誤 ({ogg_path.name})")
                
        except FileNotFoundError:
            print("\n❌ 嚴重錯誤：系統找不到 'ffmpeg' 指令！")
            print("請確認您已安裝 FFmpeg 並將其加入了系統的環境變數 (PATH)。")
            break
        except Exception as e:
            print(f"❌ 發生未知的錯誤: {e}")

if __name__ == "__main__":
    # 👇 在這裡設定你要處理的資料夾路徑 (相對路徑或絕對路徑皆可)
    # 例如： r"C:\Users\YourName\Desktop\GameAssets" 或 "./audio_folder"
    folder_path = "./" 
    
    convert_ogg_to_mp3_and_delete(folder_path)