#include "blueconfpp.hpp"

BlueConfPP::BlueConfPP(void)
{
    get_conf_();
}

void BlueConfPP::get_conf_()
{
    rList = get_conf();
}

void BlueConfPP::print_rList_()
{
    print_rList(rList->head);
}

BlueConfPP::~BlueConfPP()
{
    d_record_list(rList);
}

