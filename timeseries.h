//
// Created by dor on 10/14/21.
//Dor Huri 209409218
//Aviya Hadad 314802075
//

#ifndef AVIA1_TIMESERIES_H
#define AVIA1_TIMESERIES_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class TimeSeries {

public:
    map<string, vector<float>> tableData;
};


#endif //AVIA1_TIMESERIES_H


*/




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
    //map for the feature and his values
    map<string, vector<float>> tableData;
    //all the feature in vector
    vector<string> features;
public:
    /*
     * this constructor get a csv file and organize him as a table chart
     * where the first line of the file is the featured.
     */
    TimeSeries(const char* CSVfileName){
        //taking file using fstream
        fstream file(CSVfileName);
        string line, key, value, fileContent;
        //getting all the file content to string using iterator
        fileContent.assign((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
        //string stream get the file content
        stringstream ss(fileContent);
        //line get the first line of file
        getline(ss,line,'\n');
        stringstream head(line);
        //striping line by comma
        while(getline(head,key,',')){
            //initialize empty vector
            vector<float> initialize {};
            //inserting to map the keys with empty vector
            tableData.insert(pair<string, vector<float>>(key,initialize));
            //inserting feature to vector
            features.push_back(key);
        }
        //go throw file until last line
        while (getline(ss,line,'\n')){
            stringstream stringStream(line);
            //iterate over featured vector
            for(const auto& i:features){
                //it gets the value of key i
                auto it = tableData.find(i);
                //get the value in line
                getline(stringStream,value,',');
                //add value to vector
                it->second.push_back(stof(value));
            }
        }
        //closing file
        file.close();
    }
    /*
     * function return the value of giving key
     */
    vector<float> getData(string key)const{
        return tableData.at(key);
    }
    /*
     * function return the featured vector
     */
    vector<string> getFeatures()const{
        return features;
    }
    /*
     * function return the map
     */
    map<string, vector<float>> getTable()const{
        return tableData;
    }
    /*
     * function return the size of rows.
    */
    int getRowSize() const{
        return tableData.begin()->second.size();
    }
};



#endif /* TIMESERIES_H_ */