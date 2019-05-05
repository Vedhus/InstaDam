#ifndef ENUMCONSTANTS_H
#define ENUMCONSTANTS_H

class EnumConstants{
 public:
    EnumConstants();
    enum maskTypes{CANNY, THRESHOLD, BLUR, LABELMASK, COLORTHRESHOLD, OTHER};

    enum threshold_or_filter{THRESH, FILTER};

};


#endif  // ENUMCONSTANTS_H
