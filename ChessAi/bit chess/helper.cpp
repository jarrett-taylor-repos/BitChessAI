#include <vector>
#include <string>
using namespace std;

vector<string> split(string str, string token){
    vector<string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

int getDifferenceOffset(int start, int end) {
    int total_diff = start - end;
    int diff = 0;
    if (total_diff % 7 == 0) { diff = 7; }
    else if (total_diff % 1 == 0) { diff = 1; }
    else if (total_diff % 8 == 0) { diff = 8; }
    else if (total_diff % 9 == 0) { diff = 9; }

    if (total_diff > 0) { diff *= -1; }

    return diff;
}
