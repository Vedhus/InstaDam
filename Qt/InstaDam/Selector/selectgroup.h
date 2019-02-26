#ifndef SELECTGROUP_H
#define SELECTGROUP_H

#include "selectItem.h"
#include <vector>
using namespace std;

class SelectGroup{
public:
    SelectGroup();
    ~SelectGroup();
    void addItem(SelectItem* item);

private:
    vector<SelectItem*> items;
};

#endif // SELECTGROUP_H
