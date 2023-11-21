# Radix Spline with the power of k choices
The idea is to create multiple Radix Spline indexes to capture different underlying data distributions and improve query performance.
Results suggest a strong memmory compression and and average improvement in query performance.





### Data preparation

download wiki dataset from
https://dataverse.harvard.edu/api/access/datafile/:persistentId?persistentId=doi:10.7910/DVN/JGVF9A/SVN8PI
extract the donwloaded dataset
and put the extracted dataset to build folder

download taxi dataset from https://s3.amazonaws.com/nyc-tlc/trip+data/yellow_tripdata_2020-01.parquet
run convert_to_csv.py to extract the csv dataset file
and put the csv dataset to build folder

## Getting Started

put run_n_choices.sh to build folder
uncomment line 162 for wiki dataset
or uncomment line 161 for taxi dataset
or uncomment lines 163 to 185 for synthetic dataset
(make sure for synthetic dataset max_error = 1)

go to include/rs/builder file to change max error, 
go to line 16
change argument size_t max_error, eg. size_t max_error = 1
make is needed for every change in max_error

```mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make```

for various experiments:
 ```cd build
 make
 ./run_n_choices.sh```
 repeat for different error/dataset
