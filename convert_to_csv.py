import pandas as pd
df = pd.read_parquet('yellow_tripdata_2020-01.parquet')
df.to_csv('yellow_tripdata_2020-01.csv')
