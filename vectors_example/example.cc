#include <iostream>
#include <ctime>  
#include "include/rs/multi_map.h"
#include <bits/stdc++.h>
const size_t kNumKeys = 1e6;

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



int argmax(int a[], int n) {
  int c, index = 0;
  int max = a[0];
  for (c = 0; c < n; c++)
    if (a[c] > max)
      index = c;
      max = a[c];

  return index;
}

void RadixSplineExample() {
  srand ( unsigned ( time(0) ) );
  vector<uint64_t> keys = CreateSkewedKeys<uint64_t>(unsigned ( time(0)));//keys(1e5);

  //cout << keys[100];
  //vector<uint64_t> keys(1e6);
  //generate(keys.begin(), keys.end(), rand);
  keys.push_back(8128);
  sort(keys.begin(), keys.end());

  // Build RadixSpline.
  uint64_t min = keys.front();
  uint64_t max = keys.back();

  for(int n = 1; n < 8; n++){
    vector<rs::Builder<uint64_t>> multi_rsb;\
    //vector<uint64_t> keys_partition[n];
    vector<vector<uint64_t>> keys_partition;
    for (int i = 0; i < n; i++){
      rs::Builder<uint64_t> rsb(min, max);
      multi_rsb.push_back(rsb);
      vector<uint64_t> v;
      keys_partition.push_back(v);
    }
    
    int error_corr[n];
    int optimal_i;
    int swi = 0;
    for (const auto& key : keys){
      // fill all choices with at least two keys to create the corridor
      if(swi < 2*n){
        multi_rsb.at(swi%n).AddKey(key);
        //keys_partition[swi%n].push_back(key);
        keys_partition.at(swi%n).push_back(key);
        swi++;
      }
      // fill choices with key that is optimal for the error corridors
      else{ 
        for (int i = 0; i < n; i++){
              error_corr[i] = multi_rsb.at(i).checkCDF(key);
              //cout << error_corr[i] << endl;
          }
          //cout << endl;
          optimal_i = argmax(error_corr,n);
          multi_rsb.at(optimal_i).AddKey(key);
          //keys_partition[optimal_i].push_back(key);
          keys_partition.at(optimal_i).push_back(key);
       }
    }
    
    int sum = 0;
    for (int i = 0; i < n; i++){
      sum = sum + multi_rsb.at(i).GetNumberSplinePoints();
    }
    vector<rs::RadixSpline<uint64_t>> multi_rs;
    for (int i = 0; i < n; i++){        
          rs::RadixSpline<uint64_t> rs = multi_rsb.at(i).Finalize();
          multi_rs.push_back(rs);
    }
    cout << sum << "\n";
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(1, 10000); // define the range
    int key_index = distr(gen);
    int estimate = 0;
    uint64_t test_key = keys[key_index];
    clock_t begin_clock = clock();
    clock_t end_clock = clock();

    int test_num = 10000;
    vector<uint64_t> test_keys_index(test_num);
    for (int j = 0; j < test_num; j++){
      test_keys_index.push_back(distr(gen));
    }
    vector<vector<uint64_t>::iterator> begin_it;
    for (int i = 0; i < n; i++){
      begin_it.push_back(begin(keys_partition.at(i)));
    }
    //rs::SearchBound bound = multi_rs.at(0).GetSearchBound(test_key);
    begin_clock = clock();
    for (int j = 0; j < test_num; j++){
      key_index = 150;//test_keys_index.at(j);
      test_key = keys[key_index];
      int bound_begin = 0;
      int bound_end = 0;
      int check = 0;

      for (int i = 0; i < n; i++){
          if(keys_partition.at(i).back() < test_key){
           continue;
          }
          rs::SearchBound bound = multi_rs.at(0).GetSearchBound(test_key);
          auto start = begin_it.at(i) + bound.begin, last = begin_it.at(i) + bound.end;
          estimate = std::lower_bound(start, last, test_key) - begin_it.at(i);
          if(keys_partition.at(i).at(estimate) == test_key){
            //cout << "The actual key is  " << test_key << endl;
            check = 1;
            break;
          }

        //cout << "The actual key is  " << test_key << endl;
 
        //bound_begin = bound_begin + bound.begin;
        //bound_end = bound_end + bound.end;
        //cout << "The search key is in the range: ["
        //    << bound.begin << ", " << bound.end << ")" << endl;
      }
      if(check == 0){
         cout << "error key not found" << endl;
      }
        //auto start = begin(keys) + bound_begin, last = begin(keys) + bound_end;
        //estimate = std::lower_bound(start, last, test_key) - begin(keys);
    }
    end_clock = clock();
    double time_spent = (double)(end_clock - begin_clock) / CLOCKS_PER_SEC;
    cout << "time_spent: " << 1000000*time_spent << endl;
    //cout << sum << endl;
    //cout << "The key is at position: " << estimate << endl;
    //cout << "The key is at position: " << key_index << endl;
    cout << "\n";
  }
  
  
  /*
  rs::RadixSpline<uint64_t> rs = multi_rsb.at(0).Finalize();
  // Search using RadixSpline.
  uint64_t test_key = 111;
  rs::SearchBound bound = rs.GetSearchBound(test_key);
  cout << "The search key is in the range: ["
       << bound.begin << ", " << bound.end << ")" << endl;
  auto start = begin(keys) + bound.begin, last = begin(keys) + bound.end;
  cout << "The key is at position: " << std::lower_bound(start, last, test_key) - begin(keys) << endl;*/


/*void MultiMapExample() {
  vector<pair<uint64_t, char>> data = {{1ull, 'a'},
                                       {12ull, 'b'},
                                       {7ull, 'c'}, // Unsorted.
                                       {42ull, 'd'}};
  rs::MultiMap<uint64_t, char> map(begin(data), end(data));

  cout << "find(7): '" << map.find(7)->second << "'" << endl;
  cout << "lower_bound(3): '" << map.lower_bound(3)->second << "'" << endl;*/
}

int main(int argc, char** argv) {
  RadixSplineExample();
  //MultiMapExample();

  return 0;
}
