#include <iostream>
#include <ctime>  
#include "include/rs/multi_map.h"
#include <bits/stdc++.h>
#include <numeric>
#include "parsers.h"
//#include "osm_parser.h"
//#include "fb_parser.h"
#include "wiki_parser.h"
//#include "book_parser.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <iomanip>
#include <thread>

const size_t kNumKeys = 1e6;

using namespace std::this_thread;
using namespace std;
/*cmake -DCMAKE_BUILD_TYPE=Release ..
make
./example
./tester*/

template<class KeyType>
vector<KeyType> CreateSkewedKeys(size_t seed) {
  vector<KeyType> keys;
  keys.reserve(kNumKeys);

  // Generate lognormal values.
  mt19937 g(seed);
  lognormal_distribution<double> d(/*mean*/0, /*stddev=*/2);
  vector<double> lognormal_values;
  lognormal_values.reserve(kNumKeys);
  for (size_t i = 0; i < kNumKeys; ++i) lognormal_values.push_back(d(g));
  const auto min_max = std::minmax_element(lognormal_values.begin(), lognormal_values.end());
  const double min = *min_max.first;
  const double max = *min_max.second;
  const double diff = max - min;

  // Scale values to the entire `KeyType` domain.
  const auto domain = std::numeric_limits<KeyType>::max() - std::numeric_limits<KeyType>::min();
  for (size_t i = 0; i < kNumKeys; ++i) {
    const double ratio = (lognormal_values[i] - min) / diff;
    keys.push_back(ratio * domain);
  }

  sort(keys.begin(), keys.end());
  return keys;
}


int argmax(double a[], int n) {
  int c, index = 0;
  double max = a[0];
  for (c = 0; c < n; c++)
    if (a[c] > max)
      index = c;
      max = a[c];

  return index;
}

int argmin(double a[], int n) {
  int c, index = 0;
  double min = a[0];
  for (c = 0; c < n; c++)
    if (a[c] < min)
      index = c;
      min = a[c];

  return index;
}

int argmin(uint64_t a[], int n) {
  int c, index = 0;
  uint64_t min = a[0];
  for (c = 0; c < n; c++)
    if (a[c] < min)
      index = c;
      min = a[c];

  return index;
}

double arr_average(double a[], int n) {
  double sum = 0;
  for(int i = 0; i < n; i++){
    sum+= a[i];
  }
  return (sum/n);
}

bool check_equality(double a[], int n){
  for (unsigned i = 0; i < n; i++) {
      if (a[i] != a[0]) {
          return false;
      }
  }
  return true;
}

vector<uint64_t>::iterator serialSearch(vector<uint64_t>::iterator start,vector<uint64_t>::iterator end,uint64_t test_key){
  vector<uint64_t>::iterator it;
  for(it = start ; it != end; it++){
    if(*it == test_key){
      return it;
    }
  }
  return start;
}

void printFrequency(vector<uint64_t> vec, int test_num)
{
  vector<int> freq;
  uint64_t last = -1;
  int counter = 0;
  for (auto & index : vec) {
    if(index == last){
      freq.back()++;
    }
    else{
      counter++;
      freq.push_back(1);
      last = index;
    }
  }
  sort(freq.begin(),freq.end());
  int cached_freq = 0;
  for(int i = 0; i < 500000; i++){
    cached_freq += freq.at(counter - i - 1);
  }
  double perc = cached_freq/test_num;
  cout << "cached points frequency " << cached_freq << endl;

}




void RadixSplineExample(int n) {
  srand ( unsigned ( time(0) ) );
  //vector<uint64_t> keys = CreateSkewedKeys<uint64_t>(unsigned ( time(0)));//keys(1e5);

  //cout << keys[100];
  //vector<uint64_t> keys(1e6);
  //generate(keys.begin(), keys.end(), rand);


  std::random_device rd; // obtain a random number from hardware
  std::mt19937 gen(rd()); // seed the generator
  std::uniform_int_distribution<uint64_t> distr(1,1e15);
  long int size = 200e6;
  //size = 1e5;
  vector<uint64_t> keys;
  /*for (int j = 0; j < size; j++){
      keys.push_back(distr(gen));
  }*/
  //read_osm();
  keys = read_taxi(keys,size);
  //keys = read_fb(keys,size);
  //keys = read_wiki(keys,size);
  //keys = read_osm(keys,size);
  //keys = read_books(keys,size);
  //cout << keys.at(12) << endl;
  /*int i = 0;
  int diff = 1;
  double a = 1;
  int step = 10;
  for(int y = 1; y < 1e7; y++){
    long int x;
    if(y%2 == 1){
      x = (y*step)/a;
      keys.push_back(x);
      keys.push_back(x+diff);
      keys.push_back(x+2*diff);
    }else{
      x = (y*step)/a;
      keys.push_back(x);
      keys.push_back(x+diff);
      keys.push_back(x+2*diff);
    }
    
  }*/

  /*double a = 1;
  double step = 8;
  double epsilon = 2;
  // 2*epsilon + dt < step
  double dt  = 1;
  for(int y = 1; y < 8; y++){
    long int x;
    if(y%2 == 1){
      x = (y*step)/a;
      keys.push_back(x);
    }else{
      x = (y*step - 2*epsilon -dt)/a;
      keys.push_back(x);
    }
  }
  int y = 1;
  for (const auto& key : keys){
    cout << "x: " << key << endl;
    //cout << "y: " << y << endl;
    y++;
  }*/
  
  

   //read_taxi_keys(file,keys,locations,n);
   //fclose(file);
  


  //keys.push_back(1e6);
  cout << keys.size() << endl;
  sort(keys.begin(), keys.end());

  std::uniform_int_distribution<uint64_t> distr2(1,keys.size()-1);
  vector<uint64_t> test_keys;
  int test_num = 100000;//ceil(0.05*size);
  for (int j = 0; j < test_num; j++){
      test_keys.push_back(keys[distr2(gen)]+1);
  }

  // Build RadixSpline.
  uint64_t min = keys.front();
  uint64_t max = keys.back();

    vector<rs::Builder<uint64_t>> multi_rsb;
    vector<uint64_t> keys_partition[n];
    for (int i = 0; i < n; i++){
      rs::Builder<uint64_t> rsb(min, max);
      multi_rsb.push_back(rsb);
    }
    double margin[n];
    double key_dist[n];
    double shrinkages[n];
    int optimal_i;
    int swi = 0;
    for (const auto& key : keys){
      // fill all choices with at least two keys to create the corridor
      if(swi < 2*n){
          multi_rsb.at(swi%n).AddKey(key);
          keys_partition[swi%n].push_back(key);
          //cout << "key " << key << endl;
          //cout << "index " << swi%n << endl;
          swi++;
          
      }
      // fill choices with key that is optimal for the error corridors
      else{
          for (int i = 0; i < n; i++){
              shrinkages[i] = multi_rsb.at(i).checkCDF(key);
          }
          optimal_i = argmin(shrinkages,n); 
          if(check_equality(shrinkages,n)){
            for (int i = 0; i < n; i++){
              shrinkages[i] = multi_rsb.at(i).checkCDF(key,1);
            }
            optimal_i = argmin(shrinkages,n); 
          }
            //cout << "key " << key << endl;
            //cout << "index " << optimal_i << endl;
            //cout << endl;
            multi_rsb.at(optimal_i).AddKey(key);
            keys_partition[optimal_i].push_back(key);         
       }
    }
    cout << endl;
    int sum = 0;
    for (int i = 0; i < n; i++){
      sum = sum + multi_rsb.at(i).GetNumberSplinePoints();
    }
    vector<rs::RadixSpline<uint64_t>> multi_rs;
    for (int i = 0; i < n; i++){      
          rs::RadixSpline<uint64_t> rs = multi_rsb.at(i).Finalize();
          multi_rs.push_back(rs);
    }
    multi_rsb.clear();
    vector<vector<uint64_t>::iterator> begin_it;
    uint64_t partition_back[n] = {0};
    for (int i = 0; i < n; i++){
      //cout << multi_rs.at(i).spline_points_.size() << endl;
      begin_it.push_back(begin(keys_partition[i]));
      partition_back[i] = keys_partition[i].back();
      //cout << "partition size " << keys_partition[i].size() << endl;
    }

    for (int i = 0; i < n; i++){
      //cout << "index " << i << endl;
      for (int j = 0; j < multi_rs.at(i).spline_points_.size(); j++){
        //cout << "sp_point " << multi_rs.at(i).spline_points_.at(j).x << endl;
      }
    }

    double index_size = sum;
    //cout << "spline points " << sum << endl;
    cout << "Megabytes: " << 16*index_size/1048576 << endl;
    for (int i = 0; i < n; i++){
       //cout << multi_rs.at(i).averageSearchCost() << endl;
    }
    

    double times = 0;
    double times_rs = 0;
    double counter = 0;
    vector<uint64_t> points;
    //cout << test_num << endl;


     int check = 0;
     for (int i = 0; i < n; i++){
      int estimate = 0;
      auto t_start = chrono::high_resolution_clock::now();   
      for (int j = 0; j < test_keys.size(); j++){
        uint64_t test_key = test_keys.at(j);
        //uint64_t test_key = 43;
        if(partition_back[i] < test_key || test_key == -1){
           continue;
        }
        rs::SearchBound bound = multi_rs.at(i).GetSearchBound(test_key);
        //test_keys.at(j) = -1;
        

        //t_start = chrono::high_resolution_clock::now();
        if(multi_rs.at(i).max_error_ > 0){
          vector<uint64_t>::iterator start = begin_it.at(i) + bound.begin, last = begin_it.at(i) + bound.end;
          estimate = std::lower_bound(start, last, test_key) - begin_it.at(i);
        }else{
          vector<uint64_t>::iterator start = begin_it.at(i) + bound.begin, last = begin_it.at(i) + bound.end;
          estimate = serialSearch(start, last, test_key) - begin_it.at(i);
        }
        
        //break;
        if(keys_partition[i].at(estimate) == test_key){
              test_keys.at(j) = -1;
              //points.push_back(multi_rs.at(i).CalculateSplineIndex(test_key));
              //check = 1;
              //break;
        }
      }
      auto t_end = chrono::high_resolution_clock::now();
      double elapsed_time_ms = chrono::duration<double, std::milli>(t_end-t_start).count();
      times += 1000000*elapsed_time_ms;
      for (int j = 0; j < test_keys.size(); j++){
        if(test_keys.at(j) = -1){
            test_keys.erase(test_keys.begin()+j);
        }
      }
    }

      cout << "avg q time: " << times/test_num << endl;

      /*sort(points.begin(),points.end());
      printFrequency(points,test_num);
      vector<uint64_t>::iterator ip;
  
      ip = unique(points.begin(),points.end());
      points.resize(std::distance(points.begin(), ip));
      cout << "unique points " << points.size() << endl;*/
      //cout << "0 keys: " << counter/test_num << endl;
      //cout << "avg rs time: " << times_rs/counter << endl;
      for (int j = 0; j < test_keys.size(); j++){
        if(test_keys.at(j) != -1){
          //cout << "key not found" << endl;
        }
      }
      multi_rs.clear();
      for (int i = 0; i < n; i++){
        keys_partition[i].clear();
      }
      keys.clear(); 
  }
  

int main(int argc, char** argv) {
  int n = atoi(argv[1]);
  RadixSplineExample(n);
  //MultiMapExample();

  return 0;
}