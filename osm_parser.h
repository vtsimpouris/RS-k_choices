#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

template<class T>
bool load_binary_data(T data[], int length, const std::string &file_path) {
    std::ifstream is(file_path.c_str(), std::ios::binary | std::ios::in);
    if (!is.is_open()) {
        return false;
    }
    is.read(reinterpret_cast<char *>(data), std::streamsize(length * sizeof(T)));
    is.close();

    return true;
}

vector<uint64_t> read_osm(vector<uint64_t> osm_keys, long int n){
    size_t data_size = n;

    // prepare data
    uint64_t *keys = new uint64_t[data_size];
    load_binary_data(keys, data_size, "./osm_cellids_800M_uint64");

    vector<pair<uint64_t, uint64_t>> data;
    std::sort(keys, keys + data_size);
    for (size_t i = 0; i < data_size; i ++) {
        //data.push_back({keys[i], keys[i]});
        osm_keys.push_back(keys[i]);
    }
   return osm_keys;

}