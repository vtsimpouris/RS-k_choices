 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
using namespace std;

void read_taxi_keys(ifstream& file,vector<uint64_t> keys,long int n){
   string line, word;
   int i = 0;
   while(getline(file, line) && i < n){
       cout << "babis" << endl;
      stringstream str(line);
      while(getline(str, word, ',')){
         cout << word << endl;
      }
      cout << endl;
      i++;
   }
   // skip description of csv
   //fgets(line, sizeof(line), file);
   // fgets(line, sizeof(line), file);
   char* tmp;
   char* token;
   char* key;
   // read second column (datetime of pickup) as keys
   /*while(fgets(line, sizeof(line), file) && i < n){
       tmp = strdup(line);
       token = strtok(tmp,",");
       key = strtok(NULL,",");    
       struct tm key_tm;
       strptime(key, "%Y-%m-%d %H:%M:%S", &key_tm);
       keys[i] = mktime((struct tm*) &key_tm);
       i = i + 1;
       free(tmp);

   }*/

    
}
   std::random_device rd; // obtain a random number from hardware
   std::mt19937 gen(rd()); // seed the generator
   std::uniform_int_distribution<> distr(25, 63); // define the range

   vector<uint64_t> read_taxi(vector<uint64_t> keys, long int n){
   fstream file("yellow_tripdata_2020-01.csv", ios::in);
   string line, word;
   int i = 0;
   while(getline(file, line) && i < n){
      stringstream str(line);
      getline(str, word, ',');
      getline(str, word, ',');
      //getline(str, word, ',');
      if(i > 0){
        int ssize = word.length();
        char key[ssize + 1];
        strcpy(key, word.c_str());
        struct tm key_tm;
        strptime(key, "%Y-%m-%d %H:%M:%S", &key_tm);
        //keys.push_back((uint64_t)((struct tm*) &key_tm));
        keys.push_back(mktime((struct tm*) &key_tm));
        //cout << mktime((struct tm*) &key_tm) << endl;
      }
      i++;
   }
   //read_taxi_keys(file,keys,n);
   file.close();
   /*fstream file2("yellow_tripdata_2020-01.csv", ios::in);
   i = 0;
   while(getline(file2, line) && i < n){
      stringstream str(line);
      getline(str, word, ',');
      getline(str, word, ',');
      getline(str, word, ',');
      if(i > 0){
        int ssize = word.length();
        char key[ssize + 1];
        strcpy(key, word.c_str());
        struct tm key_tm;
        strptime(key, "%Y-%m-%d %H:%M:%S", &key_tm);
        //keys.push_back((uint64_t)((struct tm*) &key_tm));
        keys.push_back(mktime((struct tm*) &key_tm));
        //cout << mktime((struct tm*) &key_tm) << endl;
      }
      i++;
   }
   //read_taxi_keys(file,keys,n);
   file2.close();
   /*fstream file3("yellow_tripdata_2020-02.csv", ios::in);
   i = 0;
   while(getline(file3, line) && i < n){
      stringstream str(line);
      getline(str, word, ',');
      getline(str, word, ',');
      getline(str, word, ',');
      if(i > 0){
        int ssize = word.length();
        char key[ssize + 1];
        strcpy(key, word.c_str());
        struct tm key_tm;
        strptime(key, "%Y-%m-%d %H:%M:%S", &key_tm);
        //keys.push_back((uint64_t)((struct tm*) &key_tm));
        keys.push_back(mktime((struct tm*) &key_tm));
        //cout << mktime((struct tm*) &key_tm) << endl;
      }
      i++;
   }
   //read_taxi_keys(file,keys,n);
   file3.close();
   fstream file4("yellow_tripdata_2012-01.csv", ios::in);
   i = 0;
   while(getline(file4, line) && i < n){
      stringstream str(line);
      getline(str, word, ',');
      getline(str, word, ',');
      getline(str, word, ',');
      if(i > 0){
        int ssize = word.length();
        char key[ssize + 1];
        strcpy(key, word.c_str());
        struct tm key_tm;
        strptime(key, "%Y-%m-%d %H:%M:%S", &key_tm);
        //keys.push_back((uint64_t)((struct tm*) &key_tm));
        keys.push_back(mktime((struct tm*) &key_tm));
        //cout << mktime((struct tm*) &key_tm) << endl;
      }
      i++;
   }
   //read_taxi_keys(file,keys,n);
   file4.close();*/
   return keys;
}