#ifndef UCIMOVE_H
#define UCIMOVE_H
#include <string>
using std::string;
class UciMove {
    private: 
        string start;
        string target;
    public:
        UciMove();
        UciMove(string, string);
        UciMove(const UciMove&);
        UciMove& operator=(const UciMove&);
        bool operator==(const UciMove &b) const;
        string getStart();
        string getTarget();
};

#endif