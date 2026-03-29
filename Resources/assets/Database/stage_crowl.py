import requests
from bs4 import BeautifulSoup

# 1. 填入你真正要爬的目標網址
url = "https://battlecatsinfo.github.io/stage.html?s=3-9-0"

# 2. 讓程式去連線並把網頁內容抓下來
response = requests.get(url)

# 3. 檢查有沒有成功連上網頁 (狀態碼 200 代表成功)
if response.status_code == 200:
    
    html_content = response.text 

    soup = BeautifulSoup(html_content, 'html.parser')
    
    tbody = soup.find('tbody', id='lines')
    
    if tbody:
        rows = tbody.find_all('tr')
        target_index = 2
        
        if target_index < len(rows):
            target_row = rows[target_index]
            cells = target_row.find_all('td')
            row_data = [cell.text.strip() for cell in cells]
            
            print(f"成功抓取第 {target_index + 1} 行資料：")
            print(row_data)
        else:
            print("找不到該行")
    else:
        print("找不到 tbody")
else:
    print(f"網頁請求失敗，狀態碼：{response.status_code}")