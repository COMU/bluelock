#ifndef BLUECONFPP_H_
#define BLUECONFPP_H_

extern "C"
{
    #include "../../blueconf.h"
}

namespace BW {
class BlueConfPP;
}

class BlueConfPP
{

public:
    explicit BlueConfPP(void);
    ~BlueConfPP();
    
    void get_conf_();
    void print_rList_();
private:
    
};

#endif // BLUECONFPP_H_
