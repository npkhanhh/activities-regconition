#include "motion-feature.h"

int motion_feature()
{
    string folderin = "motion-data1";
    string label = folderin +"/activityLabel.txt";

    fstream f;
    f.open(label,ios::in);

    std::vector<std::string> file;
    string line;
    getline(f,line);
    while (line != "END")
    {
        string element;
        stringstream lineStream(line);
        getline(lineStream, element,',');
        file.push_back(element + ".txt");
        while (element != "")
            getline(lineStream, element,',');
        getline(f,line);
    }
    f.close();
    vector<vector<double>> motion_data;
    vector<double> frame;
    for(int i=0;i<file.size();++i)
    {
        f.open(file[i], ios::in);
        getline(f,line);
        while (line != "END")
        {
            frame.clear();
            string element;
            stringstream lineStream(line);
            getline(lineStream, element,',');
            while(element != "")
            {
                frame.push_back(stod(element));
                getline(lineStream, element,',');
            }
        }
    }
    return 0;
}
