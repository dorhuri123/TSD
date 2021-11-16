

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <sstream>
#include "vector"
#include "map"
#include "iostream"
#include "fstream"
#include "string.h"
using namespace std;

class TimeSeries{
    map<string, vector<float>> tableData;
    vector<string> features;
public:

	TimeSeries(const char* CSVfileName){
        fstream file;
        file.open(CSVfileName, ios::in);
        string line, key, value, fileContent;
        fileContent.assign((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
        getline(file,line,'\n');
        istringstream ss(line);
        while(getline(ss,key,',')){
            vector<float> initialize {};
            tableData.insert(pair<string, vector<float>>(key,initialize));
            features.push_back(key);
        }
        stringstream fileRead(fileContent);
        getline(fileRead,line,'\n');
        while (getline(fileRead,line,'\n')){
            //for (auto i = features.begin(); i != features.end(); ++i) {
            for(const auto& i:features){
                auto it = tableData.find(i);
                getline(fileRead,value,',');
                it->second.push_back(stof(value));
            }
        }
        file.close();
	}
    vector<float> getData(string key)const{
        return tableData.at(key);
    }
    vector<string> getFeatures()const{
        return features;
    }
};



#endif /* TIMESERIES_H_ */
