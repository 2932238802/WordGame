import pandas as pd
import json

def generate_item_json(excel_path, output_json):
    # 1. 读取 Excel
    df = pd.read_excel(excel_path, sheet_name="Sheet1")
    
    # 2. 关键修复：先将所有的材料数量列填充为 0，防止 NaN 报错
    # 假设你有 4 组材料，把这些列里的空值填为 0
    count_cols = ['count_1', 'count_2', 'count_3', 'count_4']
    df[count_cols] = df[count_cols].fillna(0)
    
    # 3. 将其他列（名字、描述）的空值填充为空字符串
    df = df.where(pd.notnull(df), "")

    item_list = []
    for _, row in df.iterrows():
        # 跳过 item_name 为空的行
        if not str(row['item_name']).strip():
            continue
            
        name = str(row['item_name']).strip()
        desc = str(row['desc']) if row['desc'] else ""
        ingredients = []
        is_craftable = False

        for i in range(1, 5):
            item_key = f'item_{i}'
            count_key = f'count_{i}'
            
            # 只有当材料名不为空且不是空字符串时才添加
            if row[item_key] and str(row[item_key]).strip() != "":
                is_craftable = True
                
                # 获取数量，如果因为意外还是 NaN，则默认为 0
                raw_amount = row[count_key]
                try:
                    amount = int(float(raw_amount)) # 先转 float 再转 int 可以处理 "1.0" 这种字符串
                except (ValueError, TypeError):
                    amount = 0
                
                ingredients.append({
                    "id": str(row[item_key]).strip(),
                    "amount": amount
                })

        item_obj = {
            "name": name,
            "is_craftable": is_craftable,
            "desc": desc,
            "ingredients": ingredients
        }
        
        item_list.append(item_obj)

    # 4. 写入 JSON
    with open(output_json, 'w', encoding='utf-8') as f:
        json.dump(item_list, f, indent=4, ensure_ascii=False)

    print(f"成功生成 {output_json}，共包含 {len(item_list)} 个条目。")

if __name__ == "__main__":
    generate_item_json("itemData.xlsx", "itemData.json")
