import pandas as pd;
import json;
df = pd.read_excel("foodData.xlsx",sheet_name="Sheet1")
def cleanBool(val):
    return str(val).upper() == "TRUE"
df["composed"] = df["composed"].apply(cleanBool)
cols_to_fill = [col for col in df.columns if col not in ['composed']]
df[cols_to_fill] = df[cols_to_fill].fillna(0)
data_list = df.to_dict(orient='records')

with open('foodData.json','w',encoding='utf-8') as f:
    json.dump(data_list,f,indent=4,ensure_ascii=False)



