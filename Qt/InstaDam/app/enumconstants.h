#ifndef ENUMCONSTANTS_H
#define ENUMCONSTANTS_H

class EnumConstants{
 public:
    EnumConstants();
    enum maskTypes{ BLUR, CANNY, THRESHOLD, COLORTHRESHOLD, OTSU, LAT, RIDGE, MORPH, GUIDED, LABELMASK, OTHER};
    static const int numFilters = 10;

    enum threshold_or_filter{THRESH, FILTER};

};


#endif  // ENUMCONSTANTS_H
