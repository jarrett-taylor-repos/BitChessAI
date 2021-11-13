#include "Board.cpp"

class zstuff{
    public:
    long long int zval;
    int materialadv;
    int pst;
    int eval;
    //Board b;
    zstuff(long long int zval,int materialadv,int pst);
};

zstuff::zstuff(long long int zvall,int materialadvv,int pstt){
    zval = zvall;
    materialadv = materialadvv;
    pst = pstt;
    eval = materialadv*1000+pst;
    //b = bb;
}